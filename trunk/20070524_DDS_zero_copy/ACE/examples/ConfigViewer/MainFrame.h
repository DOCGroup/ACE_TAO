/* -*- C++ -*- */
// $Id$

#ifndef _ConfigurationViewer_MainFrame_H
#define _ConfigurationViewer_MainFrame_H

class ConfigTreeCtrl;
class ValueListCtrl;

enum
{
    // menu items
    QUIT = 1,
    ABOUT,
    FILE_NEW_PERSISTENT_HEAP,
    FILE_NEW_TRANSIENT_HEAP,
    FILE_OPEN_PERSISTENT_HEAP,
    FILE_OPEN_REGISTRY,
    FILE_EXPORT,
    FILE_IMPORT,
    LEFT_SASH,
    FRAME_TREE
};


class MainFrame : public wxFrame
{
public:
  ///////////////////////////////////////////
  // Initializers
  ///////////////////////////////////////////
  MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
  virtual ~MainFrame();

  ///////////////////////////////////////////
  // Methods
  ///////////////////////////////////////////
  static MainFrame* Instance();
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
  void OnFileNewPersistentHeap(wxCommandEvent& event);
  void OnFileNewTransientHeap(wxCommandEvent& event);
  void OnFileOpenPersistentHeap(wxCommandEvent& event);
  void OnFileOpenRegistry(wxCommandEvent& event);
  void OnFileExport(wxCommandEvent& event);
  void OnFileImport(wxCommandEvent& event);
  void OnSize(wxSizeEvent& event);

  ///////////////////////////////////////////
  // Attribute Accessors
  ///////////////////////////////////////////
  ACE_Configuration* GetpConfig() {return m_pConfig;};
protected:
  // Not Used
  MainFrame(const MainFrame& RHS);
  const MainFrame& operator=(const MainFrame& RHS);

  // Operations
  void SetNewConfig(ACE_Configuration* pConfig);

  // Attributes
  wxSplitterWindow*         m_pSplitter;
  ConfigTreeCtrl*           m_pTreeCtrl;
  ValueListCtrl*            m_pListCtrl;
private:
  DECLARE_EVENT_TABLE()
  ACE_Configuration*        m_pConfig;
  static MainFrame*         m_pInstance;
};

#endif

