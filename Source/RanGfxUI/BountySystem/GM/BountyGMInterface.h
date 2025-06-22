#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "../../../RanLogic/GLCharDefine.h"

class CBasicTextBox;
class CBasicButton;
class CUIEditBox;
class CBountyManager;

/**
 * GM追殺令管理界面
 * 提供管理員對追殺令系統的管理和監控功能
 */
class CBountyGMInterface : public CUIGroup
{
private:
    enum
    {
        GM_BOUNTY_LIST = NO_ID + 1,
        GM_CREATE_BOUNTY_BUTTON,
        GM_CANCEL_BOUNTY_BUTTON,
        GM_MODIFY_REWARD_BUTTON,
        GM_FORCE_COMPLETE_BUTTON,
        GM_VIEW_LOGS_BUTTON,
        GM_PLAYER_SEARCH_EDIT,
        GM_BOUNTY_ID_EDIT,
        GM_REWARD_AMOUNT_EDIT,
        GM_REASON_EDIT,
        GM_REFRESH_BUTTON,
        GM_EXPORT_BUTTON,
        GM_CONFIG_BUTTON,
        GM_STATISTICS_BUTTON,
        GM_BAN_PLAYER_BUTTON,
        GM_UNBAN_PLAYER_BUTTON,
        GM_CLOSE_BUTTON
    };

public:
    CBountyGMInterface(EngineDeviceMan* pEngineDevice);
    virtual ~CBountyGMInterface();

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
    virtual void SetVisibleSingle(BOOL bVisible);

public:
    // 界面操作
    void SetBountyManager(CBountyManager* pManager) { m_pBountyManager = pManager; }
    void RefreshData();
    void ShowStatistics();
    void ShowConfigWindow();
    void ShowLogViewer();
    
    // GM操作
    BOOL GMCreateBounty();
    BOOL GMCancelBounty();
    BOOL GMModifyReward();
    BOOL GMForceComplete();
    BOOL GMBanPlayer();
    BOOL GMUnbanPlayer();
    
    // 數據導出
    BOOL ExportBountyData();
    BOOL ExportLogData();
    BOOL ExportStatistics();

protected:
    // UI創建
    void CreateMainPanel();
    void CreateBountyListPanel();
    void CreateActionPanel();
    void CreateSearchPanel();
    void CreateStatisticsPanel();
    
    // 事件處理
    void OnCreateBountyClicked();
    void OnCancelBountyClicked();
    void OnModifyRewardClicked();
    void OnForceCompleteClicked();
    void OnViewLogsClicked();
    void OnPlayerSearchChanged();
    void OnRefreshClicked();
    void OnExportClicked();
    void OnConfigClicked();
    void OnStatisticsClicked();
    void OnBanPlayerClicked();
    void OnUnbanPlayerClicked();
    
    // 數據更新
    void UpdateBountyList();
    void UpdateStatisticsDisplay();
    void UpdatePlayerInfo();
    
    // 驗證
    BOOL ValidateGMPermission();
    BOOL ValidateBountyID();
    BOOL ValidatePlayerID();
    BOOL ValidateRewardAmount();
    
    // 工具方法
    void ShowGMMessage(const std::string& strMessage);
    void LogGMAction(const std::string& strAction, const std::string& strDetails);

private:
    CBountyManager* m_pBountyManager;
    
    // UI控件
    CBasicTextBox* m_pBountyListBox;
    CUIEditBox* m_pPlayerSearchEdit;
    CUIEditBox* m_pBountyIDEdit;
    CUIEditBox* m_pRewardAmountEdit;
    CUIEditBox* m_pReasonEdit;
    
    // 按鈕
    CBasicButton* m_pCreateBountyButton;
    CBasicButton* m_pCancelBountyButton;
    CBasicButton* m_pModifyRewardButton;
    CBasicButton* m_pForceCompleteButton;
    CBasicButton* m_pViewLogsButton;
    CBasicButton* m_pRefreshButton;
    CBasicButton* m_pExportButton;
    CBasicButton* m_pConfigButton;
    CBasicButton* m_pStatisticsButton;
    CBasicButton* m_pBanPlayerButton;
    CBasicButton* m_pUnbanPlayerButton;
    CBasicButton* m_pCloseButton;
    
    // 狀態變量
    BOOL m_bGMMode;
    DWORD m_dwSelectedBountyID;
    DWORD m_dwSelectedPlayerID;
    std::string m_strSearchKeyword;
    
    // 統計信息
    int m_nTotalBounties;
    int m_nActiveBounties;
    int m_nCompletedToday;
    LONGLONG m_llTotalRewards;
    
    // 權限檢查
    BOOL CheckGMLevel(int nRequiredLevel);
    BOOL CheckGMPermission(const std::string& strPermission);
};

/**
 * GM命令處理器
 */
class CBountyGMCommands
{
public:
    CBountyGMCommands();
    virtual ~CBountyGMCommands();

public:
    // 初始化
    BOOL Initialize(CBountyManager* pManager);
    void Shutdown();
    
    // 命令註冊
    void RegisterCommands();
    
    // 命令處理
    BOOL ProcessCommand(DWORD dwGMCharID, const std::string& strCommand, const std::vector<std::string>& vecArgs);
    
    // 具體命令實現
    BOOL CMD_CreateBounty(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_CancelBounty(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_ModifyReward(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_ForceComplete(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_ListBounties(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_PlayerInfo(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_Statistics(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_BanPlayer(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_UnbanPlayer(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_SetConfig(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_ReloadConfig(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);
    BOOL CMD_Help(DWORD dwGMCharID, const std::vector<std::string>& vecArgs);

protected:
    // 輔助方法
    std::vector<std::string> ParseCommand(const std::string& strFullCommand);
    BOOL ValidateGMLevel(DWORD dwGMCharID, int nRequiredLevel);
    void SendGMMessage(DWORD dwGMCharID, const std::string& strMessage);
    void LogGMCommand(DWORD dwGMCharID, const std::string& strCommand, const std::string& strResult);

private:
    CBountyManager* m_pBountyManager;
    BOOL m_bInitialized;
    
    // 命令映射
    std::map<std::string, std::function<BOOL(DWORD, const std::vector<std::string>&)>> m_mapCommands;
    std::map<std::string, int> m_mapCommandPermissions; // 命令所需GM等級
    std::map<std::string, std::string> m_mapCommandHelp; // 命令幫助信息
};