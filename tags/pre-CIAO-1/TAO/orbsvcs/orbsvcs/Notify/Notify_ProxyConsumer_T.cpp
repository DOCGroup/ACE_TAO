// $Id$
#ifndef TAO_NOTIFY_PROXYCONSUMER_T_C
#define TAO_NOTIFY_PROXYCONSUMER_T_C

#include "Notify_ProxyConsumer_T.h"
#include "Notify_Event_Manager.h"
#include "Notify_SupplierAdmin_i.h"
#include "Notify_Factory.h"
#include "Notify_Channel_Objects_Factory.h"
#include "Notify_Event_Manager_Objects_Factory.h"
#include "Notify_Worker_Task.h"
#include "Notify_AdminProperties.h"

ACE_RCSID(Notify, Notify_ProxyConsumer_T, "$Id$")

template <class SERVANT_TYPE>
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::TAO_Notify_ProxyConsumer (TAO_Notify_SupplierAdmin_i* supplier_admin)
  : supplier_admin_ (supplier_admin),
    filter_eval_task_ (0)
{
  this->event_manager_ = supplier_admin->get_event_manager ();
  this->supplier_admin_->_add_ref ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::init (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  this->proxy_id_ = proxy_id;

  TAO_Notify_CO_Factory* cof =
    TAO_Notify_Factory::get_channel_objects_factory ();

  this->lock_ = cof->create_proxy_consumer_lock (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Create the task to forward filtering commands to:

  TAO_Notify_EMO_Factory* event_manager_objects_factory =
    this->event_manager_->resource_factory ();

  this->filter_eval_task_ =
    event_manager_objects_factory->create_source_eval_task (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Get hold of the admin properties.
  TAO_Notify_AdminProperties* const admin_properties =
    this->event_manager_->admin_properties ();

  // open the tasks
  this->filter_eval_task_->init_task (admin_properties,
                                      &(this->qos_admin_));
}

// Implementation skeleton destructor
template <class SERVANT_TYPE>
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::~TAO_Notify_ProxyConsumer (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  this->event_manager_->unregister_from_subscription_updates (this
                                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  delete this->lock_;

  this->supplier_admin_->proxy_pushconsumer_destroyed (this->proxy_id_);
  this->supplier_admin_->_remove_ref ();

  // @@: Move this to on_disconnected
  this->filter_eval_task_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify_EMO_Factory* event_manager_objects_factory =
    this->event_manager_->resource_factory ();

  event_manager_objects_factory->destroy_listener_eval_task (this->filter_eval_task_);
}

template <class SERVANT_TYPE> CORBA::Boolean
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::evaluate_filter (TAO_Notify_Event &event ACE_ENV_ARG_DECL)
{
  // check if it passes the parent filter.
  CORBA::Boolean bval =
    this->supplier_admin_->get_filter_admin ().match (event
                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  CORBA::Boolean ret_val;

  ret_val = this->supplier_admin_->MyOperator (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  if (ret_val == CosNotifyChannelAdmin::AND_OP)
    {
      ret_val = bval && this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }
  else
    {
      ret_val = bval || this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
    }

  return ret_val;
}

template <class SERVANT_TYPE> TAO_Notify_Worker_Task*
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::filter_eval_task (void)
{
  return this->filter_eval_task_;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::on_connected (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get hold of the admin properties.
  TAO_Notify_AdminProperties* const admin_properties =
    this->event_manager_->admin_properties ();

  TAO_Notify_Property_Long* const supplier_count =
    admin_properties->suppliers ();

  if (admin_properties->max_suppliers () != 0 &&
      supplier_count->value () >= admin_properties->max_suppliers ())
    ACE_THROW (CORBA::IMP_LIMIT ()); // we've reached the limit of suppliers connected.

  this->event_manager_->register_for_subscription_updates (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  (*supplier_count)++;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::on_disconnected (ACE_ENV_SINGLE_ARG_DECL_NOT_USED/*ACE_ENV_SINGLE_ARG_PARAMETER*/)
{
  // Get hold of the admin properties.
  TAO_Notify_AdminProperties* const admin_properties =
    this->event_manager_->admin_properties ();

  (*(admin_properties->suppliers ()))--;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::offer_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  this->event_manager_->update_publication_list (added, removed ACE_ENV_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::obtain_subscription_types (CosNotifyChannelAdmin::ObtainInfoMode mode ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  CosNotification::EventTypeSeq* event_type_seq = 0;

  if (mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      event_type_seq = this->event_manager_->obtain_subscription_types ();
    }

  if (mode == CosNotifyChannelAdmin::NONE_NOW_UPDATES_ON ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      // if updates are currently off, switch them on.
      if (this->updates_on_ == 0)
        this->event_manager_->register_for_subscription_updates (this
                                                                 ACE_ENV_ARG_PARAMETER);
    }
  else
    {
      // if updates are currently on, switch them off.
      if (this->updates_on_ == 1)
        this->event_manager_->unregister_from_subscription_updates (this
                                                                    ACE_ENV_ARG_PARAMETER);
    }

  return event_type_seq;
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::MyAdmin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->supplier_admin_->get_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::set_qos (
                           const CosNotification::QoSProperties & qos
                           ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException,
    CosNotification::UnsupportedQoS
  ))
{
  // Call our base class set_qos ().
  TAO_Notify_Proxy<SERVANT_TYPE>::set_qos (qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Then update our task's qos
  if (this->filter_eval_task_ != 0)
    {
      this->filter_eval_task_->update_qos (this->qos_admin_);
    }
}

#endif /* TAO_NOTIFY_PROXYCONSUMER_T_C */
