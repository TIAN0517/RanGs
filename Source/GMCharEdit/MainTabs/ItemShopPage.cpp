// UserSearch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GMCharEdit.h"
#include "../MainFrm.h"
#include "../../RanLogicServer/DxServerInstance.h"
#include "../SubDialogs/DlgUserEdit.h"



// ----------------------------------------------------------------------------
#include "../../SigmaCore/DebugInclude.h"
#include "ItemShopPage.h"
// ----------------------------------------------------------------------------

// CItemShopPage

IMPLEMENT_DYNCREATE(CItemShopPage, CFormView)

CItemShopPage::CItemShopPage()
	: CFormView(CItemShopPage::IDD)
	, m_bInit(FALSE)
{
	m_pDBMan = CGmCfg::GetInstance()->GetDBMan();//CADOManager::GetInstance();
}

CItemShopPage::~CItemShopPage()
{
}

void CItemShopPage::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SEARCH, m_ListSearch);
	DDX_Control(pDX, IDC_ITEMSHOP_PAGE_EDIT_SEARCH, m_EditSearch);
}

BEGIN_MESSAGE_MAP(CItemShopPage, CFormView)
	ON_BN_CLICKED(IDC_SEARCH, OnBnClickedSearch)
	ON_BN_CLICKED(IDC_RADIO_ITEMNAME, OnBnClickedRadioItemName)
	ON_BN_CLICKED(IDC_RADIO_PRODNUM, OnBnClickedRadioProductNum)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SEARCH, OnLvnItemchangedListSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SEARCH, OnNMDblclkListSearch)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SEARCH, OnNMRclickListSearch)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SEARCH, OnNMCustomdrawListSearch)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_ITEMSHOP_PAGE_BUTTON_SEARCH, &CItemShopPage::OnBnClickedSearch2)
	ON_BN_CLICKED(IDC_BUTTON_SENDALL, &CItemShopPage::OnBnClicked1210)
END_MESSAGE_MAP()


// CItemShopPage 진단입니다.

#ifdef _DEBUG
void CItemShopPage::AssertValid() const
{
	CFormView::AssertValid();
}

void CItemShopPage::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CItemShopPage 메시지 처리기입니다.
void CItemShopPage::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();

	// cfg 로드성공, db 연결성공, 로그인 성공
	if (pFrame->m_bCfgLoad && pFrame->m_bDBConnect && pFrame->m_bLogin)
	{
		// 리스트 컨트롤 초기화
		m_ListSearch.SetExtendedStyle (LVS_EX_FULLROWSELECT);
        InitListHead();	
		m_bInit = TRUE;

		CButton* pButton = (CButton*) GetDlgItem(IDC_RADIO_USERNAME);
		pButton->SetCheck(1);
		m_nRadio = CItemShopPage::SEARCH_ITEMNAME;

		UpdateData(FALSE);
	}
	else
	{
		AfxGetMainWnd()->PostMessage(WM_CLOSE);		
	} 
}

void CItemShopPage::SearchItemName(const CString& strUserName)
{
	m_pDBMan->ShopGetData( gmtoolSHOPDATA );
	FillData();
}

void CItemShopPage::SearchProductNum(int nUserNum)
{
	
}

// 리스트 컨트롤 헤더 초기화
void CItemShopPage::InitListHead() 
{
	LVCOLUMN Col;

	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 50;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= " ";
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(0, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 50;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= "Product Num";
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(1, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 120;
	Col.fmt			= LVCFMT_CENTER;		
	Col.pszText		= _T("Name");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(2, &Col);

	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 120;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Category");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(3, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 30;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Stock");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(4, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 120;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Price");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(5, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 120;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Comment");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(6, &Col);
}

void CItemShopPage::FillData()
{
	// Erase all list items
	m_ListSearch.DeleteAllItems();

	m_ListSearch.SetRedraw( FALSE );

	for( int i=0; i<(int)m_vecResult.size(); ++i )
	{
		const SGMTOOL_SHOP_ITEM sDATA = m_vecResult[i];

		m_ListSearch.InsertItem( i, "" );
		CString str;
		str.Format ( "%d",sDATA.dwProductNum );
		m_ListSearch.SetItemText( i, 1, str.GetString() );
		str.Format ( "[%03d/%03d] %s", sDATA.wItemMain, sDATA.wItemSub, sDATA.szItemName );
		m_ListSearch.SetItemText( i, 2, str.GetString() );
		str.Format ( "%d",sDATA.dwCategory );
		m_ListSearch.SetItemText( i, 3, str.GetString() );
		str.Format ( "%d",sDATA.dwItemStock );
		m_ListSearch.SetItemText( i, 4, str.GetString() );
		str.Format ( "%d",sDATA.dwItemMoney );
		m_ListSearch.SetItemText( i, 5, str.GetString() );
		m_ListSearch.SetItemText( i, 6, sDATA.szItemComment );
		m_ListSearch.SetItemData ( i, sDATA.dwProductNum );
	}

	m_ListSearch.SetRedraw( TRUE );
}

void CItemShopPage::OnBnClickedSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

    CString strSearch;
    m_EditSearch.GetWindowText(strSearch);

	if (strSearch.GetLength() < 1)
	{
		MessageBox(_T("Please type search string"));
		return;
	}

    int nUserNum;
    int nChaNum;

    switch (m_nRadio)
    {
    case SEARCH_ITEMNAME:        
        SearchItemName(strSearch);
        break;
    case SEARCH_PRODNUM:
		nUserNum = ::atoi(strSearch.GetString());         
        SearchProductNum(nUserNum);
        break;
    default:
        break;
    }
}
void CItemShopPage::OnBnClickedRadioItemName()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_nRadio = CItemShopPage::SEARCH_ITEMNAME;
}

