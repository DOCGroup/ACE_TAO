// $Id$

#include "tao/Resume_Handle.h"
#include "tao/debug.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE int
TAO_Resume_Handle_Deferred::handle_timeout_eh ()
{
  TAO_Resume_Handle resume_handle (this->orbc_, this->h_);
  if (TAO_debug_level > 5)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - TAO_Resume_Handle_Deferred[%d]::handle_timeout_eh, ")
                ACE_TEXT ("resuming handle in the reactor\n"), h_));
  return 0;
}

ACE_INLINE int
TAO_Resume_Handle_Deferred::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  if (TAO_debug_level > 5)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - TAO_Resume_Handle_Deferred[%d]::handle_close, ")
                ACE_TEXT ("performing a ritual sepuku\n"), this->h_));
  delete this;
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
