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
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "../Supplier.h"

class TAO_Notify_ProxyConsumer;

/**
 * @class TAO_Notify_SequencePushSupplier
 *
 * @brief Wrapper for the SequencePushSupplier that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_Notify_SequencePushSupplier : public TAO_Notify_Supplier
{
public:
  /// Constuctor
  TAO_Notify_SequencePushSupplier (TAO_Notify_ProxyConsumer* proxy);

  /// Destructor
  ~TAO_Notify_SequencePushSupplier ();

  /// Init
  void init (CosNotifyComm::SequencePushSupplier_ptr push_supplier ACE_ENV_ARG_DECL);

  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);

protected:
  /// The Supplier
  CosNotifyComm::SequencePushSupplier_var push_supplier_;
};

#if defined (__ACE_INLINE__)
#include "SequencePushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_Notify_SEQUENCEPUSHSUPPLIER_H */
