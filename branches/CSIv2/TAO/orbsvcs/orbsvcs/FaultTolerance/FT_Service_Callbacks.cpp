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

CORBA::Boolean
TAO_FT_Service_Callbacks::is_profile_equivalent (const TAO_Profile *this_p,
                                                 const TAO_Profile *that_p)
{
  // At this point we assume that all the checks for other things
  // within the profiles have been satisfied
  const TAO_Tagged_Components &this_comp =
    this_p->tagged_components ();

  const TAO_Tagged_Components &that_comp =
    that_p->tagged_components ();

  IOP::TaggedComponent this_tc, that_tc;
  this_tc.tag = that_tc.tag = IOP::TAG_FT_GROUP;

  if ((this_comp.get_component (this_tc) == 1) &&
      (that_comp.get_component (that_tc) == 1))
    {
      TAO_InputCDR this_cdr (ACE_reinterpret_cast (const char*,
                                              this_tc.component_data.get_buffer ()),
                             this_tc.component_data.length ());

      TAO_InputCDR that_cdr (ACE_reinterpret_cast (const char*,
                                                   that_tc.component_data.get_buffer ()),
                             that_tc.component_data.length ());

      CORBA::Boolean this_byte_order;
      CORBA::Boolean that_byte_order;

      if ((this_cdr >> ACE_InputCDR::to_boolean (this_byte_order) == 0) ||
          (that_cdr >> ACE_InputCDR::to_boolean (that_byte_order) == 0))
         {
           // return no equivalent
           return 0;
         }

      this_cdr.reset_byte_order (ACE_static_cast (int, this_byte_order));
      that_cdr.reset_byte_order (ACE_static_cast (int, that_byte_order));


      FT::TagFTGroupTaggedComponent this_group_component;
      FT::TagFTGroupTaggedComponent that_group_component;

      this_cdr >> this_group_component;
      that_cdr >> that_group_component;

      // check if domain id and group id are the same
      if ((ACE_OS::strcmp (this_group_component.ft_domain_id,
                           that_group_component.ft_domain_id) == 0) &&
          (this_group_component.object_group_id ==
           that_group_component.object_group_id))
         {
           return 1;
         }
    }

  return 0;
}

CORBA::ULong
TAO_FT_Service_Callbacks::hash_ft (TAO_Profile *p,
                                   CORBA::ULong max)
{
  // At this point we assume that all the checks for other things
  // within the profiles have been satisfied
  TAO_Tagged_Components &this_comp =
    p->tagged_components ();

  IOP::TaggedComponent tc;
  tc.tag = IOP::TAG_FT_GROUP;

  if (this_comp.get_component (tc) == 0)
    return 0;

  // extract the group component
  TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                          tc.component_data.get_buffer ()),
                    tc.component_data.length ());

  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
  {
    return 0;
  }

  cdr.reset_byte_order (ACE_static_cast(int,byte_order));

  FT::TagFTGroupTaggedComponent group_component;

  cdr >> group_component;

  return group_component.object_group_id % max;
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
