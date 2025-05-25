#pragma once
#include "resource.h"
#include "../EngineLib/G-Logic/GLDefine.h"
#include "../RanLogicServer/Server/s_CFieldServer.h"
#include "../RanLogicServer/FieldServer/GLGaeaServer.h"

class CFieldServerPostSender :
	public CDialog
{
	DECLARE_DYNAMIC(CFieldServerPostSender)
public:
	CFieldServerPostSender(CWnd* pParent, CFieldServer* pServer);
	virtual ~CFieldServerPostSender();

	enum { IDD = IDD_DIALOG_POST_SENDER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSend();
	afx_msg void OnBnClickedButtonFindItem();

	SNATIVEID sitemid;
	int ChaNum;
	std::string strChaName;
	std::string strContent;
	CFieldServer*		g_pServer;
};
