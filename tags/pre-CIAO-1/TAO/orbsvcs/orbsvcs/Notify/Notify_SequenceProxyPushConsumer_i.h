/* -*- C++ -*- */
//=============================================================================
/**
 * @file Notify_SequenceProxyPushConsumer_i.h
 *
 * $Id$
 *
 * Implements the CosNotifyChannelAdmin::SequenceProxyPushConsumer
 * interface.
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_SEQUENCEPROXYPUSHCONSUMER_I_H
#define TAO_NOTIFY_SEQUENCEPROXYPUSHCONSUMER_I_H
#include "ace/pre.h"

#include "Notify_ProxyConsumer_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_SupplierAdmin_i;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_SequenceProxyPushConsumer_i
 *
 * @brief TAO_Notify_SequenceProxyPushConsumer_i
 *
 * Implements CosNotifyChannelAdmin::SequenceProxyPushConsumer
 */
class TAO_Notify_Export TAO_Notify_SequenceProxyPushConsumer_i : public TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>
{

public:
  /// Constructor
  TAO_Notify_SequenceProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* supplier_admin);

  /// Destructor
  virtual ~TAO_Notify_SequenceProxyPushConsumer_i (void);

  // = interface methods
  virtual void connect_sequence_push_supplier (
    CosNotifyComm::SequencePushSupplier_ptr push_supplier
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

  virtual void push_structured_events (
    const CosNotification::EventBatch & notifications
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ));

  virtual void disconnect_sequence_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
  // = Helper methods
  /// Sends updates to the supplier.
 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed ACE_ENV_ARG_DECL);

  // = Data members
  /// The supplier that we're connected to.
  CosNotifyComm::SequencePushSupplier_ptr push_supplier_;

private:
  typedef TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer> proxy_inherited;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_SEQUENCEPROXYPUSHCONSUMER_I_H */
