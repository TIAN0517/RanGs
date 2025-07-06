# 追殺令系統 API 參考文檔

## 概述

本文檔提供了追殺令系統所有公共API的詳細說明，包括接口定義、參數說明、返回值和使用示例。

## 核心管理器 API

### CBountyManager 類

追殺令系統的主要管理器類，負責所有追殺令的生命週期管理。

#### 初始化和清理

```cpp
// 初始化追殺令管理器
BOOL Initialize();

// 關閉追殺令管理器
void Shutdown();
```

**說明：**
- `Initialize()`: 初始化追殺令系統，包括加載配置、連接數據庫、創建網絡處理器等
- `Shutdown()`: 清理所有資源，保存數據並關閉系統

**返回值：**
- `TRUE`: 成功
- `FALSE`: 失敗

**使用示例：**
```cpp
CBountyManager* pBountyMgr = new CBountyManager(pEngineDevice);
if (pBountyMgr->Initialize())
{
    // 系統初始化成功
}
```

#### 追殺令操作

```cpp
// 創建追殺令
BOOL CreateBounty(DWORD dwPublisherID, DWORD dwTargetID, EBOUNTY_TYPE eType, 
                 EBOUNTY_REWARD_TYPE eRewardType, LONGLONG llRewardAmount, 
                 const std::string& strReason);

// 接受追殺令
BOOL AcceptBounty(DWORD dwBountyID, DWORD dwHunterID);

// 完成追殺令
BOOL CompleteBounty(DWORD dwBountyID, DWORD dwHunterID);

// 取消追殺令
BOOL CancelBounty(DWORD dwBountyID, DWORD dwCancellerID);
```

**參數說明：**
- `dwPublisherID`: 發布者角色ID
- `dwTargetID`: 目標角色ID
- `eType`: 追殺令類型 (BOUNTY_TYPE_NORMAL, BOUNTY_TYPE_REVENGE, 等)
- `eRewardType`: 獎勵類型 (BOUNTY_REWARD_MONEY, BOUNTY_REWARD_ITEM, 等)
- `llRewardAmount`: 獎勵數量
- `strReason`: 追殺理由
- `dwBountyID`: 追殺令ID
- `dwHunterID`: 獵人角色ID
- `dwCancellerID`: 取消者角色ID

**返回值：**
- `TRUE`: 操作成功
- `FALSE`: 操作失敗

**使用示例：**
```cpp
// 創建一個普通追殺令
BOOL bResult = pBountyMgr->CreateBounty(
    12345,                      // 發布者ID
    67890,                      // 目標ID
    BOUNTY_TYPE_NORMAL,         // 普通類型
    BOUNTY_REWARD_MONEY,        // 金錢獎勵
    100000,                     // 10萬金幣
    "Constant PKing in newbie area"  // 理由
);

// 接受追殺令
bResult = pBountyMgr->AcceptBounty(1001, 11111);
```

#### 查詢接口

```cpp
// 獲取所有活躍的追殺令
std::vector<CBountyOrder*> GetActiveBounties();

// 獲取玩家發布的追殺令
std::vector<CBountyOrder*> GetPlayerBounties(DWORD dwPlayerID);

// 獲取針對玩家的追殺令
std::vector<CBountyOrder*> GetTargetBounties(DWORD dwTargetID);

// 根據ID獲取追殺令
CBountyOrder* GetBountyByID(DWORD dwBountyID);
```

**參數說明：**
- `dwPlayerID`: 玩家角色ID
- `dwTargetID`: 目標角色ID
- `dwBountyID`: 追殺令ID

**返回值：**
- 返回追殺令對象的向量或單個對象
- 如果沒有找到，返回空向量或NULL

**使用示例：**
```cpp
// 獲取所有活躍追殺令
auto vecBounties = pBountyMgr->GetActiveBounties();
for (auto pBounty : vecBounties)
{
    if (pBounty)
    {
        DWORD dwTargetID = pBounty->GetTargetID();
        LONGLONG llReward = pBounty->GetRewardAmount();
        // 處理追殺令信息...
    }
}
```

#### 排行榜接口

```cpp
// 獲取獵人排行榜
std::vector<std::pair<DWORD, int>> GetHunterRanking(int nTopCount = 10);

// 獲取目標排行榜
std::vector<std::pair<DWORD, int>> GetTargetRanking(int nTopCount = 10);
```

