// $Id$

#include "ace/Init_ACE.h"

#include "ace/Object_Manager.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Init_ACE.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

ACE_RCSID(ace, Init_ACE, "$Id$")

  // Static data members.
  u_int ACE_Init_ACE::init_fini_count_ = 0;
int
ACE_Init_ACE::init (void)
{
  // Don't use ACE_TRACE, because Object_Manager might not have been
  // instantiated yet.
  // ACE_TRACE ("ACE_Init_ACE::init");

  ++init_fini_count_;

  return ACE_Object_Manager::instance ()->init ();
}

int
ACE_Init_ACE::fini (void)
{
  ACE_TRACE ("ACE_Init_ACE::fini");

  if (init_fini_count_ > 0)
    {
      if (--init_fini_count_ == 0)
        return ACE_Object_Manager::instance ()->fini ();
      else
        // Wait for remaining fini () calls.
        return 1;
    }
  else
    // More ACE_Init_ACE::fini () calls than ACE_Init_ACE::init () calls.  Bad
    // application!
    return -1;
}
