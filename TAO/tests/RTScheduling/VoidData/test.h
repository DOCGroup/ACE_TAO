//$Id$
#include "testC.h"

class test_impl :public test
{
  void bar (CORBA::VoidData something)
  {
    ACE_DEBUG ((LM_DEBUG,
		"%d\n",
		(int*)something));
  }
};

