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
TAO_FT_Service_Callbacks::select_profile (TAO_MProfile * /*mpfile*/,
                                          TAO_Profile *& /*pfile*/)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) This method has been deprecated \n"));

  return 0;
}

CORBA::Boolean
TAO_FT_Service_Callbacks::reselect_profile (TAO_Stub * /*stub*/,
                                            TAO_Profile *& /*pfile*/)
{
#if 0

  // @@todo: This needs to go after sometime..
  // Note: We are grabbing the lock very early. We can still delay
  // that. I will address this when I get around for the next round of
  // improvements.

  // Grab the lock
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *this->profile_lock_,
                            0));

  // This method is essentially called before the Stub Object gives up
  // on invocation. We should only do this
  // 1) Check whether the first in the list or the profile_in_use ()
  // is NOT a primary. If it had been so, we would have parsed the
  // whole IOR list
  // 2) Then set the first profile for invocation

  // A secondary was already selected, so dont do that again.
  if (this->secondary_set_)
    return 0;

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = IOP::TAG_FT_PRIMARY;

  TAO_Profile *profile = stub->profile_in_use ();

  // Get the tagged component from the  profile
  TAO_Tagged_Components &pfile_tagged =
    profile->tagged_components ();

  // Search for the TaggedComponent that we want
  if (pfile_tagged.get_component (tagged_component) == 1)
    {
      // We just return here as we would have parsed the whole IOR list
      return 0;
    }

  // As the first is not a primary, it should be a secondary, we will
  // set that for invocation.
  pfile = profile;

  // Set the primary_failed_ flag
  this->primary_failed_ = 1;
  this->secondary_set_ = 1;

  return 1;

#endif /*if 0*/

  return 0;
}

