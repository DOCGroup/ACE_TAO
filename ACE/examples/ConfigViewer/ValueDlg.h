/* -*- C++ -*- */
#ifndef _ConfigurationViewer_ValueDlg_H
#define _ConfigurationViewer_ValueDlg_H

class ValueDlg : public wxDialog
{
public:
  /// @name Initializers
  /// @{
  /// New Value Ctor
  ValueDlg(wxWindow* pParent, bool String);
  /// Edit String Ctor
  ValueDlg(wxWindow* pParent, wxString const &Name, wxString& Value);
  /// Edit UINT Ctor
  ValueDlg(wxWindow* pParent, wxString const &Name, u_int Value);
  virtual ~ValueDlg();
  /// @}

  /// @name Methods
  /// @{
  const wxString& GetName();
  const wxString& GetStringValue();
  u_int GetUINTValue();
  /// @}

  /// @name Attributes
  /// @{
  wxButton*       m_pOK;
  wxButton*       m_pCancel;
  wxStaticText*   m_pName;
  wxTextCtrl*     m_pNameText;
  wxStaticText*   m_pValue;
  wxTextCtrl*     m_pValueText;
  /// @}
protected:
  /// Not Used
  ValueDlg(const ValueDlg& RHS);
  const ValueDlg& operator=(const ValueDlg& RHS);

  wxString    m_Name;
  wxString    m_Value;
  u_int       m_UINTValue;

private:
};

#endif

