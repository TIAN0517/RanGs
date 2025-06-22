-- 追殺令系統數據庫表結構
-- 適用於 RanGs 遊戲引擎

-- 1. 追殺令主表
CREATE TABLE BountyOrders
(
    BountyID            INT IDENTITY(1,1) PRIMARY KEY,
    PublisherID         DWORD NOT NULL,                 -- 發布者角色ID
    TargetID            DWORD NOT NULL,                 -- 目標角色ID  
    HunterID            DWORD DEFAULT 0,                -- 接受者角色ID (0表示未接受)
    BountyType          TINYINT NOT NULL DEFAULT 0,     -- 追殺令類型
    Status              TINYINT NOT NULL DEFAULT 0,     -- 狀態 (0:活躍, 1:已接受, 2:已完成, 3:已取消, 4:已過期)
    RewardType          TINYINT NOT NULL DEFAULT 0,     -- 獎勵類型
    RewardAmount        BIGINT NOT NULL DEFAULT 0,      -- 獎勵數量
    Reason              NVARCHAR(255) DEFAULT '',       -- 追殺理由
    CreateTime          DATETIME NOT NULL DEFAULT GETDATE(),
    AcceptTime          DATETIME NULL,
    CompleteTime        DATETIME NULL,
    ExpireTime          DATETIME NOT NULL,
    PublisherName       NVARCHAR(33) DEFAULT '',        -- 發布者名稱
    TargetName          NVARCHAR(33) DEFAULT '',        -- 目標名稱
    HunterName          NVARCHAR(33) DEFAULT '',        -- 獵人名稱
    ServerID            INT NOT NULL DEFAULT 0,         -- 服務器ID
    CreatedBy           NVARCHAR(50) DEFAULT 'SYSTEM',  -- 創建來源
    LastModified        DATETIME DEFAULT GETDATE(),
    
    -- 約束
    CONSTRAINT CHK_BountyOrders_PublisherTarget CHECK (PublisherID != TargetID),
    CONSTRAINT CHK_BountyOrders_RewardAmount CHECK (RewardAmount >= 0),
    CONSTRAINT CHK_BountyOrders_Status CHECK (Status BETWEEN 0 AND 4),
    CONSTRAINT CHK_BountyOrders_BountyType CHECK (BountyType BETWEEN 0 AND 3),
    CONSTRAINT CHK_BountyOrders_RewardType CHECK (RewardType BETWEEN 0 AND 3)
);

-- 2. 追殺記錄表 (歷史記錄)
CREATE TABLE BountyRecords
(
    RecordID            INT IDENTITY(1,1) PRIMARY KEY,
    BountyID            INT NOT NULL,
    PublisherID         DWORD NOT NULL,
    TargetID            DWORD NOT NULL,
    HunterID            DWORD,
    BountyType          TINYINT NOT NULL,
    FinalStatus         TINYINT NOT NULL,               -- 最終狀態
    RewardType          TINYINT NOT NULL,
    RewardAmount        BIGINT NOT NULL,
    ActualReward        BIGINT DEFAULT 0,               -- 實際發放獎勵
    KillType            TINYINT DEFAULT 0,              -- 擊殺類型 (與KillCard系統關聯)
    KillLocation        NVARCHAR(100) DEFAULT '',      -- 擊殺地點
    Duration            INT DEFAULT 0,                  -- 持續時間(秒)
    CreateTime          DATETIME NOT NULL,
    CompleteTime        DATETIME NULL,
    ServerID            INT NOT NULL DEFAULT 0,
    
    -- 外鍵
    FOREIGN KEY (BountyID) REFERENCES BountyOrders(BountyID)
);

-- 3. 獎勵配置表
CREATE TABLE BountyRewards
(
    RewardID            INT IDENTITY(1,1) PRIMARY KEY,
    RewardType          TINYINT NOT NULL,               -- 獎勵類型
    ItemID              INT DEFAULT 0,                  -- 道具ID (如果是道具獎勵)
    ItemCount           INT DEFAULT 1,                  -- 道具數量
    ExpMultiplier       FLOAT DEFAULT 1.0,              -- 經驗倍率
    MoneyMultiplier     FLOAT DEFAULT 1.0,              -- 金錢倍率
    MinLevel            INT DEFAULT 1,                  -- 最低等級要求
    MaxLevel            INT DEFAULT 999,                -- 最高等級限制
    IsEnabled           BIT DEFAULT 1,                  -- 是否啟用
    Description         NVARCHAR(255) DEFAULT '',      -- 描述
    CreatedTime         DATETIME DEFAULT GETDATE(),
    
    CONSTRAINT CHK_BountyRewards_RewardType CHECK (RewardType BETWEEN 0 AND 3),
    CONSTRAINT CHK_BountyRewards_Level CHECK (MinLevel <= MaxLevel)
);

