// $Id$
// wxNamingTree.cpp


#include "pch.h"
#include "wxNamingTree.h"

#include "wx/clipbrd.h"
#include "wxAutoDialog.h"
#include "wxBindNewContext.h"
#include "wxNamingObject.h"
#include "wxBindDialog.h"
#include "wxViewIORDialog.h"


BEGIN_EVENT_TABLE( WxNamingTree, wxTreeCtrl)
  EVT_MENU(
      contextPopupBindContext,
      WxNamingTree::onContextPopupBindContext)
  EVT_MENU(
      contextPopupBindNewContext,
      WxNamingTree::onContextPopupBindNewContext)
  EVT_MENU(
      contextPopupBindObject,
      WxNamingTree::onContextPopupBindObject)
  EVT_MENU(
      contextPopupDestroy,
      WxNamingTree::onContextPopupDestroy)
  EVT_MENU(
      contextPopupRefresh,
      WxNamingTree::onContextPopupRefresh)
  EVT_MENU(
      contextPopupUnbind,
      WxNamingTree::onContextPopupUnbind)
  EVT_MENU(
      contextPopupViewReference,
      WxNamingTree::onPopupViewReference)
  EVT_MENU(
      objectPopupUnbind,
      WxNamingTree::onObjectPopupUnbind)
  EVT_MENU(
      objectPopupViewReference,
      WxNamingTree::onPopupViewReference)
  EVT_TREE_ITEM_EXPANDING(
      WxNamingTree::treeCtrl,
      WxNamingTree::onItemExpanding)
  // For some reason RIGHT_UP doesn't work
  EVT_RIGHT_DOWN( WxNamingTree::onRMouseUClick)
  EVT_LEFT_DCLICK( WxNamingTree::onLeftDClick)
END_EVENT_TABLE()


WxNamingTree::WxNamingTree(
    wxWindow* parent,
    const wxWindowID id):
  wxTreeCtrl(
      parent,
      id,
      wxDefaultPosition,
      wxSize( 234, 149)),
  orb( 0)
{
  contextPopup = new wxMenu();
  contextPopup->Append( contextPopupBindContext, "Bind context");
  contextPopup->Append( contextPopupBindNewContext, "Bind new context");
  contextPopup->Append( contextPopupBindObject, "Bind object");
  contextPopup->Append( contextPopupUnbind, "Unbind");
  contextPopup->Append( contextPopupDestroy, "Destroy");
  contextPopup->Append( contextPopupViewReference, "View reference");
  contextPopup->Append( contextPopupRefresh, "Refresh");

  objectPopup = new wxMenu();
  objectPopup->Append( objectPopupUnbind, "Unbind");
  objectPopup->Append( objectPopupViewReference, "View reference");
}


WxNamingTree::~WxNamingTree()
{
  clearChildren();
}

void WxNamingTree::clearChildren( void)
{
  wxTreeItemId item = GetRootItem();
  if (item) {

    clearChildren( item);
    Delete( item);

  }
}

void WxNamingTree::clearChildren( wxTreeItemId& item)
{
  long cookie;
  wxTreeItemId child = GetFirstChild( item, cookie);
  while( child) {

    clearChildren( child);
    Delete( child);
    child = GetFirstChild( item, cookie);

  }
}


void WxNamingTree::copySelectedToClipboard()
{
  WxNamingObject* object = getTreeObject();
  try {

    wxString ior = orb->object_to_string( object->Object());
    if (wxTheClipboard->Open()) {

      wxTheClipboard->SetData( new wxTextDataObject( ior));
      wxTheClipboard->Close();

    }

  } catch( CORBA::Exception& ex) {

    wxMessageBox( ex._rep_id(), "CORBA::Exception");

  }
}


WxNamingObject* WxNamingTree::getTreeObject( void) const
{
  wxTreeItemId item = GetSelection();
  if (item == wxTreeItemId( (wxGenericTreeItem*)0)) {
    return 0;
  }
  return getTreeObject (item);
}

WxNamingObject* WxNamingTree::getTreeObject( wxTreeItemId& item) const
{
  WxNamingObject* object = static_cast<WxNamingObject*>(
      GetItemData( item));
  return object;
}


