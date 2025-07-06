#include "stdafx.h"
#include "BountyManager.h"
#include "BountyOrder.h"
#include "BountyDatabase.h"
#include "BountyConfig.h"
#include "../Network/BountyNetworkHandler.h"
#include "../../../enginelib/GUInterface/GameTextControl.h"
#include "../../../RanLogicClient/GLGaeaClient.h"

// 靜態常量定義
const float CBountyManager::BOUNTY_UPDATE_INTERVAL = 5.0f;
const int CBountyManager::MAX_ACTIVE_BOUNTIES = 1000;
const int CBountyManager::MAX_PLAYER_BOUNTIES = 5;

CBountyManager::CBountyManager(EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_pDatabase(NULL)
    , m_pConfig(NULL)
    , m_pNetworkHandler(NULL)
    , m_bInitialized(FALSE)
    , m_bEnabled(FALSE)
    , m_fUpdateTimer(0.0f)
    , m_dwFrameCount(0)
    , m_fPerformanceTimer(0.0f)
{
}

CBountyManager::~CBountyManager()
{
    Shutdown();
}

void CBountyManager::CreateSubControl()
{
    // 創建子控件 - 如果需要UI元素的話
}

BOOL CBountyManager::Initialize()
{
    if (m_bInitialized)
        return TRUE;

    try
    {
        // 創建配置管理器
        m_pConfig = new CBountyConfig();
        if (!m_pConfig->Initialize("BountySystem.xml"))
        {
            sc::writeLogError("CBountyManager::Initialize - Failed to initialize config");
            return FALSE;
        }

        // 創建數據庫操作器
        m_pDatabase = new CBountyDatabase();
        if (!m_pDatabase->Initialize(GLGaeaClient::GetInstance().GetDBManager()))
        {
            sc::writeLogError("CBountyManager::Initialize - Failed to initialize database");
            return FALSE;
        }

        // 創建網絡處理器
        m_pNetworkHandler = new CBountyNetworkHandler();
        if (!m_pNetworkHandler->Initialize())
        {
            sc::writeLogError("CBountyManager::Initialize - Failed to initialize network handler");
            return FALSE;
        }

        // 加載活躍的追殺令
        std::vector<CBountyOrder*> vecActiveBounties = m_pDatabase->GetActiveBounties();
        for (auto pBounty : vecActiveBounties)
        {
            if (pBounty)
            {
                m_mapActiveBounties[pBounty->GetBountyID()] = pBounty;
                
                // 按玩家分組
                DWORD dwPublisherID = pBounty->GetPublisherID();
                m_mapPlayerBounties[dwPublisherID].push_back(pBounty->GetBountyID());
            }
        }

        m_bEnabled = m_pConfig->GetGeneralConfig().bEnabled;
        m_bInitialized = TRUE;

        sc::writeLogInfo("CBountyManager::Initialize - Bounty system initialized successfully");
        return TRUE;
    }
    catch (const std::exception& e)
    {
        sc::writeLogError(sc::string::format("CBountyManager::Initialize - Exception: %1%", e.what()));
        return FALSE;
    }
}

void CBountyManager::Shutdown()
{
    if (!m_bInitialized)
        return;

    // 清理活躍追殺令
    for (auto& pair : m_mapActiveBounties)
    {
        SAFE_DELETE(pair.second);
    }
    m_mapActiveBounties.clear();
    m_mapPlayerBounties.clear();

    // 清理組件
    SAFE_DELETE(m_pNetworkHandler);
    SAFE_DELETE(m_pDatabase);
    SAFE_DELETE(m_pConfig);

    m_bInitialized = FALSE;
    m_bEnabled = FALSE;

    sc::writeLogInfo("CBountyManager::Shutdown - Bounty system shutdown completed");
}

void CBountyManager::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    if (!m_bInitialized || !m_bEnabled)
        return;

    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);

    // 更新計時器
    m_fUpdateTimer += fElapsedTime;
    m_fPerformanceTimer += fElapsedTime;
    m_dwFrameCount++;

    // 定期更新追殺令狀態
    if (m_fUpdateTimer >= BOUNTY_UPDATE_INTERVAL)
    {
        UpdateBounties(fElapsedTime);
        m_fUpdateTimer = 0.0f;
    }

    // 性能統計
    if (m_fPerformanceTimer >= 60.0f) // 每分鐘統計一次
    {
        float fFPS = m_dwFrameCount / m_fPerformanceTimer;
        sc::writeLogInfo(sc::string::format("CBountyManager Performance - FPS: %.2f, Active Bounties: %d", 
                                          fFPS, m_mapActiveBounties.size()));
        
        m_dwFrameCount = 0;
        m_fPerformanceTimer = 0.0f;
    }
}

void CBountyManager::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
    CUIGroup::TranslateUIMessage(ControlID, dwMsg);
}

