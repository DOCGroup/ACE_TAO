// $Id$
// wxNamingTree.h

#ifndef i_wxNamingTree_h
#define i_wxNamingTree_h

#include "wx/treectrl.h"
#include "orbsvcs/CosNamingC.h"

class wxMenu;
class WxNamingObject;

class WxNamingTree:
  public wxTreeCtrl
{
public:
  WxNamingTree(
      wxWindow* parent,
      const wxWindowID id = -1);
  virtual ~WxNamingTree();

  bool isNodeSelected() const
      { return GetSelection() != 0; }
  void copySelectedToClipboard();
  void resolve( CosNaming::NamingContext_ptr pRootContext);
  void setOrb( CORBA::ORB_ptr pOrb);

  enum {
    treeCtrl = 1000
  };

private:
  CORBA::ORB_ptr orb;

  enum {
    listQuantum = 40,
  };
  void clearChildren (void);
  void clearChildren (wxTreeItemId& item);
  WxNamingObject* getTreeObject(void) const;
  WxNamingObject* getTreeObject(wxTreeItemId& item) const;
  void listBindingList(
      wxTreeItemId& item,
      CosNaming::NamingContext_ptr context,
      CosNaming::BindingList_var& bl);
  void listContext( wxTreeItemId& item);
  void refresh( wxTreeItemId& item);
  wxMenu* contextPopup;
  wxMenu* objectPopup;
  enum {
    contextPopupBindContext,
    contextPopupBindNewContext,
    contextPopupBindObject,
    contextPopupUnbind,
    contextPopupDestroy,
    contextPopupViewReference,
    contextPopupRefresh,

    objectPopupUnbind,
    objectPopupViewReference,
  };
  void onContextPopupBindContext( wxCommandEvent& event);
  void onContextPopupBindNewContext( wxCommandEvent& event);
  void onContextPopupBindObject( wxCommandEvent& event);
  void onContextPopupDestroy( wxCommandEvent& event);
  void onContextPopupRefresh( wxCommandEvent& event);
  void onContextPopupUnbind( wxCommandEvent& event);
  void onPopupViewReference( wxCommandEvent& event);

  void onObjectPopupUnbind( wxCommandEvent& event);
  void onObjectPopupViewReference( wxCommandEvent& event);

  void onItemExpanding( wxTreeEvent& event);
  void onRMouseUClick( wxMouseEvent& event);
  void onLeftDClick( wxMouseEvent& event);

  DECLARE_EVENT_TABLE()
};

#endif
