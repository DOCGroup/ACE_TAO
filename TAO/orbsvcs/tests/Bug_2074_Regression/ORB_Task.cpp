/**
 * @file ORB_Task.cpp
 *
 * @author Carlos O'Ryan <coryan@atdesk.com>
 */
#include "ORB_Task.h"
#include "tao/Messaging/Messaging.h"
#include "orbsvcs/CosNamingC.h"

ORB_Task::ORB_Task (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
ORB_Task::svc ()
{
  try
    {
      CORBA::Object_var const nc_ref =
        this->orb_->string_to_object (
          "corbaloc:iiop:10.175.12.99:15025/NameService");

      CORBA::PolicyList policies {};

      constexpr TimeBase::TimeT timeout {5000 * 10000};

      CORBA::Any timeout_any {};
      timeout_any <<= timeout;

      policies.length (1);
      policies[0] = this->orb_->create_policy (
        Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE, timeout_any);

      CORBA::Object_var const object = nc_ref->_set_policy_overrides (
        policies, CORBA::SET_OVERRIDE);

      policies[0]->destroy ();

      CosNaming::NamingContext_var const naming_context =
        CosNaming::NamingContext::_narrow (object.in ());
      naming_context->_non_existent ();
    }
  catch (CORBA::TRANSIENT const &)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught transient\n"));
    }
  catch (CORBA::TIMEOUT const &)
    {
      ACE_DEBUG ((LM_DEBUG, "Caught timeout\n"));
    }
  catch (CORBA::Exception const &e)
    {
      e._tao_print_exception ("Exception caught");
    }

  return 0;
}
