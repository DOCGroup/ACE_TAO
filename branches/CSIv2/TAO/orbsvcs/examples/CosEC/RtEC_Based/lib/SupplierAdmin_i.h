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

#ifndef SUPPLIER_ADMIN_I_H
#define SUPPLIER_ADMIN_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "ProxyPushConsumer_i.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_RTEC_COSEC_Export TAO_CosEC_SupplierAdmin_i :
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
    obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Returns a new ProxyPushConsumer_ptr.

  virtual CosEventChannelAdmin::ProxyPullConsumer_ptr
    obtain_pull_consumer(ACE_ENV_SINGLE_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException));
  // Returns a new ProxyPullConsumer_ptr.

private:
  RtecEventChannelAdmin::SupplierQOS qos_;
  // The SupplierQOS specified by the user of this class.

  RtecEventChannelAdmin::SupplierAdmin_var rtec_supplieradmin_;
  // The RtecEventChannelAdmin::SupplierAdmin specified by the user of
  // this class.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* SUPPLIER_ADMIN_I_H */
