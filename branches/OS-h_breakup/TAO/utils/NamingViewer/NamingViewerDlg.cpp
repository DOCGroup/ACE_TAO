// $Id$
// NamingViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NamingViewer.h"
#include "NamingViewerDlg.h"
#include "ViewIORDialog.h"
#include "ORBSvcs/Naming/Naming_Utils.h"
#include "SelectNSDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamingViewerDlg dialog

CNamingViewerDlg::CNamingViewerDlg(CORBA::ORB_ptr pORB, CWnd* pParent /*=NULL*/)
	: CDialog(CNamingViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNamingViewerDlg)
	m_Server = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
  
  m_pORB = pORB;

}

void CNamingViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNamingViewerDlg)
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Text(pDX, IDC_SERVER, m_Server);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNamingViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CNamingViewerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECT_NS, OnSelectNs)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
  ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamingViewerDlg message handlers

BOOL CNamingViewerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_Tree.SetpORB(m_pORB);
  
  // Initialize the naming client and get the root context
  TAO_Naming_Client Client;
  if(!Client.init(m_pORB))
  {
    m_RootContext = Client.get_context();
    m_Server = "Default";
    // For debugging, sets up some initial contexts in the NS
    /*
    CosNaming::Name Name;
    Name.length(1);
    Name[0].id = CORBA::string_dup("test");
    CORBA::Object_var Object = CORBA::Object::_nil();
    m_RootContext->rebind(Name, Object);
    CosNaming::NamingContext_var Context = m_RootContext->new_context();
    Name[0].id = CORBA::string_dup("test context");
    m_RootContext->rebind_context(Name, Context);
    Context->rebind_context(Name, Context);
    */
  }
  Resolve();
  UpdateData(FALSE);
  return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNamingViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNamingViewerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNamingViewerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNamingViewerDlg::Resolve()
{
  m_Tree.Resolve(m_RootContext);
}

void CNamingViewerDlg::OnSelectNs() 
{
	// TODO: Add your control notification handler code here
	CSelectNSDialog Dialog;
  if(Dialog.DoModal() != IDOK)
  {
    return;
  }
  try
  {
    CORBA::Object_var Object = m_pORB->string_to_object(Dialog.GetIOR());
    m_RootContext = CosNaming::NamingContext::_narrow(Object);
    m_Server = Dialog.GetName();
    UpdateData(FALSE);
    Resolve();
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

void CNamingViewerDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
  
  // Resize the tree control
  if(m_Tree.m_hWnd)
  {
    CRect Size; 
    m_Tree.GetWindowRect(Size);
    ScreenToClient(Size);
    int Border = Size.left;
    Size.right = cx - Border;
    Size.bottom = cy - Border;
    m_Tree.MoveWindow(Size, TRUE);
  }
}

void CNamingViewerDlg::OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI )
{
  // Prevent the window from being resized too small
  CWnd* pSelect = GetDlgItem(IDC_NS);
  if(pSelect)
  {
    CRect Size;
    pSelect->GetWindowRect(Size);
    ScreenToClient(Size);
    lpMMI->ptMinTrackSize.x = Size.right + Size.left + 7;
    lpMMI->ptMinTrackSize.y = 200;
  }
}
