// Strategies.cpp
// $Id$

#if !defined (ACE_STRATEGIES_C)
#define ACE_STRATEGIES_C

#define ACE_BUILD_DLL
#include "ace/Reactor.h"
#include "ace/ReactorEx.h"
#include "ace/Strategies.h"

ACE_Notification_Strategy::ACE_Notification_Strategy (ACE_Event_Handler *eh,
						      ACE_Reactor_Mask mask)
  : eh_ (eh),
    mask_ (mask)
{
}

ACE_Notification_Strategy::~ACE_Notification_Strategy (void)
{
}

ACE_Event_Handler *
ACE_Notification_Strategy::event_handler (void)
{
  return eh_;
}

void 
ACE_Notification_Strategy::event_handler (ACE_Event_Handler *eh)
{
  this->eh_ = eh;
}

ACE_Reactor_Mask 
ACE_Notification_Strategy::mask (void)
{
  return mask_;
}

void 
ACE_Notification_Strategy::mask (ACE_Reactor_Mask m)
{
  this->mask_ = m;
}

ACE_Reactor_Notification_Strategy::ACE_Reactor_Notification_Strategy (ACE_Reactor *reactor,
								      ACE_Event_Handler *eh,
								      ACE_Reactor_Mask mask)
  : ACE_Notification_Strategy (eh, mask),
    reactor_ (reactor)
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

ACE_Reactor *
ACE_Reactor_Notification_Strategy::reactor (void)
{
  return this->reactor_;
}

void 
ACE_Reactor_Notification_Strategy::reactor (ACE_Reactor *r)
{
  this->reactor_ = r;
}
  
ACE_ReactorEx_Notification_Strategy::ACE_ReactorEx_Notification_Strategy (ACE_ReactorEx *reactorEx,
									  ACE_Event_Handler *eh,
									  ACE_Reactor_Mask mask)
  : ACE_Notification_Strategy (eh, mask),
    reactorEx_ (reactorEx)
{
}

int 
ACE_ReactorEx_Notification_Strategy::notify (void)
{
  return this->reactorEx_->notify (this->eh_, this->mask_);
}

int 
ACE_ReactorEx_Notification_Strategy::notify (ACE_Event_Handler *eh,
					     ACE_Reactor_Mask mask)
{
  return this->reactorEx_->notify (eh, mask);
}

ACE_ReactorEx *
ACE_ReactorEx_Notification_Strategy::reactorEx (void)
{
  return this->reactorEx_;
}

void 
ACE_ReactorEx_Notification_Strategy::reactorEx (ACE_ReactorEx *r)
{
  this->reactorEx_ = r;
}
  
#endif /* ACE_STRATEGIES_C */

