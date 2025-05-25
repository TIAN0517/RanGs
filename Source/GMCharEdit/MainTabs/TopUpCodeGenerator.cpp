// TopUpCodeGenerator.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "../GMCharEdit.h"
#include "TopUpCodeGenerator.h"
#include "../MainFrm.h"

#include "../../RanLogicServer/DxServerInstance.h"
#include "../DoCharEdit/PropertySheetCharEdit.h"
#include "../SubDialogs/DlgMakeCronCharacter.h"
#include "../SubDialogs/DlgUserEdit.h"
#include "../../MfcExLib/ExLibs/EtcFunction.h"
#include "../../SigmaCore/DebugSet.h"




// ----------------------------------------------------------------------------
#include "../../SigmaCore/DebugInclude.h"
// ----------------------------------------------------------------------------

// CTopUpCodeGenerator

IMPLEMENT_DYNCREATE(CTopUpCodeGenerator, CFormView)

CTopUpCodeGenerator::CTopUpCodeGenerator()
	: CFormView(CTopUpCodeGenerator::IDD)
	, m_nRadio(CTopUpCodeGenerator::SEARCH_CODEPINUSER)
	, m_bInit(FALSE)
{
	m_pDBMan = CGmCfg::GetInstance()->GetDBMan();//CADOManager::GetInstance();
}

CTopUpCodeGenerator::~CTopUpCodeGenerator()
{
}

void CTopUpCodeGenerator::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SEARCH, m_ListSearch);
	DDX_Control(pDX, IDC_EDIT1, m_EditSearch);
	DDX_Control(pDX, IDC_CHECK_USED, m_CheckUsed);

	DDX_Control(pDX, IDC_EDIT_GENNUM, m_EditGenNum);
	DDX_Control(pDX, IDC_EDIT_GENVALUE, m_EditGenValue);
}

BEGIN_MESSAGE_MAP(CTopUpCodeGenerator, CFormView)
	ON_BN_CLICKED(IDC_SEARCH, OnBnClickedSearch)
	ON_BN_CLICKED(IDC_RADIO_USERNAME, OnBnClickedRadioUserName)
	ON_BN_CLICKED(IDC_RADIO_USERNUM, OnBnClickedRadioUsernum)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SEARCH, OnLvnItemchangedListSearch)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_SEARCH, OnNMDblclkListSearch)
	ON_COMMAND(ID_CONTEXT1_MAKECRONCHARACTER, OnContext1Makecroncharacter)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SEARCH, OnNMCustomdrawListSearch)
	ON_BN_CLICKED(IDC_BUTTON_GENERATE, OnBnClickedGenerateCodes)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CTopUpCodeGenerator 진단입니다.

#ifdef _DEBUG
void CTopUpCodeGenerator::AssertValid() const
{
	CFormView::AssertValid();
}

void CTopUpCodeGenerator::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CTopUpCodeGenerator 메시지 처리기입니다.
void CTopUpCodeGenerator::OnInitialUpdate()
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
		m_nRadio = CTopUpCodeGenerator::SEARCH_CODEPINUSER;

		UpdateData(FALSE);
	}
	else
	{
		AfxGetMainWnd()->PostMessage(WM_CLOSE);		
	} 
}

void CTopUpCodeGenerator::SearchUsed(int nNum)
{
    int nRetCode;
		
    m_vTopUpCodesInfo.clear();

    std::string Query;

    Query = sc::string::format("\
BEGIN \
    SELECT Num, Code, Pin, GenerateDate, UsedDate, IsUsed, UsedBy, Value \
	FROM TopUpCodesInfo WHERE and IsUsed = 1 Num = %1% \
    ORDER BY UsedDate desc \
END",
        nNum);

    nRetCode = m_pDBMan->SearchTopUpCodes(Query, m_vTopUpCodesInfo);

    if (nRetCode == DB_ERROR) 
	{
        MessageBox(_T("Database Error"));
	}
    else
	{
		if (m_vTopUpCodesInfo.size() == 0)
		{
			MessageBox(_T("No Result"));		
		}
		else
		{
			FillData();
		}
		m_strSearch = Query.c_str();
	}	
}

