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

#ifndef TAO_NS_SEQUENCEPUSHSUPPLIER_H
#define TAO_NS_SEQUENCEPUSHSUPPLIER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "../Supplier.h"

class TAO_NS_ProxyConsumer;

/**
 * @class TAO_NS_SequencePushSupplier
 *
 * @brief Wrapper for the SequencePushSupplier that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_NS_SequencePushSupplier : public TAO_NS_Supplier
{
public:
  /// Constuctor
  TAO_NS_SequencePushSupplier (TAO_NS_ProxyConsumer* proxy);

  /// Destructor
  ~TAO_NS_SequencePushSupplier ();

  /// Init
  void init (CosNotifyComm::SequencePushSupplier_ptr push_supplier ACE_ENV_ARG_DECL);

  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

protected:
  /// The Supplier
  CosNotifyComm::SequencePushSupplier_var push_supplier_;
};

#if defined (__ACE_INLINE__)
#include "SequencePushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_SEQUENCEPUSHSUPPLIER_H */