-- 4. 排行榜數據表
CREATE TABLE BountyRankings
(
    RankingID           INT IDENTITY(1,1) PRIMARY KEY,
    PlayerID            DWORD NOT NULL,
    PlayerName          NVARCHAR(33) NOT NULL,
    RankingType         TINYINT NOT NULL,               -- 排行類型 (0:獵人, 1:目標, 2:獎勵)
    KillCount           INT DEFAULT 0,                  -- 擊殺數量
    DeathCount          INT DEFAULT 0,                  -- 死亡數量
    BountiesPublished   INT DEFAULT 0,                  -- 發布追殺令數
    BountiesAccepted    INT DEFAULT 0,                  -- 接受追殺令數
    BountiesCompleted   INT DEFAULT 0,                  -- 完成追殺令數
    TotalRewards        BIGINT DEFAULT 0,               -- 總獲得獎勵
    TotalPaid           BIGINT DEFAULT 0,               -- 總支付金額
    SuccessRate         FLOAT DEFAULT 0.0,              -- 成功率
    LastKillTime        DATETIME NULL,                  -- 最後擊殺時間
    LastUpdateTime      DATETIME DEFAULT GETDATE(),    -- 最後更新時間
    ServerID            INT NOT NULL DEFAULT 0,
    
    CONSTRAINT UQ_BountyRankings_Player UNIQUE (PlayerID, RankingType, ServerID),
    CONSTRAINT CHK_BountyRankings_RankingType CHECK (RankingType BETWEEN 0 AND 2),
    CONSTRAINT CHK_BountyRankings_SuccessRate CHECK (SuccessRate BETWEEN 0.0 AND 1.0)
);

-- 5. GM操作日誌表
CREATE TABLE BountyGMLogs
(
    LogID               INT IDENTITY(1,1) PRIMARY KEY,
    GMCharID            DWORD NOT NULL,                 -- GM角色ID
    GMCharName          NVARCHAR(33) NOT NULL,          -- GM角色名
    Action              NVARCHAR(50) NOT NULL,          -- 操作類型
    BountyID            INT NULL,                       -- 相關追殺令ID
    TargetPlayerID      DWORD NULL,                     -- 目標玩家ID
    TargetPlayerName    NVARCHAR(33) DEFAULT '',        -- 目標玩家名
    OldValue            NVARCHAR(500) DEFAULT '',       -- 修改前值
    NewValue            NVARCHAR(500) DEFAULT '',       -- 修改後值
    Reason              NVARCHAR(255) DEFAULT '',       -- 操作理由
    ClientIP            NVARCHAR(15) DEFAULT '',        -- 客戶端IP
    ServerID            INT NOT NULL DEFAULT 0,
    ActionTime          DATETIME DEFAULT GETDATE(),
    
    FOREIGN KEY (BountyID) REFERENCES BountyOrders(BountyID)
);

-- 6. 系統配置表
CREATE TABLE BountyConfig
(
    ConfigID            INT IDENTITY(1,1) PRIMARY KEY,
    ConfigKey           NVARCHAR(50) NOT NULL UNIQUE,
    ConfigValue         NVARCHAR(255) NOT NULL,
    Description         NVARCHAR(255) DEFAULT '',
    ConfigType          TINYINT DEFAULT 0,              -- 配置類型 (0:字符串, 1:整數, 2:浮點數, 3:布爾值)
    IsEditable          BIT DEFAULT 1,                  -- 是否可編輯
    LastModified        DATETIME DEFAULT GETDATE(),
    ModifiedBy          NVARCHAR(33) DEFAULT 'SYSTEM'
);

-- 7. 防刷新控制表
CREATE TABLE BountyAntiSpam
(
    SpamID              INT IDENTITY(1,1) PRIMARY KEY,
    PlayerID            DWORD NOT NULL,
    ActionType          TINYINT NOT NULL,               -- 操作類型 (0:創建, 1:接受, 2:取消)
    ActionCount         INT DEFAULT 1,                  -- 操作次數
    LastActionTime      DATETIME DEFAULT GETDATE(),    -- 最後操作時間
    CooldownEndTime     DATETIME NULL,                  -- 冷卻結束時間
    IsBlocked           BIT DEFAULT 0,                  -- 是否被阻止
    ServerID            INT NOT NULL DEFAULT 0,
    
    CONSTRAINT UQ_BountyAntiSpam_Player UNIQUE (PlayerID, ActionType, ServerID),
    CONSTRAINT CHK_BountyAntiSpam_ActionType CHECK (ActionType BETWEEN 0 AND 2)
);

