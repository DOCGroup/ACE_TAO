/* -*- C++ -*- */
/**
 *  @file Direct_Supplier.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_Notify_Tests_DIRECT_SUPPLIER_H
#define TAO_Notify_Tests_DIRECT_SUPPLIER_H
#include /**/ "ace/pre.h"

#include "notify_test_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Periodic_Supplier.h"

/**
 * @class TAO_Notify_Tests_Direct_Supplier
 *
 * @brief Send Directly to a consumer.
 *
 */
class TAO_NOTIFY_TEST_Export TAO_Notify_Tests_Direct_Supplier : public TAO_Notify_Tests_Periodic_Supplier
{
public:
  /// Constuctor
  TAO_Notify_Tests_Direct_Supplier (ACE_CString& target);

  /// Destructor
  ~TAO_Notify_Tests_Direct_Supplier ();

  /// Connect using options parsed and set initial QoS.
  virtual void connect (ACE_ENV_SINGLE_ARG_DECL);

  // Send one event. Bypass sending to the Notify and send directly to taget consumer.
  virtual void send_event (const CosNotification::StructuredEvent& event
                           ACE_ENV_ARG_DECL);

protected:
  /// Target object.
  ACE_CString target_;

  /// This object is resolved in the connect method.
  CosNotifyComm::StructuredPushConsumer_var target_object_;
};

#if defined (__ACE_INLINE__)
#include "Direct_Supplier.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_Notify_Tests_DIRECT_SUPPLIER_H */
