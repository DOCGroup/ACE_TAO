// $Id$

// serverDoc.h : interface of the CServerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERDOC_H__7B1C2A4C_7230_11D3_B94A_0010A4052E2A__INCLUDED_)
#define AFX_SERVERDOC_H__7B1C2A4C_7230_11D3_B94A_0010A4052E2A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CServerDoc : public CDocument
{
protected: // create from serialization only
	CServerDoc();
	DECLARE_DYNCREATE(CServerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CServerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CServerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERDOC_H__7B1C2A4C_7230_11D3_B94A_0010A4052E2A__INCLUDED_)
