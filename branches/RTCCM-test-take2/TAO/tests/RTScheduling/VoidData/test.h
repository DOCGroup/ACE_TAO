//$Id$

#include "ace/Log_Msg.h"

class test_impl :public test
{
 public:
  virtual void bar (CORBA::VoidData something ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))

  {
    ACE_DEBUG ((LM_DEBUG,
		"Void Data Passed to method %d\n",
		*(int*)something));
  }
};

