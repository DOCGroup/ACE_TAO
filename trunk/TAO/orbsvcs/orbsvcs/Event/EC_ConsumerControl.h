/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_ConsumerControl
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
//   More details can be found in:
//   http://www.cs.wustl.edu/~coryan/EC/index.html
//
// ============================================================================

#ifndef TAO_EC_CONSUMERCONTROL_H
#define TAO_EC_CONSUMERCONTROL_H

#include "orbsvcs/orbsvcs_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_EC_Event_Channel;
class TAO_EC_ProxyPushSupplier;

class TAO_ORBSVCS_Export TAO_EC_ConsumerControl
{
  // = TITLE
  //   ConsumerControl
  //
  // = DESCRIPTION
  //   Defines the interface for the consumer control strategy.
  //   This strategy handles misbehaving or failing consumers.
  //
  // = MEMORY MANAGMENT
  //
  // = LOCKING
  //
  // = TODO
  //
public:
  TAO_EC_ConsumerControl (void);
  // Constructor.  It does not assume ownership of the <event_channel>
  // parameter.

  virtual ~TAO_EC_ConsumerControl (void);
  // destructor...

  virtual int activate (void);
  virtual int shutdown (void);
  // Activate any internal threads or timers used to poll the state of
  // the consumers

  virtual void consumer_not_exist (TAO_EC_ProxyPushSupplier *proxy,
                                   CORBA::Environment &);
  // When pushing an event to the consumer a CORBA::OBJECT_NOT_EXIST
  // exception was raised. The only interpretation is that the object
  // has been destroyed.  The strategy has to (at the very least),
  // reclaim all the resources attached to that object.

  virtual void system_exception (TAO_EC_ProxyPushSupplier *proxy,
                                 CORBA::SystemException &,
                                 CORBA::Environment &);
  // Some system exception was rasied while trying to push an event.
};

#if defined (__ACE_INLINE__)
#include "EC_ConsumerControl.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_CONSUMERCONTROL_H */
