// Strategies.cpp
// $Id$

#if !defined (ACE_STRATEGIES_C)
#define ACE_STRATEGIES_C

#define ACE_BUILD_DLL
#include "ace/Reactor.h"
#include "ace/ReactorEx.h"
#include "ace/Strategies.h"

int 
ACE_Notification_Strategy::notify (void)
{
  return 0;
}

ACE_Notification_Strategy::~ACE_Notification_Strategy (void)
{
}

int 
ACE_Notification_Strategy::notify (ACE_Event_Handler *,
				   ACE_Reactor_Mask)
{
  return 0;
}

ACE_Reactor_Notification_Strategy::ACE_Reactor_Notification_Strategy (ACE_Reactor *reactor,
								      ACE_Event_Handler *eh,
								      ACE_Reactor_Mask mask)
  : reactor_ (reactor),
    eh_ (eh),
    mask_ (mask)
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

ACE_ReactorEx_Notification_Strategy::ACE_ReactorEx_Notification_Strategy (ACE_ReactorEx *reactorex,
									  ACE_Event_Handler *eh,
									  ACE_Reactor_Mask mask)
  : reactorex_ (reactorex),
    eh_ (eh),
    mask_ (mask)
{
}

int 
ACE_ReactorEx_Notification_Strategy::notify (void)
{
  return this->reactorex_->notify (this->eh_, this->mask_);
}

int 
ACE_ReactorEx_Notification_Strategy::notify (ACE_Event_Handler *eh,
					     ACE_Reactor_Mask mask)
{
  return this->reactorex_->notify (eh, mask);
}

#endif /* ACE_STRATEGIES_C */
