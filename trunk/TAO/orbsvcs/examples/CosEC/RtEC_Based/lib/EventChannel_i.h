/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   EventChannel_i
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * This has the implementation of the
 * CosEventChannelAdmin::EventChannel interface.
 *
 *
 */
//=============================================================================


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
  /// Constructor.
  TAO_CosEC_EventChannel_i (void);

  /// Destructor.
  ~TAO_CosEC_EventChannel_i (void);

  /// Activates the ConsumerAdmin and SupplierAdmin servants.  Returns
  /// -1 on error, 0 on success.
  int init (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
            const RtecEventChannelAdmin::SupplierQOS &supplierqos,
            RtecEventChannelAdmin::EventChannel_ptr rtec);

  /// The for_consumers method will return the same ConsumerAdmin_ptr
  /// everytime its called.
  virtual CosEventChannelAdmin::ConsumerAdmin_ptr for_consumers (void);

  /// The for_suppliers method will return the same SupplierAdmin_ptr
  /// everytime its called.
  virtual CosEventChannelAdmin::SupplierAdmin_ptr for_suppliers (void);

  /// Destroys this Event Channel object.
  virtual void destroy (void);

  /// destroys this Event Channel object and <delete>s this object.
  void shutdown (void);

private:
  /// ConsumerAdmin servant object.
  TAO_CosEC_ConsumerAdmin_i* consumer_admin_;

  /// SupplierAdmin servant object.
  TAO_CosEC_SupplierAdmin_i* supplier_admin_;

  /// The reference to the ConsumerAdmin interface returned after
  /// activating the servant in the ORB.
  CosEventChannelAdmin::ConsumerAdmin_var consumeradmin_;

  /// The reference to the SupplierAdmin interface returned after
  /// activating the servant in the ORB.
  CosEventChannelAdmin::SupplierAdmin_var supplieradmin_;
};

#include /**/ "ace/post.h"
#endif /* EVENTCHANNEL_I_H */
