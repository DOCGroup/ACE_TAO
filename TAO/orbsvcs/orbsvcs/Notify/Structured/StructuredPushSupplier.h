/* -*- C++ -*- */
/**
 *  @file StructuredPushSupplier.h
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 */

#ifndef TAO_Notify_STRUCTUREDPUSHSUPPLIER_H
#define TAO_Notify_STRUCTUREDPUSHSUPPLIER_H
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
 * @class TAO_Notify_StructuredPushSupplier
 *
 * @brief Wrapper for the StructuredPushSupplier that connect to the EventChannel.
 */
class TAO_Notify_Serv_Export TAO_Notify_StructuredPushSupplier : public TAO_Notify_Supplier
{
public:
  /// Constructor
  TAO_Notify_StructuredPushSupplier (TAO_Notify_ProxyConsumer* proxy);

  /// Destructor
  virtual ~TAO_Notify_StructuredPushSupplier ();

  /// Init
  void init (CosNotifyComm::StructuredPushSupplier_ptr push_supplier);

  /// Retrieve the ior of this peer
  virtual ACE_CString get_ior () const;

protected:
  virtual CORBA::Object_ptr get_supplier ();

  /// The Supplier
  CosNotifyComm::StructuredPushSupplier_var push_supplier_;

private:
  /// Release
  virtual void release ();
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_STRUCTUREDPUSHSUPPLIER_H */
