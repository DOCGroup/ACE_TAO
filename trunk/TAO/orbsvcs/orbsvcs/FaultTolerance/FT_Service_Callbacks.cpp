#include "FT_Service_Callbacks.h"
#include "tao/MProfile.h"
#include "tao/Tagged_Components.h"
#include "tao/Stub.h"


#if !defined (__ACE_INLINE__)
# include "FT_Service_Callbacks.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(FaultTolerance, FT_Service_Callbacks, "$Id$")

CORBA::Boolean
TAO_FT_Service_Callbacks::select_profile (TAO_MProfile *mpfile,
                                          TAO_Profile *&pfile)
{
  // If we know that the primary has failed just return
  if (this->primary_failed_)
    return 0;

  IOP::TaggedComponent tagged_component;
  tagged_component.tag = IOP::TAG_FT_PRIMARY;

  // Get the current profile
  TAO_Profile *temp_profile =
    mpfile->get_current_profile ();

  // While the profile is not null
  while (temp_profile)
    {
      // Get the tagged component from the  profile
      TAO_Tagged_Components &pfile_tagged =
        temp_profile->tagged_components ();

      // Search for the TaggedComponent that we want
      if (pfile_tagged.get_component (tagged_component) == 1)
        {
          // Set the profile and return success
          pfile = ACE_const_cast (TAO_Profile *,
                                  temp_profile);

          return 1;
        }
      temp_profile = mpfile->get_next ();
    }


  // Return failure. We havent found any
  // @@ Here is where another issue kicks in. Do we flag this
  //    condition as an error. May be not. Probably the client
  //    ORB that is FT compliant, has received an IOR from a
  //    non-compliant ORB. We cannot think this of an error. Rather we
  //    need to fall back on the default methodology. That would mean
  //    that we return 0 and allow the ORB to do the rest.
  return 0;

  // @@ All said and done, what if we receive an IOGR from a
  //    FT-compliant ORB with no primaries?
  //    Ans: The default usage of the profile in the list
  //    as the start point would NOT be a mistake. We should
  //    get a LOCATION_FORWARD or some such thing to get to
  //    the primary finally.
}

CORBA::Boolean
TAO_FT_Service_Callbacks::reselect_profile (TAO_Stub *stub,
                                            TAO_Profile *&pfile)
{
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
      // We just return here as we would have parsed he whole IOR list
      return 0;
    }

  // As the first is not a primary, it should be a secondary, we will
  // set that for invocation.
  pfile = profile;

  // Set the primary_failed_ flag
  this->primary_failed_ = 1;
  this->secondary_set_ = 1;

  return 1;
}

void
TAO_FT_Service_Callbacks::reset_profile_flags (void)
{
  // Reset the flags that we may have
  this->primary_failed_ = 0;
  this->secondary_set_ = 0;
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