void CBountyManager::SetVisibleSingle(BOOL bVisible)
{
    CUIGroup::SetVisibleSingle(bVisible);
}

BOOL CBountyManager::CreateBounty(DWORD dwPublisherID, DWORD dwTargetID, EBOUNTY_TYPE eType, 
                                 EBOUNTY_REWARD_TYPE eRewardType, LONGLONG llRewardAmount, 
                                 const std::string& strReason)
{
    if (!m_bInitialized || !m_bEnabled)
        return FALSE;

    // 驗證請求
    if (!ValidateBountyRequest(dwPublisherID, dwTargetID, llRewardAmount))
        return FALSE;

    // 檢查玩家發布數量限制
    if (m_mapPlayerBounties[dwPublisherID].size() >= static_cast<size_t>(m_pConfig->GetGeneralConfig().nMaxPlayerBounties))
    {
        sc::writeLogWarn(sc::string::format("CBountyManager::CreateBounty - Player %d exceeded max bounties limit", dwPublisherID));
        return FALSE;
    }

    // 創建追殺令數據
    CBountyOrder::BountyData data;
    data.dwBountyID = 0; // 由數據庫生成
    data.dwPublisherID = dwPublisherID;
    data.dwTargetID = dwTargetID;
    data.dwHunterID = 0;
    data.eBountyType = eType;
    data.eStatus = BOUNTY_STATUS_ACTIVE;
    data.eRewardType = eRewardType;
    data.llRewardAmount = llRewardAmount;
    data.strReason = strReason;
    data.tCreateTime = _time64(NULL);
    data.tAcceptTime = 0;
    data.tCompleteTime = 0;
    data.tExpireTime = data.tCreateTime + (m_pConfig->GetGeneralConfig().nDefaultExpireHours * 3600);

    // 保存到數據庫
    if (!m_pDatabase->CreateBounty(data))
    {
        sc::writeLogError("CBountyManager::CreateBounty - Failed to save bounty to database");
        return FALSE;
    }

    // 創建追殺令對象並添加到緩存
    CBountyOrder* pBounty = new CBountyOrder(data);
    m_mapActiveBounties[data.dwBountyID] = pBounty;
    m_mapPlayerBounties[dwPublisherID].push_back(data.dwBountyID);

    // 發送網絡通知
    if (m_pNetworkHandler)
    {
        m_pNetworkHandler->NotifyBountyCreated(data.dwBountyID);
    }

    NotifyBountyStatusChange(data.dwBountyID, BOUNTY_STATUS_ACTIVE);

    sc::writeLogInfo(sc::string::format("CBountyManager::CreateBounty - Bounty %d created successfully", data.dwBountyID));
    return TRUE;
}

BOOL CBountyManager::AcceptBounty(DWORD dwBountyID, DWORD dwHunterID)
{
    if (!m_bInitialized || !m_bEnabled)
        return FALSE;

    auto it = m_mapActiveBounties.find(dwBountyID);
    if (it == m_mapActiveBounties.end())
        return FALSE;

    CBountyOrder* pBounty = it->second;
    if (!pBounty || !pBounty->CanBeAcceptedBy(dwHunterID))
        return FALSE;

    // 獲取獵人名稱
    std::string strHunterName = ""; // TODO: 從玩家數據獲取名稱

    if (!pBounty->Accept(dwHunterID, strHunterName))
        return FALSE;

    // 更新數據庫
    if (!m_pDatabase->UpdateBounty(pBounty->GetData()))
    {
        sc::writeLogError("CBountyManager::AcceptBounty - Failed to update database");
        return FALSE;
    }

    // 發送網絡通知
    if (m_pNetworkHandler)
    {
        m_pNetworkHandler->NotifyBountyAccepted(dwBountyID, dwHunterID);
    }

    NotifyBountyStatusChange(dwBountyID, BOUNTY_STATUS_ACCEPTED);

    sc::writeLogInfo(sc::string::format("CBountyManager::AcceptBounty - Bounty %d accepted by hunter %d", dwBountyID, dwHunterID));
    return TRUE;
}

BOOL CBountyManager::CompleteBounty(DWORD dwBountyID, DWORD dwHunterID)
{
    if (!m_bInitialized || !m_bEnabled)
        return FALSE;

    auto it = m_mapActiveBounties.find(dwBountyID);
    if (it == m_mapActiveBounties.end())
        return FALSE;

    CBountyOrder* pBounty = it->second;
    if (!pBounty || pBounty->GetHunterID() != dwHunterID)
        return FALSE;

    if (!pBounty->Complete())
        return FALSE;

    // 分發獎勵
    DistributeReward(dwHunterID, pBounty->GetRewardType(), pBounty->GetRewardAmount());

    // 更新數據庫
    if (!m_pDatabase->UpdateBounty(pBounty->GetData()))
    {
        sc::writeLogError("CBountyManager::CompleteBounty - Failed to update database");
        return FALSE;
    }

    // 更新統計
    m_mapKillCounts[dwHunterID]++;
    m_mapEarnedRewards[dwHunterID] += pBounty->GetRewardAmount();

    // 從活躍列表移除
    m_mapActiveBounties.erase(it);

    // 發送網絡通知
    if (m_pNetworkHandler)
    {
        m_pNetworkHandler->NotifyBountyCompleted(dwBountyID, dwHunterID);
    }

    NotifyBountyStatusChange(dwBountyID, BOUNTY_STATUS_COMPLETED);

    sc::writeLogInfo(sc::string::format("CBountyManager::CompleteBounty - Bounty %d completed by hunter %d", dwBountyID, dwHunterID));
    return TRUE;
}

