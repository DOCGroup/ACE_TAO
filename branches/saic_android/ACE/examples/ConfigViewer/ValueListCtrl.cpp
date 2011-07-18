// $Id$
#include "stdafx.h"
#include "ValueListCtrl.h"
#include "MainFrame.h"
#include "ValueDlg.h"


enum {VALUEMODIFY=200, VALUEDELETE, VALUERENAME};

BEGIN_EVENT_TABLE(ValueListCtrl, wxListCtrl)
  EVT_RIGHT_DOWN(ValueListCtrl::OnRightDown)
  EVT_MENU(VALUEMODIFY, ValueListCtrl::OnModify)
  EVT_MENU(VALUEDELETE, ValueListCtrl::OnDelete)
  EVT_MENU(VALUERENAME, ValueListCtrl::OnRename)
END_EVENT_TABLE()


ValueListCtrl::ValueListCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxListCtrl(parent, id, pos, size, style | wxLC_REPORT | wxLC_SINGLE_SEL, validator, name)
{
  InsertColumn(0, "Name");
  InsertColumn(1, "Type");
  InsertColumn(2, "Data");
}

ValueListCtrl::~ValueListCtrl()
{
}

void ValueListCtrl::DisplaySection(const ACE_Configuration_Section_Key& Key)
{
  m_Key = Key;
  DeleteAllItems();
  m_pConfig = MainFrame::Instance()->GetpConfig();
  ACE_TString Name;
  int Index = 0;
  ACE_Configuration::VALUETYPE Type;
  ACE_TString StringValue;
  u_int UINTValue;
  while(!m_pConfig->enumerate_values(Key, Index, Name, Type))
  {
    int Row = InsertItem(0, Name.fast_rep());
    switch(Type)
    {
    case ACE_Configuration::STRING:
      SetItem(Row, 1, "String");
      m_pConfig->get_string_value(Key, Name.fast_rep(), StringValue);
      SetItem(Row, 2, StringValue.fast_rep());
      break;
    case ACE_Configuration::INTEGER:
      {
        SetItem(Row, 1, "Integer");
        m_pConfig->get_integer_value(Key, Name.fast_rep(), UINTValue);
        wxString Text;
        Text.sprintf("%d", UINTValue);
        SetItem(Row, 2, Text);
      }
      break;
    case ACE_Configuration::BINARY:
      SetItem(Row, 1, "Binary");
      break;
    }
    SetItemData(Row, Type);
    ++Index;
  }
}

long ValueListCtrl::GetSelectedItem()
{
  return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

void ValueListCtrl::SelectItem(long ItemID)
{
  // XXX Something isn't right here...  When I use a mask it doesn't work at
  // all someone explain..
  long State = wxLIST_STATE_SELECTED | wxLIST_STATE_FOCUSED;
  SetItemState(ItemID, State, State);
}

void ValueListCtrl::OnRightDown(wxMouseEvent& event)
{
  int Flags = wxLIST_HITTEST_ONITEM;
  long ItemID = HitTest(wxPoint(event.m_x, event.m_y), Flags);
  if(ItemID < 0)
  {
    return;
  }
  SelectItem(ItemID);

  wxMenu* pMenu = new wxMenu;
  pMenu->Append(VALUEMODIFY, "Modify");
  pMenu->AppendSeparator();
  pMenu->Append(VALUEDELETE, "Delete");
  pMenu->Append(VALUERENAME, "Rename");
  PopupMenu(pMenu, event.m_x, event.m_y);
  delete pMenu;
}

void ValueListCtrl::OnModify(wxCommandEvent& event)
{
  long Item = GetSelectedItem();
  if(Item == -1)
  {
    return ;
  }
  wxListItem ListItem;
  ACE_Configuration::VALUETYPE Type = (ACE_Configuration::VALUETYPE)GetItemData(Item);
  wxString Name = GetItemText(Item);

  switch(Type)
  {
    case ACE_Configuration::STRING:
      {
        ACE_TString Value;
        m_pConfig->get_string_value(m_Key, Name, Value);
        wxString ValueText(Value.fast_rep());
        ValueDlg Dlg(this, Name, ValueText);
        if(Dlg.ShowModal() != wxID_OK)
        {
          return;
        }

        Value = (const char*)Dlg.GetStringValue();
        m_pConfig->set_string_value(m_Key, Name, Value);
      }
      break;
    case ACE_Configuration::INTEGER:
      {
        u_int Value;
        m_pConfig->get_integer_value(m_Key, Name, Value);
        ValueDlg Dlg(this, Name, Value);
        if(Dlg.ShowModal() != wxID_OK)
        {
          return;
        }

        Value = Dlg.GetUINTValue();
        m_pConfig->set_integer_value(m_Key, Name, Value);

      }
      break;
    case ACE_Configuration::BINARY:
      {
        wxMessageBox("Binary modification not supported (why don't you implement it?)");
        //assert(0);
      }
      break;
  }
  DisplaySection(m_Key);
}

void ValueListCtrl::OnDelete(wxCommandEvent& event)
{
  wxMessageDialog Dlg(this, "Are you sure you want to delete this value?", "Confirm Value Delete", wxYES_NO | wxICON_EXCLAMATION );
  if(Dlg.ShowModal() != wxID_YES)
  {
    return;
  }
  long Item = GetSelectedItem();
  if(Item == -1)
  {
    return ;
  }
  wxString Text = GetItemText(Item);
  m_pConfig->remove_value(m_Key, Text);
  DeleteItem(Item);
}

void ValueListCtrl::OnRename(wxCommandEvent& event)
{
  long Item = GetSelectedItem();
  if(Item == -1)
  {
    return ;
  }
  wxListItem ListItem;
  ACE_Configuration::VALUETYPE Type = (ACE_Configuration::VALUETYPE)GetItemData(Item);
  wxString Name = GetItemText(Item);
  wxString Message;
  Message.sprintf("Enter new name for '%s'", Name);
  wxTextEntryDialog Dlg(this, Message, "Please enter text", Name);
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }
  wxString NewName = Dlg.GetValue();
  if(NewName == Name)
  {
    return;
  }

  // XXX Check to see if an entry with this new name already exists

  switch(Type)
  {
    case ACE_Configuration::STRING:
      {
        ACE_TString Value;
        m_pConfig->get_string_value(m_Key, Name, Value);
        m_pConfig->set_string_value(m_Key, NewName, Value);
      }
      break;
    case ACE_Configuration::INTEGER:
      {
        u_int Value;
        m_pConfig->get_integer_value(m_Key, Name, Value);
        m_pConfig->set_integer_value(m_Key, NewName, Value);
      }
      break;
    case ACE_Configuration::BINARY:
      {
        wxMessageBox("Rename binary not supported (Why don't you implement it?)");
        assert(0);
      }
  }
  m_pConfig->remove_value(m_Key, Name);
  DisplaySection(m_Key);
}

void ValueListCtrl::ChangeConfig(ACE_Configuration* pConfig)
{
  m_pConfig = pConfig;
  DisplaySection(pConfig->root_section());
}