**參數說明：**
- `nTopCount`: 排行榜顯示數量，默認10個

**返回值：**
- 返回 (玩家ID, 數量) 的配對向量
- 按數量降序排列

**使用示例：**
```cpp
// 獲取前10名獵人
auto vecHunters = pBountyMgr->GetHunterRanking(10);
for (size_t i = 0; i < vecHunters.size(); ++i)
{
    DWORD dwPlayerID = vecHunters[i].first;
    int nKillCount = vecHunters[i].second;
    // 顯示排行信息...
}
```

#### 統計接口

```cpp
// 獲取玩家擊殺數
int GetPlayerKillCount(DWORD dwPlayerID);

// 獲取玩家死亡數
int GetPlayerDeathCount(DWORD dwPlayerID);

// 獲取玩家獲得的總獎勵
LONGLONG GetPlayerEarnedRewards(DWORD dwPlayerID);
```

**參數說明：**
- `dwPlayerID`: 玩家角色ID

**返回值：**
- 返回相應的統計數據

**使用示例：**
```cpp
int nKills = pBountyMgr->GetPlayerKillCount(12345);
int nDeaths = pBountyMgr->GetPlayerDeathCount(12345);
LONGLONG llEarned = pBountyMgr->GetPlayerEarnedRewards(12345);

float fKDRatio = (nDeaths > 0) ? (float)nKills / nDeaths : nKills;
```

#### 事件處理

```cpp
// 玩家擊殺事件處理
void OnPlayerKilled(DWORD dwKillerID, DWORD dwVictimID, EKILL_TYPE eKillType);

// 玩家登出事件處理
void OnPlayerLogout(DWORD dwPlayerID);
```

**參數說明：**
- `dwKillerID`: 擊殺者角色ID
- `dwVictimID`: 受害者角色ID
- `eKillType`: 擊殺類型 (與KillCard系統關聯)
- `dwPlayerID`: 玩家角色ID

**使用示例：**
```cpp
// 在玩家擊殺事件中調用
void GameLogic::OnPlayerKilled(DWORD dwKiller, DWORD dwVictim, EKILL_TYPE eType)
{
    // 觸發擊殺卡效果
    m_pKillCardMgr->TriggerKillCard(eType);
    
    // 檢查追殺令完成
    m_pBountyMgr->OnPlayerKilled(dwKiller, dwVictim, eType);
}
```

#### GM接口

```cpp
// GM創建追殺令
BOOL GMCreateBounty(DWORD dwTargetID, EBOUNTY_TYPE eType, LONGLONG llRewardAmount);

// GM取消追殺令
BOOL GMCancelBounty(DWORD dwBountyID);

// GM修改獎勵
BOOL GMModifyReward(DWORD dwBountyID, LONGLONG llNewAmount);
```

**參數說明：**
- `dwTargetID`: 目標角色ID
- `eType`: 追殺令類型
- `llRewardAmount`: 獎勵數量
- `dwBountyID`: 追殺令ID
- `llNewAmount`: 新的獎勵數量

**返回值：**
- `TRUE`: 操作成功
- `FALSE`: 操作失敗

**使用示例：**
```cpp
// GM創建特殊追殺令
BOOL bResult = pBountyMgr->GMCreateBounty(
    67890,                      // 目標ID
    BOUNTY_TYPE_EVENT,          // 活動類型
    1000000                     // 100萬金幣獎勵
);
```

## 追殺令對象 API

### CBountyOrder 類

表示單個追殺令的所有信息和操作。

#### 基本信息獲取

```cpp
// 獲取追殺令ID
DWORD GetBountyID() const;

// 獲取發布者ID
DWORD GetPublisherID() const;

// 獲取目標ID
DWORD GetTargetID() const;

// 獲取獵人ID
DWORD GetHunterID() const;

// 獲取追殺令類型
EBOUNTY_TYPE GetBountyType() const;

// 獲取狀態
EBOUNTY_STATUS GetStatus() const;

// 獲取獎勵類型
EBOUNTY_REWARD_TYPE GetRewardType() const;

// 獲取獎勵數量
LONGLONG GetRewardAmount() const;

// 獲取追殺理由
const std::string& GetReason() const;
```

**返回值：**
- 返回相應的屬性值