std::vector<CBountyOrder*> CBountyManager::GetActiveBounties()
{
    std::vector<CBountyOrder*> vecBounties;
    for (auto& pair : m_mapActiveBounties)
    {
        if (pair.second)
            vecBounties.push_back(pair.second);
    }
    return vecBounties;
}

void CBountyManager::OnPlayerKilled(DWORD dwKillerID, DWORD dwVictimID, EKILL_TYPE eKillType)
{
    if (!m_bInitialized || !m_bEnabled)
        return;

    // 檢查是否有針對受害者的活躍追殺令
    for (auto& pair : m_mapActiveBounties)
    {
        CBountyOrder* pBounty = pair.second;
        if (pBounty && pBounty->GetTargetID() == dwVictimID && 
            pBounty->GetHunterID() == dwKillerID && pBounty->IsAccepted())
        {
            // 完成追殺令
            CompleteBounty(pBounty->GetBountyID(), dwKillerID);
            break;
        }
    }
}

void CBountyManager::UpdateBounties(float fElapsedTime)
{
    // 處理過期的追殺令
    ProcessExpiredBounties();

    // 其他定期維護操作
    if (m_pDatabase)
    {
        m_pDatabase->CleanupExpiredBounties();
    }
}

void CBountyManager::ProcessExpiredBounties()
{
    __time64_t tCurrentTime = _time64(NULL);
    std::vector<DWORD> vecExpiredBounties;

    for (auto& pair : m_mapActiveBounties)
    {
        CBountyOrder* pBounty = pair.second;
        if (pBounty && pBounty->ShouldExpire())
        {
            vecExpiredBounties.push_back(pBounty->GetBountyID());
        }
    }

    // 處理過期的追殺令
    for (DWORD dwBountyID : vecExpiredBounties)
    {
        auto it = m_mapActiveBounties.find(dwBountyID);
        if (it != m_mapActiveBounties.end())
        {
            CBountyOrder* pBounty = it->second;
            if (pBounty)
            {
                pBounty->Expire();
                m_pDatabase->UpdateBounty(pBounty->GetData());
                
                NotifyBountyStatusChange(dwBountyID, BOUNTY_STATUS_EXPIRED);
                
                // 從活躍列表移除
                m_mapActiveBounties.erase(it);
                SAFE_DELETE(pBounty);
            }
        }
    }

    if (!vecExpiredBounties.empty())
    {
        sc::writeLogInfo(sc::string::format("CBountyManager::ProcessExpiredBounties - Expired %d bounties", vecExpiredBounties.size()));
    }
}

BOOL CBountyManager::ValidateBountyRequest(DWORD dwPublisherID, DWORD dwTargetID, LONGLONG llRewardAmount)
{
    if (!m_pConfig)
        return FALSE;

    // 檢查基本條件
    if (dwPublisherID == dwTargetID)
        return FALSE;

    if (llRewardAmount < m_pConfig->GetGeneralConfig().llMinRewardAmount ||
        llRewardAmount > m_pConfig->GetGeneralConfig().llMaxRewardAmount)
        return FALSE;

    // TODO: 添加更多驗證邏輯
    // - 檢查玩家等級
    // - 檢查公會保護
    // - 檢查防刷新限制
    // - 檢查玩家金錢

    return TRUE;
}

void CBountyManager::DistributeReward(DWORD dwHunterID, EBOUNTY_REWARD_TYPE eType, LONGLONG llAmount)
{
    // TODO: 實現獎勵分發邏輯
    // 根據獎勵類型分發對應的獎勵
    
    switch (eType)
    {
    case BOUNTY_REWARD_MONEY:
        // 分發金錢獎勵
        break;
    case BOUNTY_REWARD_ITEM:
        // 分發道具獎勵
        break;
    case BOUNTY_REWARD_EXP:
        // 分發經驗獎勵
        break;
    case BOUNTY_REWARD_MIXED:
        // 分發混合獎勵
        break;
    }
}

void CBountyManager::NotifyBountyStatusChange(DWORD dwBountyID, EBOUNTY_STATUS eNewStatus)
{
    // TODO: 實現狀態變更通知
    // 可以觸發UI更新、音效播放等
}