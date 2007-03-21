//
// $Id$
//

#include "MyInterfaceImpl.h"

const char*
MyInterfaceImpl::my_string = "a string";

MyInterfaceImpl::MyInterfaceImpl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}
CORBA::Boolean
MyInterfaceImpl::myMethod (const char* mystring)
{
  return mystring == my_string;
}

