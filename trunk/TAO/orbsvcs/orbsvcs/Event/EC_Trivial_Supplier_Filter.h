/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_trivial_Supplier_Filter
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

#ifndef TAO_EC_TRIVIAL_SUPPLIER_FILTER_H
#define TAO_EC_TRIVIAL_SUPPLIER_FILTER_H

#include "EC_SupplierFiltering.h"
#include "EC_Supplier_Filter_Builder.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_EC_ProxyPushSupplier_Set;

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_Trivial_Supplier_Filter : public TAO_EC_SupplierFiltering
{
  // = TITLE
  //   A null filtering strategy.
  //
  // = DESCRIPTION
  //   This strategy does no filtering, it is useful for backends of
  //   the CosEventChannel, testing and broadcasters; it uses the
  //   ConsumerAdmin to find all the consumers and pushes the event to
  //   all of them.
  //
public:
  TAO_EC_Trivial_Supplier_Filter (TAO_EC_Event_Channel* ec);
  // Constructor

  // = The TAO_EC_SupplierFiltering methods.
  virtual void bind (TAO_EC_ProxyPushConsumer* consumer);
  virtual void unbind (TAO_EC_ProxyPushConsumer* consumer);
  virtual void connected (TAO_EC_ProxyPushSupplier* supplier,
			  CORBA::Environment &env);
  virtual void disconnected (TAO_EC_ProxyPushSupplier* supplier,
			     CORBA::Environment &env);
  virtual void push (const RtecEventComm::EventSet& event,
                     CORBA::Environment &);

private:
  TAO_EC_Event_Channel *event_channel_;
  // The event channel, used to locate the set of consumers.
};

// ****************************************************************

class TAO_ORBSVCS_Export TAO_EC_Trivial_Supplier_Filter_Builder : public TAO_EC_Supplier_Filter_Builder
{
  // = TITLE
  //   Create a single Trivial_Supplier_Filter.
  //
  // = DESCRIPTION
  //   This Factory creates a single Trivial_Supplier_Filter that is
  //   used by all the suppliers (i.e. ProxyConsumers) of an event
  //   channel.
  //
  //
public:
  TAO_EC_Trivial_Supplier_Filter_Builder (TAO_EC_Event_Channel* ec);
  // constructor....

  // = The TAO_EC_Supplier_Filter_Builder methods...
  virtual TAO_EC_SupplierFiltering*
      create (RtecEventChannelAdmin::SupplierQOS& qos);
  virtual void
      destroy (TAO_EC_SupplierFiltering *filter);

private:
  TAO_EC_Trivial_Supplier_Filter filter_;
  // The filter....
};

#if defined (__ACE_INLINE__)
#include "EC_Trivial_Supplier_Filter.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_TRIVIAL_SUPPLIER_FILTER_H */
