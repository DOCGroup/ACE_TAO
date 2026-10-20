
#include "MyInterfaceImpl.h"

char const *
MyInterfaceImpl::my_string = "a string";

MyInterfaceImpl::MyInterfaceImpl (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}
CORBA::Boolean
MyInterfaceImpl::myMethod (char const *mystring)
{
  return mystring == my_string;
}