**使用示例：**
```cpp
CBountyOrder* pBounty = pBountyMgr->GetBountyByID(1001);
if (pBounty)
{
    DWORD dwTargetID = pBounty->GetTargetID();
    LONGLONG llReward = pBounty->GetRewardAmount();
    std::string strReason = pBounty->GetReason();
    
    // 顯示追殺令信息...
}
```

#### 時間信息

```cpp
// 獲取創建時間
__time64_t GetCreateTime() const;

// 獲取接受時間
__time64_t GetAcceptTime() const;

// 獲取完成時間
__time64_t GetCompleteTime() const;

// 獲取過期時間
__time64_t GetExpireTime() const;

// 獲取剩餘小時數
int GetRemainingHours() const;

// 獲取已經過小時數
int GetElapsedHours() const;
```

**返回值：**
- 時間戳或小時數

**使用示例：**
```cpp
int nRemainingHours = pBounty->GetRemainingHours();
if (nRemainingHours < 1)
{
    // 即將過期，顯示警告
}
```

#### 狀態操作

```cpp
// 接受追殺令
BOOL Accept(DWORD dwHunterID, const std::string& strHunterName);

// 完成追殺令
BOOL Complete();

// 取消追殺令
BOOL Cancel();

// 使追殺令過期
BOOL Expire();
```

**參數說明：**
- `dwHunterID`: 獵人角色ID
- `strHunterName`: 獵人名稱

**返回值：**
- `TRUE`: 操作成功
- `FALSE`: 操作失敗

#### 狀態檢查

```cpp
// 是否活躍
BOOL IsActive() const;

// 是否已接受
BOOL IsAccepted() const;

// 是否已完成
BOOL IsCompleted() const;

// 是否已取消
BOOL IsCancelled() const;

// 是否已過期
BOOL IsExpired() const;

// 是否即將過期
BOOL IsExpiring() const;

// 是否應該過期
BOOL ShouldExpire() const;
```

**返回值：**
- `TRUE`/`FALSE`

**使用示例：**
```cpp
if (pBounty->IsActive() && !pBounty->IsExpiring())
{
    // 可以接受的追殺令
}
```

#### 驗證方法

```cpp
// 是否是有效目標
BOOL IsValidTarget(DWORD dwPlayerID) const;

// 是否可以被指定玩家接受
BOOL CanBeAcceptedBy(DWORD dwPlayerID) const;

// 是否可以被指定玩家取消
BOOL CanBeCancelledBy(DWORD dwPlayerID) const;
```

**參數說明：**
- `dwPlayerID`: 玩家角色ID

**返回值：**
- `TRUE`: 滿足條件
- `FALSE`: 不滿足條件

#### 格式化方法

```cpp
// 獲取狀態字符串
std::string GetStatusString() const;

// 獲取類型字符串
std::string GetTypeString() const;

// 獲取獎勵字符串
std::string GetRewardString() const;

// 獲取時間字符串
std::string GetTimeString() const;

// 獲取格式化描述
std::string GetFormattedDescription() const;
```

**返回值：**
- 格式化的字符串

**使用示例：**
```cpp
std::string strDisplay = pBounty->GetFormattedDescription();
// 可能返回: "獵殺 PlayerName (等級50戰士) - 獎勵: 100,000金幣 - 剩餘: 12小時"
```

## 數據庫操作 API

### CBountyDatabase 類

負責所有與追殺令相關的數據庫操作。

#### 初始化和清理

```cpp
// 初始化數據庫連接
BOOL Initialize(db::IDbManager* pDbManager);

// 關閉數據庫連接
void Shutdown();
```

#### 基本操作

```cpp
// 創建追殺令
BOOL CreateBounty(const CBountyOrder::BountyData& data);

// 更新追殺令
BOOL UpdateBounty(const CBountyOrder::BountyData& data);

// 刪除追殺令
BOOL DeleteBounty(DWORD dwBountyID);
```

#### 查詢操作

```cpp
// 根據ID獲取追殺令
CBountyOrder* GetBountyByID(DWORD dwBountyID);

// 獲取活躍追殺令
std::vector<CBountyOrder*> GetActiveBounties();

// 搜索追殺令
std::vector<CBountyOrder*> SearchBounties(const BountySearchCriteria& criteria);
```

#### 統計操作

