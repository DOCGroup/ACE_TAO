// $Id$
// NamingObject.h: interface for the CNamingObject class.

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMINGOBJECT_H__1F34ADC4_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_)
#define AFX_NAMINGOBJECT_H__1F34ADC4_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "orbsvcs/CosNamingC.h"

class CNamingObject  
{
public:
  CNamingObject (CosNaming::Name& Name,
                 CORBA::Object_ptr pObject,
                 bool Context);
  ~CNamingObject (void);

  CosNaming::NamingContext_ptr NamingContext (void);

  // Returns a duplicated naming context ptr if this is a context or
  // nil if its not.
  CORBA::Object_ptr Object (void);

  // returns a non duplicated object reference
  CosNaming::Name& Name (void);
  // returns the name of the object

  bool IsContext (void) { return m_Context; } 

  // Returns true if this is a context
private:
  CORBA::Object_var m_Object;
  CosNaming::Name m_Name;
  bool m_Context;
};

#endif // !defined(AFX_NAMINGOBJECT_H__1F34ADC4_B0D8_11D3_BF27_00C0F03C565A__INCLUDED_)
