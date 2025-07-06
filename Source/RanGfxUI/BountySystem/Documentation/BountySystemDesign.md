# 追殺令系統設計文檔

## 系統概述

追殺令系統是基於 RanGs 遊戲引擎框架開發的玩家互動功能，允許玩家對其他玩家發布懸賞追殺令，其他玩家可以接受並完成這些任務以獲得獎勵。

## 核心功能

### 1. 懸賞發布系統
- 玩家可以對其他玩家發布追殺令
- 支持多種獎勵類型：金錢、道具、經驗值、混合獎勵
- 可設置追殺理由和過期時間
- 自動扣除發布者的獎勵金額作為保證金

### 2. 接單機制
- 追殺者可以瀏覽所有活躍的追殺令
- 支持按獎勵金額、目標等級、發布時間等條件篩選
- 每個玩家同時最多接受3個追殺令
- 接受後追殺令狀態變為"已接受"

### 3. 任務流程管理
- 完整的狀態跟蹤：活躍 → 已接受 → 已完成/已取消/已過期
- 自動檢測目標擊殺事件
- 支持手動取消和GM強制完成
- 過期自動處理機制

### 4. 自動獎勵分發
- 任務完成後自動計算並分發獎勵
- 支持系統手續費扣除
- 連續擊殺獎勵加成
- 經驗值倍率調整

### 5. 歷史查詢
- 完整的追殺記錄保存
- 支持按時間、玩家、狀態等條件查詢
- 詳細的統計信息展示
- 數據導出功能

### 6. 排行榜系統
- 獵人排行榜（按擊殺數排序）
- 目標排行榜（按被追殺次數排序）
- 獎勵排行榜（按獲得獎勵總額排序）
- 實時更新和週期性重置

### 7. GM管理介面
- 創建/取消/修改追殺令
- 玩家追殺數據查看
- 系統配置動態調整
- 操作日誌記錄和審計

## 技術架構

### 1. 核心組件

#### CBountyManager
- 系統的主要管理器類
- 負責所有追殺令的生命週期管理
- 提供公共API接口
- 處理事件響應和狀態更新

#### CBountyOrder
- 追殺令實體類
- 封裝追殺令的所有屬性和行為
- 提供狀態轉換方法
- 支持序列化和反序列化

#### CBountyDatabase
- 數據庫操作封裝類
- 實現CRUD操作
- 提供查詢和統計接口
- 支持事務操作

#### CBountyConfig
- 配置管理類
- 支持XML配置文件
- 動態配置更新
- 參數驗證功能

### 2. UI組件

#### CBountyUI
- 主要用戶界面
- 追殺令列表顯示
- 創建和接受操作
- 篩選和搜索功能

#### CBountyListUI
- 追殺令列表控件
- 支持分頁和排序
- 詳情預覽功能

#### CBountyDetailUI
- 詳情查看界面
- 完整信息展示
- 操作按鈕集成

#### CBountyRankingUI
- 排行榜界面
- 多種排行類型
- 實時數據更新

### 3. 網絡層

#### CBountyNetworkHandler
- 網絡消息處理
- 客戶端-服務器通信
- 實時通知機制
- 錯誤處理和重試

### 4. 數據庫設計

#### 主要數據表
- `BountyOrders`: 追殺令主表
- `BountyRecords`: 歷史記錄表
- `BountyRankings`: 排行榜數據表
- `BountyRewards`: 獎勵配置表
- `BountyGMLogs`: GM操作日誌表
- `BountyConfig`: 系統配置表
- `BountyAntiSpam`: 防刷新控制表

#### 索引優化
- 按狀態、玩家ID、時間等建立索引
- 支持高併發查詢
- 定期清理歷史數據

## 安全機制

### 1. 防刷新控制
- 時間間隔限制
- 操作次數限制
- IP地址監控
- 異常行為檢測

### 2. 權限驗證
- 玩家等級限制
- 公會成員保護
- GM權限分級
- 操作日誌記錄

### 3. 數據驗證
- 輸入參數檢查
- 業務邏輯驗證
- 數據完整性約束
- 異常情況處理

## 性能優化

### 1. 緩存機制
- 活躍追殺令內存緩存
- 玩家統計數據緩存
- 配置參數緩存
- 查詢結果緩存

### 2. 數據庫優化
- 索引優化
- 查詢語句優化
- 分頁查詢
- 定期清理