-- 創建索引以提高查詢性能
CREATE INDEX IX_BountyOrders_Status ON BountyOrders(Status);
CREATE INDEX IX_BountyOrders_PublisherID ON BountyOrders(PublisherID);
CREATE INDEX IX_BountyOrders_TargetID ON BountyOrders(TargetID);
CREATE INDEX IX_BountyOrders_HunterID ON BountyOrders(HunterID);
CREATE INDEX IX_BountyOrders_CreateTime ON BountyOrders(CreateTime);
CREATE INDEX IX_BountyOrders_ExpireTime ON BountyOrders(ExpireTime);
CREATE INDEX IX_BountyOrders_ServerID ON BountyOrders(ServerID);

CREATE INDEX IX_BountyRecords_BountyID ON BountyRecords(BountyID);
CREATE INDEX IX_BountyRecords_PublisherID ON BountyRecords(PublisherID);
CREATE INDEX IX_BountyRecords_TargetID ON BountyRecords(TargetID);
CREATE INDEX IX_BountyRecords_HunterID ON BountyRecords(HunterID);
CREATE INDEX IX_BountyRecords_CreateTime ON BountyRecords(CreateTime);
CREATE INDEX IX_BountyRecords_ServerID ON BountyRecords(ServerID);

CREATE INDEX IX_BountyRankings_PlayerID ON BountyRankings(PlayerID);
CREATE INDEX IX_BountyRankings_RankingType ON BountyRankings(RankingType);
CREATE INDEX IX_BountyRankings_KillCount ON BountyRankings(KillCount DESC);
CREATE INDEX IX_BountyRankings_TotalRewards ON BountyRankings(TotalRewards DESC);
CREATE INDEX IX_BountyRankings_ServerID ON BountyRankings(ServerID);

CREATE INDEX IX_BountyGMLogs_GMCharID ON BountyGMLogs(GMCharID);
CREATE INDEX IX_BountyGMLogs_Action ON BountyGMLogs(Action);
CREATE INDEX IX_BountyGMLogs_ActionTime ON BountyGMLogs(ActionTime);
CREATE INDEX IX_BountyGMLogs_BountyID ON BountyGMLogs(BountyID);

CREATE INDEX IX_BountyAntiSpam_PlayerID ON BountyAntiSpam(PlayerID);
CREATE INDEX IX_BountyAntiSpam_LastActionTime ON BountyAntiSpam(LastActionTime);

-- 插入默認配置
INSERT INTO BountyConfig (ConfigKey, ConfigValue, Description, ConfigType) VALUES
('BountySystemEnabled', '1', '追殺令系統是否啟用', 3),
('MaxActiveBounties', '1000', '最大活躍追殺令數量', 1),
('MaxPlayerBounties', '5', '每個玩家最大發布數量', 1),
('MaxAcceptedBounties', '3', '每個玩家最大接受數量', 1),
('DefaultExpireHours', '24', '默認過期時間(小時)', 1),
('MinPlayerLevel', '10', '最低玩家等級', 1),
('MinRewardAmount', '10000', '最小獎勵金額', 1),
('MaxRewardAmount', '100000000', '最大獎勵金額', 1),
('SystemFeeRate', '0.1', '系統手續費率', 2),
('UpdateInterval', '5.0', '更新間隔(秒)', 2),
('AntiSpamEnabled', '1', '防刷新是否啟用', 3),
('SpamTimeLimit', '60', '刷新時間限制(秒)', 1),
('MaxSpamAttempts', '3', '最大刷新嘗試次數', 1),
('GuildMemberProtection', '1', '公會成員保護', 3),
('MinTargetLevel', '10', '最低目標等級', 1),
('MaxLevelDifference', '20', '最大等級差', 1);

-- 插入默認獎勵配置
INSERT INTO BountyRewards (RewardType, ItemID, ItemCount, ExpMultiplier, MoneyMultiplier, Description) VALUES
(0, 0, 0, 1.0, 1.0, '金錢獎勵'),
(1, 100001, 1, 1.0, 1.0, '特殊道具獎勵'),
(2, 0, 0, 1.5, 1.0, '經驗獎勵'),
(3, 100001, 1, 1.2, 0.8, '混合獎勵');