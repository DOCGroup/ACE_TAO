/* -*- C++ -*- */
/**
 *  @file PushSupplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_PUSHSUPPLIER_H
#define TAO_Notify_PUSHSUPPLIER_H
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
 * @brief Wrapper for the PushSupplier that connect to the EventChannel.
 *
 */
class TAO_Notify_Export TAO_Notify_PushSupplier : public TAO_Notify_Supplier
{
public:
  /// Constuctor
  TAO_Notify_PushSupplier (TAO_Notify_ProxyConsumer* proxy);

  /// Destructor
  ~TAO_Notify_PushSupplier ();

  /// Init
  void init (CosEventComm::PushSupplier_ptr push_supplier ACE_ENV_ARG_DECL);

  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);

protected:
  /// The Supplier
  CosEventComm::PushSupplier_var push_supplier_;
};

#if defined (__ACE_INLINE__)
#include "PushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PUSHSUPPLIER_H */
