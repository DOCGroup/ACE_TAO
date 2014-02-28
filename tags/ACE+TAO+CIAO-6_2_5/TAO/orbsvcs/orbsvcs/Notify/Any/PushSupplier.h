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
 * @brief Wrapper for the PushSupplier that connect to the EventChannel.
 *
 */
class TAO_Notify_Serv_Export TAO_Notify_PushSupplier : public TAO_Notify_Supplier
{
public:
  /// Constructor
  TAO_Notify_PushSupplier (TAO_Notify_ProxyConsumer* proxy);

  /// Destructor
  virtual ~TAO_Notify_PushSupplier ();

  /// Init
  void init (CosEventComm::PushSupplier_ptr push_supplier);

  /// Retrieve the ior of this peer
  virtual ACE_CString get_ior (void) const;

protected:

  virtual CORBA::Object_ptr get_supplier (void);

  /// The Supplier
  CosEventComm::PushSupplier_var push_supplier_;

private:
  /// TAO_Notify_Destroy_Callback methods
  virtual void release (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_Notify_PUSHSUPPLIER_H */
