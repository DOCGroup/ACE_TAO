/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   TAO_Notify_SequenceProxyPushConsumer_i.h
//
// = DESCRIPTION
//   Implements the CosNotifyChannelAdmin::SequenceProxyPushConsumer
//   interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_SEQUENCEPROXYPUSHCONSUMER_I_H
#define TAO_NOTIFY_SEQUENCEPROXYPUSHCONSUMER_I_H

#include "Notify_ProxyConsumer_T.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_SupplierAdmin_i;
class TAO_Notify_Resource_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORBSVCS_Export TAO_Notify_SequenceProxyPushConsumer_i : public TAO_Notify_ProxyConsumer <POA_CosNotifyChannelAdmin::SequenceProxyPushConsumer>, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_SequenceProxyPushConsumer_i
  //
  // = DESCRIPTION
  //   Implements CosNotifyChannelAdmin::SequenceProxyPushConsumer
  //

public:
  TAO_Notify_SequenceProxyPushConsumer_i (TAO_Notify_SupplierAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager);
  // Constructor

  virtual ~TAO_Notify_SequenceProxyPushConsumer_i (void);
  // Destructor

  // = interface methods
  virtual void connect_sequence_push_supplier (
    CosNotifyComm::SequencePushSupplier_ptr push_supplier,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventChannelAdmin::AlreadyConnected
  ));

virtual void push_structured_events (
    const CosNotification::EventBatch & notifications,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosEventComm::Disconnected
  ));

  virtual void disconnect_sequence_push_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
 virtual void dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV);
  // Sends updates to the supplier.

  // = Helper methods
  void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Cleanup all resources used by this object.

  // = Data members
  CosNotifyComm::SequencePushSupplier_ptr push_supplier_;
  // The supplier that we're connected to.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_NOTIFY_SEQUENCEPROXYPUSHCONSUMER_I_H */
