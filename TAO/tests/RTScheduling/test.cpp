//$Id$
#include "testS.h"

class foo_impl :public POA_foo
{
  void bar (CORBA::VoidData something)
  {
    ACE_DEBUG ((LM_DEBUG,
		"%d\n",
		(int*)something))
  }
}

main (int argc, char* argv[])
{
  
}
