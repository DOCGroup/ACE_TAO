// $Id$
#include "stdafx.h"
#include "ConfigTreeCtrl.h"
#include "MainFrame.h"
#include "ValueDlg.h"
#include "ValueListCtrl.h"

enum {CFGNEWKEY=100, CFGNEWSTRING, CFGNEWUINT, CFGNEWBINARY, CFGNEWSUBMENU, CFGFIND, CFGDELETE, CFGRENAME, CFGCOPYKEYNAME};


BEGIN_EVENT_TABLE(ConfigTreeCtrl, wxTreeCtrl)
  EVT_RIGHT_DOWN(ConfigTreeCtrl::OnRightDown)
  EVT_RIGHT_UP(ConfigTreeCtrl::OnRightUp)
  EVT_MENU(CFGNEWKEY, ConfigTreeCtrl::OnNewKey)
  EVT_MENU(CFGNEWSTRING, ConfigTreeCtrl::OnNewString)
  EVT_MENU(CFGNEWUINT, ConfigTreeCtrl::OnNewUINT)
  EVT_MENU(CFGNEWBINARY, ConfigTreeCtrl::OnNewBinary)
  EVT_MENU(CFGFIND, ConfigTreeCtrl::OnFind)
  EVT_MENU(CFGDELETE, ConfigTreeCtrl::OnDelete)
  EVT_TREE_SEL_CHANGED(FRAME_TREE, ConfigTreeCtrl::OnSelChanged)
END_EVENT_TABLE()


ConfigTreeCtrl::ConfigTreeCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxValidator& validator, const wxString& name)
: wxTreeCtrl(parent, id, pos, size, style, validator, name)
{
  // Load the tree
  LoadTree();
}

ConfigTreeCtrl::~ConfigTreeCtrl()
{
}

void ConfigTreeCtrl::LoadTree()
{
  m_pConfig = MainFrame::Instance()->GetpConfig();
  const ACE_Configuration_Section_Key& Key = m_pConfig->root_section();
  wxTreeItemId Root = AppendItem(GetRootItem(), "Root");
  LoadSection(Root, Key);
}

void ConfigTreeCtrl::LoadSection(wxTreeItemId& ParentItem, const ACE_Configuration_Section_Key& Key)
{
  ACE_TString Name;
  int Index = 0;
  while(!m_pConfig->enumerate_sections(Key, Index, Name))
  {
    wxTreeItemId Item = AppendItem(ParentItem, Name.fast_rep());
    ACE_Configuration_Section_Key Child;
    m_pConfig->open_section(Key, Name.fast_rep(), 0, Child);
    LoadSection( Item, Child);
    ++Index;
  }
}

void ConfigTreeCtrl::OnRightDown(wxMouseEvent& event)
{
  //EndEditLabel(TRUE);
  int Flags = wxTREE_HITTEST_ONITEMLABEL | wxTREE_HITTEST_ONITEMICON;
  long ItemID = HitTest(wxPoint(event.m_x, event.m_y), Flags);
  SelectItem(ItemID);
}

void ConfigTreeCtrl::OnRightUp(wxMouseEvent& event)
{
  wxTreeItemId ItemID = GetSelection();

  wxMenu* pMenu = new wxMenu;
  wxMenu* pNewMenu = new wxMenu;
  pNewMenu->Append(CFGNEWKEY, "Key");
  pNewMenu->AppendSeparator();
  pNewMenu->Append(CFGNEWSTRING, "String");
  pNewMenu->Append(CFGNEWUINT, "Unsigned Int");
  //pNewMenu->Append(CFGNEWBINARY, "Binary");
  pMenu->Append(CFGNEWSUBMENU, "New", pNewMenu);
  pMenu->Append(CFGFIND, "Find");
  pMenu->AppendSeparator();
  pMenu->Append(CFGDELETE, "Delete");
  //pMenu->Append(CFGRENAME, "Rename");  // not supported
  //pMenu->AppendSeparator();
  //pMenu->Append(CFGCOPYKEYNAME, "Copy Key Name"); // not supported
  PopupMenu(pMenu, event.m_x, event.m_y);
  delete pMenu;
}

