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

  /// Returns a duplicated naming context pointer if this is a context, or nil otherwise.
  CosNaming::NamingContext_ptr NamingContext();

  /// Returns a non-duplicated object reference.
  CORBA::Object_ptr Object();

  /// Returns the name of the object.
  CosNaming::Name& Name();

  /// Returns whether this object is a naming context.
  bool IsContext() {return m_Context;};

private:
  CORBA::Object_var     m_Object;
  CosNaming::Name       m_Name;
  bool                  m_Context;
};

#endif

