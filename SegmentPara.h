#if !defined(AFX_SEGMENTPARA_H__D2E26113_0297_4A6E_A47E_0C1078F3C9FD__INCLUDED_)
#define AFX_SEGMENTPARA_H__D2E26113_0297_4A6E_A47E_0C1078F3C9FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SegmentPara.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSegmentPara dialog

class CSegmentPara : public CDialog
{
// Construction
public:
	CSegmentPara(CWnd* pParent = NULL);   // standard constructor
    void  AddExistClass(CString className);//�ڡ��ִ��ࡱ�б�������
	CString   GetClassToAddPoints();//�õ���ǰѡ����ִ���࣬׼������Ϊ�������ѵ���㣻
	CString*  GetSelectClass(INT& count, INT& selIndex);//�õ���ǰѡ�����ڷָ������, �����������飬count��Ϊ�����е�Ԫ�ظ���, selIndexΪ��ǰѡ�������ţ�

	INT GetCurEvent();//�õ���ǰ�������¼���
	void ClearEvent();//���¼���ǣ�
	CString GetExistListSelect();//�õ��ִ��б��е�ѡ���

// Dialog Data
	//{{AFX_DATA(CSegmentPara)
	enum { IDD = IDD_SEARCHPARA };
	CListBox	mySelectClass;
	CListBox	myExistClass;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSegmentPara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	afx_msg void OnContextMenu(CWnd*, CPoint point);
	CString newAddName;
	BOOL isSelectChange, isExistChange, isAddSelect, isRemoveSelect, isAddNewClass, isDeleteClass;

	// Generated message map functions
	//{{AFX_MSG(CSegmentPara)
	afx_msg void OnNew();
	afx_msg void OnDelete();
	afx_msg void OnAdd();
	afx_msg void OnRemove();
	afx_msg void OnSelchangeExistclass();
	afx_msg void OnSelchangeSelectlist();
	afx_msg void OnDestroy();
	afx_msg void OnAddall();
	afx_msg void OnClearall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEGMENTPARA_H__D2E26113_0297_4A6E_A47E_0C1078F3C9FD__INCLUDED_)
