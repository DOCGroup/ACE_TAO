/* -*- C++ -*- */
/**
 *  @file   EC_Supplier_Filter_Builder.h
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

#ifndef TAO_EC_SUPPLIER_FILTER_BUILDER_H
#define TAO_EC_SUPPLIER_FILTER_BUILDER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "event_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_Supplier_Filter;
class TAO_EC_ProxyPushConsumer;

/**
 * @class TAO_EC_Supplier_Filter_Builder
 *
 * @brief Abstract base class for the supplier filter builders.
 *
 * The creation of the right filter for each supplier controlled
 * by a Supplier_Filter_Builder.
 */
class TAO_RTEvent_Export TAO_EC_Supplier_Filter_Builder
{
public:
  /// Destructor...
  virtual ~TAO_EC_Supplier_Filter_Builder (void);

  /**
   * Create the filter. The <consumer> is bound to the returned
   * Supplier_Filter, it must be <unbound> upon destruction and/or
   * disconnection.
   */
  virtual TAO_EC_Supplier_Filter*
      create (RtecEventChannelAdmin::SupplierQOS& qos) = 0;

  /// The user is returning the filter for destruction/recycling.
  virtual void
      destroy (TAO_EC_Supplier_Filter *filter) = 0;
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Supplier_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_EC_FILTER_BUILDER_H */
