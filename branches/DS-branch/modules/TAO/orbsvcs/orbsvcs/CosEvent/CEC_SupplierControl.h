// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_SupplierControl.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/
 */
//=============================================================================


#ifndef TAO_CEC_SUPPLIERCONTROL_H
#define TAO_CEC_SUPPLIERCONTROL_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableServer/PortableServer.h"
#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushConsumer;
class TAO_CEC_ProxyPullConsumer;

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
class TAO_CEC_TypedProxyPushConsumer;
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

namespace CORBA
{
  class SystemException;
}

/**
 * @class TAO_CEC_SupplierControl
 *
 * @brief SupplierControl
 *
 * Defines the interface for the supplier control strategy.
 * This strategy handles misbehaving or failing suppliers.
 * = MEMORY MANAGMENT
 * = LOCKING
 * = TODO
 */
class TAO_Event_Serv_Export TAO_CEC_SupplierControl
{
public:
  /// Constructor.  It does not assume ownership of the <event_channel>
  /// parameter.
  TAO_CEC_SupplierControl (void);

  /// destructor...
  virtual ~TAO_CEC_SupplierControl (void);

  /// Activate any internal threads or timers used to poll the state of
  /// the suppliers
  virtual int activate (void);
  virtual int shutdown (void);

  /**
   * Invoked by helper classes when they detect that a supplier does
   * not exists (i.e. _non_existent() returns true and/or the
   * CORBA::OBJECT_NOT_EXIST exception has been raised).
   */
  virtual void supplier_not_exist (TAO_CEC_ProxyPushConsumer *proxy);

#if defined (TAO_HAS_TYPED_EVENT_CHANNEL)
  virtual void supplier_not_exist (TAO_CEC_TypedProxyPushConsumer *proxy);
#endif /* TAO_HAS_TYPED_EVENT_CHANNEL */

  /**
   * Invoked by helper classes when they detect that a supplier does
   * not exists (i.e. _non_existent() returns true and/or the
   * CORBA::OBJECT_NOT_EXIST exception has been raised).
   */
  virtual void supplier_not_exist (TAO_CEC_ProxyPullConsumer *proxy);

  /// Some system exception was rasied while trying to push an event.
  virtual void system_exception (TAO_CEC_ProxyPullConsumer *proxy,
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
#endif /* TAO_CEC_SUPPLIERCONTROL_H */
