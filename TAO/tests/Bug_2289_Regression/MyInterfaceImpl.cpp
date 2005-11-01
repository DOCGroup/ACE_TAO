//
// $Id$
//

#include "MyInterfaceImpl.h"

MyInterfaceImpl::MyInterfaceImpl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

void
MyInterfaceImpl::myMethod (const MyStruct & ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

