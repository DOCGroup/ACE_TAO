// $Id$
// NamingTreeCtrl.cpp : implementation file

#include "stdafx.h"
#include "NamingViewer.h"
#include "NamingTreeCtrl.h"
#include "ViewIORDialog.h"
#include "NamingObject.h"
#include "BindDialog.h"
#include "BindNewContext.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamingTreeCtrl

CNamingTreeCtrl::CNamingTreeCtrl (void)
{
  m_ContextPopup.LoadMenu (IDR_CONTEXT_POPUP);
  m_ObjectPopup.LoadMenu (IDR_OBJECT_POPUP);
}

CNamingTreeCtrl::~CNamingTreeCtrl (void)
{
}

BEGIN_MESSAGE_MAP(CNamingTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CNamingTreeCtrl)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_CONTEXT_POPUP_VIEWREFERENCE, OnContextPopupViewreference)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_COMMAND(ID_CONTEXT_POPUP_REFRESH, OnContextPopupRefresh)
	ON_COMMAND(ID_CONTEXT_POPUP_UNBIND, OnContextPopupUnbind)
	ON_COMMAND(ID_CONTEXT_POPUP_DESTROY, OnContextPopupDestroy)
	ON_COMMAND(ID_CONTEXT_POPUP_BIND_CONTEXT, OnContextPopupBindContext)
	ON_COMMAND(ID_CONTEXT_POPUP_BINDOBJECT, OnContextPopupBindobject)
	ON_WM_DESTROY()
	ON_COMMAND(ID_CONTEXTPOPUP_BINDNEWCONTEXT, OnContextpopupBindnewcontext)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_OBJECTPOPUP_UNBIND, OnObjectpopupUnbind)
	ON_COMMAND(ID_OBJECTPOPUP_VIEWREFRENCE, OnObjectpopupViewrefrence)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamingTreeCtrl message handlers

void CNamingTreeCtrl::OnRButtonDown (UINT nFlags, CPoint point) 
{
  // TODO: Add your message handler code here and/or call default
  // Special case here - this causes the entry to be selected when the
  // right button is the first to be hit.  strange
  OnLButtonDown (nFlags, point);
  
  // Now find the item were hitting
  HTREEITEM hItem = HitTest(point);
  if(!hItem)
    {
      return;
    }
  SelectItem(hItem);

  POINT Point = point;
  ClientToScreen(&Point);
  CNamingObject* pObject = GetTreeObject(hItem);

  // If this is not a context, show the object popup
  if(!pObject->IsContext())
    {
      if(!m_ObjectPopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this))
        {
          TRACE0("TrackPopupMenu Failed");
        }
    }
  else
    {
      if(!m_ContextPopup.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, Point.x, Point.y, this))
        {
          TRACE0("TrackPopupMenu Failed");
        }
    }
}

void CNamingTreeCtrl::OnContextPopupViewreference() 
{
	// TODO: Add your command handler code here
	ViewIORDialog Dialog(m_pORB, GetTreeObject()->Object());
  Dialog.DoModal();
}

void CNamingTreeCtrl::SetpORB(CORBA::ORB_ptr pORB)
{
  m_pORB = pORB;
}

CNamingObject* CNamingTreeCtrl::GetTreeObject(HTREEITEM hItem)
{
  if(hItem == NULL)
  {
    hItem = GetSelectedItem();
    if(!hItem)
    {
      return 0;
    }
  }
  CNamingObject* pObject = (CNamingObject*)GetItemData(hItem);
  return pObject;
}

void CNamingTreeCtrl::ClearChildren(HTREEITEM hItem)
{
  if(hItem == NULL)
  {
    HTREEITEM hItem = GetRootItem();
    if(hItem)
    {
      
      //CORBA::Object_var Object = (CORBA::Object_ptr)GetItemData(hItem);
      ClearChildren(hItem);
      delete GetTreeObject(hItem);
      DeleteItem(hItem);
    }
    return;
  }

  HTREEITEM hChild;
  while(hChild = GetNextItem(hItem, TVGN_CHILD))
  {
    // Remove our reference count on the object reference
    ClearChildren(hChild);
    delete GetTreeObject(hChild);
    DeleteItem(hChild);
  }
}


#define LISTQUANTUM 40

