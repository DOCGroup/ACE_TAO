// $Id$
// NamingObject.cpp: implementation of the CNamingObject class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NamingViewer.h"
#include "NamingObject.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNamingObject::CNamingObject(CosNaming::Name& Name, CORBA::Object_ptr pObject, bool Context)
: m_Name(Name), m_Object(CORBA::Object::_duplicate(pObject)), m_Context(Context)
{

}


CNamingObject::CNamingObject(CORBA::Object_ptr pObject)
: m_Object(CORBA::Object::_duplicate(pObject))
{
}

CNamingObject::~CNamingObject()
{
}

CosNaming::NamingContext_ptr CNamingObject::NamingContext()
{
  return CosNaming::NamingContext::_narrow(m_Object);
}

CORBA::Object_ptr CNamingObject::Object()
{
  return m_Object;
}

CosNaming::Name& CNamingObject::Name()
{
  return m_Name;
}
