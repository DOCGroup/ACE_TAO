/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Notify_SupplierAdmin_i.h
 *
 *  $Id$
 *
 * Implements the CosNotifyChannelAdmin::SupplierAdmin interface.
 *
 *
 *  @author Pradeep Gore <pradeep@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_NOTIFY_SUPPLIERADMIN_I_H
#define TAO_NOTIFY_SUPPLIERADMIN_I_H
#include "ace/pre.h"
#include "orbsvcs/CosNotifyChannelAdminS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Notify_ID_Pool_T.h"
#include "Notify_QoSAdmin_i.h"
#include "Notify_FilterAdmin_i.h"

class TAO_Notify_EventChannel_i;
class TAO_Notify_CO_Factory;
class TAO_Notify_POA_Factory;
class TAO_Notify_Event_Manager;

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class TAO_Notify_SupplierAdmin_i
 *
 * @brief TAO_Notify_SupplierAdmin_i
 *
 * Implements the CosNotifyChannelAdmin::SupplierAdmin interface.
 */
class TAO_Notify_Export TAO_Notify_SupplierAdmin_i : public POA_CosNotifyChannelAdmin::SupplierAdmin, public PortableServer::RefCountServantBase
{

public:
  // @@ Pradeep: you may want to drop the _i suffix, it buys you
  // nothing..

  /// Constructor
  /// <event_channel> is this objects parent.
  TAO_Notify_SupplierAdmin_i (TAO_Notify_EventChannel_i* event_channel);

  /// Destructor
  virtual ~TAO_Notify_SupplierAdmin_i ();

  ///Initialize the Supplier Admin.
  void init (CosNotifyChannelAdmin::AdminID myID,
             CosNotifyChannelAdmin::InterFilterGroupOperator myOperator,
             PortableServer::POA_ptr my_POA
             ACE_ENV_ARG_DECL);

  // @@ Pradeep: could this method be const?  Try to use const
  // operations when possible.
  /// Get our filter admin.
  TAO_Notify_FilterAdmin_i& get_filter_admin (void);

  /// Deactivate servant from <proxy_pushconsumer_POA_>.
  void deactivate_proxy_pushconsumer (PortableServer::Servant servant
                                      ACE_ENV_ARG_DECL);

  /// This id is no longer in use.It can be reused by <proxy_pushconsumer_ids_>
  void proxy_pushconsumer_destroyed (CosNotifyChannelAdmin::ProxyID proxyID);

  // = Interface methods
  virtual CosNotifyChannelAdmin::AdminID MyID (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

  /// Return the CORBA object for this servant.
  CosNotifyChannelAdmin::SupplierAdmin_ptr get_ref (ACE_ENV_SINGLE_ARG_DECL);

  /// Accesor for the event manager.
  TAO_Notify_Event_Manager* get_event_manager (void);

  // @@ Pradeep: Don't forget to indent this stuff, at the very least
  // it should not start in the first column!
  virtual CosNotifyChannelAdmin::EventChannel_ptr MyChannel (
      ACE_ENV_SINGLE_ARG_DECL
    )
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ));

virtual CosNotifyChannelAdmin::InterFilterGroupOperator MyOperator (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyIDSeq * pull_consumers (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyIDSeq * push_consumers (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotifyChannelAdmin::ProxyConsumer_ptr get_proxy_consumer (
    CosNotifyChannelAdmin::ProxyID proxy_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::ProxyNotFound
  ));

virtual CosNotifyChannelAdmin::ProxyConsumer_ptr obtain_notification_pull_consumer (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual CosNotifyChannelAdmin::ProxyConsumer_ptr obtain_notification_push_consumer (
    CosNotifyChannelAdmin::ClientType ctype,
    CosNotifyChannelAdmin::ProxyID_out proxy_id
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyChannelAdmin::AdminLimitExceeded
  ));

virtual void destroy (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosNotification::QoSProperties * get_qos (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void set_qos (
    const CosNotification::QoSProperties & qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void validate_qos (
    const CosNotification::QoSProperties & required_qos,
    CosNotification::NamedPropertyRangeSeq_out available_qos
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ));

virtual void offer_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyComm::InvalidEventType
  ));

virtual CosNotifyFilter::FilterID add_filter (
    CosNotifyFilter::Filter_ptr new_filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::Filter_ptr get_filter (
    CosNotifyFilter::FilterID filter
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotifyFilter::FilterNotFound
  ));

virtual CosNotifyFilter::FilterIDSeq * get_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual void remove_all_filters (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPushConsumer_ptr obtain_push_consumer (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

virtual CosEventChannelAdmin::ProxyPullConsumer_ptr obtain_pull_consumer (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ));

protected:
  // = Helper methods
  /// Obtain a proxy pushconsumer object
  CORBA::Object_ptr obtain_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL);

  /// Obtain a structured proxy pushconsumer object.
  CORBA::Object_ptr obtain_struct_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL);

  /// Obtain a sequence pushconsumer object
  CORBA::Object_ptr obtain_sequence_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL);

  // = Data members
  /// The channel to which we belong.
  TAO_Notify_EventChannel_i* event_channel_;

  /// The factory for channel objects.
  TAO_Notify_CO_Factory* channel_objects_factory_;

  /// The factory for POA based containers.
  TAO_Notify_POA_Factory* poa_factory_;

  /// The locking strategy.
  ACE_Lock* lock_;

  /// Flag to tell if the child poa's should be destroyed.
  CORBA::Boolean destory_child_POAs_;

  /// The inter filter operator to use.
  CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator_;

  /// My ID.
  CosNotifyChannelAdmin::AdminID my_id_;

  /// This is the POA in which we live.
  PortableServer::POA_var my_POA_;

  /// The POA in which all our push consumers live.
  /// We create and own this POA.
  PortableServer::POA_var proxy_pushconsumer_POA_;

  // @@ Pradeep: you may want to use a typedef for that template.
  /// Id generator for proxy push consumers.
  TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ProxyID,
    CosNotifyChannelAdmin::ProxyIDSeq> proxy_pushconsumer_ids_;

  /// Handle QoS admin methods.
  TAO_Notify_QoSAdmin_i qos_admin_;

  /// Handles the Filter Admin methods.
  TAO_Notify_FilterAdmin_i filter_admin_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_NOTIFY_SUPPLIERADMIN_I_H */
