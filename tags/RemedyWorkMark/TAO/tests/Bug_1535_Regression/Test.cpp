//
// $Id$
//
#include "Test.h"

Hello::Hello (PortableServer::POA_ptr poa) :
  poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
Hello::print_hello_world (void)
{
}

PortableServer::POA_ptr
Hello::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

