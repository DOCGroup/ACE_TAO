// $Id$
// wxNamingObject.cpp

#include "pch.h"
#include "wxNamingObject.h"


WxNamingObject::WxNamingObject(
    CosNaming::Name& Name,
    CORBA::Object_ptr pObject,
    bool Context):
  m_Name(Name),
  m_Object(CORBA::Object::_duplicate(pObject)),
  m_Context(Context)
{
  // Empty
}


WxNamingObject::WxNamingObject( CORBA::Object_ptr pObject):
  m_Object(CORBA::Object::_duplicate(pObject))
{
  // Empty
}

WxNamingObject::~WxNamingObject()
{
  // Empty
}


CosNaming::NamingContext_ptr WxNamingObject::NamingContext()
{
  return CosNaming::NamingContext::_narrow(m_Object);
}


CORBA::Object_ptr WxNamingObject::Object()
{
  return m_Object;
}


CosNaming::Name& WxNamingObject::Name()
{
  return m_Name;
}
