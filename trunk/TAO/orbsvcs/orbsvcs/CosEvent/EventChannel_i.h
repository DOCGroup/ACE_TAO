/* -*- C++ -*- */
// $Id$

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
//   This has the implementation of the
//   CosEventChannelAdmin::EventChannel interface.
//
// ============================================================================

#if !defined (_EVENTCHANNEL_I_H)
#define _EVENTCHANNEL_I_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEvent/ConsumerAdmin_i.h"
#include "orbsvcs/CosEvent/SupplierAdmin_i.h"

class TAO_ORBSVCS_Export EventChannel_i : public POA_CosEventChannelAdmin::EventChannel
{
  // = TITLE
  //   class EventChannel_i implements the standard Cos EventChannel
  //   interface.
  //
  // = DESCRIPTION
  //   This Cos EventChannel implementation is based on TAO's
  //   Real-time Event Channel.  This EventChannel_i implementation
  //   delegates all its responsibility to corresponding objects from
  //   the real-time implementation.
  //
public:
  // = Initialization and termination methods.
  EventChannel_i (void);
  // Constructor.

  ~EventChannel_i (void);
  // Destructor.

  int init (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
            const RtecEventChannelAdmin::SupplierQOS &supplierqos,
            RtecEventChannelAdmin::EventChannel_ptr rtec,
            CORBA::Environment &TAO_TRY_ENV);
  // Activates the ConsumerAdmin and SupplierAdmin servants.  Returns
  // -1 on error, 0 on success.

  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (CORBA::Environment &TAO_TRY_ENV);
  // The for_consumers method will return the same ConsumerAdmin_ptr
  // everytime its called.

  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (CORBA::Environment &TAO_TRY_ENV);
  // The for_suppliers method will return the same SupplierAdmin_ptr
  // everytime its called.

  virtual void destroy (CORBA::Environment &TAO_TRY_ENV);
  // Destroys this Event Channel object.

  void shutdown (CORBA::Environment &TAO_TRY_ENV);
  // destroys the object and deletes it also.

private:
  ConsumerAdmin_i consumer_admin_;
  // ConsumerAdmin servant object.

  SupplierAdmin_i supplier_admin_;
  // SupplierAdmin servant object.

  CosEventChannelAdmin::ConsumerAdmin_ptr consumeradmin_;
  // @@ Pradeep, please add comments here

  CosEventChannelAdmin::SupplierAdmin_ptr supplieradmin_;
  // @@ Pradeep, please add comments here
};

// @@ Pradeep, please don't use // here, but use /* ... */ instead.
// Please fix all uses of this in your code since it's not portable.
#endif //_EVENTCHANNEL_I_H
