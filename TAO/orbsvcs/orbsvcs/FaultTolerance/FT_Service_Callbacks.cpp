#include "orbsvcs/FaultTolerance/FT_Service_Callbacks.h"
#include "orbsvcs/FaultTolerance/FT_ClientPolicy_i.h"

#include "ace/OS_NS_sys_time.h"

#include "tao/MProfile.h"
#include "tao/Profile.h"
#include "tao/Tagged_Components.h"
#include "tao/Stub.h"
#include "tao/Invocation_Utils.h"
#include "tao/ORB_Core.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/CDR.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FT_Service_Callbacks::TAO_FT_Service_Callbacks (TAO_ORB_Core *)
{
}

TAO_FT_Service_Callbacks::~TAO_FT_Service_Callbacks () = default;

CORBA::Boolean
TAO_FT_Service_Callbacks::select_profile (TAO_MProfile const& mprofile,
    TAO_Profile *&pfile)
{
  CORBA::ULong const size = mprofile.size ();

  // Iterate through the list in a circular fashion. Stop one before
  // the list instead of trying the same thing again.
  for (CORBA::ULong i = 0;
       i != size;
       ++i)
    {
      TAO_Profile const* const current_profile = mprofile.get_profile (i);

      IOP::TaggedComponent tagged_component;
      tagged_component.tag = IOP::TAG_FT_PRIMARY;

      // Get the tagged component from the  profile
      TAO_Tagged_Components const& pfile_tagged =
        current_profile->tagged_components ();

      // Search for the TaggedComponent that we want
      if (pfile_tagged.get_component (tagged_component) == 1)
      {
        // We have found a primary IOR
        pfile = const_cast<TAO_Profile *> (current_profile);
        return true;
      }
    }

  return false;
}

CORBA::Boolean
TAO_FT_Service_Callbacks::object_is_nil (CORBA::Object_ptr const obj)
{
  // Get the count
  CORBA::ULong const count = obj->_stubobj ()->base_profiles ().profile_count ();

  // If the profile count is zero then return true
  if (count == 0)
    return true;

  // Otherwise, check whether any profile is not nil.
  for (CORBA::ULong cnt = 0;
       cnt < count;
       ++cnt)
    {
      TAO_Profile * const profile =
        obj->_stubobj ()->base_profiles ().get_profile (cnt);

      if (profile != nullptr)
        return false;
    }

  // If it reaches here then it should be nill
  return true;
}

TAO_Service_Callbacks::Profile_Equivalence
TAO_FT_Service_Callbacks::is_profile_equivalent (TAO_Profile const* const this_p,
                                                 TAO_Profile const* const that_p)
{
  TAO_Tagged_Components const& this_comp =
    this_p->tagged_components ();

  TAO_Tagged_Components const& that_comp =
    that_p->tagged_components ();

  IOP::TaggedComponent this_tc, that_tc;
  this_tc.tag = that_tc.tag = IOP::TAG_FT_GROUP;

  if (this_comp.get_component (this_tc) == 1)
    {
      if (that_comp.get_component (that_tc) == 1)
        {
          TAO_InputCDR this_cdr (reinterpret_cast<char const *> (this_tc.component_data.get_buffer ()),
                             this_tc.component_data.length ());

          TAO_InputCDR that_cdr (reinterpret_cast<char const *> (that_tc.component_data.get_buffer ()),
                             that_tc.component_data.length ());

          CORBA::Boolean this_byte_order {};
          CORBA::Boolean that_byte_order {};

          if (!(this_cdr >> ACE_InputCDR::to_boolean (this_byte_order)) ||
              !(that_cdr >> ACE_InputCDR::to_boolean (that_byte_order)))
            {
              // Have tags but full of garbage - not equivalent
              return TAO_Service_Callbacks::NOT_EQUIVALENT;
            }

          this_cdr.reset_byte_order (static_cast<int> (this_byte_order));
          that_cdr.reset_byte_order (static_cast<int> (that_byte_order));


          FT::TagFTGroupTaggedComponent this_group_component;
          FT::TagFTGroupTaggedComponent that_group_component;

          this_cdr >> this_group_component;
          that_cdr >> that_group_component;

          // check if domain id and group id are the same
          if ((ACE_OS::strcmp (this_group_component.group_domain_id,
                               that_group_component.group_domain_id) == 0) &&
              (this_group_component.object_group_id ==
               that_group_component.object_group_id))
             {
               // Both have matching tags - true
               return TAO_Service_Callbacks::EQUIVALENT;
             }
        }

      return TAO_Service_Callbacks::NOT_EQUIVALENT;
    }
  else
    {
      if (that_comp.get_component (that_tc) == 1)
        {
          return TAO_Service_Callbacks::NOT_EQUIVALENT;
        }
    }

  return TAO_Service_Callbacks::DONT_KNOW;
}

