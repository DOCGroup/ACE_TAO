/**
 * @file ORB_Task.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 *
 */
#include "ORB_Task.h"
#include "tao/Messaging/Messaging.h"
#include "orbsvcs/CosNamingC.h"



ORB_Task::ORB_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
ORB_Task::svc (void)
{
  try
  {
    CORBA::Object_var ncRef =
        orb_->string_to_object(
            "corbaloc:iiop:10.175.12.99:15025/NameService" );

    CORBA::PolicyList policies;

    TimeBase::TimeT timeout = 5000 * 10000;

    CORBA::Any timeoutAny;
    timeoutAny <<= timeout;

    policies.length(1);
    policies[0] = orb_->create_policy(
      Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
      timeoutAny );

    CORBA::Object_var object = ncRef->_set_policy_overrides(
      policies, CORBA::SET_OVERRIDE );

    policies[0]->destroy();

    CosNaming::NamingContext_var namingContext =
      CosNaming::NamingContext::_narrow( object.in() );
    namingContext->_non_existent();
  }
  catch ( const CORBA::TRANSIENT&)
  {
    ACE_DEBUG ((LM_DEBUG, "Caught transient\n"));
  }
  catch ( const CORBA::TIMEOUT&)
  {
    ACE_DEBUG ((LM_DEBUG, "Caught timeout\n"));
  }
  catch ( const CORBA::Exception& e )
  {
      e._tao_print_exception ("Exception caught");
  }

  return 0;
}
