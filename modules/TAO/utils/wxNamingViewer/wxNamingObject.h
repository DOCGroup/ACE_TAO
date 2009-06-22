// $Id$
// wxNamingObject.h

#ifndef i_wxNamingObject_h
#define i_wxNamingObject_h

#include "orbsvcs/CosNamingC.h"
#include "wx/treectrl.h"

class WxNamingObject:
  public wxTreeItemData
{
public:
  WxNamingObject(
      CosNaming::Name& Name,
      CORBA::Object_ptr pObject,
      bool Context);
  WxNamingObject( CORBA::Object_ptr pObject);
  ~WxNamingObject();

  CosNaming::NamingContext_ptr NamingContext();
  // Returns a duplicated naming context ptr if this is a context or nil if its not
  CORBA::Object_ptr Object();
  // returns a non duplicated object reference
  CosNaming::Name& Name();
  // returns the name of the object
  bool IsContext() {return m_Context;};
  // Returns true if this is a context

private:
  CORBA::Object_var     m_Object;
  CosNaming::Name       m_Name;
  bool                  m_Context;
};

#endif