```cpp
// 獲取全局統計
BountyStatistics GetGlobalStatistics();

// 獲取玩家統計
BountyStatistics GetPlayerStatistics(DWORD dwPlayerID);

// 獲取排行榜
std::vector<std::pair<DWORD, int>> GetHunterRanking(int nTopCount);
```

## 網絡處理 API

### CBountyNetworkHandler 類

處理追殺令系統的網絡通信。

#### 初始化

```cpp
// 初始化網絡處理器
BOOL Initialize();

// 關閉網絡處理器
void Shutdown();
```

#### 消息發送

```cpp
// 發送創建追殺令請求
BOOL SendCreateBountyRequest(DWORD dwTargetID, EBOUNTY_TYPE eType, 
                            EBOUNTY_REWARD_TYPE eRewardType, LONGLONG llAmount, 
                            const std::string& strReason);

// 發送接受追殺令請求
BOOL SendAcceptBountyRequest(DWORD dwBountyID);

// 發送追殺令列表請求
BOOL SendBountyListRequest();
```

#### 消息處理

```cpp
// 處理網絡消息
BOOL ProcessMessage(NET_MSG_GENERIC* pMsg);
```

## 配置管理 API

### CBountyConfig 類

管理追殺令系統的配置參數。

#### 初始化

```cpp
// 初始化配置
BOOL Initialize(const std::string& strConfigFile);

// 重新加載配置
BOOL ReloadConfig();
```

#### 配置獲取

```cpp
// 獲取通用配置
const GeneralConfig& GetGeneralConfig() const;

// 獲取獎勵配置
const RewardConfig& GetRewardConfig() const;

// 獲取UI配置
const UIConfig& GetUIConfig() const;

// 獲取安全配置
const SecurityConfig& GetSecurityConfig() const;
```

#### 驗證方法

```cpp
// 驗證追殺令請求
BOOL ValidateBountyRequest(DWORD dwPublisherID, DWORD dwTargetID, LONGLONG llReward);

// 驗證玩家等級
BOOL ValidatePlayerLevel(int nLevel, BOOL bAsTarget = FALSE);

// 驗證獎勵數量
BOOL ValidateRewardAmount(LONGLONG llAmount);
```

#### 計算方法

```cpp
// 計算系統手續費
LONGLONG CalculateSystemFee(LONGLONG llRewardAmount);

// 計算過期時間
int CalculateExpireTime(EBOUNTY_TYPE eType);

// 計算成功獎勵加成
float CalculateSuccessBonus(int nConsecutiveKills);
```

## GM管理 API

### CBountyGMInterface 類

提供GM管理界面功能。

#### 界面操作

```cpp
// 打開GM界面
void Open();

// 關閉GM界面
void Close();

// 刷新數據
void RefreshData();

// 顯示統計信息
void ShowStatistics();
```

#### GM操作

```cpp
// GM創建追殺令
BOOL GMCreateBounty();

// GM取消追殺令
BOOL GMCancelBounty();

// GM修改獎勵
BOOL GMModifyReward();

// GM強制完成
BOOL GMForceComplete();

// GM封禁玩家
BOOL GMBanPlayer();
```

### CBountyGMCommands 類

處理GM命令。

#### 命令處理

```cpp
// 處理GM命令
BOOL ProcessCommand(DWORD dwGMCharID, const std::string& strCommand, 
                   const std::vector<std::string>& vecArgs);
```

#### 具體命令

```cpp
// 創建追殺令命令
BOOL CMD_CreateBounty(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);

// 取消追殺令命令
BOOL CMD_CancelBounty(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);

// 查看統計命令
BOOL CMD_Statistics(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);

// 幫助命令
BOOL CMD_Help(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
```

## 錯誤碼定義

