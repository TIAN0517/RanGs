#pragma once

#include "../../../enginelib/GUInterface/UIGroup.h"
#include "KillCardManager.h"

class CBasicTextBox;

/**
 * 击杀卡片UI界面
 * 处理用户界面展示和交互
 */
class CKillCardUI : public CUIGroup
{
private:
    enum UI_CONTROL_IDS
    {
        KILLCARD_BACKGROUND = 0,
        KILLCARD_TITLE_TEXT,
        KILLCARD_TYPE_TEXT,
        KILLCARD_COMBO_TEXT,
        KILLCARD_SETTINGS_BUTTON,
        
        KILLCARD_UI_SIZE
    };

public:
    CKillCardUI(EngineDeviceMan* pEngineDevice);
    virtual ~CKillCardUI();

public:
    void CreateSubControl();
    
    virtual void Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl);
    virtual void TranslateUIMessage(UIGUID ControlID, DWORD dwMsg);
    virtual void SetVisibleSingle(BOOL bVisible);

public:
    // UI显示接口
    void ShowKillCard(EKILL_CARD_TYPE cardType, int nComboCount = 1);
    void HideKillCard();
    void UpdateComboDisplay(int nComboCount);
    
    // UI配置
    void SetCardPosition(int x, int y);
    void SetCardSize(int width, int height);
    void SetTextColor(DWORD dwColor) { m_dwTextColor = dwColor; }
    void SetBackgroundAlpha(WORD wAlpha) { m_wBackgroundAlpha = wAlpha; }

private:
    // 内部UI方法
    void UpdateCardDisplay();
    void SetupCardText(EKILL_CARD_TYPE cardType);
    void AnimateTextEntrance();
    void UpdateTextAnimation(float fElapsedTime);
    
    // 文本辅助方法
    const char* GetCardTypeString(EKILL_CARD_TYPE cardType);
    const char* GetCardDescriptionString(EKILL_CARD_TYPE cardType);
    DWORD GetCardColor(EKILL_CARD_TYPE cardType);

private:
    // UI控件
    CUIControl* m_pBackground;
    CBasicTextBox* m_pTitleText;
    CBasicTextBox* m_pTypeText;
    CBasicTextBox* m_pComboText;
    CUIControl* m_pCardIcon;
    
    // UI状态
    EKILL_CARD_TYPE m_eCurrentCard;
    BOOL m_bUIVisible;
    int m_nCurrentCombo;
    
    // 显示参数
    DWORD m_dwTextColor;
    WORD m_wBackgroundAlpha;
    float m_fTextAnimTime;
    
    // 位置和大小
    int m_nCardPosX;
    int m_nCardPosY;
    int m_nCardWidth;
    int m_nCardHeight;
};