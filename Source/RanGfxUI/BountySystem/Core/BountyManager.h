#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "../../../RanLogic/GLCharDefine.h"

class CBountyDatabase;
class CBountyConfig;
class CBountyOrder;
class CBountyNetworkHandler;

// 追殺令狀態枚舉
enum EBOUNTY_STATUS
{
    BOUNTY_STATUS_ACTIVE = 0,      // 活躍中
    BOUNTY_STATUS_ACCEPTED,        // 已接受
    BOUNTY_STATUS_COMPLETED,       // 已完成
    BOUNTY_STATUS_CANCELLED,       // 已取消
    BOUNTY_STATUS_EXPIRED,         // 已過期
    
    BOUNTY_STATUS_SIZE
};

// 追殺令類型
enum EBOUNTY_TYPE
{
    BOUNTY_TYPE_NORMAL = 0,        // 普通追殺令
    BOUNTY_TYPE_REVENGE,           // 復仇追殺令
    BOUNTY_TYPE_GUILD_WAR,         // 公會戰追殺令
    BOUNTY_TYPE_EVENT,             // 活動追殺令
    
    BOUNTY_TYPE_SIZE
};

// 追殺令獎勵類型
enum EBOUNTY_REWARD_TYPE
{
    BOUNTY_REWARD_MONEY = 0,       // 金錢獎勵
    BOUNTY_REWARD_ITEM,            // 道具獎勵
    BOUNTY_REWARD_EXP,             // 經驗獎勵
    BOUNTY_REWARD_MIXED,           // 混合獎勵
    
    BOUNTY_REWARD_TYPE_SIZE
};

/**
 * 追殺令管理器
 * 負責管理追殺令系統的核心功能
 */
class CBountyManager : public CUIGroup
{
private:
    static const float BOUNTY_UPDATE_INTERVAL;    // 更新間隔
    static const int MAX_ACTIVE_BOUNTIES;         // 最大活躍追殺令數量
    static const int MAX_PLAYER_BOUNTIES;         // 每個玩家最大發布數量
    
public:
    CBountyManager(EngineDeviceMan* pEngineDevice);
    virtual ~CBountyManager();

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
    virtual void SetVisibleSingle(BOOL bVisible);

public:
    // 主要接口
    BOOL Initialize();
    void Shutdown();
    
    // 追殺令操作
    BOOL CreateBounty(DWORD dwPublisherID, DWORD dwTargetID, EBOUNTY_TYPE eType, 
                     EBOUNTY_REWARD_TYPE eRewardType, LONGLONG llRewardAmount, 
                     const std::string& strReason);
    BOOL AcceptBounty(DWORD dwBountyID, DWORD dwHunterID);
    BOOL CompleteBounty(DWORD dwBountyID, DWORD dwHunterID);
    BOOL CancelBounty(DWORD dwBountyID, DWORD dwCancellerID);
    
    // 查詢接口
    std::vector<CBountyOrder*> GetActiveBounties();
    std::vector<CBountyOrder*> GetPlayerBounties(DWORD dwPlayerID);
    std::vector<CBountyOrder*> GetTargetBounties(DWORD dwTargetID);
    CBountyOrder* GetBountyByID(DWORD dwBountyID);
    
    // 排行榜
    std::vector<std::pair<DWORD, int>> GetHunterRanking(int nTopCount = 10);
    std::vector<std::pair<DWORD, int>> GetTargetRanking(int nTopCount = 10);
    
    // 統計信息
    int GetPlayerKillCount(DWORD dwPlayerID);
    int GetPlayerDeathCount(DWORD dwPlayerID);
    LONGLONG GetPlayerEarnedRewards(DWORD dwPlayerID);
    
    // 事件處理
    void OnPlayerKilled(DWORD dwKillerID, DWORD dwVictimID, EKILL_TYPE eKillType);
    void OnPlayerLogout(DWORD dwPlayerID);
    
    // GM接口
    BOOL GMCreateBounty(DWORD dwTargetID, EBOUNTY_TYPE eType, LONGLONG llRewardAmount);
    BOOL GMCancelBounty(DWORD dwBountyID);
    BOOL GMModifyReward(DWORD dwBountyID, LONGLONG llNewAmount);
    
protected:
    // 內部方法
    void UpdateBounties(float fElapsedTime);
    void ProcessExpiredBounties();
    void NotifyBountyStatusChange(DWORD dwBountyID, EBOUNTY_STATUS eNewStatus);
    BOOL ValidateBountyRequest(DWORD dwPublisherID, DWORD dwTargetID, LONGLONG llRewardAmount);
    void DistributeReward(DWORD dwHunterID, EBOUNTY_REWARD_TYPE eType, LONGLONG llAmount);
    
private:
    CBountyDatabase* m_pDatabase;
    CBountyConfig* m_pConfig;
    CBountyNetworkHandler* m_pNetworkHandler;
    
    // 狀態變量
    BOOL m_bInitialized;
    BOOL m_bEnabled;
    float m_fUpdateTimer;
    
    // 緩存數據
    std::map<DWORD, CBountyOrder*> m_mapActiveBounties;
    std::map<DWORD, std::vector<DWORD>> m_mapPlayerBounties;
    
    // 統計數據
    std::map<DWORD, int> m_mapKillCounts;
    std::map<DWORD, int> m_mapDeathCounts;
    std::map<DWORD, LONGLONG> m_mapEarnedRewards;
    
    // 性能統計
    DWORD m_dwFrameCount;
    float m_fPerformanceTimer;
};