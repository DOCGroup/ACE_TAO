// -*- C++ -*-

/**
 *  @file   EC_ConsumerControl.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_CONSUMERCONTROL_H
#define TAO_EC_CONSUMERCONTROL_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#include /**/ "orbsvcs/Event/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_Event_Channel_Base;
class TAO_EC_ProxyPushSupplier;

namespace CORBA
{
  class Environment;
  class SystemException;
}

/**
 * @class TAO_EC_ConsumerControl
 *
 * @brief ConsumerControl
 *
 * Defines the interface for the consumer control strategy.
 * This strategy handles misbehaving or failing consumers.
 */
class TAO_RTEvent_Serv_Export TAO_EC_ConsumerControl
{
public:
  /// Constructor.
  TAO_EC_ConsumerControl (void);

  /// Destructor.
  virtual ~TAO_EC_ConsumerControl (void);

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
  virtual void consumer_not_exist (TAO_EC_ProxyPushSupplier *proxy);

  /// Some system exception was raised while trying to contact the
  /// consumer
  virtual void system_exception (TAO_EC_ProxyPushSupplier *proxy,
                                 CORBA::SystemException &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_EC_CONSUMERCONTROL_H */
