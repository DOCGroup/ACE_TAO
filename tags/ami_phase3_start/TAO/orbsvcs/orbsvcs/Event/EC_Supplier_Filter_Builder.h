/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Supplier_Filter_Builder
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_SUPPLIER_FILTER_BUILDER_H
#define TAO_EC_SUPPLIER_FILTER_BUILDER_H

#include "orbsvcs/RtecEventChannelAdminC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_SupplierFiltering;
class TAO_EC_ProxyPushConsumer;

class TAO_ORBSVCS_Export TAO_EC_Supplier_Filter_Builder
{
  // = TITLE
  //   Abstract base class for the supplier filter builders.
  //
  // = DESCRIPTION
  //   The creation of the right filter for each supplier controlled
  //   by a Supplier_Filter_Builder.
  //
  //
public:
  virtual ~TAO_EC_Supplier_Filter_Builder (void);
  // destructor...

  virtual TAO_EC_SupplierFiltering*
      create (RtecEventChannelAdmin::SupplierQOS& qos) = 0;
  // Create the filter. The <consumer> is bound to the returned
  // Supplier_Filter, it must be <unbound> upon destruction and/or
  // disconnection.

  virtual void
      destroy (TAO_EC_SupplierFiltering *filter) = 0;
  // The user is returning the filter for destruction/recycling.
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "EC_Supplier_Filter_Builder.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_FILTER_BUILDER_H */
