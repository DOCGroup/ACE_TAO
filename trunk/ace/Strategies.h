/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//   ACE_Strategies.h 
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_STRATEGIES_H)
#define ACE_STRATEGIES_H

#include "ace/Event_Handler.h"

// Forward decls.
class ACE_Reactor;
class ACE_ReactorEx;

class ACE_Export ACE_Notification_Strategy 
  // = TITLE
  //     Abstract class used for notifing an interested party
  //
  // = DESCRIPTION
  //     A vehicle for extending the behavior of ACE_Message_Queue wrt
  //     notification *without subclassing*.  Thus, it's an example of
  //     the Bridge/Strategy patterns.
{
public:
  ACE_Notification_Strategy (ACE_Event_Handler *eh,
			     ACE_Reactor_Mask mask);
  virtual ~ACE_Notification_Strategy (void);

  virtual int notify (void) = 0;
  virtual int notify (ACE_Event_Handler *,
		      ACE_Reactor_Mask mask) = 0;


protected:
  ACE_Event_Handler *eh_;
  ACE_Reactor_Mask mask_;
};

class ACE_Export ACE_Reactor_Notification_Strategy : public ACE_Notification_Strategy
  // = TITLE 
  //     Used to notify an ACE_Reactor
  // = DESCRIPTION
  //     Integrates the ACE_Message_Queue notification into the
  //     ACE_Reactor::notify() method.
{
public:
  ACE_Reactor_Notification_Strategy (ACE_Reactor *reactor,
				     ACE_Event_Handler *eh,
				     ACE_Reactor_Mask mask);

  virtual int notify (void);

  virtual int notify (ACE_Event_Handler *,
		      ACE_Reactor_Mask mask);

private:
  ACE_Reactor *reactor_;
};

class ACE_Export ACE_ReactorEx_Notification_Strategy : public ACE_Notification_Strategy
  // = TITLE 
  //     Used to notify an ACE_ReactorEx
  // = DESCRIPTION
  //     Integrates the ACE_Message_Queue notification into the
  //     ACE_ReactorEx::notify() method.
{
public:
  ACE_ReactorEx_Notification_Strategy (ACE_ReactorEx *reactorex,
				       ACE_Event_Handler *eh,
				       ACE_Reactor_Mask mask);

  virtual int notify (void);

  virtual int notify (ACE_Event_Handler *,
		      ACE_Reactor_Mask mask);

private:
  ACE_ReactorEx *reactorex_;
};

// This needs to come here to avoid circular dependencies.
#include "ace/Strategies_T.h"

#endif /* ACE_STRATEGIES_H */
