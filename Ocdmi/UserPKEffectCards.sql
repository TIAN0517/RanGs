-- PK Effect Cards Database Schema
-- 玩家PK卡片表

-- 检查表是否已存在，如果存在则删除
IF EXISTS (SELECT * FROM sysobjects WHERE id = object_id(N'[dbo].[UserPKEffectCards]') AND OBJECTPROPERTY(id, N'IsUserTable') = 1)
DROP TABLE [dbo].[UserPKEffectCards]
GO

-- 创建UserPKEffectCards表
CREATE TABLE [dbo].[UserPKEffectCards] (
    [UserNum] [int] NOT NULL,                    -- 用户编号
    [CardType] [int] NOT NULL,                   -- 卡片类型 (EMPK_EFFECT_CARD_TYPE)
    [StartTime] [datetime] NOT NULL,             -- 开始时间
    [Duration] [int] NOT NULL,                   -- 持续天数
    [IsActive] [bit] NOT NULL DEFAULT(0),        -- 是否激活
    [CreateDate] [datetime] NOT NULL DEFAULT(GETDATE()), -- 创建时间
    CONSTRAINT [PK_UserPKEffectCards] PRIMARY KEY CLUSTERED 
    (
        [UserNum] ASC,
        [CardType] ASC
    )
) ON [PRIMARY]
GO

-- 添加索引以提高查询性能
CREATE NONCLUSTERED INDEX [IX_UserPKEffectCards_UserNum_Active] ON [dbo].[UserPKEffectCards] 
(
    [UserNum] ASC,
    [IsActive] ASC
)
INCLUDE ([CardType], [StartTime], [Duration]) ON [PRIMARY]
GO

-- 添加检查约束
ALTER TABLE [dbo].[UserPKEffectCards] ADD CONSTRAINT [CK_UserPKEffectCards_CardType] 
CHECK ([CardType] >= 0 AND [CardType] <= 5)
GO

ALTER TABLE [dbo].[UserPKEffectCards] ADD CONSTRAINT [CK_UserPKEffectCards_Duration] 
CHECK ([Duration] > 0)
GO

-- 创建存储过程：获取用户激活的PK卡片
CREATE PROCEDURE [dbo].[sp_GetUserActivePKCard]
    @UserNum INT
AS
BEGIN
    SET NOCOUNT ON;
    
    SELECT TOP 1 
        CardType,
        StartTime,
        Duration,
        IsActive,
        DATEDIFF(second, GETDATE(), DATEADD(day, Duration, StartTime)) AS RemainingSeconds
    FROM UserPKEffectCards 
    WHERE UserNum = @UserNum 
      AND IsActive = 1 
      AND DATEADD(day, Duration, StartTime) > GETDATE()
    ORDER BY StartTime DESC
END
GO

-- 创建存储过程：激活PK卡片
CREATE PROCEDURE [dbo].[sp_ActivatePKCard]
    @UserNum INT,
    @CardType INT,
    @Duration INT
AS
BEGIN
    SET NOCOUNT ON;
    
    BEGIN TRANSACTION
    
    -- 先停用所有其他卡片
    UPDATE UserPKEffectCards 
    SET IsActive = 0 
    WHERE UserNum = @UserNum AND IsActive = 1
    
    -- 激活新卡片
    IF EXISTS (SELECT 1 FROM UserPKEffectCards WHERE UserNum = @UserNum AND CardType = @CardType)
    BEGIN
        UPDATE UserPKEffectCards 
        SET StartTime = GETDATE(),
            Duration = @Duration,
            IsActive = 1
        WHERE UserNum = @UserNum AND CardType = @CardType
    END
    ELSE
    BEGIN
        INSERT INTO UserPKEffectCards (UserNum, CardType, StartTime, Duration, IsActive)
        VALUES (@UserNum, @CardType, GETDATE(), @Duration, 1)
    END
    
    COMMIT TRANSACTION
END
GO

-- 创建存储过程：检查并处理过期卡片
CREATE PROCEDURE [dbo].[sp_ProcessExpiredPKCards]
AS
BEGIN
    SET NOCOUNT ON;
    
    UPDATE UserPKEffectCards 
    SET IsActive = 0
    WHERE IsActive = 1 
      AND DATEADD(day, Duration, StartTime) <= GETDATE()
      
    SELECT @@ROWCOUNT AS ExpiredCount
END
GO

-- 插入测试数据 (可选)
/*
INSERT INTO UserPKEffectCards (UserNum, CardType, StartTime, Duration, IsActive)
VALUES 
(1, 0, GETDATE(), 7, 1),   -- 全息蓝光卡 7天
(2, 1, GETDATE(), 15, 1),  -- 霓虹紫光卡 15天
(3, 2, GETDATE(), 30, 1)   -- 烈焰金光卡 30天
*/