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

#ifndef EVENTCHANNEL_I_H
#define EVENTCHANNEL_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "ConsumerAdmin_i.h"
#include "SupplierAdmin_i.h"

class TAO_RTEC_COSEC_Export TAO_CosEC_EventChannel_i :
  public virtual POA_CosEventChannelAdmin::EventChannel
{
  // = TITLE
  //   class TAO_CosEC_EventChannel_i implements the standard Cos EventChannel
  //   interface.
  //
  // = DESCRIPTION
  //   This Cos EventChannel implementation is based on TAO's
  //   Real-time Event Channel.  This TAO_CosEC_EventChannel_i implementation
  //   delegates all its responsibility to corresponding objects from
  //   the real-time implementation.
  //
public:
  // = Initialization and termination methods.
  TAO_CosEC_EventChannel_i (void);
  // Constructor.

  ~TAO_CosEC_EventChannel_i (void);
  // Destructor.

  int init (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
            const RtecEventChannelAdmin::SupplierQOS &supplierqos,
            RtecEventChannelAdmin::EventChannel_ptr rtec
            ACE_ENV_ARG_DECL);
  // Activates the ConsumerAdmin and SupplierAdmin servants.  Returns
  // -1 on error, 0 on success.

  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // The for_consumers method will return the same ConsumerAdmin_ptr
  // everytime its called.

  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // The for_suppliers method will return the same SupplierAdmin_ptr
  // everytime its called.

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Destroys this Event Channel object.

  void shutdown (ACE_ENV_SINGLE_ARG_DECL);
  // destroys this Event Channel object and <delete>s this object.

private:
  TAO_CosEC_ConsumerAdmin_i* consumer_admin_;
  // ConsumerAdmin servant object.

  TAO_CosEC_SupplierAdmin_i* supplier_admin_;
  // SupplierAdmin servant object.

  CosEventChannelAdmin::ConsumerAdmin_var consumeradmin_;
  // The reference to the ConsumerAdmin interface returned after
  // activating the servant in the ORB.

  CosEventChannelAdmin::SupplierAdmin_var supplieradmin_;
  // The reference to the SupplierAdmin interface returned after
  // activating the servant in the ORB.
};

#include /**/ "ace/post.h"
#endif /* EVENTCHANNEL_I_H */
