// $Id$

#include "tao/Resume_Handle_Deferred.h"
#include "tao/Resume_Handle.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

#if !defined (__ACE_INLINE__)
#  include "Resume_Handle_Deferred.inl"
#endif /* __ACE_INLINE__ */

TAO_Resume_Handle_Deferred::TAO_Resume_Handle_Deferred  (TAO_ORB_Core* orbc, ACE_HANDLE h)
      : ACE_Event_Handler (orbc->reactor())
      , h_ (h)
      , orbc_ (orbc)
{
}

TAO_Resume_Handle_Deferred::~TAO_Resume_Handle_Deferred  (void)
{
}

int
TAO_Resume_Handle_Deferred:: handle_timeout (const ACE_Time_Value &, const void *)
{
  if (handle_timeout_eh () == -1)
    return -1;

  // Send a notification to the reactor to cause the awakening of a new
  // follower, if there is one already available.
  ACE_Reactor *reactor = this->orbc_->reactor ();
  int retval = reactor->notify (0, ACE_Event_Handler::READ_MASK);
  if (TAO_debug_level > 2)
    {
      // @@todo: need to think about what is the action that
      // we can take when we get here with an error?!
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - TAO_Resume_Handle_Deferred[%d]::handle_timeout, ")
                  ACE_TEXT ("a connection_handler has been resumed, ")
                  ACE_TEXT ("notified the reactor, retval=%d.\n"), h_, retval));
    }
  return -1; // -1 will cause the reactor to call handle_close() and dispose of the instance
}


