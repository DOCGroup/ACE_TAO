/**
 * @file Echo_Caller.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "Echo_Caller.h"
#include "Server_Timer.h"
#include "tao/ORB_Core.h"
#include "Server_Thread_Pool.h"

ACE_RCSID(Bug_1270_Regression, Echo_Caller, "$Id$")

Echo_Caller::Echo_Caller(CORBA::ORB_ptr orb, Thread_Pool *pool)
  : orb_(CORBA::ORB::_duplicate(orb)),
    pool_(pool)
{
}

void
Echo_Caller::start_task(Test::Echo_ptr client
                        ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  pool_->put(Test::Echo::_duplicate(client));
}

void
Echo_Caller::shutdown(ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG, "Server received shutdown message\n"));
  orb_->shutdown(0 ACE_ENV_ARG_PARAMETER);
}
