// -*- C++ -*-

#include "FT_Invocation_Endpoint_Selectors.h"
#include "tao/Invocation.h"
#include "tao/Stub.h"
#include "tao/Profile.h"
#include "tao/Endpoint.h"
#include "tao/Base_Transport_Property.h"


#if !defined (__ACE_INLINE__)
#include "tao/Invocation_Endpoint_Selectors.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (FaultTolerance,
           FT_Invocation_Endpoint_Selectors,
           "$Id$")



TAO_FT_Invocation_Endpoint_Selector::TAO_FT_Invocation_Endpoint_Selector (void)
  : TAO_Default_Endpoint_Selector (),
    is_primary_alive_ (1),
    is_rewound_ (0)
{
}

TAO_FT_Invocation_Endpoint_Selector::~TAO_FT_Invocation_Endpoint_Selector (void)
{
}

void
TAO_FT_Invocation_Endpoint_Selector::select_endpoint (
  TAO_GIOP_Invocation *invocation
  ACE_ENV_ARG_DECL)
{

  int retval = this->select_endpoint_i (invocation
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (retval == 0)
    {
      if (TAO_debug_level > 6)
        ACE_DEBUG ((LM_DEBUG,
                    "(%P|%t) TAO-FT - Primary doesnt exist.",
                    "Falling back on the default selection routines \n"));

      TAO_Default_Endpoint_Selector::select_endpoint (invocation
                                                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  return;
}

// @@ RTCORBA_Subsetting - next should be deprecated...
void
TAO_FT_Invocation_Endpoint_Selector::next (TAO_GIOP_Invocation *
                                     ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("This method is DEPRECATED!\n")));
  // if (invocation->stub_->next_profile_retry () == 0)
  //     ACE_THROW (CORBA::TRANSIENT (TAO_OMG_VMCID | 2,
  //                                  CORBA::COMPLETED_NO));
}

void
TAO_FT_Invocation_Endpoint_Selector::forward (TAO_GIOP_Invocation
                                        *invocation,
                                        const TAO_MProfile &mprofile
                                        ACE_ENV_ARG_DECL)
{
  invocation->stub ()->add_forward_profiles (mprofile);
  // This has to be and is thread safe.
  // TAO_Stub::add_forward_profiles() already makes a copy of the
  // MProfile, so do not make a copy here.


  // We may not need to do this since TAO_GIOP_Invocations
  // get created on a per-call basis. For now we'll play it safe.
  if (invocation->stub ()->next_profile () == 0)
    ACE_THROW (CORBA::TRANSIENT (
                                 CORBA_SystemException::_tao_minor_code (
                                       TAO_INVOCATION_LOCATION_FORWARD_MINOR_CODE,
                                       errno),
                                 CORBA::COMPLETED_NO));
}

void
TAO_FT_Invocation_Endpoint_Selector::success (TAO_GIOP_Invocation *invocation)
{
  invocation->stub ()->set_valid_profile ();
}

void
TAO_FT_Invocation_Endpoint_Selector::close_connection (TAO_GIOP_Invocation *invocation)
{
  // Get rid of any forwarding profiles and reset
  // the profile list to point to the first profile!
  // FRED For now we will not deal with recursive forwards!
  invocation->stub ()->reset_profiles ();
}


int
TAO_FT_Invocation_Endpoint_Selector::select_endpoint_i(
    TAO_GIOP_Invocation *invocation
    ACE_ENV_ARG_DECL)
{
  int retval =
    this->select_primary (invocation
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  // We havent got any information on the primary ie. we dont have a
  // primary in the profile at all or we have a primary selected. We
  // return now.
  if (retval == 0 || retval == 1)
    return retval;

  // Ok, to the tricky portion. Primary is dead.
  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "(%P|%t) The primary is dead,"
                " selecting a secondary \n"));

  // @@ todo: We could hold & release the locks a couple of times
  // here. We need to look into this after 1.2.1 for better
  // performance.
  if (this->is_rewound_ == 0)
    {
      // .. grab the lock
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                                guard,
                                *(invocation->stub ()->profile_lock ()),
                                0));

      // DOUBLE checked optimization pattern
      if (this->is_rewound_ == 0)
        {
          // Reset the profile list
          invocation->stub ()->reset_profiles_i ();

          // Set the flag
          this->is_rewound_ = 1;
        }
    }

  do
    {
      IOP::TaggedComponent tagged_component;
      tagged_component.tag = IOP::TAG_FT_PRIMARY;

      // Get the tagged component from the  profile
      TAO_Tagged_Components &pfile_tagged =
        invocation->stub ()->profile_in_use ()->tagged_components ();

      // If it is a primary just skip to the next profile. We know
      // that the primary has failed anyway..
      if (pfile_tagged.get_component (tagged_component) != 1)
        {
          invocation->profile (invocation->stub ()->profile_in_use ());
          invocation->endpoint (invocation->profile ()->endpoint ());

          int status =
            this->endpoint_from_profile (invocation
                                         ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          if (status == 1)
            return 1;
        }
    }
  while (invocation->stub ()->next_profile_retry () != 0);

  // If we get here, we completely failed to find an endpoint selector
  // that we know how to use, so throw an exception.
  ACE_THROW_RETURN (CORBA::TRANSIENT (TAO_OMG_VMCID | 2,
                                      CORBA::COMPLETED_NO),
                    0);
}

