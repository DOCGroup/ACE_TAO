/* -*- C++ -*- */
// $Id$

#ifndef _ConfigurationViewer_ValueListCtrl_H
#define _ConfigurationViewer_ValueListCtrl_H

class ValueListCtrl : public wxListCtrl
{
public:
  ///////////////////////////////////////////
  // Initializers
  ///////////////////////////////////////////
  ValueListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = wxLC_ICON,
        const wxValidator& validator = wxDefaultValidator,
        const wxString& name = "listCtrl");
  virtual ~ValueListCtrl();

  ///////////////////////////////////////////
  // Methods
  ///////////////////////////////////////////
  void DisplaySection(const ACE_Configuration_Section_Key& Key);
  long GetSelectedItem();
  void SelectItem(long ItemID);
  void OnRightDown(wxMouseEvent& event);
  void OnModify(wxCommandEvent& event);
  void OnDelete(wxCommandEvent& event);
  void OnRename(wxCommandEvent& event);
  void ChangeConfig(ACE_Configuration* pConfig);
  ///////////////////////////////////////////
  // Attribute Accessors
  ///////////////////////////////////////////

protected:
  // Not Used
  ValueListCtrl(const ValueListCtrl& RHS);
  const ValueListCtrl& operator=(const ValueListCtrl& RHS);

  DECLARE_EVENT_TABLE()
private:

  ACE_Configuration*              m_pConfig;
  ACE_Configuration_Section_Key   m_Key;
};

#endif

