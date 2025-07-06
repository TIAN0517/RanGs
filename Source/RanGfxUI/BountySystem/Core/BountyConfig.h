#pragma once

#include "../../../SigmaCore/Xml/2.5.3/ticpp.h"
#include <string>
#include <map>

/**
 * 追殺令配置管理類
 * 負責管理追殺令系統的所有配置參數
 */
class CBountyConfig
{
public:
    // 配置結構體
    struct GeneralConfig
    {
        BOOL bEnabled;                    // 系統是否啟用
        int nMaxActiveBounties;           // 最大活躍追殺令數
        int nMaxPlayerBounties;           // 每個玩家最大發布數
        int nMaxAcceptedBounties;         // 每個玩家最大接受數
        float fUpdateInterval;            // 更新間隔(秒)
        int nDefaultExpireHours;          // 默認過期時間(小時)
        int nMinPlayerLevel;              // 最低玩家等級
        LONGLONG llMinRewardAmount;       // 最小獎勵金額
        LONGLONG llMaxRewardAmount;       // 最大獎勵金額
        float fSystemFeeRate;             // 系統手續費率
        
        GeneralConfig()
            : bEnabled(TRUE)
            , nMaxActiveBounties(1000)
            , nMaxPlayerBounties(5)
            , nMaxAcceptedBounties(3)
            , fUpdateInterval(5.0f)
            , nDefaultExpireHours(24)
            , nMinPlayerLevel(10)
            , llMinRewardAmount(10000)
            , llMaxRewardAmount(100000000)
            , fSystemFeeRate(0.1f)
        {
        }
    };
    
    struct RewardConfig
    {
        BOOL bMoneyRewardEnabled;         // 金錢獎勵是否啟用
        BOOL bItemRewardEnabled;          // 道具獎勵是否啟用
        BOOL bExpRewardEnabled;           // 經驗獎勵是否啟用
        float fExpMultiplier;             // 經驗倍率
        int nBonusExpLevel;               // 獎勵經驗等級差
        std::vector<DWORD> vecRewardItems; // 獎勵道具列表
        
        RewardConfig()
            : bMoneyRewardEnabled(TRUE)
            , bItemRewardEnabled(TRUE)
            , bExpRewardEnabled(TRUE)
            , fExpMultiplier(1.5f)
            , nBonusExpLevel(5)
        {
        }
    };
    
    struct UIConfig
    {
        BOOL bShowBountyNotifications;    // 顯示追殺令通知
        BOOL bShowKillNotifications;      // 顯示擊殺通知
        BOOL bPlaySoundEffects;           // 播放音效
        int nNotificationDuration;        // 通知持續時間(秒)
        std::string strSoundFile;         // 音效文件
        float fUIScale;                   // UI縮放比例
        
        UIConfig()
            : bShowBountyNotifications(TRUE)
            , bShowKillNotifications(TRUE)
            , bPlaySoundEffects(TRUE)
            , nNotificationDuration(5)
            , strSoundFile("bounty_notification.wav")
            , fUIScale(1.0f)
        {
        }
    };
    
    struct SecurityConfig
    {
        BOOL bAntiSpamEnabled;            // 防刷新啟用
        int nSpamTimeLimit;               // 刷新時間限制(秒)
        int nMaxSpamAttempts;             // 最大刷新嘗試次數
        BOOL bIPRestrictionEnabled;       // IP限制啟用
        BOOL bGuildMemberProtection;      // 公會成員保護
        int nMinTargetLevel;              // 最低目標等級
        int nMaxLevelDifference;          // 最大等級差
        
        SecurityConfig()
            : bAntiSpamEnabled(TRUE)
            , nSpamTimeLimit(60)
            , nMaxSpamAttempts(3)
            , bIPRestrictionEnabled(FALSE)
            , bGuildMemberProtection(TRUE)
            , nMinTargetLevel(10)
            , nMaxLevelDifference(20)
        {
        }
    };

public:
    CBountyConfig();
    virtual ~CBountyConfig();

public:
    // 初始化和清理
    BOOL Initialize(const std::string& strConfigFile);
    void Shutdown();
    
