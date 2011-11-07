// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_ConsumerControl.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)More details can be found in:http://www.cs.wustl.edu/~coryan/EC/index.html
 */
//=============================================================================


#ifndef TAO_CEC_CONSUMERCONTROL_H
#define TAO_CEC_CONSUMERCONTROL_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushSupplier;
class TAO_CEC_ProxyPullSupplier;

namespace CORBA
{
  class SystemException;
}

/**
 * @class TAO_CEC_ConsumerControl
 *
 * @brief ConsumerControl
 *
 * Defines the interface for the consumer control strategy.
 * This strategy handles misbehaving or failing consumers.
 * = MEMORY MANAGMENT
 * = LOCKING
 * = TODO
 */
class TAO_Event_Serv_Export TAO_CEC_ConsumerControl
{
public:
  /// Constructor.  It does not assume ownership of the <event_channel>
  /// parameter.
  TAO_CEC_ConsumerControl (void);

  /// destructor...
  virtual ~TAO_CEC_ConsumerControl (void);

  /// Activate any internal threads or timers used to poll the state of
  /// the consumers
  virtual int activate (void);
  virtual int shutdown (void);

  /**
   * When pushing an event to the consumer a CORBA::OBJECT_NOT_EXIST
   * exception was raised. The only interpretation is that the object
   * has been destroyed.  The strategy has to (at the very least),
   * reclaim all the resources attached to that object.
   */
  virtual void consumer_not_exist (TAO_CEC_ProxyPushSupplier *proxy);

  /**
   * Invoked by helper classes when they detect that a consumer no
   * longer exists (i.e. _non_existent() returns true and/or the
   * CORBA::OBJECT_NOT_EXIST exception has been raised).
   */
  virtual void consumer_not_exist (TAO_CEC_ProxyPullSupplier *proxy);

  /// Some system exception was rasied while trying to push an event.
  virtual void system_exception (TAO_CEC_ProxyPushSupplier *proxy,
                                 CORBA::SystemException &);

  /// Do we need to disconnect this supplier?  The parameter type for
  /// proxy is PortableServer::ServantBase* due to the fact that this
  /// method will be used for TAO_CEC_ProxyPushSupplier's and
  /// TAO_CEC_ProxyPullSupplier's.
  virtual bool need_to_disconnect (PortableServer::ServantBase* proxy);

  /// Allow others to inform us when a send or receive was successful.
  virtual void successful_transmission (PortableServer::ServantBase* proxy);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_CEC_CONSUMERCONTROL_H */
