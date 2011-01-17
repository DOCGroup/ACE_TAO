// -*- C++ -*-
// $Id$

#include "tao/PortableServer/Default_Acceptor_Filter.h"
#include "tao/Transport_Acceptor.h"
#include "tao/MProfile.h"
#include "tao/Profile.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Default_Acceptor_Filter::TAO_Default_Acceptor_Filter (void)
{
}

int
TAO_Default_Acceptor_Filter::fill_profile (const TAO::ObjectKey &object_key,
                                           TAO_MProfile &mprofile,
                                           TAO_Acceptor **acceptors_begin,
                                           TAO_Acceptor **acceptors_end,
                                           CORBA::Short priority)
{
  // Go through all the acceptors.
  for (TAO_Acceptor** acceptor = acceptors_begin;
       acceptor != acceptors_end;
       ++acceptor)
    {
      // Ask each acceptor to make a profile.
      if ((*acceptor)->create_profile (object_key,
                                       mprofile,
                                       priority) == -1)
        return -1;
    }

  return 0;
}

int
TAO_Default_Acceptor_Filter::encode_endpoints (TAO_MProfile &mprofile)
{
  // if -ORBUseSharedProfile is set, there may be multiple endpoints
  // per profile, even without priority.
  for (CORBA::ULong i = 0;
       i < mprofile.profile_count ();
       ++i)
    {
      TAO_Profile *profile = mprofile.get_profile (i);
      if (profile->encode_alternate_endpoints () == -1)
        return -1;
    }

  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
