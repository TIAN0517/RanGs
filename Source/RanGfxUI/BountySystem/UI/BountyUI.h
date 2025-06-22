#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "../Core/BountyOrder.h"

class CBasicScrollBarEx;
class CBasicTextBox;
class CUIEditBox;
class CBasicButton;
class CBountyManager;

/**
 * 追殺令主界面
 * 提供追殺令系統的主要用戶操作界面
 */
class CBountyUI : public CUIGroup
{
private:
    enum
    {
        BOUNTY_UI_LIST_BOX = NO_ID + 1,
        BOUNTY_UI_SCROLL_BAR,
        BOUNTY_UI_CREATE_BUTTON,
        BOUNTY_UI_ACCEPT_BUTTON,
        BOUNTY_UI_CANCEL_BUTTON,
        BOUNTY_UI_REFRESH_BUTTON,
        BOUNTY_UI_RANKING_BUTTON,
        BOUNTY_UI_HISTORY_BUTTON,
        BOUNTY_UI_CLOSE_BUTTON,
        BOUNTY_UI_TAB_ACTIVE,
        BOUNTY_UI_TAB_MY_BOUNTIES,
        BOUNTY_UI_TAB_MY_ACCEPTED,
        BOUNTY_UI_SEARCH_EDIT,
        BOUNTY_UI_FILTER_COMBO,
        BOUNTY_UI_SORT_COMBO
    };

public:
    CBountyUI(EngineDeviceMan* pEngineDevice);
    virtual ~CBountyUI();

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
    virtual void SetVisibleSingle(BOOL bVisible);

public:
    // 界面操作
    void SetBountyManager(CBountyManager* pManager) { m_pBountyManager = pManager; }
    void RefreshBountyList();
    void ShowCreateBountyDialog();
    void ShowBountyDetails(DWORD dwBountyID);
    void ShowRankingWindow();
    void ShowHistoryWindow();
    
    // 標籤頁切換
    void SwitchToTab(int nTabIndex);
    void UpdateTabContent();
    
    // 篩選和搜索
    void ApplyFilter();
    void ApplySort();
    void SearchBounties(const std::string& strKeyword);

protected:
    // UI創建輔助方法
    void CreateMainPanel();
    void CreateTabButtons();
    void CreateBountyList();
    void CreateActionButtons();
    void CreateSearchControls();
    
    // 事件處理
    void OnCreateBountyClicked();
    void OnAcceptBountyClicked();
    void OnCancelBountyClicked();
    void OnRefreshClicked();
    void OnRankingClicked();
    void OnHistoryClicked();
    void OnTabClicked(int nTabIndex);
    void OnBountySelected(int nIndex);
    void OnSearchTextChanged();
    
    // 數據更新
    void UpdateBountyListDisplay();
    void UpdateButtonStates();
    void LoadActiveBounties();
    void LoadMyBounties();
    void LoadAcceptedBounties();

private:
    CBountyManager* m_pBountyManager;
    
    // UI 控件
    CBasicScrollBarEx* m_pScrollBar;
    CBasicTextBox* m_pListBox;
    CUIEditBox* m_pSearchEdit;
    
    // 按鈕
    CBasicButton* m_pCreateButton;
    CBasicButton* m_pAcceptButton;
    CBasicButton* m_pCancelButton;
    CBasicButton* m_pRefreshButton;
    CBasicButton* m_pRankingButton;
    CBasicButton* m_pHistoryButton;
    CBasicButton* m_pCloseButton;
    
    // 標籤頁按鈕
    CBasicButton* m_pTabActive;
    CBasicButton* m_pTabMyBounties;
    CBasicButton* m_pTabMyAccepted;
    
    // 狀態變量
    int m_nCurrentTab;
    int m_nSelectedBounty;
    std::vector<CBountyOrder*> m_vecDisplayBounties;
    std::string m_strSearchKeyword;
    EBOUNTY_STATUS m_eFilterStatus;
    int m_nSortType;
    
    // 界面設置
    BOOL m_bAutoRefresh;
    float m_fRefreshTimer;
    BOOL m_bShowDetails;
};