#include "../../stdafx.h"
#include "KillCardUI.h"
#include "../../../enginelib/GUInterface/BasicTextBox.h"

#include "../../../SigmaCore/DebugInclude.h"

CKillCardUI::CKillCardUI(EngineDeviceMan* pEngineDevice)
    : CUIGroup(pEngineDevice)
    , m_pBackground(NULL)
    , m_pTitleText(NULL)
    , m_pTypeText(NULL)
    , m_pComboText(NULL)
    , m_pCardIcon(NULL)
    , m_eCurrentCard(KILL_CARD_TYPE_SIZE)
    , m_bUIVisible(FALSE)
    , m_nCurrentCombo(1)
    , m_dwTextColor(0xFFFFFFFF)
    , m_wBackgroundAlpha(180)
    , m_fTextAnimTime(0.0f)
    , m_nCardPosX(0)
    , m_nCardPosY(0)
    , m_nCardWidth(300)
    , m_nCardHeight(150)
{
}

CKillCardUI::~CKillCardUI()
{
    // 子控件会由基类自动删除
}

void CKillCardUI::CreateSubControl()
{
    // 创建背景
    {
        m_pBackground = new CUIControl(m_pEngineDevice);
        m_pBackground->CreateSub(this, "KILLCARD_UI_BACKGROUND", UI_FLAG_DEFAULT, KILLCARD_BACKGROUND);
        m_pBackground->SetTransparentOption(TRUE);
        m_pBackground->SetDiffuseAlpha(m_wBackgroundAlpha);
        m_pBackground->SetVisibleSingle(FALSE);
        RegisterControl(m_pBackground);
    }

    // 创建标题文本
    {
        m_pTitleText = new CBasicTextBox(m_pEngineDevice);
        m_pTitleText->CreateSub(this, "KILLCARD_TITLE_TEXT", UI_FLAG_DEFAULT, KILLCARD_TITLE_TEXT);
        m_pTitleText->SetFont(DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 16, _DEFAULT_FONT_FLAG));
        m_pTitleText->SetTextAlign(TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
        m_pTitleText->SetVisibleSingle(FALSE);
        RegisterControl(m_pTitleText);
    }

    // 创建类型文本
    {
        m_pTypeText = new CBasicTextBox(m_pEngineDevice);
        m_pTypeText->CreateSub(this, "KILLCARD_TYPE_TEXT", UI_FLAG_DEFAULT, KILLCARD_TYPE_TEXT);
        m_pTypeText->SetFont(DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 14, _DEFAULT_FONT_FLAG));
        m_pTypeText->SetTextAlign(TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
        m_pTypeText->SetVisibleSingle(FALSE);
        RegisterControl(m_pTypeText);
    }

    // 创建连击文本
    {
        m_pComboText = new CBasicTextBox(m_pEngineDevice);
        m_pComboText->CreateSub(this, "KILLCARD_COMBO_TEXT", UI_FLAG_DEFAULT, KILLCARD_COMBO_TEXT);
        m_pComboText->SetFont(DxFontMan::GetInstance().LoadDxFont(_DEFAULT_FONT, 12, _DEFAULT_FONT_FLAG));
        m_pComboText->SetTextAlign(TEXT_ALIGN_CENTER_X | TEXT_ALIGN_CENTER_Y);
        m_pComboText->SetVisibleSingle(FALSE);
        RegisterControl(m_pComboText);
    }

    // 创建卡片图标
    {
        m_pCardIcon = new CUIControl(m_pEngineDevice);
        m_pCardIcon->CreateSub(this, "KILLCARD_ICON", UI_FLAG_DEFAULT);
        m_pCardIcon->SetTransparentOption(TRUE);
        m_pCardIcon->SetVisibleSingle(FALSE);
        RegisterControl(m_pCardIcon);
    }

    // 设置默认位置
    SetCardPosition(100, 100);
}

void CKillCardUI::Update(int x, int y, BYTE LB, BYTE MB, BYTE RB, int nScroll, float fElapsedTime, BOOL bFirstControl)
{
    CUIGroup::Update(x, y, LB, MB, RB, nScroll, fElapsedTime, bFirstControl);
    
    if (m_bUIVisible)
    {
        UpdateTextAnimation(fElapsedTime);
    }
}

