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

#ifndef TAO_NS_STRUCTUREDPUSHSUPPLIER_H
#define TAO_NS_STRUCTUREDPUSHSUPPLIER_H
#include "ace/pre.h"

#include "../notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/CosNotifyCommC.h"
#include "../Supplier.h"
#include "../Destroy_Callback.h"

/**
 * @class TAO_NS_StructuredPushSupplier
 *
 * @brief Wrapper for the StructuredPushSupplier that connect to the EventChannel. 
 *
 */
class TAO_Notify_Export TAO_NS_StructuredPushSupplier : public TAO_NS_Supplier, public TAO_NS_Destroy_Callback
{
public:
  /// Constuctor
  TAO_NS_StructuredPushSupplier (CosNotifyComm::StructuredPushSupplier_ptr push_supplier);

  /// Destructor
  ~TAO_NS_StructuredPushSupplier (); 
  
  /// TAO_NS_Destroy_Callback methods
  virtual void release (void);

protected:
  /// The Supplier 
  CosNotifyComm::StructuredPushSupplier_var push_supplier_;
};

#if defined (__ACE_INLINE__)
#include "StructuredPushSupplier.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_STRUCTUREDPUSHSUPPLIER_H */
