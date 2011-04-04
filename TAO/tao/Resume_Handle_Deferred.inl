// $Id$

#include "tao/Resume_Handle.h"
#include "tao/debug.h"

ACE_INLINE int 
TAO_Resume_Handle_Deferred::handle_timeout_eh ()
{
  TAO_Resume_Handle resume_handle (orbc_, h_);
  if (TAO_debug_level > 5)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - TAO_Resume_Handle_Deferred::handle_timeout_eh, ")
                ACE_TEXT ("resuming handle %d in the reactor\n"), h_));
  return 0;
}

ACE_INLINE int 
TAO_Resume_Handle_Deferred::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  if (TAO_debug_level > 5)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - TAO_Resume_Handle_Deferred::handle_close, ")
                ACE_TEXT ("performing a ritual sepuku\n"), h_));
  delete this;
  return 0;
}