void WxNamingTree::listBindingList(
      wxTreeItemId& item,
      CosNaming::NamingContext_ptr context,
      CosNaming::BindingList_var& bl)
{
  try {

    for( unsigned int i=0; i < bl->length(); i++) {

      // Add each entry into the tree control
      CORBA::Object_var object = context->resolve( bl[i].binding_name);
      bool isContext =(bl[i].binding_type == CosNaming::ncontext);
      WxNamingObject* newObject = new WxNamingObject(
          bl[i].binding_name,
          object,
          isContext);
      wxString name = static_cast<const char*>( (bl[i].binding_name[0]).id);
      const wxString kind = static_cast<const char*>( (bl[i].binding_name[0]).kind);
      if (!kind.IsNull()) {

        name << " | " << kind;

      }
      wxTreeItemId contextItem = AppendItem( item, name);
      SetItemData( contextItem, newObject);
      switch( bl[i].binding_type) {

        case CosNaming::ncontext: {

          // TODO: set a different icon for contexts
          // TODO: set only if there are children
          SetItemHasChildren( contextItem);

        }
        break;

        case CosNaming::nobject:
          break;

      }

    }

  } catch( CORBA::Exception& ex) {

    wxMessageBox( ex._rep_id(), "CORBA::Exception");

  }
}


void WxNamingTree::listContext( wxTreeItemId& item)
{
// TODO: use hourglass
//  SetCursor( *wxHOURGLASS_CURSOR);
  try {

    // Get the item's object and make sure we have a context
    WxNamingObject* namingObject = getTreeObject( item);
    CosNaming::NamingContext_var context = namingObject->NamingContext();
    if (CORBA::is_nil( context.in ())) {

      return;

    }

    // List the context's entries
    CosNaming::BindingList_var bl;
    CosNaming::BindingIterator_var bi;
    context->list( listQuantum, bl, bi);
    listBindingList( item, context, bl);
    if (!CORBA::is_nil( bi.in ())) {

      while( bl->length()) {

        wxString text;
        text << "This context contains more than " << listQuantum <<
            " entries, list the next " << listQuantum << "?";
        if (wxMessageBox(
            text,
            "Question",
            wxYES_NO | wxICON_QUESTION) == wxYES) {

          bi->next_n( listQuantum, bl);
          listBindingList( item, context, bl);

        }

      }
      bi->destroy();

    }

  } catch( CORBA::Exception& ex) {

    wxMessageBox( ex._rep_id(), "CORBA::Exception");

  }

}


void WxNamingTree::onContextPopupBindContext( wxCommandEvent& event)
{
  WxAutoDialog<WxBindDialog> dialog( new WxBindDialog(
      true,
      orb,
      this));
  if (dialog->ShowModal() != wxID_OK) {

    return;

  }
  try {

    WxNamingObject* object = getTreeObject();
    CosNaming::NamingContext_var context = object->NamingContext();
    if (CORBA::is_nil( context.in ())) {

      return;

    }
    CosNaming::NamingContext_var newContext =
        CosNaming::NamingContext::_narrow( dialog->getObject());
    if (CORBA::is_nil( newContext.in ())) {

      wxMessageBox(
          "Object is not a CosNaming::NamingContext",
          "Error",
          wxOK | wxICON_EXCLAMATION,
          this);
      return;

    }
    context->bind_context(
        dialog->getName(),
        newContext.in ());
    onContextPopupRefresh( event);

  } catch( CORBA::Exception& ex) {

    wxMessageBox(
        ex._rep_id(),
        "CORBA::Exception");

  }
}


void WxNamingTree::onContextPopupBindObject( wxCommandEvent& event)
{
  WxAutoDialog<WxBindDialog> dialog( new WxBindDialog(
      false,
      orb,
      this));
  if (dialog->ShowModal() != wxID_OK) {

    return;

  }
  try {

    wxTreeItemId item = GetSelection();
    WxNamingObject* object = getTreeObject( item);
    CosNaming::NamingContext_var context = object->NamingContext();
    if (CORBA::is_nil( context.in ())) {

      return;

    }
    context->bind( dialog->getName(), dialog->getObject());
    onContextPopupRefresh( event);

  } catch( CORBA::Exception& ex) {

    wxMessageBox(
        ex._rep_id(),
        "CORBA::Exception");

  }
}


void WxNamingTree::onContextPopupBindNewContext( wxCommandEvent& event)
{
  wxTreeItemId item = GetSelection();
  WxNamingObject* object = getTreeObject( item);
  CosNaming::NamingContext_var context = object->NamingContext();
  if (CORBA::is_nil( context.in ())) {

    return;

  }
  WxAutoDialog<WxBindNewContext> dialog( new WxBindNewContext( this));
  if (dialog->ShowModal() != wxID_OK) {

    return;

  }
  try {

    CosNaming::NamingContext_var newContext = context->new_context();
    context->bind_context( dialog->getName(), newContext.in ());
    onContextPopupRefresh( event);

  } catch( CORBA::Exception& ex) {

    wxMessageBox(
        ex._rep_id(),
        "CORBA::Exception");

  }
}


