-- Kill Animation System Database Script
-- Author: TIAN0517 - Jy技術團隊

-- Create table for kill animation configurations
CREATE TABLE KillAnimation (
    AnimationID INT PRIMARY KEY,
    AnimationName NVARCHAR(100) NOT NULL,
    TexturePath NVARCHAR(255),
    SoundPath NVARCHAR(255),
    Duration FLOAT DEFAULT 2.5,
    ColorR INT DEFAULT 255,
    ColorG INT DEFAULT 255,
    ColorB INT DEFAULT 255,
    Scale FLOAT DEFAULT 1.0,
    UseParticle BIT DEFAULT 1,
    Created DATETIME DEFAULT GETDATE(),
    Modified DATETIME DEFAULT GETDATE()
);

-- Insert the 6 kill animations
INSERT INTO KillAnimation (AnimationID, AnimationName, TexturePath, SoundPath, Duration, ColorR, ColorG, ColorB, Scale) VALUES
(12001, '劍氣砍殺', 'effect/kill/sword_slash.dds', 'effect/kill/sword_slash.wav', 2.5, 255, 215, 0, 1.2),
(12002, '穿刺致命', 'effect/kill/pierce_fatal.dds', 'effect/kill/pierce_fatal.wav', 1.8, 192, 192, 192, 1.0),
(12003, '爆炸轟殺', 'effect/kill/explosion_blast.dds', 'effect/kill/explosion_blast.wav', 3.0, 255, 69, 0, 1.5),
(12004, '冰凍粉碎', 'effect/kill/ice_shatter.dds', 'effect/kill/ice_shatter.wav', 2.2, 173, 216, 230, 1.1),
(12005, '烈焰焚燒', 'effect/kill/flame_burn.dds', 'effect/kill/flame_burn.wav', 2.8, 220, 20, 60, 1.3),
(12006, '雷電轟擊', 'effect/kill/lightning_strike.dds', 'effect/kill/lightning_strike.wav', 2.0, 138, 43, 226, 1.0);

-- Create index for faster lookups
CREATE INDEX IDX_KillAnimation_ID ON KillAnimation(AnimationID);

-- Create table for kill animation logs (optional, for tracking usage)
CREATE TABLE KillAnimationLog (
    LogID INT IDENTITY(1,1) PRIMARY KEY,
    AnimationID INT,
    KillerCharID INT,
    TargetCharID INT,
    MapID INT,
    PositionX FLOAT,
    PositionY FLOAT,
    PositionZ FLOAT,
    Timestamp DATETIME DEFAULT GETDATE(),
    FOREIGN KEY (AnimationID) REFERENCES KillAnimation(AnimationID)
);

-- Add some GM commands support data
CREATE TABLE KillAnimationCommands (
    CommandID INT IDENTITY(1,1) PRIMARY KEY,
    CommandName NVARCHAR(50) NOT NULL,
    AnimationID INT,
    Description NVARCHAR(255),
    RequiredGMLevel INT DEFAULT 3,
    FOREIGN KEY (AnimationID) REFERENCES KillAnimation(AnimationID)
);

-- Insert GM command data
INSERT INTO KillAnimationCommands (CommandName, AnimationID, Description, RequiredGMLevel) VALUES
('killanim_sword', 12001, 'Play sword slash kill animation', 3),
('killanim_pierce', 12002, 'Play pierce fatal kill animation', 3),
('killanim_explode', 12003, 'Play explosion blast kill animation', 3),
('killanim_ice', 12004, 'Play ice shatter kill animation', 3),
('killanim_flame', 12005, 'Play flame burn kill animation', 3),
('killanim_lightning', 12006, 'Play lightning strike kill animation', 3),
('killanim_test', NULL, 'Test all kill animations', 3),
('killanim_random', NULL, 'Play random kill animation', 3);

-- Add a view for easy querying
CREATE VIEW V_KillAnimationStats AS
SELECT 
    ka.AnimationID,
    ka.AnimationName,
    COUNT(kal.LogID) as UsageCount,
    MAX(kal.Timestamp) as LastUsed
FROM KillAnimation ka
LEFT JOIN KillAnimationLog kal ON ka.AnimationID = kal.AnimationID
GROUP BY ka.AnimationID, ka.AnimationName;

-- Grant permissions (adjust as needed for your database setup)
-- GRANT SELECT, INSERT ON KillAnimation TO [GameServerRole];
-- GRANT SELECT, INSERT ON KillAnimationLog TO [GameServerRole];
-- GRANT SELECT ON KillAnimationCommands TO [GameServerRole];
-- GRANT SELECT ON V_KillAnimationStats TO [GMRole];

PRINT 'Kill Animation System database schema created successfully!';
PRINT 'Created tables: KillAnimation, KillAnimationLog, KillAnimationCommands';
PRINT 'Created view: V_KillAnimationStats';
PRINT 'Inserted 6 kill animation configurations (12001-12006)';
PRINT 'Inserted 8 GM commands for testing';