void CTopUpCodeGenerator::SearchUsed(const CString& strUserName)
{
    int nRetCode;
		
    m_vTopUpCodesInfo.clear();

    std::string Used(strUserName.GetString());
    sc::string::replaceForDb(Used);

    std::string Query;

    Query = sc::string::format("\
BEGIN \
    SELECT Num, Code, Pin, GenerateDate, UsedDate, IsUsed, UsedBy, Value \
	FROM TopUpCodesInfo WHERE IsUsed = 1 AND (Code LIKE '%%%1%%%' OR Pin LIKE '%%%2%%%') \
    ORDER BY GenerateDate desc \
END",
        Used, Used);

    nRetCode = m_pDBMan->SearchTopUpCodes(Query, m_vTopUpCodesInfo);

    if (nRetCode == DB_ERROR) 
	{
        MessageBox(_T("Database Error"));
	}
    else
	{
		if (m_vTopUpCodesInfo.size() == 0)
		{
			MessageBox(_T("No Result"));		
		}
		else
		{
			FillData();
		}
		m_strSearch = Query.c_str();
	}	
}

void CTopUpCodeGenerator::SearchUnUsed(int nNum)
{
	if (nNum == 0)
	{
		MessageBox(_T("Input must be numeric type"));
		return;
	}

    int nRetCode;
		
    m_vTopUpCodesInfo.clear();

    std::strstream strTemp;
	strTemp << " SELECT Num, Code, Pin, GenerateDate, UsedDate, IsUsed, UsedBy, Value ";
	strTemp << " From TopUpCodesInfo WHERE IsUsed = 0 and Num=" << nNum;
	strTemp << " ORDER BY GenerateDate desc ";
	strTemp << '\0';

    nRetCode = m_pDBMan->SearchTopUpCodes(strTemp, m_vTopUpCodesInfo);

    if (nRetCode == DB_ERROR) 
	{
        MessageBox(_T("Database Error"));
	}
    else
	{
        if (m_vTopUpCodesInfo.size() == 0)
		{
			MessageBox(_T("No Result"));		
		}
		else
		{
			FillData();
		}
		m_strSearch = strTemp.str();
	}

	strTemp.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
}


void CTopUpCodeGenerator::SearchUnUsed(const CString& strUnUsed)
{
    int nRetCode;
		
    m_vTopUpCodesInfo.clear();

    std::string Used(strUnUsed.GetString());
    sc::string::replaceForDb(Used);

    std::string Query;

    Query = sc::string::format("\
BEGIN \
    SELECT Num, Code, Pin, GenerateDate, UsedDate, IsUsed, UsedBy, Value \
	FROM TopUpCodesInfo WHERE IsUsed = 0 AND (Code LIKE '%%%1%%%' OR Pin LIKE '%%%2%%%') \
    ORDER BY GenerateDate desc \
END",
        Used, Used);

    nRetCode = m_pDBMan->SearchTopUpCodes(Query, m_vTopUpCodesInfo);

    if (nRetCode == DB_ERROR) 
	{
        MessageBox(_T("Database Error"));
	}
    else
	{
        if (m_vTopUpCodesInfo.size() == 0)
		{
			MessageBox(_T("No Result"));		
		}
		else
		{
			FillData();
		}
		m_strSearch = Query.c_str();
	}
}

