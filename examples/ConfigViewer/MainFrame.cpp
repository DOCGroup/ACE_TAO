// $Id$
#include "stdafx.h"
#include "MainFrame.h"
#include "ConfigTreeCtrl.h"
#include "ValueListCtrl.h"

// Singleton
MainFrame* MainFrame::m_pInstance = 0;

// IDs for the controls and the menu commands

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    EVT_MENU(FILE_NEW_PERSISTENT_HEAP, MainFrame::OnFileNewPersistentHeap)
    EVT_MENU(FILE_NEW_TRANSIENT_HEAP, MainFrame::OnFileNewTransientHeap)
    EVT_MENU(FILE_OPEN_PERSISTENT_HEAP, MainFrame::OnFileOpenPersistentHeap)
    EVT_MENU(FILE_OPEN_REGISTRY, MainFrame::OnFileOpenRegistry)
    EVT_MENU(FILE_EXPORT, MainFrame::OnFileExport)
    EVT_MENU(FILE_IMPORT, MainFrame::OnFileImport)
    EVT_MENU(QUIT,  MainFrame::OnQuit)
    EVT_MENU(ABOUT, MainFrame::OnAbout)
END_EVENT_TABLE()

// frame constructor
MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame((wxFrame *)0, -1, title, pos, size),
  m_pConfig(0)
{
  m_pInstance = this;

  // Create a persistent heap based configuration

  ACE_Configuration_Heap* pHeapConfig = new ACE_Configuration_Heap;
  pHeapConfig->open();
  m_pConfig = pHeapConfig;

  // set the frame icon
  SetIcon(wxICON(mondrian));

  // Create Splitter
  m_pSplitter = new wxSplitterWindow(this, -1);
  wxSize sz( m_pSplitter->GetSize() );
  sz.SetWidth(sz.GetWidth() / 2);

  // List Control
  m_pListCtrl = new ValueListCtrl(m_pSplitter, -1, wxDefaultPosition, sz);

  // Tree Control
  m_pTreeCtrl = new ConfigTreeCtrl(m_pSplitter, FRAME_TREE, wxDefaultPosition, sz,
                  wxTR_EDIT_LABELS | wxTR_HAS_BUTTONS | wxTR_LINES_AT_ROOT);
  m_pTreeCtrl->SetpListCtrl(m_pListCtrl);


  // Setup splitter
  m_pSplitter->SplitVertically(m_pTreeCtrl, m_pListCtrl);
  m_pSplitter->SetMinimumPaneSize(100);
  m_pSplitter->SetSashPosition(size.GetWidth() / 3);

  // create a menu bar
  wxMenu *menuFile = new wxMenu("", wxMENU_TEAROFF);
  menuFile->Append(FILE_NEW_PERSISTENT_HEAP, "New Persistent Heap", "Create a new persistent heap");
  menuFile->Append(FILE_NEW_TRANSIENT_HEAP, "New Transient Heap", "Create a new transient heap");
  menuFile->Append(FILE_OPEN_PERSISTENT_HEAP, "Open Persistent Heap", "Open Persistent Heap");
#if defined (ACE_WIN32)
  menuFile->Append(FILE_OPEN_REGISTRY, "Open Win32 Registry", "Open Win32 Registry");
#endif
  menuFile->AppendSeparator();
  menuFile->Append(FILE_IMPORT, "Import from INI file", "Import from INI file");
  menuFile->Append(FILE_EXPORT, "Export to INI file", "Export to INI file");
  menuFile->AppendSeparator();
  menuFile->Append(ABOUT, "&About...\tCtrl-A", "Show about dialog");
  menuFile->AppendSeparator();
  menuFile->Append(QUIT, "E&xit\tAlt-X", "Quit this program");

  // now append the freshly created menu to the menu bar...
  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(menuFile, "&File");

  // ... and attach this menu bar to the frame
  SetMenuBar(menuBar);

#if wxUSE_STATUSBAR
  CreateStatusBar(2);
  SetStatusText("Ready");
#endif // wxUSE_STATUSBAR
}


MainFrame::~MainFrame()
{
  delete m_pConfig;
  m_pInstance = 0;
}

MainFrame* MainFrame::Instance()
{
  assert(m_pInstance);
  return m_pInstance;
}


// event handlers

void MainFrame::OnSize(wxSizeEvent& event)
{
  wxLayoutAlgorithm layout;
  layout.LayoutFrame(this, m_pListCtrl);
}


void MainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  // TRUE is to force the frame to close
  Close(TRUE);
}

void MainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  wxString msg;
  msg.Printf( _T("Configuration Viewer v1.0\nWritten by Chris Hafey (chris@stentorsoft.com)\n"));
  wxMessageBox(msg, "About", wxOK | wxICON_INFORMATION, this);
}

void MainFrame::OnFileNewPersistentHeap(wxCommandEvent& event)
{
  wxFileDialog Dlg(this, "Enter Filename:", "", "", "*.*", 0);
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }
  delete m_pConfig;
  ACE_Configuration_Heap* pHeapConfig = new ACE_Configuration_Heap;
  pHeapConfig->open(Dlg.GetFilename());
  SetNewConfig(pHeapConfig);
}

void MainFrame::OnFileNewTransientHeap(wxCommandEvent& event)
{
  delete m_pConfig;
  ACE_Configuration_Heap* pHeapConfig = new ACE_Configuration_Heap;
  pHeapConfig->open();
  SetNewConfig(pHeapConfig);
}

void MainFrame::OnFileOpenPersistentHeap(wxCommandEvent& event)
{
  wxFileDialog Dlg(this, "Choose a file", "", "", "*.*", wxOPEN);
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }
  delete m_pConfig;
  ACE_Configuration_Heap* pHeapConfig = new ACE_Configuration_Heap;
  pHeapConfig->open(Dlg.GetFilename());
  SetNewConfig(pHeapConfig);
}

void MainFrame::OnFileOpenRegistry(wxCommandEvent& event)
{
#if defined (ACE_WIN32)
  wxTextEntryDialog Dlg(this, "Enter Root:");
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }
  HKEY Root = ACE_Configuration_Win32Registry::resolve_key(HKEY_LOCAL_MACHINE, Dlg.GetValue(), 0);
  ACE_Configuration_Win32Registry* pWin32Reg = new ACE_Configuration_Win32Registry(Root);;
  delete m_pConfig;
  SetNewConfig(pWin32Reg);
#endif
}

void MainFrame::OnFileExport(wxCommandEvent& event)
{
  wxFileDialog Dlg(this, "Enter Filename:", "", "", "*.*",0);
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }
  m_pConfig->export_config(Dlg.GetFilename());
}

void MainFrame::OnFileImport(wxCommandEvent& event)
{
  wxFileDialog Dlg(this, "Choose a file", "", "", "*.*", wxOPEN);
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }
  m_pConfig->import_config(Dlg.GetFilename());
  SetNewConfig(m_pConfig);
}

void MainFrame::SetNewConfig(ACE_Configuration* pConfig)
{
  m_pConfig = pConfig;
  m_pListCtrl->ChangeConfig(pConfig);
  m_pTreeCtrl->ChangeConfig(pConfig);
}