int
TAO_FT_Invocation_Endpoint_Selector::select_primary (
    TAO_GIOP_Invocation *invoc
    ACE_ENV_ARG_DECL)
{
  // Get the current profile
  TAO_Profile *temp_profile =
    invoc->stub ()->base_profiles ().get_current_profile ();

  int retval = 0;

  if (temp_profile)
    {
      // Check whether we have a primary profile in hand.
      retval =
        this->check_profile_for_primary (invoc,
                                         temp_profile
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }
  else
    return 0;

  // If we have a primary just return
  if (retval == 1)
    return retval;

  // If we have detected a failure
  if (retval == -1)
    {
      // .. grab the lock
      ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                                guard,
                                *invoc->stub ()->profile_lock (),
                                0));

      // set the primary flag to be dead..
      this->is_primary_alive_ = 0;

      // .. and return
      return retval;
    }


  // Doing the above helps us to avoid a lock for every
  // invocation. The funda is this. Try to select the first profile in
  // the list of profiles. If it is a primary, life is just great from
  // there. If not, hold a lock and go through the list to pick
  // one. The second invocation would just use that profile as the
  // starting point. Theoretically we should have just one lock if the
  // primary is hidden somewhere else.

  // If we know that the primary is dead return a -1, to choose a
  // secondary.
  if (this->is_primary_alive_ == 0)
    return -1;

  // Now grab the lock
  ACE_MT (ACE_GUARD_RETURN (ACE_Lock,
                            guard,
                            *invoc->stub ()->profile_lock (),
                            0));

  temp_profile =
    invoc->stub ()->base_profiles ().get_next ();

  // While the profile is not null
  while (temp_profile)
    {
      // Check whether we have a primary profile in hand.
      retval =
        this->check_profile_for_primary (invoc,
                                         temp_profile
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (retval == 1 || retval == -1)
        return retval;

      temp_profile =
        invoc->stub ()->base_profiles ().get_next ();
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


int
TAO_FT_Invocation_Endpoint_Selector::check_profile_for_primary (
    TAO_GIOP_Invocation *invocation,
    TAO_Profile*pfile
    ACE_ENV_ARG_DECL)
{
  IOP::TaggedComponent tagged_component;
  tagged_component.tag = IOP::TAG_FT_PRIMARY;

  // Get the tagged component from the  profile
  TAO_Tagged_Components &pfile_tagged =
    pfile->tagged_components ();

  // Search for the TaggedComponent that we want
  if (pfile_tagged.get_component (tagged_component) == 1)
    {
      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) TAO-FT - Got a primary component \n"));
        }

      invocation->profile (pfile);
      invocation->endpoint (invocation->profile ()->endpoint ());

      int status =
        this->endpoint_from_profile (invocation
                                     ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);

      if (status == 1)
        {
          if (TAO_debug_level > 4)
            ACE_DEBUG ((LM_DEBUG,
                        "(P|%t) TAO-FT - Got a valid primary \n"));

          return 1;
        }
      else
        {
          if (TAO_debug_level > 2)
            ACE_ERROR ((LM_ERROR,
                        "(P|%t) TAO-FT - We have a primary profile,"
                        " but it is unreachable \n"));
          return -1;
        }

    }

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                "(P|%t) TAO-FT - Not a primary profile  \n"));
  return 0;
}
