/* -*- C++ -*- */
/**
 *  @file StructuredPushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_STRUCTUREDPUSHSUPPLIER_H
#define TAO_Notify_STRUCTUREDPUSHSUPPLIER_H
#include /**/ "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "../Supplier.h"

class TAO_Notify_ProxyConsumer;

/**
 * @class TAO_Notify_StructuredPushSupplier
 *
 * @brief Wrapper for the StructuredPushSupplier that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_Notify_StructuredPushSupplier : public TAO_Notify_Supplier
{
public:
  /// Constuctor
  TAO_Notify_StructuredPushSupplier (TAO_Notify_ProxyConsumer* proxy);

  /// Destructor
  ~TAO_Notify_StructuredPushSupplier ();

  /// Init
  void init (CosNotifyComm::StructuredPushSupplier_ptr push_supplier ACE_ENV_ARG_DECL);

  /// Release
  virtual void release (void);

protected:
  /// The Supplier
  CosNotifyComm::StructuredPushSupplier_var push_supplier_;
};

#if defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_STRUCTUREDPUSHSUPPLIER_H */