void CNamingTreeCtrl::ListContext(HTREEITEM hItem)
{
  CWaitCursor Waiter;
  try
  {
    // Get the items object and make sure we have a context
    CNamingObject* pObject = GetTreeObject(hItem);
    CosNaming::NamingContext_var Context = pObject->NamingContext();
    if(CORBA::is_nil(Context))
    {
      return;
    }
    
    // List the contexts entries
    CosNaming::BindingList_var bl;
    CosNaming::BindingIterator_var bi;
    Context->list(LISTQUANTUM, bl, bi);
    ListBindingList(hItem, Context, bl);
    
    if(!CORBA::is_nil(bi))
    {
      while(bl->length())
      {
        CString Text;
        Text.Format("This context contains more than %d entries, list the next %d?", LISTQUANTUM, LISTQUANTUM);
        if(MessageBox(Text, "Question", MB_YESNO) == IDNO)
        {
          return;
        }
        bi->next_n(LISTQUANTUM, bl);
        ListBindingList(hItem, Context, bl);
      }
      bi->destroy();
    }
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

void CNamingTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
  // If this item has a child, it has already been listed so nothing to do..
  if(GetChildItem(pNMTreeView->itemNew.hItem))
  {
    return;
  }
  ListContext(pNMTreeView->itemNew.hItem);
}

void CNamingTreeCtrl::OnContextPopupRefresh() 
{
	// TODO: Add your command handler code here
	HTREEITEM hItem = GetSelectedItem();
  ClearChildren(hItem);
  ListContext(hItem);
}

void CNamingTreeCtrl::OnContextPopupUnbind() 
{
	// TODO: Add your command handler code here
  if(MessageBox("Are you sure you want to unbind this object?", "Confirm", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
  {
    return;
  }
  HTREEITEM hItem = GetSelectedItem();
  HTREEITEM hParent = GetParentItem(hItem);
  if(!hParent)
  {
    return;
  }
  CNamingObject* pObject = GetTreeObject(hItem);
  CNamingObject* pParent= GetTreeObject(hParent);
  CosNaming::NamingContext_var Context = pParent->NamingContext();
  try
  {
    Context->unbind(pObject->Name());
    ClearChildren(hItem);
    delete pObject;
    DeleteItem(hItem);
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

void CNamingTreeCtrl::Resolve(CosNaming::NamingContext_ptr pRootContext)
{
  ClearChildren();
  if(!CORBA::is_nil(pRootContext))
  {
    HTREEITEM hItem = InsertItem("Root");
    CosNaming::Name Name;
    Name.length(1);
    Name[0].id = CORBA::string_dup("Root");
    SetItemData(hItem, (DWORD)new CNamingObject(Name, pRootContext, true));
    ListContext(hItem);
  }
}

void CNamingTreeCtrl::OnContextPopupDestroy() 
{
	// TODO: Add your command handler code here
  if(MessageBox("Are you sure you want to destroy this context?", "Confirm", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
  {
    return;
  }
  HTREEITEM hItem = GetSelectedItem();
  HTREEITEM hParent = GetParentItem(hItem);
  if(!hParent)
  {
    return;
  }
  CNamingObject* pObject = GetTreeObject(hItem);
  CNamingObject* pParent= GetTreeObject(hParent);
  CosNaming::NamingContext_var Parent = pParent->NamingContext();
  try
  {
    // First try to destroy, it will raise exception if its not empty
    CosNaming::NamingContext_var Context = pObject->NamingContext();
    Context->destroy();
    // Ok its destroyed, clean up any children we might have laying around
    ClearChildren(hItem);
    DeleteItem(hItem);
    // do the unbind
    Parent->unbind(pObject->Name());
    delete pObject;
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

void CNamingTreeCtrl::OnContextPopupBindContext() 
{
	// TODO: Add your command handler code here
  CBindDialog Dialog(true, m_pORB);
  if(Dialog.DoModal() != IDOK)
  {
    return;
  }
  try
  {
    CNamingObject* pObject = GetTreeObject();
    CosNaming::NamingContext_var Context = pObject->NamingContext();
    if(CORBA::is_nil(Context))
    {
      return;
    }
    CosNaming::NamingContext_var NewContext = CosNaming::NamingContext::_narrow(Dialog.GetObject());
    if(CORBA::is_nil(NewContext))
    {
      AfxMessageBox("Object is not a CosNaming::NamingContext");
      return;
    }
    Context->bind_context(Dialog.GetName(), NewContext);
    OnContextPopupRefresh();
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

void CNamingTreeCtrl::OnContextPopupBindobject() 
{
	// TODO: Add your command handler code here
  CBindDialog Dialog(false, m_pORB);
  if(Dialog.DoModal() != IDOK)
  {
    return;
  }
  try
  {
    HTREEITEM hItem = GetSelectedItem();
    CNamingObject* pObject = GetTreeObject(hItem);
    CosNaming::NamingContext_var Context = pObject->NamingContext();
    if(CORBA::is_nil(Context))
    {
      return;
    }
    Context->bind(Dialog.GetName(), Dialog.GetObject());
    OnContextPopupRefresh();
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

void CNamingTreeCtrl::OnDestroy() 
{
	CTreeCtrl::OnDestroy();
	
	// TODO: Add your message handler code here
	ClearChildren();
	
}


void CNamingTreeCtrl::OnContextpopupBindnewcontext() 
{
	// TODO: Add your command handler code here
  HTREEITEM hItem = GetSelectedItem();
  CNamingObject* pObject = GetTreeObject(hItem);
  CosNaming::NamingContext_var Context = pObject->NamingContext();
  if(CORBA::is_nil(Context))
  {
    return;
  }
	CBindNewContext Dialog;
  if(Dialog.DoModal() != IDOK)
  {
    return;
  }
  try
  {
    CosNaming::NamingContext_var NewContext;
    NewContext = Context->new_context();
    Context->bind_context(Dialog.GetName(), NewContext);
    OnContextPopupRefresh();
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

void CNamingTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CNamingObject* pObject = GetTreeObject();
  // Only display non contexts
  if(!pObject->IsContext())
  {
    ViewIORDialog Dialog(m_pORB, pObject->Object());
    Dialog.DoModal();
  }
	
	*pResult = 0;
}

void CNamingTreeCtrl::OnCopy() 
{
	// TODO: Add your command handler code here
  CNamingObject* pObject = GetTreeObject();
  try
  {
    CString IOR = m_pORB->object_to_string(pObject->Object());
    // Copy to the clipboard by using the CEdit control.  This is easier
    // that doing it the right way
    CEdit Temp;
    CRect None(0,0, 1, 1);
    Temp.Create(0, None, this, 0);
    Temp.SetWindowText(IOR);
    Temp.SetSel(0, IOR.GetLength());
    Temp.Copy();
    Temp.PostMessage(WM_CLOSE);
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

LRESULT CNamingTreeCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message == WM_HOTKEY)
  {
    // To trap control-c (for copying) we registered a hotkey.  For some reason
    // MFC wasn't calling my OnHotKey() function that I registered so I am forcing
    // it this way.  Anyone know the right way to do this?
    OnCopy();
  }
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

void CNamingTreeCtrl::OnObjectpopupUnbind() 
{
	// TODO: Add your command handler code here
  if(MessageBox("Are you sure you want to unbind this object?", "Confirm", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
  {
    return;
  }
  HTREEITEM hItem = GetSelectedItem();
  HTREEITEM hParent = GetParentItem(hItem);
  if(!hParent)
  {
    return;
  }
  CNamingObject* pObject = GetTreeObject(hItem);
  CNamingObject* pParent= GetTreeObject(hParent);
  CosNaming::NamingContext_var Context = pParent->NamingContext();
  try
  {
    Context->unbind(pObject->Name());
    ClearChildren(hItem);
    delete pObject;
    DeleteItem(hItem);
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }	
}

void CNamingTreeCtrl::OnObjectpopupViewrefrence() 
{
	// TODO: Add your command handler code here
	ViewIORDialog Dialog(m_pORB, GetTreeObject()->Object());
  Dialog.DoModal();
}

void CNamingTreeCtrl::ListBindingList(HTREEITEM hItem, CosNaming::NamingContext_ptr pContext, CosNaming::BindingList_var& bl)
{
  try
  {
    for(unsigned int i=0; i < bl->length(); i++)
    {
      // Add each entry into the tree control
      CORBA::Object_var Object = pContext->resolve(bl[i].binding_name);
      bool Context = (bl[i].binding_type == CosNaming::ncontext);
      CNamingObject* pNewObject = new CNamingObject(bl[i].binding_name, Object, Context);
      CString Name;
      const char* pKind = (bl[i].binding_name[0]).kind;
      if(*pKind)
      {
        Name.Format("%s | %s", (bl[i].binding_name[0]).id, pKind);
      }
      else
      {
        Name.Format("%s", (bl[i].binding_name[0]).id);
      }
      HTREEITEM hContext = InsertItem(Name, hItem);
      SetItemData(hContext, (DWORD)pNewObject);
      switch(bl[i].binding_type)
      {
      case CosNaming::ncontext:
        {
          // Set the children flag so the + button is displayed
          TV_ITEM Item;
          Item.mask = TVIF_CHILDREN | TVIF_HANDLE;
          Item.cChildren = 1;
          Item.hItem = hContext;
          SetItem(&Item);
        }
        break;
      case CosNaming::nobject:
        break;
      }
    }
  }
  catch(CORBA::Exception& ex)
  {
    MessageBox(ex._id(), "CORBA::Exception");
  }
}