```cpp
enum BOUNTY_ERROR_CODE
{
    BOUNTY_ERROR_SUCCESS = 0,           // 成功
    BOUNTY_ERROR_INVALID_PARAMETER,     // 無效參數
    BOUNTY_ERROR_INSUFFICIENT_LEVEL,    // 等級不足
    BOUNTY_ERROR_INSUFFICIENT_MONEY,    // 金錢不足
    BOUNTY_ERROR_TARGET_NOT_FOUND,      // 目標不存在
    BOUNTY_ERROR_BOUNTY_NOT_FOUND,      // 追殺令不存在
    BOUNTY_ERROR_ALREADY_ACCEPTED,      // 已被接受
    BOUNTY_ERROR_ALREADY_EXPIRED,       // 已過期
    BOUNTY_ERROR_PERMISSION_DENIED,     // 權限不足
    BOUNTY_ERROR_SYSTEM_DISABLED,       // 系統已關閉
    BOUNTY_ERROR_DATABASE_ERROR,        // 數據庫錯誤
    BOUNTY_ERROR_NETWORK_ERROR,         // 網絡錯誤
    BOUNTY_ERROR_GUILD_PROTECTION,      // 公會保護
    BOUNTY_ERROR_ANTI_SPAM,             // 防刷新限制
    BOUNTY_ERROR_LEVEL_DIFFERENCE,      // 等級差距過大
    BOUNTY_ERROR_MAX_BOUNTIES_REACHED,  // 達到最大追殺令數
    BOUNTY_ERROR_UNKNOWN = 999          // 未知錯誤
};
```

## 事件回調

```cpp
// 追殺令狀態變化回調
typedef std::function<void(DWORD dwBountyID, EBOUNTY_STATUS eOldStatus, EBOUNTY_STATUS eNewStatus)> BountyStatusChangeCallback;

// 追殺令創建回調
typedef std::function<void(DWORD dwBountyID, DWORD dwPublisherID, DWORD dwTargetID)> BountyCreatedCallback;

// 追殺令完成回調
typedef std::function<void(DWORD dwBountyID, DWORD dwHunterID, LONGLONG llReward)> BountyCompletedCallback;

// 註冊回調
void RegisterBountyStatusChangeCallback(BountyStatusChangeCallback callback);
void RegisterBountyCreatedCallback(BountyCreatedCallback callback);
void RegisterBountyCompletedCallback(BountyCompletedCallback callback);
```

## 使用指南

### 1. 基本集成步驟

1. 包含必要的頭文件
2. 創建並初始化 CBountyManager
3. 在適當的位置調用事件處理方法
4. 創建UI組件並註冊到界面系統
5. 處理網絡消息
6. 配置數據庫表結構

### 2. 常見使用場景

#### 場景1：玩家創建追殺令
```cpp
BOOL CreatePlayerBounty(DWORD dwPlayerID, DWORD dwTargetID, LONGLONG llReward, const std::string& strReason)
{
    CBountyManager* pBountyMgr = GetBountyManager();
    if (!pBountyMgr)
        return FALSE;
        
    return pBountyMgr->CreateBounty(
        dwPlayerID,
        dwTargetID,
        BOUNTY_TYPE_NORMAL,
        BOUNTY_REWARD_MONEY,
        llReward,
        strReason
    );
}
```

#### 場景2：顯示追殺令列表
```cpp
void DisplayBountyList()
{
    CBountyManager* pBountyMgr = GetBountyManager();
    if (!pBountyMgr)
        return;
        
    auto vecBounties = pBountyMgr->GetActiveBounties();
    for (auto pBounty : vecBounties)
    {
        if (pBounty && pBounty->IsActive())
        {
            std::string strDisplay = pBounty->GetFormattedDescription();
            // 顯示到UI列表中...
        }
    }
}
```

#### 場景3：處理玩家擊殺
```cpp
void OnPlayerKilled(DWORD dwKiller, DWORD dwVictim, EKILL_TYPE eKillType)
{
    // 1. 觸發擊殺卡效果
    if (m_pKillCardManager)
    {
        m_pKillCardManager->TriggerKillCard(eKillType);
    }
    
    // 2. 檢查追殺令完成
    CBountyManager* pBountyMgr = GetBountyManager();
    if (pBountyMgr)
    {
        pBountyMgr->OnPlayerKilled(dwKiller, dwVictim, eKillType);
    }
}
```

### 3. 注意事項

1. **線程安全**: 所有API調用都應在主線程中進行
2. **錯誤處理**: 始終檢查返回值並處理錯誤情況
3. **內存管理**: 正確管理追殺令對象的生命週期
4. **性能考慮**: 避免頻繁的數據庫查詢，使用緩存機制
5. **安全性**: 驗證所有用戶輸入，防止惡意操作

### 4. 調試技巧

1. 啟用詳細日誌記錄
2. 使用GM命令進行測試
3. 監控數據庫性能
4. 檢查網絡消息流
5. 驗證配置參數

這個API參考文檔提供了追殺令系統的完整接口說明，開發者可以根據這些API實現自己的追殺令功能。