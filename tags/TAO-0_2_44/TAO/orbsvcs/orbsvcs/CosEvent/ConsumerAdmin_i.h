/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   ConsumerAdmin_i
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This has the implementation of the CosEventChannelAdmin::ConsumerAdmin interface.
// ============================================================================


#if !defined (_CONSUMERADMIN_I_H)
#define _CONSUMERADMIN_I_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "orbsvcs/CosEvent/ProxyPushSupplier_i.h"

class ConsumerAdmin_i : public POA_CosEventChannelAdmin::ConsumerAdmin
{
  // = TITLE
  //   class ConsumerAdmin_i implements the ConsumerAdmin interface.
  // = DESCRIPTION
  //   This implementation of the ConsumerAdmin uses the RtecEventChannelAdmin::ConsumerAdmin.
  //
public:
  // = Initialization and termination methods.
  ConsumerAdmin_i (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
                   RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin);

  ~ConsumerAdmin_i ();

  virtual CosEventChannelAdmin::ProxyPushSupplier_ptr
    obtain_push_supplier(CORBA::Environment &TAO_TRY_ENV);
  // returns a new ProxyPushSupplier_ptr.

  virtual CosEventChannelAdmin::ProxyPullSupplier_ptr
    obtain_pull_supplier(CORBA::Environment &TAO_TRY_ENV);
  // returns a new ProxyPullSupplier_ptr.

private:
  const RtecEventChannelAdmin::ConsumerQOS &qos_;
  // The ConsumerQOS specified by the user of this class.

  RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin_;
  // The RtecEventChannelAdmin::ConsumerAdmin specified by the user of this class.
};

#endif //_CONSUMERADMIN_I_H
