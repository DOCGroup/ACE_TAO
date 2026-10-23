/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   SupplierAdmin_i.h
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * This has the implementation of the
 * CosEventChannelAdmin::SupplierAdmin interface.
 */
//=============================================================================

#ifndef SUPPLIER_ADMIN_I_H
#define SUPPLIER_ADMIN_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminC.h"
#include "ProxyPushConsumer_i.h"

class TAO_RTEC_COSEC_Export TAO_CosEC_SupplierAdmin_i :
  public virtual POA_CosEventChannelAdmin::SupplierAdmin
{
  // = TITLE
  //   class TAO_CosEC_SupplierAdmin_i implements the SupplierAdmin interface.
  //
  // = DESCRIPTION
  //   This COS-compliant implementation of the SupplierAdmin uses
  //   TAO's RtecEventChannelAdmin::SupplierAdmin.
public:
  /// Constructor.
  TAO_CosEC_SupplierAdmin_i ();

  /// Destructor.
  ~TAO_CosEC_SupplierAdmin_i () override = default;
  TAO_CosEC_SupplierAdmin_i (TAO_CosEC_SupplierAdmin_i const &) = delete;
  TAO_CosEC_SupplierAdmin_i (TAO_CosEC_SupplierAdmin_i &&) = delete;
  TAO_CosEC_SupplierAdmin_i &operator= (TAO_CosEC_SupplierAdmin_i const &) = delete;
  TAO_CosEC_SupplierAdmin_i &operator= (TAO_CosEC_SupplierAdmin_i &&) = delete;

  /// Initializes the SupplierAdmin.  Returns 0 on success, -1 on
  /// error.
  int init (RtecEventChannelAdmin::SupplierQOS const &supplierqos,
            RtecEventChannelAdmin::SupplierAdmin_ptr rtec_supplieradmin);

  /// Returns a new ProxyPushConsumer_ptr.
  CosEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer () override;

  /// Returns a new ProxyPullConsumer_ptr.
  CosEventChannelAdmin::ProxyPullConsumer_ptr obtain_pull_consumer () override;

private:
  /// The SupplierQOS specified by the user of this class.
  RtecEventChannelAdmin::SupplierQOS qos_;

  /// The RtecEventChannelAdmin::SupplierAdmin specified by the user of
  /// this class.
  RtecEventChannelAdmin::SupplierAdmin_var rtec_supplieradmin_;
};

#include /**/ "ace/post.h"
#endif /* SUPPLIER_ADMIN_I_H */
