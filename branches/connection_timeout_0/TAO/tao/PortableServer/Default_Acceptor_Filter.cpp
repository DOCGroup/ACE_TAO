// @(#) $Id$

#include "Default_Acceptor_Filter.h"
#include "tao/Pluggable.h"

ACE_RCSID(tao, Default_Acceptor_Filter, "$Id$")

TAO_Default_Acceptor_Filter::TAO_Default_Acceptor_Filter (void)
{
}

int
TAO_Default_Acceptor_Filter::fill_profile (const TAO_ObjectKey &object_key,
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
TAO_Default_Acceptor_Filter::encode_endpoints (TAO_MProfile &)
{
  // No encoding required.
  return 0;
}