// 리스트 컨트롤 헤더 초기화
void CTopUpCodeGenerator::InitListHead() 
{
	LVCOLUMN Col;

	// Num, Name, Class, Tribe, Level,	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 50;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= "Num";
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(0, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 120;
	Col.fmt			= LVCFMT_CENTER;		
	Col.pszText		= _T("Used?");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(1, &Col);

	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 120;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Date Generated");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(2, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 30;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Date Used");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(3, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 120;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Code");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(4, &Col);
	
	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 120;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Pin");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(5, &Col);

	Col.mask		= LVCF_TEXT | LVCF_WIDTH;
	Col.cx			= 60;
	Col.fmt			= LVCFMT_CENTER;	
	Col.pszText		= _T("Value");
	Col.iSubItem	= 0;
	Col.iImage		= 0;
	m_ListSearch.InsertColumn(6, &Col);
}

void CTopUpCodeGenerator::FillData()
{
	// Erase all list items
	m_ListSearch.DeleteAllItems();
	
	for (int i=0; i<(int) m_vTopUpCodesInfo.size(); i++)
	{		
        // UserNum, UserID, UserLoginState, UserAvailable, CreateDate, LastLoginDate, UserType
		int nCount; 
		CString strTemp;
		nCount = m_ListSearch.GetItemCount();

		LV_ITEM lvItem;
		::memset(&lvItem, 0, sizeof(LV_ITEM));

		lvItem.mask = LVIF_TEXT; // 속성 지정
		lvItem.iItem = nCount;
		lvItem.iSubItem = 0;
		strTemp.Format("%d", m_vTopUpCodesInfo[i].nNum);
		lvItem.pszText = strTemp.GetBuffer();		
		m_ListSearch.InsertItem(&lvItem); // 새로운 열의 시작은 InsertItem을 사용한다.     

		if (m_vTopUpCodesInfo[i].Used == 1)
		{
			strTemp.Format("Used(%s)", m_vTopUpCodesInfo[i].UsedByUser);
			m_ListSearch.SetItemText(i, 1, strTemp);
		}
		else
			m_ListSearch.SetItemText(i, 1, _T("UnUsed"));

		strTemp.Format("%s", m_vTopUpCodesInfo[i].DateGenerated);
		m_ListSearch.SetItemText(i, 2, strTemp);   

		strTemp.Format("%s", m_vTopUpCodesInfo[i].DateUsed);
		m_ListSearch.SetItemText(i, 3, strTemp);   

		strTemp.Format("%s", m_vTopUpCodesInfo[i].strCode);
		m_ListSearch.SetItemText(i, 4, strTemp);  

		strTemp.Format("%s", m_vTopUpCodesInfo[i].strPIN);
		m_ListSearch.SetItemText(i, 5, strTemp);  

		strTemp.Format("%d", m_vTopUpCodesInfo[i].Value);
		m_ListSearch.SetItemText(i, 6, strTemp);  

	}
}

void CTopUpCodeGenerator::OnBnClickedSearch()
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
	bool bUsed(false);

	bUsed = m_CheckUsed.GetCheck();

    switch (m_nRadio)
    {
    case SEARCH_CODEPINUSER:
		if ( bUsed )
        SearchUsed(strSearch);
		else SearchUnUsed(strSearch);
        break;
    case SEARCH_NUMBER:
		nUserNum = ::atoi(strSearch.GetString());     
		if ( bUsed )
			SearchUsed(nUserNum);
		else SearchUnUsed(nUserNum);
        break;
    default:
        break;
    }
}
void CTopUpCodeGenerator::OnBnClickedRadioUserName()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_nRadio = CTopUpCodeGenerator::SEARCH_CODEPINUSER;
}

void CTopUpCodeGenerator::OnBnClickedRadioUsernum()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
    m_nRadio = CTopUpCodeGenerator::SEARCH_NUMBER;
}

void CTopUpCodeGenerator::OnLvnItemchangedListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CTopUpCodeGenerator::OnNMDblclkListSearch(NMHDR *pNMHDR, LRESULT *pResult)
{	
    int nSelected = m_ListSearch.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	
	if (nSelected != -1) // 선택된 항목이 있음
	{
		if (!OpenClipboard()) 
			return; 
		EmptyClipboard(); 

		CString strCode,strPin,strValue,strCombine;
		CStringW strFinal;

		strCode = m_ListSearch.GetItemText(nSelected,4).GetString();
		strPin = m_ListSearch.GetItemText(nSelected,5).GetString();
		strValue = m_ListSearch.GetItemText(nSelected,6).GetString();

		strCombine.Format(" Code: %s \n Pin: %s \n Value: %s",strCode,strPin,strValue);
		{
			strFinal = strCombine.GetString();
			const wchar_t* output = strFinal;
			const size_t len = (wcslen(output) + 1) * sizeof (wchar_t);
			HGLOBAL hMem =  GlobalAlloc(GMEM_MOVEABLE, len);
			memcpy(GlobalLock(hMem), output, len );
			GlobalUnlock(hMem);
			OpenClipboard();
			EmptyClipboard();
			SetClipboardData(CF_UNICODETEXT, hMem);
			CloseClipboard();
			MessageBox(strCombine,"Copy Success!",MB_OK);
		}
	}

    *pResult = 0;
}

