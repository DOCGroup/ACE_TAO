/* -*- C++ -*- */
// $Id$

#ifndef _ConfigurationViewer_ConfigTreeCtrl_H
#define _ConfigurationViewer_ConfigTreeCtrl_H

class ValueListCtrl;

class ConfigTreeCtrl : public wxTreeCtrl
{
public:
  ///////////////////////////////////////////
  // Initializers
  ///////////////////////////////////////////
  ConfigTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxTR_HAS_BUTTONS, const wxValidator& validator = wxDefaultValidator, const wxString& name = "listCtrl");
  virtual ~ConfigTreeCtrl();

  ///////////////////////////////////////////
  // Methods
  ///////////////////////////////////////////
  void LoadTree();
  void OnRightDown(wxMouseEvent& event);
  void OnRightUp(wxMouseEvent& event);
  void OnNewKey(wxCommandEvent& event);
  void OnNewString(wxCommandEvent& event);
  void OnNewUINT(wxCommandEvent& event);
  void OnNewBinary(wxCommandEvent& event);
  void OnFind(wxCommandEvent& event);
  void OnDelete(wxCommandEvent& event);
  void OnSelChanged(wxTreeEvent& event);
  void ChangeConfig(ACE_Configuration* pConfig);
  ///////////////////////////////////////////
  // Attribute Accessors
  ///////////////////////////////////////////
  void SetpListCtrl(ValueListCtrl* pListCtrl) {m_pListCtrl = pListCtrl;};

protected:
  // Not Used
  ConfigTreeCtrl(const ConfigTreeCtrl& RHS);
  const ConfigTreeCtrl& operator=(const ConfigTreeCtrl& RHS);

  void LoadSection(wxTreeItemId& ParentItem, const ACE_Configuration_Section_Key& Key);
  void ResolveKey(wxTreeItemId Item, ACE_Configuration_Section_Key& Key);
private:
  DECLARE_EVENT_TABLE()

  ACE_Configuration*    m_pConfig;
  ValueListCtrl*        m_pListCtrl;
};

#endif

