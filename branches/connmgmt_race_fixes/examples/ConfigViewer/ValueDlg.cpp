// $Id$
#include "stdafx.h"
#include "ValueDlg.h"

ValueDlg::ValueDlg(wxWindow* pParent, bool String)
: wxDialog(pParent, -1, String ? "New String" : "New Value", wxDefaultPosition, wxSize(300,140))
{
  m_pName= new wxStaticText(this, -1, "Name:", wxPoint(20, 20));
  m_pNameText = new wxTextCtrl(this, -1, "", wxPoint(60, 20), wxDefaultSize, 0, wxTextValidator(wxFILTER_NONE, &m_Name));
  m_pValue = new wxStaticText(this, -1, "Value:", wxPoint(20, 50));
  m_pValueText = new wxTextCtrl(this, -1, "", wxPoint(60, 50), wxDefaultSize, 0, wxTextValidator(String ? wxFILTER_NONE : wxFILTER_NUMERIC, &m_Value));
  m_pOK = new wxButton(this, wxID_OK, "OK", wxPoint(60, 80));
  m_pCancel = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(160, 80));
}

ValueDlg::ValueDlg(wxWindow* pParent, wxString& Name, wxString& Value)
: wxDialog(pParent, -1, "Edit String", wxDefaultPosition, wxSize(300,140))
{
  m_Name = Name;
  m_Value = Value;
  m_pName= new wxStaticText(this, -1, "Name:", wxPoint(20, 20));
  m_pNameText = new wxTextCtrl(this, -1, "", wxPoint(60, 20), wxDefaultSize, 0, wxTextValidator(wxFILTER_NONE, &m_Name));
  m_pNameText->SetEditable(false);
  m_pValue = new wxStaticText(this, -1, "Value:", wxPoint(20, 50));
  m_pValueText = new wxTextCtrl(this, -1, Value, wxPoint(60, 50), wxDefaultSize, 0, wxTextValidator(wxFILTER_NONE, &m_Value));
  m_pOK = new wxButton(this, wxID_OK, "OK", wxPoint(60, 80));
  m_pCancel = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(160, 80));
}

ValueDlg::ValueDlg(wxWindow* pParent, wxString& Name, u_int Value)
: wxDialog(pParent, -1, "Edit String", wxDefaultPosition, wxSize(300,140))
{
  m_Name = Name;
  m_Value.sprintf("%d", Value);
  m_pName= new wxStaticText(this, -1, "Name:", wxPoint(20, 20));
  m_pNameText = new wxTextCtrl(this, -1, "", wxPoint(60, 20), wxDefaultSize, 0, wxTextValidator(wxFILTER_NONE, &m_Name));
  m_pNameText->SetEditable(false);
  m_pValue = new wxStaticText(this, -1, "Value:", wxPoint(20, 50));
  m_pValueText = new wxTextCtrl(this, -1, m_Value, wxPoint(60, 50), wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC, &m_Value));
  m_pOK = new wxButton(this, wxID_OK, "OK", wxPoint(60, 80));
  m_pCancel = new wxButton(this, wxID_CANCEL, "Cancel", wxPoint(160, 80));
}


ValueDlg::~ValueDlg()
{
}

const wxString& ValueDlg::GetName()
{
  return m_Name;
}

const wxString& ValueDlg::GetStringValue()
{
  return m_Value;
}

u_int ValueDlg::GetUINTValue()
{
  return atoi(m_Value);
}

