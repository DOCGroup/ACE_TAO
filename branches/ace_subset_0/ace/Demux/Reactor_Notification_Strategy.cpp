#include "ace/Demux/Reactor_Notification_Strategy.h"
#include "ace/Demux/Reactor.h"

#if !defined (__ACE_INLINE__)
#include "ace/Demux/Reactor_Notification_Strategy.inl"
#endif /* __ACE_INLINE __ */

ACE_RCSID(ace, Strategies, "$Id$")

ACE_Reactor_Notification_Strategy::ACE_Reactor_Notification_Strategy (ACE_Reactor *reactor,
                                                                      ACE_Event_Handler *eh,
                                                                      ACE_Reactor_Mask mask)
  : ACE_Notification_Strategy (eh, mask),
    reactor_ (reactor)
{
}

ACE_Reactor_Notification_Strategy::~ACE_Reactor_Notification_Strategy (void)
{
}

int
ACE_Reactor_Notification_Strategy::notify (void)
{
  return this->reactor_->notify (this->eh_, this->mask_);
}

int
ACE_Reactor_Notification_Strategy::notify (ACE_Event_Handler *eh,
                                           ACE_Reactor_Mask mask)
{
  return this->reactor_->notify (eh, mask);
}
