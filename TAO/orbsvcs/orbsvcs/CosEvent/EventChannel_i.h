/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   EventChannel_i
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This has the implementation of the CosEventChannelAdmin::EventChannel interface.
// ============================================================================

#if !defined (_EVENTCHANNEL_I_H)
#define _EVENTCHANNEL_I_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEvent/ConsumerAdmin_i.h"
#include "orbsvcs/CosEvent/SupplierAdmin_i.h"

class EventChannel_i : public POA_CosEventChannelAdmin::EventChannel
{
  // = TITLE
  //   class EventChannel-i implements the EventChannel interface.
  // = DESCRIPTION
  //   The Cos EventChannel is based on the Real time event channel.
  //   This EventChannel_i implementation delegates all its responsibility to
  //   corresponding objects from the real time implementation.
  //
public:
  // = Initialization and termination methods.
  EventChannel_i (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
                  const RtecEventChannelAdmin::SupplierQOS &supplierqos,
                  RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin,
                  RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin);
  // Initializes the data members.

  ~EventChannel_i ();
  // deactivates the EventChannel.

  int init (CORBA::Environment &TAO_TRY_ENV);
  // activates the ConsumerAdmin and SupplierAdmin servants.
  // returns -1 on error, 0 on success.

  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (CORBA::Environment &TAO_TRY_ENV);
  // The for_consumers method will return the same ConsumerAdmin_ptr everytime its called.

  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (CORBA::Environment &TAO_TRY_ENV);
  // The for_suppliers method will return the same SupplierAdmin_ptr everytime its called.

  virtual void destroy (CORBA::Environment &TAO_TRY_ENV);
  // destroys this object.

private:
  ConsumerAdmin_i consumer_admin_;
  SupplierAdmin_i supplier_admin_;

  CosEventChannelAdmin::ConsumerAdmin_ptr consumeradmin_ ;
  CosEventChannelAdmin::SupplierAdmin_ptr supplieradmin_;
};

#endif //_EVENTCHANNEL_I_H