void CTopUpCodeGenerator::OnBnClickedGenerateCodes()
{
	int nLoop(0),nValue(0);
	CString strTemp;
	m_EditGenNum.GetWindowText(strTemp);
	nLoop = atoi(strTemp.GetString());
	if ( nLoop <= 0 ) return;
	m_EditGenValue.GetWindowText(strTemp);
	nValue = atoi(strTemp.GetString());
	if ( nValue < 100 ) return;

	strTemp.Format ( "You are about to Generate %d TopUP Codes Value : %d",nLoop,nValue );

	int Count(0);

	if ( MessageBox ( strTemp, "Question", MB_YESNO ) == IDYES )
	{
		for ( int i=0; i<nLoop; ++i )
		{
			strTemp.Empty();
			TopUpCodesInfo sData;
			sData.Value = nValue;
			std::string strTemp2;

			CTime Time = CTime::GetCurrentTime();

			CString strCo1,strCo2,strCo3;
			strCo1.Format ( "%d%d", Time.GetYear(), Time.GetMonth());
			strCo2.Format ( "%d%d", Time.GetDay(),Time.GetHour() );
			strCo3.Format ( "%d%d%d", Time.GetMinute(), Time.GetSecond(), Time.GetDayOfWeek() );

			// 4 random letters
			sc::string::generateRandomString(strTemp,3,0,1,0);
			strTemp2 += strTemp;
			strTemp.Empty();

			strTemp2 += strCo2;

			strTemp2 += "-";

			// 5 random char with random number
			sc::string::generateRandomString(strTemp,5,0,1,1);
			strTemp2 += strTemp;
			strTemp.Empty();

			strCo3 += strCo3;
			strTemp2 += "-";

			// 3 random char with random number
			sc::string::generateRandomString(strTemp,3,0,1,1);
			strTemp2 += strTemp;
			strTemp.Empty();

			transform(strTemp2.begin(),strTemp2.end(),strTemp2.begin(),toupper);
			sData.strCode = strTemp2.c_str();

			strTemp2 = "";
			sc::string::generateRandomString(strTemp,12,0,1,1);

			strTemp2 += strTemp;
			transform(strTemp2.begin(),strTemp2.end(),strTemp2.begin(),toupper);
			sData.strPIN = strTemp2.c_str();
			strTemp.Empty();

			int nResult = m_pDBMan->InsertTopUpCodes(&sData);
			if ( nResult == DB_OK )
			{
				Count++;
				CString str ="";
				str.Format("Code: %s\n Pin: %s \nValue:(%d)",sData.strCode.GetString(),sData.strPIN.GetString(),sData.Value);

				CString strUserIP = m_pDBMan->GetLocalIP();
				CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
				int nGmNum = pFrame->m_nUserNum;

				sc::DebugSet::GetInstance()->LogToFileWithTime(sc::string::format("Generated Code By UserNum %d IP %s\n%s\n",nGmNum,strUserIP.GetString(),str.GetString()));
			}
		}
		if ( Count > 0 )
		{

			///////////////////////////////////////////////////////////////////////////
			// GM Log
			CString strUserIP = m_pDBMan->GetLocalIP();
			CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
			int nGmNum = pFrame->m_nUserNum;
			std::strstream strGmCmd;
			strGmCmd << "Generated Top up %d Codes Value %d" << nLoop , nValue;
			strGmCmd << '\0';
			m_pDBMan->WriteGMLog(nGmNum, strGmCmd, strUserIP);
			strGmCmd.freeze( false );	// Note : std::strstream의 freeze. 안 하면 Leak 발생.
			///////////////////////////////////////////////////////////////////////////

			strTemp.Format ( "Successfully Generated %d Codes",Count );
			MessageBox ( strTemp, "Question", MB_OK );
		}
	}
}

