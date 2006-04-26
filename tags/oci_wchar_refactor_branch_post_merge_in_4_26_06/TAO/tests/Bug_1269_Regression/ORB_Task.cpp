/**
 * @file ORB_Task.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "ORB_Task.h"
#include "tao/Environment.h"

ACE_RCSID(Bug_1269_Regression, ORB_Task, "$Id$")

ORB_Task::ORB_Task(CORBA::ORB_ptr orb)
  : orb_(CORBA::ORB::_duplicate(orb))
{
}

int
ORB_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      this->orb_->run(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH(CORBA::Exception, ex)
    {
    }
  ACE_ENDTRY;
  return 0;
}
