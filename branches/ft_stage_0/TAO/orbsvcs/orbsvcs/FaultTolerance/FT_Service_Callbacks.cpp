// $Id$

#include "FT_Service_Callbacks.h"
#include "FT_Policy_i.h"

#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/Tagged_Components.h"
#include "tao/Stub.h"

#include "tao/Invocation.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"

#if !defined (__ACE_INLINE__)
# include "FT_Service_Callbacks.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (FaultTolerance,
           FT_Service_Callbacks,
           "$Id$")

TAO_FT_Service_Callbacks::TAO_FT_Service_Callbacks (
    TAO_ORB_Core *orb_core)

  : orb_core_ (orb_core),
    profile_lock_ (0),
    primary_failed_ (0),
    secondary_set_ (0),
    group_component_ (),
    group_component_flag_ (0)
{
  this->profile_lock_ =
    this->orb_core_->client_factory ()->create_profile_lock ();
}

TAO_FT_Service_Callbacks::~TAO_FT_Service_Callbacks (void)
{
  // Delete the memeory for the lock
  delete this->profile_lock_;
}

CORBA::Boolean
TAO_FT_Service_Callbacks::object_is_nil (CORBA::Object_ptr obj)
{
  // Get the count
  CORBA::ULong count =
    obj->_stubobj ()->base_profiles ().profile_count ();

  // If the profile count is zero then return true
  if (count == 0)
    return 1;
  else
    {
      // else if each of the profile is nil
      for (CORBA::ULong cnt = 0;
           cnt < count;
           cnt++)
        {
          TAO_Profile *pfile =
            obj->_stubobj ()->base_profiles ().get_profile (cnt);

          if (pfile != 0)
            return 0;
        }
    }

  // If it reaches here then it should be nill
  return 1;

}


int
TAO_FT_Service_Callbacks::raise_comm_failure (
    TAO_GIOP_Invocation *invoke,
    TAO_Profile *profile
    ACE_ENV_ARG_DECL)
{
  if (this->restart_policy_check (
          invoke->request_service_context ().service_info (),
          profile))
    {
      TAO_GIOP_Twoway_Invocation *invoc =
        ACE_dynamic_cast (TAO_GIOP_Twoway_Invocation *,
                          invoke);

      if (invoc)
        {
          // Reset the states to start invocation on a different
          // target
          invoc->reset_states ();
        }
      return TAO_INVOKE_RESTART;
    }

  // As the right tags are not found close the connection and throw an
  // exception
  invoke->close_connection ();
  ACE_THROW_RETURN (CORBA::COMM_FAILURE (
      CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
          errno),
      CORBA::COMPLETED_MAYBE),
      TAO_INVOKE_EXCEPTION);
}

int
TAO_FT_Service_Callbacks::raise_transient_failure (
    TAO_GIOP_Invocation *invoke,
    TAO_Profile *profile
    ACE_ENV_ARG_DECL)
{
  if (restart_policy_check (
        invoke->request_service_context ().service_info (),
        profile))
    {
      TAO_GIOP_Twoway_Invocation *invoc =
        ACE_dynamic_cast (TAO_GIOP_Twoway_Invocation *,
                          invoke);

      if (invoc)
        {
          // Reset the states to start invocation on a different
          // target
          invoc->reset_states ();
        }
      return TAO_INVOKE_RESTART;
    }

  // As the right tags are not found close the connection and throw an
  // exception
  ACE_THROW_RETURN (CORBA::TRANSIENT (
      CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
          errno),
      CORBA::COMPLETED_MAYBE),
      TAO_INVOKE_EXCEPTION);
}

CORBA::Boolean
TAO_FT_Service_Callbacks::restart_policy_check (
    IOP::ServiceContextList &service_list,
    const TAO_Profile *profile)
{
  // Check whether the IOP::FT_REQUEST exists within the service
  // context list and FT::FT_GROUP exists within the profile that we
  // have
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = IOP::TAG_FT_GROUP;

  if (profile->tagged_components ().get_component (tagged_component)
      == 1)
    {
      // Look for the FT_REQUEST context id
      for (CORBA::ULong i = 0;
           i < service_list.length ();
           i++)
        {
          if (service_list[i].context_id == IOP::FT_REQUEST)
            {
              // Success
              return 1;
            }
        }
    }

  // Failure
  return 0;
}