void ConfigTreeCtrl::ResolveKey(wxTreeItemId Item, ACE_Configuration_Section_Key& Key)
{
  wxTreeItemId OriginalItem = Item;
  ACE_TString Path("");
  ACE_TString Temp;
  while(Item != GetRootItem())
  {
    wxString Text = GetItemText(Item);
    Temp = Path;
    Path = Text.c_str();
    if(Temp.length())
    {
      Path += "\\";
      Path += Temp;
    }
    Item = GetParent(Item);
  }
  if(Path.length())
  {
    m_pConfig->expand_path(m_pConfig->root_section(), Path, Key, 0);
  }
  else
  {
    Key = m_pConfig->root_section();
  }
}


void ConfigTreeCtrl::OnNewKey(wxCommandEvent& event)
{
  wxTextEntryDialog Dlg(this, "Test", "Key Name");
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }

  wxString Value = Dlg.GetValue();

  // Get the key for this node
  wxTreeItemId ItemID = GetSelection();
  ACE_Configuration_Section_Key Key, NewKey;
  ResolveKey(ItemID, Key);
  m_pConfig->open_section(Key, Value, 1, NewKey);
  wxTreeItemId NewItemID = AppendItem(ItemID, Value);
  EnsureVisible(NewItemID);
}

void ConfigTreeCtrl::OnNewString(wxCommandEvent& event)
{
  ValueDlg Dlg(this, true);
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }

  ACE_TString Value = Dlg.GetStringValue();
  ACE_TString Name = Dlg.GetName();

  // Get the key for this node
  wxTreeItemId ItemID = GetSelection();
  ACE_Configuration_Section_Key Key;
  ResolveKey(ItemID, Key);
  m_pConfig->set_string_value(Key, Name.fast_rep(), Value);
  m_pListCtrl->DisplaySection(Key);
}

void ConfigTreeCtrl::OnNewUINT(wxCommandEvent& event)
{
  ValueDlg Dlg(this, false);
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }

  u_int Value = Dlg.GetUINTValue();
  ACE_TString Name = Dlg.GetName();

  // Get the key for this node
  wxTreeItemId ItemID = GetSelection();
  ACE_Configuration_Section_Key Key;
  ResolveKey(ItemID, Key);
  m_pConfig->set_integer_value(Key, Name.fast_rep(), Value);
  m_pListCtrl->DisplaySection(Key);
}

void ConfigTreeCtrl::OnNewBinary(wxCommandEvent& event)
{
  assert(0);
  /*
  ValueDlg Dlg(this, true);
  if(Dlg.ShowModal() != wxID_OK)
  {
    return;
  }

  ACE_TString Value = Dlg.GetStringValue();
  ACE_TString Name = Dlg.GetName();

  // Get the key for this node
  wxTreeItemId ItemID = GetSelection();
  ACE_Configuration_Section_Key Key;
  ResolveKey(ItemID, Key);
  m_pConfig->set_string_value(Key, Name.fast_rep(), Value);
  m_pListCtrl->DisplaySection(Key);
  */
}

void ConfigTreeCtrl::OnSelChanged(wxTreeEvent& event)
{
  wxTreeItemId ItemID = GetSelection();
  ACE_Configuration_Section_Key Key;
  ResolveKey(ItemID, Key);
  m_pListCtrl->DisplaySection(Key);
}

void ConfigTreeCtrl::OnFind(wxCommandEvent& event)
{
}

void ConfigTreeCtrl::OnDelete(wxCommandEvent& event)
{
  wxTreeItemId ItemID = GetSelection();
  wxTreeItemId Parent = GetParent(ItemID);
  ACE_Configuration_Section_Key Key;
  ResolveKey(Parent, Key);
  wxMessageDialog Dlg(this, "Are you sure you want to delete this section?", "Confirm Section Delete", wxYES_NO | wxICON_EXCLAMATION );
  if(Dlg.ShowModal() != wxID_YES)
  {
    return;
  }
  wxString Text = GetItemText(ItemID);
  m_pConfig->remove_section(Key, Text, 1);
  // Reload parent
  Delete(ItemID);
}

void ConfigTreeCtrl::ChangeConfig(ACE_Configuration* pConfig)
{
  m_pConfig = pConfig;
  DeleteAllItems();
  LoadTree();
}