void WxNamingTree::onContextPopupDestroy( wxCommandEvent&)
{
  if (wxMessageBox(
      "Are you sure you want to destroy this object?",
      "Confirm",
      wxYES_NO | wxICON_QUESTION) != wxYES) {

    return;

  }
  wxTreeItemId item = GetSelection();
  wxTreeItemId parentItem = GetParent( item);
  if (parentItem == 0) {

    return;

  }
  WxNamingObject* object = getTreeObject( item);
  WxNamingObject* parentObject = getTreeObject( parentItem);
  CosNaming::NamingContext_var parentNaming = parentObject->NamingContext();
  try {

    // First try to destroy, it will raise an exception if it's not empty
    CosNaming::NamingContext_var context = object->NamingContext();
    context->destroy();
    // OK it's destroyed, cleanup any children we might have lying aroung
    clearChildren( item);
    Delete( item);
    // Do the unbind
    parentNaming->unbind( object->Name());

  } catch( CORBA::Exception& ex) {

    wxMessageBox( ex._rep_id(), "CORBA::Exception");

  }
}


void WxNamingTree::onContextPopupRefresh( wxCommandEvent&)
{
  wxTreeItemId item = GetSelection();
  clearChildren( item);
  listContext( item);
}


void WxNamingTree::onContextPopupUnbind( wxCommandEvent&)
{
  if (wxMessageBox(
      "Are you sure you want to unbind this context?",
      "Confirm",
      wxYES_NO | wxICON_QUESTION) != wxYES) {

    return;

  }
  wxTreeItemId item = GetSelection();
  wxTreeItemId parentItem = GetParent( item);
  if (parentItem == 0) {

    return;

  }
  WxNamingObject* object = getTreeObject( item);
  WxNamingObject* parent = getTreeObject( parentItem);
  CosNaming::NamingContext_var context = parent->NamingContext();
  try {

    context->unbind( object->Name());
    clearChildren( item);
    Delete( item);

  } catch( CORBA::Exception& ex) {

    wxMessageBox( ex._rep_id(), "CORBA::Exception");

  }
}


void WxNamingTree::onItemExpanding( wxTreeEvent& event)
{

  wxTreeItemId item = event.GetItem();
  // If this item has a child it has already been listed so nothing to do.
  if (GetLastChild( item) != wxTreeItemId( 0L)) {

    return;

  }
  listContext( item);
}


void WxNamingTree::onLeftDClick( wxMouseEvent& event)
{
  wxTreeItemId item = HitTest( event.GetPosition());
  if (!item) {

    return;
  }
  WxAutoDialog<WxViewIORDialog> dialog( new WxViewIORDialog(
      orb,
      getTreeObject( item)->Object(),
      this));

  dialog->ShowModal();
}


void WxNamingTree::onObjectPopupUnbind( wxCommandEvent& )
{
  if (wxMessageBox(
      "Are you sure you want to unbind this object?",
      "Confirm",
      wxYES_NO | wxICON_QUESTION) != wxYES) {

    return;

  }
  wxTreeItemId item = GetSelection();

  // Make sure we don't unbind "Root"
  wxTreeItemId parentItem = GetParent( item);
  if (parentItem == 0) {

    return;

  }
  WxNamingObject* object = getTreeObject( item);
  WxNamingObject* parent = getTreeObject( parentItem);
  CosNaming::NamingContext_var context = parent->NamingContext();
  try {

    context->unbind( object->Name());
    clearChildren( item);
    Delete( item);

  } catch( CORBA::Exception& ex) {

    wxMessageBox( ex._rep_id(), "CORBA::Exception");

  }
}


void WxNamingTree::onPopupViewReference( wxCommandEvent&)
{
  WxAutoDialog<WxViewIORDialog> dialog( new WxViewIORDialog(
      orb,
      getTreeObject()->Object(),
      this));

  dialog->ShowModal();
}


void WxNamingTree::onRMouseUClick( wxMouseEvent& event)
{
  wxTreeItemId item = HitTest( event.GetPosition());
  if (!item) {

    return;
  }
  // First select the item, then popup the appropriate menu
  SelectItem( item);
  WxNamingObject* object = getTreeObject( item);
  CosNaming::NamingContext_var context = object->NamingContext();
  if (CORBA::is_nil( context.in ())) {

    PopupMenu( objectPopup, event.m_x, event.m_y);

  } else {

    contextPopup->Enable( contextPopupDestroy, item != GetRootItem());
    contextPopup->Enable( contextPopupUnbind, item != GetRootItem());
    PopupMenu( contextPopup, event.m_x, event.m_y);

  }
}

void WxNamingTree::resolve( CosNaming::NamingContext_ptr pRootContext)
{
  clearChildren();
  if (!CORBA::is_nil( pRootContext)) {

    wxTreeItemId item = AddRoot( "Root");
    SetItemData( item, new WxNamingObject( pRootContext));
    listContext( item);

  } else {

    AddRoot( "<null>");

  }
}

void WxNamingTree::setOrb( CORBA::ORB_ptr pOrb)
{
  // This can only be called once!
  assert( orb == 0);
  orb = pOrb;
}