CORBA::ULong
TAO_FT_Service_Callbacks::hash_ft (TAO_Profile * const p, CORBA::ULong const max)
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
  TAO_InputCDR cdr (reinterpret_cast<char const *> (tc.component_data.get_buffer ()),
                    tc.component_data.length ());

  CORBA::Boolean byte_order {};
  if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
    {
      return 0;
    }

  cdr.reset_byte_order (static_cast<int> (byte_order));

  FT::TagFTGroupTaggedComponent group_component;

  cdr >> group_component;

  return (CORBA::ULong) group_component.object_group_id % max;
}

CORBA::Boolean
TAO_FT_Service_Callbacks::is_permanent_forward_condition (CORBA::Object_ptr const obj,
                                                          TAO_Service_Context const& service_context) const
{
  // do as much as possible outside of lock
  IOP::ServiceContext sc;
  sc.context_id = IOP::FT_GROUP_VERSION;

  if (service_context.get_context (sc) == 0)
      return false;

  IOP::TaggedComponent tc;
  tc.tag = IOP::TAG_FT_GROUP;

  TAO_Stub const* const stub = obj->_stubobj ();
  // check for forward_profiles, branching to speed up operation on base_profiles
  if (stub->forward_profiles ())
    {
      // set lock, as forward_profiles might be deleted concurrently
      ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                                guard,
                                const_cast <TAO_SYNCH_MUTEX&>(stub->profile_lock ()),
                                false));

      // even now, the forward profiles might have been deleted in the meanwhile
      TAO_MProfile const& mprofile = stub->forward_profiles()
          ? *(stub->forward_profiles())
          : stub->base_profiles();

      if (mprofile.profile_count () == 0)
        // releasing lock
        return false;

      // assuming group-attributes are set for all profiles, check
      // only the first profile
      TAO_Tagged_Components const& tagged_components =
              mprofile.get_profile (0)->tagged_components ();

      if (tagged_components.get_component (tc) == 0)
        // releasing lock
        return false;

      return true; /* true */

      // releasing lock
    }
  else /* operate on constant basic_profiles */
    {
      TAO_MProfile const& mprofile = stub->base_profiles ();

      if (mprofile.profile_count () == 0)
        return false;

      // assuming group-attributes are set for all profiles, check only the first profile
      TAO_Tagged_Components const& tagged_components =
              mprofile.get_profile (0)->tagged_components ();

      if (tagged_components.get_component (tc) == 0)
        return false;

      return true; /* true */
    }
}


TAO::Invocation_Status
TAO_FT_Service_Callbacks::raise_comm_failure (
    IOP::ServiceContextList& context_list,
    TAO_Profile *profile)
{
  if (this->restart_policy_check (context_list,
                                  profile))
    return TAO::TAO_INVOKE_RESTART;

  // As the right tags are not found close the connection and throw an
  // exception
  throw CORBA::COMM_FAILURE (
    CORBA::SystemException::_tao_minor_code (
      TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
      errno),
    CORBA::COMPLETED_MAYBE);
}

TAO::Invocation_Status
TAO_FT_Service_Callbacks::raise_transient_failure (
    IOP::ServiceContextList& service,
    TAO_Profile *profile)
{
  if (this->restart_policy_check (service,
                                  profile))
    return TAO::TAO_INVOKE_RESTART;

  // Unlike COMM_FAILURE do not raise an exception since the
  // completion status is not known and no assumption can be made.
  return TAO::TAO_INVOKE_SYSTEM_EXCEPTION;
}

CORBA::Boolean
TAO_FT_Service_Callbacks::restart_policy_check (
    IOP::ServiceContextList const& service_list,
    TAO_Profile const* const profile) const
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
           ++i)
        {
          if (service_list[i].context_id == IOP::FT_REQUEST)
            {
              // This would be a heck of a lot easier if we had the invocation
              // here rather than just the contexts, but lemons -> lemonade I guess.
              TAO_InputCDR cdr (reinterpret_cast <char const *> (service_list[i].context_data.get_buffer ()),
                                service_list[i].context_data.length ());
              CORBA::Boolean byte_order {};
              if (!(cdr >> ACE_InputCDR::to_boolean (byte_order)))
                return false;
              cdr.reset_byte_order (static_cast <int> (byte_order));
              FT::FTRequestServiceContext ftsrc;
              if (!(cdr >> ftsrc))
                return false;

              return (ftsrc.expiration_time > now ());
            }
        }
    }

  // Failure
  return false;
}

TimeBase::TimeT
TAO_FT_Service_Callbacks::now ()
{
  // Grab the localtime on the machine where this is running
  ACE_Time_Value const time_val = ACE_OS::gettimeofday ();
  TimeBase::TimeT const sec_part = ((TimeBase::TimeT)time_val.sec ()) * 10000000;
  TimeBase::TimeT const usec_part = ((TimeBase::TimeT)time_val.usec ()) * 10;

  // Add the offset to convert from posix time.
  return (sec_part + usec_part + ACE_UINT64_LITERAL (0x1B21DD213814000));
}

TAO_END_VERSIONED_NAMESPACE_DECL
