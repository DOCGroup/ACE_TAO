//$Id$
#ifndef Thread_Action_H
#define Thread_Action_H

#include "tao/RTScheduling/RTSchedulerC.h"

class Test_Thread_Action:
public RTScheduling::ThreadAction
{
 public:
  Test_Thread_Action (void);

  virtual void _cxx_do (CORBA::VoidData data
		   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
		    ACE_THROW_SPEC ((CORBA::SystemException));
};

#endif /*Thread_Action_H*/


Test_Thread_Action::Test_Thread_Action (void)
{
}

void
Test_Thread_Action::_cxx_do (CORBA::VoidData data
			ACE_ENV_ARG_DECL)
     ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
	      "Test_Thread_Action::do %s\n",
	      (char*) data));
}
