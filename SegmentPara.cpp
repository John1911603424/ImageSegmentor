// SegmentPara.cpp : implementation file
//

#include "stdafx.h"
#include "myimagedb.h"
#include "SegmentPara.h"
#include "newclassnamedlg.h"
#include "mainfrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSegmentPara dialog


CSegmentPara::CSegmentPara(CWnd* pParent /*=NULL*/)
	: CDialog(CSegmentPara::IDD, pParent)
{
	newAddName = "";
    isSelectChange = isAddSelect = isRemoveSelect 
		= isAddNewClass = isDeleteClass = FALSE;
	//{{AFX_DATA_INIT(CSegmentPara)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSegmentPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSegmentPara)
	DDX_Control(pDX, IDC_SELECTLIST, mySelectClass);
	DDX_Control(pDX, IDC_EXISTCLASS, myExistClass);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSegmentPara, CDialog)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CSegmentPara)
	ON_BN_CLICKED(IDC_NEW, OnNew)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_LBN_SELCHANGE(IDC_EXISTCLASS, OnSelchangeExistclass)
	ON_LBN_SELCHANGE(IDC_SELECTLIST, OnSelchangeSelectlist)
	ON_WM_DESTROY()
	ON_COMMAND(ID_ADDALL, OnAddall)
	ON_COMMAND(ID_CLEARALL, OnClearall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSegmentPara message handlers

void CSegmentPara::OnNew() 
{
	CNewClassNameDlg mynewclassnamedlg;
	mynewclassnamedlg.DoModal();
	newAddName = mynewclassnamedlg.myClassName;
	if (newAddName!="")
	{
		myExistClass.AddString(newAddName);
	}
	//����֪ͨMAINFRM���¼���
	//��Ȼ��������Ķ����ԣ�������ûʲô��ĺð취��
    CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
	isAddNewClass = TRUE;
	pMainFrame->OnSegParaChanged();
}

void CSegmentPara::AddExistClass(CString className)
//�ڡ��ִ��ࡱ�б�������
{
	INT findpos = mySelectClass.FindString(0, className);
	if (findpos>=0)
	{
		return;
	}
	myExistClass.AddString(className);
}

void CSegmentPara::OnDelete() 
{
	INT sel = myExistClass.GetCurSel();
	if (sel<0)
	{
		return;
	}

	CString selstr = "";
	myExistClass.GetText(sel, selstr);
	myExistClass.DeleteString(sel);
	//����֪ͨMAINFRM���¼���
	//��Ȼ��������Ķ����ԣ�������ûʲô��ĺð취��
    CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
	isDeleteClass = TRUE;
	pMainFrame->OnSegParaChanged();
}

void CSegmentPara::OnAdd() 
{
	INT sel = myExistClass.GetCurSel();
	INT selcount = myExistClass.GetSelCount();
	INT * selitems = new INT[selcount];
    myExistClass.GetSelItems(selcount, selitems);

	for (INT i=0; i<selcount; i++)
	{
		CString selstr = "";
		myExistClass.GetText(selitems[i], selstr);
		mySelectClass.AddString(selstr);
	}

	delete [] selitems;
	selitems = NULL;
	
	//����֪ͨMAINFRM���¼���
	//��Ȼ��������Ķ����ԣ�������ûʲô��ĺð취��
    CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
	isAddSelect = TRUE;
	pMainFrame->OnSegParaChanged();
}

void CSegmentPara::OnRemove() 
{
    //����̫���ˣ�һ��Ҫ����ɾ��β���ɾ�ɾ���
	while (mySelectClass.GetSelCount()>0)
	{
		INT selcount = mySelectClass.GetSelCount();
		INT * selitems = new INT[selcount];
		mySelectClass.GetSelItems(selcount, selitems);
		
		for (INT i=0; i<selcount; i++)
		{
			mySelectClass.DeleteString(selitems[i]);
		}
		delete [] selitems;
		selitems = NULL;
	}	

	mySelectClass.SetCurSel(-1);
	mySelectClass.RedrawWindow();
	//����֪ͨMAINFRM���¼���
	//��Ȼ��������Ķ����ԣ�������ûʲô��ĺð취��
    CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
	isRemoveSelect = TRUE;
	pMainFrame->OnSegParaChanged();
}

CString   CSegmentPara::GetClassToAddPoints()
//�õ���ǰѡ����ִ���࣬׼������Ϊ�������ѵ���㣻
{
	INT sel = myExistClass.GetCurSel();
	if (sel<0)
	{
		return "";
	}

	CString selstr = "";
	myExistClass.GetText(sel, selstr);

	return selstr;	
}

CString*  CSegmentPara::GetSelectClass(INT& count, INT& selIndex)
//�õ���ǰѡ������
{
	count = mySelectClass.GetCount();
	selIndex = mySelectClass.GetCurSel();
	if (count<=0)
	{
		return NULL;
	}

	CString* outitems = new CString[count];

	for (INT i=0; i<count; i++)
	{
		mySelectClass.GetText(i, outitems[i]);
	}

	return outitems;
}

void CSegmentPara::OnSelchangeExistclass() 
{
	//����֪ͨMAINFRM���¼���
	//��Ȼ��������Ķ����ԣ�������ûʲô��ĺð취��
    CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
	isExistChange = TRUE;
	pMainFrame->OnSegParaChanged();
}

INT CSegmentPara::GetCurEvent()
//�õ���ǰ�������¼���
{
	if (isAddNewClass)
	{
		return 0;
	}
	if (isAddSelect)
	{
		return 1;
	}
	if (isDeleteClass)
	{
		return 2;
	}
	if (isExistChange)
	{
		return 3;
	}
	if (isRemoveSelect)
	{
		return 4;
	}
	if (isSelectChange)
	{
		return 5;
	}

	return -1;//δ�����κ��¼���
}

void CSegmentPara::ClearEvent()
//���¼���ǣ�
{
    isExistChange = isSelectChange = isAddSelect 
		= isRemoveSelect = isAddNewClass = isDeleteClass 
		= FALSE;
}

void CSegmentPara::OnSelchangeSelectlist() 
{
	//����֪ͨMAINFRM���¼���
	//��Ȼ��������Ķ����ԣ�������ûʲô��ĺð취��
    CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
	isSelectChange = TRUE;
	pMainFrame->OnSegParaChanged();	
}

CString CSegmentPara::GetExistListSelect()
//�õ��ִ��б��е�ѡ���
{
	INT sel = myExistClass.GetCurSel();
	if (sel<0)
	{
		return "";
	}

	CString selstr = "";
	myExistClass.GetText(sel, selstr);

	return selstr;
}

void CSegmentPara::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here	
}