void CItemShopPage::OnBnClickedRadioProductNum()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_nRadio = CItemShopPage::SEARCH_PRODNUM;
}

void CItemShopPage::OnLvnItemchangedListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CItemShopPage::OnNMDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{	
    *pResult = 0;
}

void CItemShopPage::OnNMRclickListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 선택된 캐릭터가 있는지 확인한다.
	int nSelected = m_ListSearch.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	
	if (nSelected != -1) // 선택된 캐릭터가 있음
	{	
		// 마우스 좌표 얻기
		POINT point;
		GetCursorPos(&point);

		// 메뉴 표시
		CMenu menuTemp, *pContextMenu;
		menuTemp.LoadMenu(IDR_CONTEXT_MAIN);
		pContextMenu = menuTemp.GetSubMenu(0);
		pContextMenu->TrackPopupMenu(TPM_LEFTALIGN,		
									point.x,          // 메뉴가 표시될 x 좌표
									point.y,          // 메뉴가 표시될 y 좌표
									AfxGetMainWnd()   // WM_COMMAND 메시지를 받을 윈도우
									);
	}
	*pResult = 0;
}

void CItemShopPage::OnNMCustomdrawListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{
	// MFC 기본코드 삭제
	//LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;	

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*> (pNMHDR);
    *pResult = 0;

	// First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
    if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
    {
        *pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
    {
        // This is the notification message for an item.  We'll request
        // notifications before each subitem's prepaint stage.
        *pResult = CDRF_NOTIFYSUBITEMDRAW;
    }
    else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
    {
        // This is the prepaint stage for a subitem. Here's where we set the
        // item's text and background colors. Our return value will tell 
        // Windows to draw the subitem itself, but it will use the new colors
        // we set here.
        // The text color will cycle through red, green, and light blue.
        // The background color will be light blue for column 0, red for
        // column 1, and black for column 2.    
        COLORREF crText, crBkgnd;
        
		crText  = RGB(0, 0, 0);
		crBkgnd = RGB(255, 255, 255);

		if (pLVCD->iSubItem == 5)
		{
			int nItemMain = static_cast<int> (pLVCD->nmcd.dwItemSpec);
			if (m_ListSearch.GetItemText(nItemMain, pLVCD->iSubItem) ==	_T("Online"))
			{
				crText  = RGB(  0,   0,   0);
				crBkgnd = RGB(  0, 204, 255);				
			}
		}
		else if (pLVCD->iSubItem == 6)
		{
			int nItemMain = static_cast<int> (pLVCD->nmcd.dwItemSpec);
			if (m_ListSearch.GetItemText(nItemMain, pLVCD->iSubItem) ==	_T("Deleted"))
			{
				crText  = RGB(  0,   0,   0);				
				crBkgnd = RGB(255, 102, 102);
			}
		}
		else
		{
		}
		
		// Store the colors back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText   = crText;
		pLVCD->clrTextBk = crBkgnd;
        // Tell Windows to paint the control itself.
        *pResult = CDRF_DODEFAULT;
    }
}

void CItemShopPage::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CItemShopPage::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_bInit)
	{

		int nWidth  = cx - 5;
		int nHeight = cy - 110;

		if (nWidth < 0)  cx = 0;
		if (nHeight < 0) cy = 0;

		m_ListSearch.MoveWindow(0, // rect.left,
								100, // rect.top,
								nWidth,
								nHeight);
	}
}

void CItemShopPage::OnBnClickedItemshopPageButtonSearch()
{
}

void CItemShopPage::OnBnClickedSearch2()
{
	
}

SGMTOOL_SHOP_ITEM* CItemShopPage::FindShopItem( DWORD _dwProductNum )
{
	SGMTOOL_SHOP_ITEM_MAP_ITER iter = gmtoolSHOPDATA.find( _dwProductNum );
	if ( iter != gmtoolSHOPDATA.end() ){
		return &(*iter).second;
	}

	return NULL;
};

BOOL CItemShopPage::DeleteShopItem( DWORD _dwProductNum )
{
	SGMTOOL_SHOP_ITEM_MAP_ITER iter = gmtoolSHOPDATA.find( _dwProductNum );
	if ( iter != gmtoolSHOPDATA.end() ){
		gmtoolSHOPDATA.erase( iter );
		return TRUE;
	}

	return FALSE;
};

void CItemShopPage::OnBnClicked1210()
{
	int nSelect = m_ListSearch.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	if( nSelect == -1 ){
		return;
	}

	DWORD dwProductNum = (DWORD) m_ListSearch.GetItemData ( nSelect );

	SGMTOOL_SHOP_ITEM_MAP_ITER iterpos = gmtoolSHOPDATA.find(dwProductNum);
	if ( iterpos != gmtoolSHOPDATA.end() ){
		const SGMTOOL_SHOP_ITEM sDATA = (*iterpos).second;

		if ( DB_OK == m_pDBMan->UserBankInsertAll( sDATA.dwProductNum ) )
		{
			MessageBox("Send to Bank OK!");
		}
	}
}