void CKillCardUI::TranslateUIMessage(UIGUID ControlID, DWORD dwMsg)
{
    CUIGroup::TranslateUIMessage(ControlID, dwMsg);
    
    switch (ControlID)
    {
    case KILLCARD_BACKGROUND:
    case KILLCARD_TITLE_TEXT:
    case KILLCARD_TYPE_TEXT:
    case KILLCARD_COMBO_TEXT:
        // 处理UI交互
        break;
    }
}

void CKillCardUI::SetVisibleSingle(BOOL bVisible)
{
    CUIGroup::SetVisibleSingle(bVisible);
    
    if (!bVisible)
    {
        HideKillCard();
    }
}

void CKillCardUI::ShowKillCard(EKILL_CARD_TYPE cardType, int nComboCount)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;
        
    m_eCurrentCard = cardType;
    m_nCurrentCombo = nComboCount;
    m_bUIVisible = TRUE;
    m_fTextAnimTime = 0.0f;
    
    // 设置卡片文本
    SetupCardText(cardType);
    
    // 更新连击显示
    UpdateComboDisplay(nComboCount);
    
    // 显示所有UI元素
    if (m_pBackground)
    {
        m_pBackground->SetVisibleSingle(TRUE);
        m_pBackground->SetDiffuseAlpha(0); // 从透明开始淡入
    }
    
    if (m_pTitleText)
        m_pTitleText->SetVisibleSingle(TRUE);
        
    if (m_pTypeText)
        m_pTypeText->SetVisibleSingle(TRUE);
        
    if (m_pComboText && nComboCount > 1)
        m_pComboText->SetVisibleSingle(TRUE);
        
    if (m_pCardIcon)
        m_pCardIcon->SetVisibleSingle(TRUE);
    
    // 启动入场动画
    AnimateTextEntrance();
}

void CKillCardUI::HideKillCard()
{
    m_bUIVisible = FALSE;
    m_eCurrentCard = KILL_CARD_TYPE_SIZE;
    
    // 隐藏所有UI元素
    if (m_pBackground)
        m_pBackground->SetVisibleSingle(FALSE);
        
    if (m_pTitleText)
        m_pTitleText->SetVisibleSingle(FALSE);
        
    if (m_pTypeText)
        m_pTypeText->SetVisibleSingle(FALSE);
        
    if (m_pComboText)
        m_pComboText->SetVisibleSingle(FALSE);
        
    if (m_pCardIcon)
        m_pCardIcon->SetVisibleSingle(FALSE);
}

void CKillCardUI::UpdateComboDisplay(int nComboCount)
{
    if (!m_pComboText)
        return;
        
    m_nCurrentCombo = nComboCount;
    
    if (nComboCount > 1)
    {
        CString strCombo;
        strCombo.Format("COMBO x%d", nComboCount);
        m_pComboText->SetText(strCombo.GetString());
        m_pComboText->SetTextColor(0, GetCardColor(m_eCurrentCard));
        m_pComboText->SetVisibleSingle(TRUE);
    }
    else
    {
        m_pComboText->SetVisibleSingle(FALSE);
    }
}

void CKillCardUI::SetCardPosition(int x, int y)
{
    m_nCardPosX = x;
    m_nCardPosY = y;
    
    // 更新所有控件位置
    UIRECT rcCard(x, y, m_nCardWidth, m_nCardHeight);
    SetGlobalPos(rcCard);
}

void CKillCardUI::SetCardSize(int width, int height)
{
    m_nCardWidth = width;
    m_nCardHeight = height;
    
    // 重新设置位置以应用新尺寸
    SetCardPosition(m_nCardPosX, m_nCardPosY);
}

void CKillCardUI::UpdateCardDisplay()
{
    // 更新卡片显示内容
    if (m_eCurrentCard < KILL_CARD_TYPE_SIZE)
    {
        SetupCardText(m_eCurrentCard);
        UpdateComboDisplay(m_nCurrentCombo);
    }
}

void CKillCardUI::SetupCardText(EKILL_CARD_TYPE cardType)
{
    if (cardType >= KILL_CARD_TYPE_SIZE)
        return;
        
    const char* strTitle = GetCardTypeString(cardType);
    const char* strDescription = GetCardDescriptionString(cardType);
    DWORD dwCardColor = GetCardColor(cardType);
    
    if (m_pTitleText)
    {
        m_pTitleText->SetText(strTitle);
        m_pTitleText->SetTextColor(0, dwCardColor);
    }
    
    if (m_pTypeText)
    {
        m_pTypeText->SetText(strDescription);
        m_pTypeText->SetTextColor(0, dwCardColor);
    }
}

void CKillCardUI::AnimateTextEntrance()
{
    // 启动文本入场动画
    m_fTextAnimTime = 0.0f;
    
    // 设置初始透明度
    if (m_pTitleText)
        m_pTitleText->SetDiffuseAlpha(0);
        
    if (m_pTypeText)
        m_pTypeText->SetDiffuseAlpha(0);
        
    if (m_pComboText)
        m_pComboText->SetDiffuseAlpha(0);
}

void CKillCardUI::UpdateTextAnimation(float fElapsedTime)
{
    m_fTextAnimTime += fElapsedTime;
    
    // 淡入动画持续0.5秒
    float fadeProgress = min(m_fTextAnimTime / 0.5f, 1.0f);
    WORD wAlpha = (WORD)(fadeProgress * 255);
    
    // 应用透明度
    if (m_pBackground)
    {
        WORD bgAlpha = (WORD)(fadeProgress * m_wBackgroundAlpha);
        m_pBackground->SetDiffuseAlpha(bgAlpha);
    }
    
    if (m_pTitleText)
        m_pTitleText->SetDiffuseAlpha(wAlpha);
        
    if (m_pTypeText)
        m_pTypeText->SetDiffuseAlpha(wAlpha);
        
    if (m_pComboText && m_nCurrentCombo > 1)
        m_pComboText->SetDiffuseAlpha(wAlpha);
        
    if (m_pCardIcon)
        m_pCardIcon->SetDiffuseAlpha(wAlpha);
}

const char* CKillCardUI::GetCardTypeString(EKILL_CARD_TYPE cardType)
{
    switch (cardType)
    {
    case KILL_CARD_QUANTUM_FISSION:
        return "QUANTUM FISSION";
        
    case KILL_CARD_HOLOGRAM_DISRUPTION:
        return "HOLOGRAM DISRUPTION";
        
    case KILL_CARD_SPACETIME_DISTORTION:
        return "SPACE-TIME DISTORTION";
        
    case KILL_CARD_PLASMA_EXPLOSION:
        return "PLASMA EXPLOSION";
        
    case KILL_CARD_NANO_DISSOLUTION:
        return "NANO DISSOLUTION";
        
    case KILL_CARD_VIRTUAL_RECONSTRUCTION:
        return "VIRTUAL RECONSTRUCTION";
        
    default:
        return "UNKNOWN KILL";
    }
}

const char* CKillCardUI::GetCardDescriptionString(EKILL_CARD_TYPE cardType)
{
    switch (cardType)
    {
    case KILL_CARD_QUANTUM_FISSION:
        return "Critical Strike Kill";
        
    case KILL_CARD_HOLOGRAM_DISRUPTION:
        return "Skill Kill";
        
    case KILL_CARD_SPACETIME_DISTORTION:
        return "Multi Kill";
        
    case KILL_CARD_PLASMA_EXPLOSION:
        return "Ranged Kill";
        
    case KILL_CARD_NANO_DISSOLUTION:
        return "Melee Kill";
        
    case KILL_CARD_VIRTUAL_RECONSTRUCTION:
        return "Headshot Kill";
        
    default:
        return "Unknown";
    }
}

DWORD CKillCardUI::GetCardColor(EKILL_CARD_TYPE cardType)
{
    switch (cardType)
    {
    case KILL_CARD_QUANTUM_FISSION:
        return 0xFF4080FF; // 蓝白色
        
    case KILL_CARD_HOLOGRAM_DISRUPTION:
        return 0xFF20FF40; // 绿色
        
    case KILL_CARD_SPACETIME_DISTORTION:
        return 0xFF8040FF; // 紫色
        
    case KILL_CARD_PLASMA_EXPLOSION:
        return 0xFFFF4080; // 紫红色
        
    case KILL_CARD_NANO_DISSOLUTION:
        return 0xFFC0C0C0; // 银色
        
    case KILL_CARD_VIRTUAL_RECONSTRUCTION:
        return 0xFF00FFFF; // 青色
        
    default:
        return 0xFFFFFFFF; // 白色
    }
}