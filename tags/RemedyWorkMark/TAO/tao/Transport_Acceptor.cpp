// -*- C++ -*-
// $Id$

#include "tao/Transport_Acceptor.h"
#include "ace/Reactor.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/Transport_Acceptor.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Acceptor::TAO_Acceptor (CORBA::ULong tag)
  : tag_ (tag),
    error_retry_delay_ (5)
{
}

TAO_Acceptor::~TAO_Acceptor (void)
{

  //@@ TAO_ACCEPTOR_DESTRUCTOR_ADD_HOOK
}

int
TAO_Acceptor::handle_accept_error (ACE_Event_Handler* base_acceptor)
{
  if (errno == EMFILE || errno == ENFILE)
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - "
                              "TAO_Acceptor::handle_accept_error - "
                              "Too many files open\n"));

      // If the user has decided to stop accepting when the file handles
      // run out, just return -1;
      if (this->error_retry_delay_ == 0)
        return -1;

      // Get the reactor.  If there isn't one, which isn't very likely,
      // then just return -1.
      ACE_Reactor* reactor = base_acceptor->reactor ();
      if (reactor == 0)
        return -1;

      // So that the reactor doesn't completely remove this handler from
      // the reactor, register it with the except mask.  It should be
      // removed in the timer handler.
      reactor->register_handler (base_acceptor,
                                 ACE_Event_Handler::EXCEPT_MASK);

      // Remove the handler so that the reactor doesn't attempt to
      // process this handle again (and tightly spin).
      reactor->remove_handler (base_acceptor,
                               ACE_Event_Handler::ACCEPT_MASK |
                               ACE_Event_Handler::DONT_CALL);

      // Schedule a timer so that we can resume the handler in hopes
      // that some file handles have freed up.
      ACE_Time_Value timeout (this->error_retry_delay_);
      reactor->schedule_timer (base_acceptor, 0, timeout);
    }

  // We want to keep accepting in all other situations.
  return 0;
}

int
TAO_Acceptor::handle_expiration (ACE_Event_Handler* base_acceptor)
{
  // Get the reactor.  If there isn't one, which isn't very likely, then
  // just return -1;
  ACE_Reactor* reactor = base_acceptor->reactor ();
  if (reactor == 0)
    return -1;

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "TAO (%P|%t) - "
                          "TAO_Acceptor::handle_expiration - "
                          "Re-registering the acceptor\n"));

  // Try again to allow incoming connections
  reactor->register_handler (base_acceptor, ACE_Event_Handler::ACCEPT_MASK);

  // Remove the except mask that was added during the handling of the
  // accept() error.  That is the only reason that we're in this method
  // in the first place.
  reactor->remove_handler (base_acceptor, ACE_Event_Handler::EXCEPT_MASK |
                                          ACE_Event_Handler::DONT_CALL);
  return 0;
}

//@@ TAO_ACCEPTOR_SPL_METHODS_ADD_HOOK

TAO_END_VERSIONED_NAMESPACE_DECL
