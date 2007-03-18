// -*- C++ -*-

/**
 *  @file   EC_SupplierControl.h
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

#ifndef TAO_EC_SUPPLIERCONTROL_H
#define TAO_EC_SUPPLIERCONTROL_H

#include /**/ "ace/pre.h"
#include "ace/CORBA_macros.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "orbsvcs/Event/event_serv_export.h"

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_EC_ProxyPushConsumer;

namespace CORBA
{
  class Environment;
  class SystemException;
}

/**
 * @class TAO_EC_SupplierControl
 *
 * @brief SupplierControl
 *
 * Defines the interface for the supplier control strategy.
 * This strategy handles misbehaving or failing suppliers.
 */
class TAO_RTEvent_Serv_Export TAO_EC_SupplierControl
{
public:
  /// Constructor.
  TAO_EC_SupplierControl (void);

  /// Destructor.
  virtual ~TAO_EC_SupplierControl (void);

  /// Activate any internal threads or timers used to poll the state of
  /// the suppliers
  virtual int activate (void);
  virtual int shutdown (void);

  /**
   * Invoked by helper classes when they detect that a supplier does
   * not exists (i.e. _non_existent() returns true and/or the
   * CORBA::OBJECT_NOT_EXIST exception has been raised).
   */
  virtual void supplier_not_exist (TAO_EC_ProxyPushConsumer *proxy);

  /// Some system exception was raised while trying to contact the
  /// supplier
  virtual void system_exception (TAO_EC_ProxyPushConsumer * proxy,
                                 CORBA::SystemException &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_EC_SUPPLIERCONTROL_H */
