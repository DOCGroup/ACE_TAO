/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//   TAO services
//
// = FILENAME
//   SupplierAdmin_i.h
//
// = AUTHOR
//   Pradeep Gore <pradeep@cs.wustl.edu>
//
// = DESCRIPTION
//   This has the implementation of the
//   CosEventChannelAdmin::SupplierAdmin interface.
//
// ============================================================================

#if !defined (SUPPLIER_ADMIN_I_H)
#define SUPPLIER_ADMIN_I_H

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "orbsvcs/CosEvent/ProxyPushConsumer_i.h"

class TAO_ORBSVCS_Export TAO_CosEC_SupplierAdmin_i :
  public virtual POA_CosEventChannelAdmin::SupplierAdmin,
  public virtual PortableServer::RefCountServantBase
{
  // = TITLE
  //   class TAO_CosEC_SupplierAdmin_i implements the SupplierAdmin interface.
  //
  // = DESCRIPTION
  //   This COS-compliant implementation of the SupplierAdmin uses
  //   TAO's RtecEventChannelAdmin::SupplierAdmin.
public:
  // = Initialization and termination methods.
  TAO_CosEC_SupplierAdmin_i (void);
  // Constructor.

  ~TAO_CosEC_SupplierAdmin_i (void);
  // Destructor.

  int init (const RtecEventChannelAdmin::SupplierQOS &supplierqos,
            RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin);
  // Initializes the SupplierAdmin.  Returns 0 on success, -1 on
  // error.

  virtual CosEventChannelAdmin::ProxyPushConsumer_ptr
  obtain_push_consumer (CORBA::Environment &ACE_TRY_ENV);
  // Returns a new ProxyPushConsumer_ptr.

  virtual CosEventChannelAdmin::ProxyPullConsumer_ptr
  obtain_pull_consumer(CORBA::Environment &ACE_TRY_ENV);
  // Returns a new ProxyPullConsumer_ptr.

private:
  RtecEventChannelAdmin::SupplierQOS qos_;
  // The SupplierQOS specified by the user of this class.

  RtecEventChannelAdmin::SupplierAdmin_var rtec_supplieradmin_;
  // The RtecEventChannelAdmin::SupplierAdmin specified by the user of
  // this class.
};

#endif /* SUPPLIER_ADMIN_I_H */