void CTopUpCodeGenerator::OnNMRclickListSearch(NMHDR *pNMHDR, LRESULT *pResult)
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

void CTopUpCodeGenerator::OnContext1Makecroncharacter()
{
	//// TODO: 여기에 명령 처리기 코드를 추가합니다.
	//CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();
	//if (pFrame->m_nUserLevel < USER_MASTER)
	//{
	//	MessageBox(_T("Permission Denied"));
	//	return;
	//}

	//int nSelected = m_ListSearch.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED);
	//
	//if (nSelected != -1) // 선택된 캐릭터가 있음
	//{
	//	int nChaNum = 
	//	int nUserNum = 0; // 사용자번호
	//	CString strChaName;

	//	GLCHARLOGIC* pChar = new GLCHARLOGIC;

	//	if (m_pDBMan->GetCharacterInfo(nChaNum, m_vTopUpCodesInfo[nSelected].UserNum, pChar) == DB_ERROR)
	//	{			
	//		MessageBox(_T("Database Error"));
	//	}
	//	else if (m_pDBMan->GetUserInven(pChar) == DB_ERROR)
	//	{		
	//		MessageBox(_T("Database Error"));
	//	}
	//	else
	//	{
	//		// 사용자번호 및 캐릭터 이름을 설정한다.
	//		CDlgMakeCronCharacter dlgTemp;			
	//		if (dlgTemp.DoModal() == IDOK)
	//		{
	//			// 캐릭터를 생성한다.
	//			// 캐릭터 초기 수치 설정.
	//			nUserNum   = dlgTemp.m_nUserNum;
	//			strChaName = dlgTemp.m_strChaName;

	//			GLCHARLOGIC NewCharLogic;
	//			EMCHARINDEX emIndex = CharClassToIndex(pChar->m_emClass);
	//			NewCharLogic.INIT_NEW_CHAR((EMCHARINDEX)emIndex, (DWORD) nUserNum, (DWORD)m_vTopUpCodesInfo[nSelected].SGNum, strChaName.GetString(), pChar->m_wSchool, pChar->m_wHair, pChar->m_wFace, pChar->m_wHairColor, pChar->m_wSex);

	//			// 캐릭터 생성
 //               CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd(); // 서버 번호
	//			int nNewChaNum = m_pDBMan->CreateNewCharacter2(&NewCharLogic, pFrame->m_nSvrGrp);

	//			if (nNewChaNum == DB_ERROR)
	//			{
	//				MessageBox("Make character failed. check amount of user character / character name");
	//			}
	//			// 생성된 캐릭터에 이전 캐릭터 데이타를 복제해서 저장한다.
	//			pChar->m_CharDbNum = (DWORD) nNewChaNum;

	//			if (nNewChaNum > 0)
	//			{
	//				pChar->m_sSaveMapID = NATIVEID_NULL();
	//				m_pDBMan->SaveCharacterBase     (pChar);
	//				m_pDBMan->SaveCharacterInven    ( pChar, true );
	//				m_pDBMan->SaveCharacterPutOnItem( pChar, true );
	//				m_pDBMan->SaveCharacterSkill    (pChar);
	//				m_pDBMan->SaveCharacterQuest    (pChar);
	//				m_pDBMan->SaveCharacterSkillQuickSlot(pChar);
	//				m_pDBMan->SaveCharacterActionQuickSlot(pChar);
	//				m_pDBMan->SaveCharacterSkillFact(pChar);
	//				// 인벤토리 저장은 제외했음
	//				// m_pDBMan->SaveCharacterUserInven(pChar);

 //                   //! 기본 정보를 복사 한 후에 추가로 복사가 필요한 부분
 //                   m_pDBMan->CopyCharacterProductInfo( nChaNum, nNewChaNum );

	//			}
	//		}
	//	}
	//	delete pChar;
	//}
}

void CTopUpCodeGenerator::OnNMCustomdrawListSearch(NMHDR *pNMHDR, LRESULT *pResult)
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

void CTopUpCodeGenerator::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFormView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

void CTopUpCodeGenerator::OnSize(UINT nType, int cx, int cy)
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
