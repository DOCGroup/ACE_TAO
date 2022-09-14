#include "Test.h"

Hello::Hello (PortableServer::POA_ptr poa) :
  poa_ (PortableServer::POA::_duplicate (poa))
{
}

void
Hello::print_hello_world ()
{
}

PortableServer::POA_ptr
Hello::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