void CSegmentPara::OnContextMenu(CWnd*, CPoint point)
{

	// CG: This block was added by the Pop-up Menu component
	{
		if (point.x == -1 && point.y == -1){
			//keystroke invocation
			CRect rect;
			GetClientRect(rect);
			ClientToScreen(rect);

			point = rect.TopLeft();
			point.Offset(5, 5);
		}

		CMenu menu;
		VERIFY(menu.LoadMenu(CG_IDR_POPUP_SEGMENT_PARA));

		CMenu* pPopup = menu.GetSubMenu(0);
		ASSERT(pPopup != NULL);
		CWnd* pWndPopupOwner = this;

		while (pWndPopupOwner->GetStyle() & WS_CHILD)
			pWndPopupOwner = pWndPopupOwner->GetParent();

		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);
	}
}

void CSegmentPara::OnAddall() 
{
	INT sel = myExistClass.GetCurSel();
	INT allcount = myExistClass.GetCount();
	
	for (sel=0; sel<allcount; sel++)
	{
		CString selstr = "";
		myExistClass.GetText(sel, selstr);
		
		INT findpos = mySelectClass.FindString(0, selstr);
		if (findpos>=0)
		{
			return;
		}
		mySelectClass.AddString(selstr);
	}
	
	//����֪ͨMAINFRM���¼���
	//��Ȼ��������Ķ����ԣ�������ûʲô��ĺð취��
    CMainFrame* pMainFrame = reinterpret_cast<CMainFrame*>(AfxGetMainWnd());
	isAddSelect = TRUE;
	pMainFrame->OnSegParaChanged();	
}

void CSegmentPara::OnClearall() 
{
    mySelectClass.ResetContent();	
}
