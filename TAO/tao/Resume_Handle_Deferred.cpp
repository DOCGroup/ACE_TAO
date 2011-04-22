// $Id$

#include "tao/Resume_Handle_Deferred.h"
#include "tao/Resume_Handle.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Resume_Handle_Deferred::TAO_Resume_Handle_Deferred  (TAO_ORB_Core* orbc, ACE_Event_Handler* h)
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
  // Send a notification to the reactor to cause the awakening of a new
  // follower, if there is one already available.
  ACE_Reactor *reactor = this->orbc_->reactor ();
  int const retval = reactor->notify (this->h_, ACE_Event_Handler::READ_MASK);
  if (TAO_debug_level > 2)
    {
      // @@todo: need to think about what is the action that
      // we can take when we get here with an error?!
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - TAO_Resume_Handle_Deferred[%d]::handle_timeout, ")
                  ACE_TEXT ("a connection_handler has been resumed, ")
                  ACE_TEXT ("notified the reactor, retval=%d.\n"), h_->get_handle (), retval));
    }
  return -1; // -1 will cause the reactor to call handle_close() and dispose of the instance
}

int
TAO_Resume_Handle_Deferred::handle_close (ACE_HANDLE, ACE_Reactor_Mask)
{
  if (TAO_debug_level > 5)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - TAO_Resume_Handle_Deferred[%d]::handle_close, ")
                ACE_TEXT ("performing a ritual sepuku\n"), this->h_->get_handle ()));
  delete this;
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL

