/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   CEC_SupplierControl
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)More details can be found in:http://www.cs.wustl.edu/~coryan/EC/index.html
 */
//=============================================================================


#ifndef TAO_CEC_SUPPLIERCONTROL_H
#define TAO_CEC_SUPPLIERCONTROL_H
#include "ace/pre.h"

#include "orbsvcs/CosEvent/event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/corba.h"

class TAO_CEC_EventChannel;
class TAO_CEC_ProxyPushConsumer;
class TAO_CEC_ProxyPullConsumer;

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
class TAO_Event_Export TAO_CEC_SupplierControl
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
  virtual void supplier_not_exist (TAO_CEC_ProxyPushConsumer *proxy,
                                   CORBA::Environment &);

  /**
   * Invoked by helper classes when they detect that a supplier does
   * not exists (i.e. _non_existent() returns true and/or the
   * CORBA::OBJECT_NOT_EXIST exception has been raised).
   */
  virtual void supplier_not_exist (TAO_CEC_ProxyPullConsumer *proxy,
                                   CORBA::Environment &);

  /// Some system exception was rasied while trying to push an event.
  virtual void system_exception (TAO_CEC_ProxyPullConsumer *proxy,
                                 CORBA::SystemException &,
                                 CORBA::Environment &);
};

#if defined (__ACE_INLINE__)
#include "CEC_SupplierControl.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_CEC_SUPPLIERCONTROL_H */
