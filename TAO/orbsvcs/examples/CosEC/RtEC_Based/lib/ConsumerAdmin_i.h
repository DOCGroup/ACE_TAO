/* -*- C++ -*- */

//=============================================================================
/**
 *  @file   ConsumerAdmin_i
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 *
 * This has the implementation of the
 * CosEventChannelAdmin::ConsumerAdmin interface.
 *
 *
 */
//=============================================================================


#ifndef CONSUMERADMIN_I_H
#define CONSUMERADMIN_I_H
#include /**/ "ace/pre.h"

#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/CosEventChannelAdminS.h"
#include "ProxyPushSupplier_i.h"

class TAO_RTEC_COSEC_Export TAO_CosEC_ConsumerAdmin_i :
  public virtual POA_CosEventChannelAdmin::ConsumerAdmin
{
  // = TITLE
  //   class TAO_CosEC_ConsumerAdmin_i implements the ConsumerAdmin interface.
  //
  // = DESCRIPTION
  //   This implementation of the ConsumerAdmin uses the
  //   RtecEventChannelAdmin::ConsumerAdmin.
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_CosEC_ConsumerAdmin_i (void);

  /// Destructor.
  ~TAO_CosEC_ConsumerAdmin_i (void);

  int init (const RtecEventChannelAdmin::ConsumerQOS &consumerqos,
            RtecEventChannelAdmin::ConsumerAdmin_ptr rtec_consumeradmin);

  /// Returns a new ProxyPushSupplier_ptr.
  virtual CosEventChannelAdmin::ProxyPushSupplier_ptr
    obtain_push_supplier(void);

  /// Returns a new ProxyPullSupplier_ptr.
  virtual CosEventChannelAdmin::ProxyPullSupplier_ptr
    obtain_pull_supplier(void);

private:
  /// The ConsumerQOS specified by the user of this class.
  RtecEventChannelAdmin::ConsumerQOS qos_;

  /// The RtecEventChannelAdmin::ConsumerAdmin specified by the user of
  /// this class.
  RtecEventChannelAdmin::ConsumerAdmin_var rtec_consumeradmin_;
};

#include /**/ "ace/post.h"
#endif /* CONSUMERADMIN_I_H */
