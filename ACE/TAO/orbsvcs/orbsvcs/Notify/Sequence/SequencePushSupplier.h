/* -*- C++ -*- */
/**
 *  @file SequencePushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_SEQUENCEPUSHSUPPLIER_H
#define TAO_Notify_SEQUENCEPUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Notify/notify_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "orbsvcs/Notify/Supplier.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_ProxyConsumer;

/**
 * @class TAO_Notify_SequencePushSupplier
 *
 * @brief Wrapper for the SequencePushSupplier that connect to the EventChannel.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_SequencePushSupplier : public TAO_Notify_Supplier
{
public:
  /// Constuctor
  TAO_Notify_SequencePushSupplier (TAO_Notify_ProxyConsumer* proxy);

  /// Destructor
  virtual ~TAO_Notify_SequencePushSupplier ();

  /// Init
  void init (CosNotifyComm::SequencePushSupplier_ptr push_supplier ACE_ENV_ARG_DECL);

  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);

  /// Retrieve the ior of this peer
  virtual ACE_CString get_ior (void) const;

protected:
  /// The Supplier
  CosNotifyComm::SequencePushSupplier_var push_supplier_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_SEQUENCEPUSHSUPPLIER_H */