    // 配置加載和保存
    BOOL LoadConfig();
    BOOL SaveConfig();
    BOOL ReloadConfig();
    
    // 配置獲取
    const GeneralConfig& GetGeneralConfig() const { return m_GeneralConfig; }
    const RewardConfig& GetRewardConfig() const { return m_RewardConfig; }
    const UIConfig& GetUIConfig() const { return m_UIConfig; }
    const SecurityConfig& GetSecurityConfig() const { return m_SecurityConfig; }
    
    // 配置設置
    void SetGeneralConfig(const GeneralConfig& config);
    void SetRewardConfig(const RewardConfig& config);
    void SetUIConfig(const UIConfig& config);
    void SetSecurityConfig(const SecurityConfig& config);
    
    // 動態配置修改
    void SetEnabled(BOOL bEnabled);
    void SetMaxActiveBounties(int nMax);
    void SetUpdateInterval(float fInterval);
    void SetExpireHours(int nHours);
    void SetRewardLimits(LONGLONG llMin, LONGLONG llMax);
    void SetSystemFeeRate(float fRate);
    
    // 驗證方法
    BOOL ValidateBountyRequest(DWORD dwPublisherID, DWORD dwTargetID, LONGLONG llReward);
    BOOL ValidatePlayerLevel(int nLevel, BOOL bAsTarget = FALSE);
    BOOL ValidateRewardAmount(LONGLONG llAmount);
    BOOL ValidateLevelDifference(int nPublisherLevel, int nTargetLevel);
    
    // 計算方法
    LONGLONG CalculateSystemFee(LONGLONG llRewardAmount);
    int CalculateExpireTime(EBOUNTY_TYPE eType);
    float CalculateSuccessBonus(int nConsecutiveKills);
    
    // 特殊配置
    BOOL IsGuildProtectionEnabled() const;
    BOOL IsAntiSpamActive() const;
    int GetSpamCooldownTime() const;
    
    // 調試和日誌
    void PrintCurrentConfig();
    std::string GetConfigSummary();

protected:
    // XML操作
    BOOL LoadFromXML(ticpp::Document& doc);
    BOOL SaveToXML(ticpp::Document& doc);
    void LoadGeneralConfigFromXML(ticpp::Element* pElement);
    void LoadRewardConfigFromXML(ticpp::Element* pElement);
    void LoadUIConfigFromXML(ticpp::Element* pElement);
    void LoadSecurityConfigFromXML(ticpp::Element* pElement);
    void SaveGeneralConfigToXML(ticpp::Element* pElement);
    void SaveRewardConfigToXML(ticpp::Element* pElement);
    void SaveUIConfigToXML(ticpp::Element* pElement);
    void SaveSecurityConfigToXML(ticpp::Element* pElement);
    
    // 默認值設置
    void SetDefaultValues();
    void ValidateConfigValues();

private:
    std::string m_strConfigFile;
    BOOL m_bInitialized;
    __time64_t m_tLastModified;
    
    // 配置數據
    GeneralConfig m_GeneralConfig;
    RewardConfig m_RewardConfig;
    UIConfig m_UIConfig;
    SecurityConfig m_SecurityConfig;
    
    // 動態配置緩存
    std::map<std::string, std::string> m_mapDynamicConfig;
    
    // 配置變更回調
    typedef std::function<void(const std::string&)> ConfigChangeCallback;
    std::vector<ConfigChangeCallback> m_vecChangeCallbacks;
    
public:
    // 配置變更通知
    void RegisterChangeCallback(ConfigChangeCallback callback);
    void UnregisterChangeCallback(ConfigChangeCallback callback);

protected:
    void NotifyConfigChange(const std::string& strSection);
};