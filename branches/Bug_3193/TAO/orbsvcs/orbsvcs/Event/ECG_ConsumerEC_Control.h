// -*- C++ -*-

/**
 *  @file   ECG_ConsumerEC_Control.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen (jwillemsen@remedy.nl)
 *
 */

#ifndef TAO_ECG_ConsumerEC_Control_H
#define TAO_ECG_ConsumerEC_Control_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Gateway_IIOP;
class TAO_EC_ProxyPushSupplier;

namespace CORBA
{
  class Environment;
  class SystemException;
}

/**
 * @class TAO_ECG_ConsumerEC_Control
 *
 * @brief EventChannelControl
 *
 * Defines the interface for the consumer event channel control strategy.
 * This strategy handles misbehaving or failing event channels that are consumer
 * of a gateway.
 */
class TAO_RTEvent_Serv_Export TAO_ECG_ConsumerEC_Control
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
  virtual void event_channel_not_exist (TAO_EC_Gateway_IIOP * gateway);

  /// Some system exception was raised while trying to contact the
  /// event channel
  virtual void system_exception (TAO_EC_Gateway_IIOP * gateway,
                                 CORBA::SystemException &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ECG_ConsumerEC_Control_H */