### 3. 併發處理
- 多線程支持
- 異步操作
- 鎖機制優化
- 批量處理

## 集成說明

### 1. 與現有系統集成

#### InnerInterface集成
```cpp
// 在InnerInterface.h中添加
IBountyUI* GetBountyUI() { return m_pBountyUI; }

// 在InnerInterface.cpp中初始化
m_pBountyUI = new CBountyUI(m_pEngineDevice);
m_pBountyUI->CreateSubControl();
```

#### 與KillCard系統聯動
```cpp
// 在擊殺事件中調用
void OnPlayerKilled(DWORD dwKiller, DWORD dwVictim, EKILL_TYPE eKillType)
{
    // 觸發擊殺卡效果
    m_pKillCardManager->TriggerKillCard(eKillType);
    
    // 檢查追殺令完成
    m_pBountyManager->OnPlayerKilled(dwKiller, dwVictim, eKillType);
}
```

### 2. 數據庫集成
```cpp
// 在DbManager中添加追殺令操作
void AddBountyJob(DbActionPtr spJob, IDbManager* pSubDbMan = NULL);
```

### 3. 網絡消息集成
```cpp
// 在消息處理中添加追殺令消息
case NET_MSG_BOUNTY_CREATE_REQ:
    m_pBountyNetworkHandler->ProcessMessage(pMsg);
    break;
```

## 配置文件示例

```xml
<?xml version="1.0" encoding="utf-8"?>
<BountySystemConfig>
    <General>
        <Enabled>true</Enabled>
        <MaxActiveBounties>1000</MaxActiveBounties>
        <MaxPlayerBounties>5</MaxPlayerBounties>
        <DefaultExpireHours>24</DefaultExpireHours>
        <MinPlayerLevel>10</MinPlayerLevel>
        <MinRewardAmount>10000</MinRewardAmount>
        <MaxRewardAmount>100000000</MaxRewardAmount>
        <SystemFeeRate>0.1</SystemFeeRate>
    </General>
    <Security>
        <AntiSpamEnabled>true</AntiSpamEnabled>
        <SpamTimeLimit>60</SpamTimeLimit>
        <GuildMemberProtection>true</GuildMemberProtection>
        <MaxLevelDifference>20</MaxLevelDifference>
    </Security>
    <UI>
        <ShowNotifications>true</ShowNotifications>
        <PlaySoundEffects>true</PlaySoundEffects>
        <NotificationDuration>5</NotificationDuration>
    </UI>
</BountySystemConfig>
```

## 使用示例

### 1. 創建追殺令
```cpp
CBountyManager* pBountyMgr = GetBountyManager();
BOOL bResult = pBountyMgr->CreateBounty(
    12345,                      // 發布者ID
    67890,                      // 目標ID
    BOUNTY_TYPE_NORMAL,         // 普通追殺令
    BOUNTY_REWARD_MONEY,        // 金錢獎勵
    100000,                     // 10萬金幣
    "PKer needs to be stopped"  // 追殺理由
);
```

### 2. 接受追殺令
```cpp
BOOL bResult = pBountyMgr->AcceptBounty(
    1001,   // 追殺令ID
    11111   // 獵人ID
);
```

### 3. 查詢排行榜
```cpp
auto vecHunterRanking = pBountyMgr->GetHunterRanking(10); // 前10名獵人
auto vecTargetRanking = pBountyMgr->GetTargetRanking(10); // 前10名目標
```

## 測試覆蓋

### 1. 單元測試
- 核心類功能測試
- 邊界條件測試
- 異常情況測試
- 性能基準測試

### 2. 集成測試
- 系統間交互測試
- 數據庫操作測試
- 網絡通信測試
- UI界面測試

### 3. 壓力測試
- 高並發創建測試
- 大量數據查詢測試
- 長時間運行測試
- 內存洩漏檢測

## 部署注意事項

### 1. 數據庫準備
- 執行數據表創建腳本
- 設置適當的權限
- 配置備份策略
- 建立監控機制

### 2. 配置文件
- 根據服務器環境調整參數
- 設置合適的安全限制
- 配置日誌級別
- 啟用性能監控

### 3. 權限設置
- GM權限分級配置
- 功能開關設置
- 安全策略配置
- 審計要求設置

這個追殺令系統將為 RanGs 遊戲增加豐富的玩家互動功能，提升遊戲的社交性和競技性，同時保證系統的穩定性和安全性。