void
TAO_FT_Service_Callbacks::reset_profile_flags (void)
{
#if 0
  // Grab the lock
  ACE_MT (ACE_GUARD (ACE_Lock,
                     guard,
                     *this->profile_lock_));

  // Reset the flags that we may have
  this->primary_failed_ = 0;
  this->secondary_set_ = 0;
#endif /*if 0*/
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


void
TAO_FT_Service_Callbacks::service_context_list (
    TAO_Stub *&stub,
    IOP::ServiceContextList &service_list,
    CORBA::Boolean restart
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  // If the restart flag is true, then this call for a
  // reinvocation. We need not prepare the Service Context List once
  // again. We can use the already existing one.
  if (!restart)
    {
      // Pack the group version service context
      this->group_version_service_context (stub,
                                           service_list
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Pack the request service context
      this->request_service_context (stub,
                                     service_list
                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  return;
}

void
TAO_FT_Service_Callbacks::request_service_context (
    TAO_Stub *&stub,
    IOP::ServiceContextList &service_list
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;
  if (cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)
       == 0)
    ACE_THROW (CORBA::MARSHAL ());

  // Marshall the data in to the CDR streams
  if (cdr <<
      stub->orb_core ()->fault_tolerance_service ().client_id ().c_str ()
      == 0)
    ACE_THROW (CORBA::MARSHAL ());

  if (cdr <<
      stub->orb_core ()->fault_tolerance_service ().retention_id ()
      == 0)
    ACE_THROW (CORBA::MARSHAL ());


  // Check whether the FT::RequestDurationPolicy has been set
  CORBA::Policy_var policy =
    stub->get_policy (FT::REQUEST_DURATION_POLICY
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // @@ There are two possibilities a) Duration policy has not been
  // set b)Duration policy has been set. Point 'b' will give no
  // problems. Some issues kick in for point 'a'. The best is to set a
  // default value. BUT, what is the "right" value? Need to discuss
  // this with Andy when I get a chance. For the present let us assume
  // we have 15 seconds. This is purely an assumption..

  TimeBase::TimeT exp_time = 0;

  // if we have a non-null policy set
  if (!CORBA::is_nil (policy.in ()) &&
      policy->policy_type () == FT::REQUEST_DURATION_POLICY)
    {
      FT::RequestDurationPolicy_var duration_policy =
        FT::RequestDurationPolicy::_narrow (policy.in ());

      exp_time =
        duration_policy->request_duration_value (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      // The assumption that we are making
      exp_time = 15 * 10000000;
    }

  // Calculaton of the expiration time

  // @@ Note: This is so poorly defined in the spec. All that the
  // @@ spec says is that add the RequestDurationPolicy value with
  // @@ the 'local clock value', whatever the local clock
  // @@ means. IMHO, we need something like UTC value or a more clear
  // @@ definition to send across the wire.

  // Grab the localtime on the machine where this is running
  ACE_Time_Value time_val = ACE_OS::gettimeofday ();


  TimeBase::TimeT sec_part  = time_val.sec () * 10000000;
  TimeBase::TimeT usec_part = time_val.usec ()* 10;

  // Now we have the total time
  exp_time += sec_part + usec_part;


  // Marshall the TimeBase::TimeT in to the CDR stream
  if (cdr << exp_time == 0)
    ACE_THROW (CORBA::MARSHAL ());

  // Add the CDR encapsulation in to the ServiceContextList
  CORBA::ULong l = service_list.length ();
  service_list.length (l + 1);
  service_list[l].context_id = IOP::FT_REQUEST;

  // Make a *copy* of the CDR stream...
  CORBA::ULong length = cdr.total_length ();
  service_list[l].context_data.length (length);
  CORBA::Octet *buf = service_list[l].context_data.get_buffer ();

  for (const ACE_Message_Block *i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }

  return;
}


void
TAO_FT_Service_Callbacks::group_version_service_context (
    TAO_Stub *&stub,
    IOP::ServiceContextList &service_list
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_OutputCDR cdr;
  if (cdr << ACE_OutputCDR::from_boolean (TAO_ENCAP_BYTE_ORDER)
       == 0)
    ACE_THROW (CORBA::MARSHAL ());

  if (!this->group_component_flag_)
    this->get_object_group_version (stub->profile_in_use ());

  // Marshall the data in to the CDR streams
  // Marshall the TimeBase::TimeT in to the CDR stream
  if (cdr << this->group_component_ == 0)
    ACE_THROW (CORBA::MARSHAL ());

  // Add the CDR encapsulation in to the ServiceContextList
  CORBA::ULong l = service_list.length ();
  service_list.length (l + 1);
  service_list[l].context_id = IOP::FT_GROUP_VERSION;

  // Make a *copy* of the CDR stream...
  CORBA::ULong length = cdr.total_length ();
  service_list[l].context_data.length (length);
  CORBA::Octet *buf = service_list[l].context_data.get_buffer ();

  for (const ACE_Message_Block *i = cdr.begin ();
       i != 0;
       i = i->cont ())
    {
      ACE_OS::memcpy (buf, i->rd_ptr (), i->length ());
      buf += i->length ();
    }

  return;
}


void
TAO_FT_Service_Callbacks::get_object_group_version (TAO_Profile *profile)
{
  // For the group version service context, we need to get the
  // ObjectGroupRefVersion. So we just look at the profile in
  // use. Look at the TaggedComponents in the profile in use, look for
  // the TAG_FT_GROUP and get the properties. This is the general idea.

  // Get the TAO_TaggedComponents from the profile
  const TAO_Tagged_Components &tagged_components =
    profile->tagged_components ();

  IOP::TaggedComponent tagged;
  tagged.tag = IOP::TAG_FT_GROUP;

  // Get the TaggedComponent
  // If it doesn't exist then just return
  // It just means that the IOR that we got is not from a FT ORB.
  if (tagged_components.get_component (tagged) != 1)
    return;

  TAO_InputCDR cdr (ACE_reinterpret_cast (const char*,
                                          tagged.component_data.get_buffer ()
                                          ),
                    tagged.component_data.length ());
  CORBA::Boolean byte_order;
  if ((cdr >> ACE_InputCDR::to_boolean (byte_order)) == 0)
    return;
  cdr.reset_byte_order (ACE_static_cast(int,byte_order));

  // Extract the group component
  cdr >> this->group_component_;

  // Set the flag
  this->group_component_flag_ = 1;
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


void
TAO_FT_Service_Callbacks::service_log_msg_rcv (
    TAO_Message_State_Factory & /*state*/)
{
  // Oscar & ANDY to fill up
  return;
}

void
TAO_FT_Service_Callbacks::service_log_msg_pre_upcall (
    TAO_ServerRequest & /*req*/)
{
  // Oscar & ANDY to fill up
  return;
}

void
TAO_FT_Service_Callbacks::service_log_msg_post_upcall (
    TAO_ServerRequest & /*req*/)
{
  // Oscar & ANDY to fill up
  return;
}
