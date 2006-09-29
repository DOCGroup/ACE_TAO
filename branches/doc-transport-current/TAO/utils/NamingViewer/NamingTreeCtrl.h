// $Id$
#if !defined(AFX_NAMINGTREECTRL_H__1F34ADC2_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_)
#define AFX_NAMINGTREECTRL_H__1F34ADC2_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NamingTreeCtrl.h : header file
//
#include "orbsvcs/CosNamingC.h"
class CNamingObject;

/////////////////////////////////////////////////////////////////////////////
// CNamingTreeCtrl window

class CNamingTreeCtrl : public CTreeCtrl
{
// Construction
public:
  CNamingTreeCtrl();

// Attributes
public:
  void SetpORB(CORBA::ORB_ptr pORB);
// Operations
public:
  CNamingObject* GetTreeObject(HTREEITEM hItem = NULL);
  void ClearChildren(HTREEITEM hItem = NULL);
  void ListContext(HTREEITEM hItem);
  void Resolve(CosNaming::NamingContext_ptr pRootContext);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamingTreeCtrl)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNamingTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CNamingTreeCtrl)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnContextPopupViewreference();
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextPopupRefresh();
	afx_msg void OnContextPopupUnbind();
	afx_msg void OnContextPopupDestroy();
	afx_msg void OnContextPopupBindContext();
	afx_msg void OnContextPopupBindobject();
	afx_msg void OnDestroy();
	afx_msg void OnContextpopupBindnewcontext();
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnObjectpopupUnbind();
	afx_msg void OnObjectpopupViewrefrence();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

  void Refresh(HTREEITEM hItem);
  void OnCopy();
  void ListBindingList(HTREEITEM hItem, CosNaming::NamingContext_ptr pContext, CosNaming::BindingList_var& bl);
  CMenu               m_ContextPopup;
  CMenu               m_ObjectPopup;
  CORBA::ORB_ptr      m_pORB;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMINGTREECTRL_H__1F34ADC2_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_)
