/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   SupplierAdmin_i
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This has the implementation of the
//   CosEventChannelAdmin::SupplierAdmin interface.
//
// ============================================================================

#if !defined (_SUPPLIERADMIN_I_H)
#define _SUPPLIERADMIN_I_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEvent/ProxyPushConsumer_i.h"

class TAO_ORBSVCS_Export SupplierAdmin_i : public POA_CosEventChannelAdmin::SupplierAdmin
{
  // = TITLE
  //   class SupplierAdmin_i implements the SupplierAdmin interface.
  //
  // = DESCRIPTION
  //   This COS-compliant implementation of the SupplierAdmin uses
  //   TAO's RtecEventChannelAdmin::SupplierAdmin.
public:
  // = Initialization and termination methods.
  SupplierAdmin_i (void);
  // Constructor.

  ~SupplierAdmin_i (void);
  // Destructor.

  int init (const RtecEventChannelAdmin::SupplierQOS &supplierqos,
            RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin);
  // Initializes the SupplierAdmin.
  // Returns 0 on success, -1 on error.

  virtual CosEventChannelAdmin::ProxyPushConsumer_ptr
  obtain_push_consumer (CORBA::Environment &TAO_TRY_ENV);
  // Returns a new ProxyPushConsumer_ptr.

  virtual CosEventChannelAdmin::ProxyPullConsumer_ptr
  obtain_pull_consumer(CORBA::Environment &TAO_TRY_ENV);
  // Returns a new ProxyPullConsumer_ptr.

private:
  RtecEventChannelAdmin::SupplierQOS qos_;
  // The SupplierQOS specified by the user of this class.

  RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin_;
  // The RtecEventChannelAdmin::SupplierAdmin specified by the user of
  // this class.
};

#endif /* _SUPPLIERADMIN_I_H */
