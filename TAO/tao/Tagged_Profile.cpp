// $Id$

#include "tao/Tagged_Profile.h"
#include "tao/ORB_Core.h"
#include "tao/Acceptor_Registry.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Pluggable.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Tagged_Profile.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Tagged_Profile, "$Id$")

CORBA::Boolean
TAO_Tagged_Profile::extract_object_key (IOP::TaggedProfile &profile)
{
  // Get our Acceptor registry
  TAO_Acceptor_Registry &acceptor_registry =
    this->orb_core_->lane_resources ().acceptor_registry ();

  // Get the right acceptor for the tag in the TaggedProfile
  TAO_Acceptor *acceptor =
    acceptor_registry.get_acceptor (profile.tag);

  if (acceptor)
    {
      // Get the object key
      int retval =
        acceptor->object_key (profile,
                              this->object_key_);
      if (retval == -1)
        return 0;
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%P|%t)TAO_Tagged_Profile \n")),
                          0);
    }

  return 1;
}
