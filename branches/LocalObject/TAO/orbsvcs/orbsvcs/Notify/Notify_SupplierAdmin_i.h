/* -*- C++ -*- */
// $Id$
// ==========================================================================
//
// = LIBRARY
//   orbsvcs
//
// = FILENAME
//   Notify_SupplierAdmin_i.h
//
// = DESCRIPTION
//   Implements the CosNotifyChannelAdmin::SupplierAdmin interface.
//
// = AUTHOR
//    Pradeep Gore <pradeep@cs.wustl.edu>
//
// ==========================================================================

#ifndef TAO_NOTIFY_SUPPLIERADMIN_I_H
#define TAO_NOTIFY_SUPPLIERADMIN_I_H
#include "ace/pre.h"

// @@ Pradeep: i believe that the #pragma once guideline is still in
// effect.

#include "Notify_ID_Pool_T.h"
#include "Notify_QoSAdmin_i.h"
#include "Notify_FilterAdmin_i.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

class TAO_Notify_EventChannel_i;
class TAO_Notify_Resource_Manager;
class TAO_Notify_Event_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_Notify_Export TAO_Notify_SupplierAdmin_i : public POA_CosNotifyChannelAdmin::SupplierAdmin, public PortableServer::RefCountServantBase
{
  // = TITLE
  //   TAO_Notify_SupplierAdmin_i
  //
  // = DESCRIPTION
  //   Implements the CosNotifyChannelAdmin::SupplierAdmin interface.
  //

public:
  // @@ Pradeep: you may want to drop the _i suffix, it buys you
  // nothing..

  TAO_Notify_SupplierAdmin_i (TAO_Notify_EventChannel_i* myChannel,
                              TAO_Notify_Resource_Manager* resource_manager);
  // Constructor
  // <myChannel> is this objects parent.

  virtual ~TAO_Notify_SupplierAdmin_i (void);
  // Destructor

  void init (CosNotifyChannelAdmin::AdminID myID,
             CosNotifyChannelAdmin::InterFilterGroupOperator myOperator,
             PortableServer::POA_ptr my_POA,
             CORBA::Environment &ACE_TRY_ENV);
  //Initialize the Supplier Admin.

  // @@ Pradeep: could this method be const?  Try to use const
  // operations when possible.
  TAO_Notify_FilterAdmin_i& get_filter_admin (void);
  // Get our filter admin.

  void deactivate_proxy_pushconsumer (PortableServer::Servant servant,
                                      CORBA::Environment &ACE_TRY_ENV);
  // Deactivate servant from <proxy_pushconsumer_POA_>.

  void proxy_pushconsumer_destroyed (CosNotifyChannelAdmin::ProxyID proxyID);
  // This id is no longer in use.It can be reused by <proxy_pushconsumer_ids_>

  // = Interface methods
  virtual CosNotifyChannelAdmin::AdminID MyID (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  CosNotifyChannelAdmin::SupplierAdmin_ptr get_ref (CORBA::Environment &ACE_TRY_ENV);
  // Return the CORBA object for this servant.

  TAO_Notify_Event_Manager* get_event_manager (void);
  // Accesor for the event manager.

  // @@ Pradeep: Don't forget to indent this stuff, at the very least
  // it should not start in the first column!
  virtual CosNotifyChannelAdmin::EventChannel_ptr MyChannel (
      CORBA::Environment &ACE_TRY_ENV
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

virtual CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyIDSeq * pull_consumers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyIDSeq * push_consumers (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyConsumer_ptr get_proxy_consumer (
    CosNotifyChannelAdmin::ProxyID proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ));

virtual CosNotifyChannelAdmin::ProxyConsumer_ptr obtain_notification_pull_consumer (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual CosNotifyChannelAdmin::ProxyConsumer_ptr obtain_notification_push_consumer (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual void destroy (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotification::QoSProperties * get_qos (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void set_qos (
    const CosNotification::QoSProperties & qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter,
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_filters (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPullConsumer_ptr obtain_pull_consumer (
    CORBA::Environment &ACE_TRY_ENV
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
  // = Helper methods
  // @@ Pradeep: you don't need to provide default values for
  // private, protected or implementation methods.  In fact it is a
  // good idea *not* to provide the default value.  That way you make
  // sure that you are passing it around.
 void cleanup_i (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Cleanup all resources used by this object.

  CORBA::Object_ptr obtain_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV);
  // Obtain a proxy pushconsumer object

  CORBA::Object_ptr obtain_struct_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV);
  // Obtain a structured proxy pushconsumer object.

  CORBA::Object_ptr obtain_sequence_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment &ACE_TRY_ENV);
  // Obtain a sequence pushconsumer object

 // = Data members
  TAO_Notify_EventChannel_i* my_channel_;
  // The channel to which we belong.

  TAO_Notify_Resource_Manager* resource_manager_;
  // The resource factory that we use.

  CosNotifyChannelAdmin::InterFilterGroupOperator myOperator_;
  // The inter filter operator to use.

  CosNotifyChannelAdmin::AdminID myID_;
  // My ID.

  PortableServer::POA_var my_POA_;
  // This is the POA in which we live.

  PortableServer::POA_var proxy_pushconsumer_POA_;
  // The POA in which all our push consumers live.
  // We create and own this POA.

  // @@ Pradeep: you may want to use a typedef for that template.
  TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ProxyID,
    CosNotifyChannelAdmin::ProxyIDSeq> proxy_pushconsumer_ids_;
  // Id generator for proxy push consumers.

  CORBA::Boolean is_destroyed_;
  // Are we dead?
  // @@ Pradeep: you may want to explain why such a flag is needed. I
  // assume it is set when the a request to destroy the object
  // arrives, but you must perform some cleanup, meanwhile any new
  // requests are rejected, right?

  TAO_Notify_QoSAdmin_i qos_admin_;
  // Handle QoS admin methods.

  TAO_Notify_FilterAdmin_i filter_admin_;
  // Handles the Filter Admin methods.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_SUPPLIERADMIN_I_H */
