/* -*- C++ -*- */
/**
 *  @file   ECG_ConsumerEventChannelControl.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 */

#ifndef TAO_ECG_ConsumerEC_Control_H
#define TAO_ECG_ConsumerEC_Control_H
#include /**/ "ace/pre.h"

#include /**/ "event_export.h"
#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_EC_Gateway_IIOP;
class TAO_EC_ProxyPushSupplier;


/**
 * @class TAO_ECG_ConsumerEC_Control
 *
 * @brief EventChannelControl
 *
 * Defines the interface for the consumer event channel control strategy.
 * This strategy handles misbehaving or failing event channels that are consumer
 * of a gateway.
 */
class TAO_RTEvent_Export TAO_ECG_ConsumerEC_Control
{
public:
  /// Constructor.
  TAO_ECG_ConsumerEC_Control (void);

  /// Destructor.
  virtual ~TAO_ECG_ConsumerEC_Control (void);

  /// Activate any internal threads or timers used to poll the state of
  /// the event channel.
  virtual int activate (void);

  /// Shutdown any internal threads or timers used to poll the state of
  /// the event channel.
  virtual int shutdown (void);

  /**
   * When pushing an event to the event channel a CORBA::OBJECT_NOT_EXIST
   * exception was raised. The only interpretation is that the object
   * has been destroyed.  The strategy has to (at the very least),
   * reclaim all the resources attached to that object.
   */
  virtual void event_channel_not_exist (TAO_EC_Gateway_IIOP * gateway
                                        ACE_ENV_ARG_DECL_NOT_USED);

  /// Some system exception was raised while trying to contact the
  /// event channel
  virtual void system_exception (TAO_EC_Gateway_IIOP * gateway,
                                 CORBA::SystemException &
                                 ACE_ENV_ARG_DECL_NOT_USED);
};

#if defined (__ACE_INLINE__)
#include "ECG_ConsumerEC_Control.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ECG_ConsumerEC_Control_H */
