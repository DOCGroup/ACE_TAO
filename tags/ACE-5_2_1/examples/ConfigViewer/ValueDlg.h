/* -*- C++ -*- */
// $Id$

#ifndef _ConfigurationViewer_ValueDlg_H
#define _ConfigurationViewer_ValueDlg_H

class ValueDlg : public wxDialog
{
public:
  ///////////////////////////////////////////
  // Initializers
  ///////////////////////////////////////////
  ValueDlg(wxWindow* pParent, bool String);
  // New Value Ctor
  ValueDlg(wxWindow* pParent, wxString& Name, wxString& Value);
  // Edit String Ctor
  ValueDlg(wxWindow* pParent, wxString& Name, u_int Value);
  // Edit UINT Ctor
  virtual ~ValueDlg();

  ///////////////////////////////////////////
  // Methods
  ///////////////////////////////////////////
  const wxString& GetName();
  const wxString& GetStringValue();
  u_int GetUINTValue();

  ///////////////////////////////////////////
  // Attribute Accessors
  ///////////////////////////////////////////
  wxButton*       m_pOK;
  wxButton*       m_pCancel;
  wxStaticText*   m_pName;
  wxTextCtrl*     m_pNameText;
  wxStaticText*   m_pValue;
  wxTextCtrl*     m_pValueText;
protected:
  // Not Used
  ValueDlg(const ValueDlg& RHS);
  const ValueDlg& operator=(const ValueDlg& RHS);

  wxString    m_Name;
  wxString    m_Value;
  u_int       m_UINTValue;

private:

};

#endif

