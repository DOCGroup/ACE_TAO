// $Id$

#ifndef TAO_NOTIFY_PROXYSUPPLIER_T_C
#define TAO_NOTIFY_PROXYSUPPLIER_T_C

#include "Notify_ProxySupplier_T.h"
#include "Notify_Event_Manager.h"
#include "Notify_ConsumerAdmin_i.h"
#include "Notify_Factory.h"
#include "Notify_Channel_Objects_Factory.h"
#include "Notify_Event_Manager_Objects_Factory.h"
#include "Notify_Worker_Task.h"
#include "Notify_AdminProperties.h"

ACE_RCSID(Notify, Notify_ProxySupplier_T, "$Id$")

template <class SERVANT_TYPE>
TAO_Notify_ProxySupplier<SERVANT_TYPE>::TAO_Notify_ProxySupplier (TAO_Notify_ConsumerAdmin_i* consumer_admin)
  :consumer_admin_ (consumer_admin),
   is_suspended_ (0),
   dispatching_task_ (0),
   filter_eval_task_ (0)
{
  this->event_manager_ = consumer_admin->get_event_manager ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::init (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  consumer_admin_->_add_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_id_ = proxy_id;

  TAO_Notify_CO_Factory* cof =
    TAO_Notify_Factory::get_channel_objects_factory ();

  this->lock_ = cof->create_proxy_supplier_lock (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify_EMO_Factory* event_manager_objects_factory =
    this->event_manager_->resource_factory ();

  // Create the task to forward filtering/dispatching commands to:
  this->dispatching_task_ =
    event_manager_objects_factory->create_dispatching_task (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->filter_eval_task_ =
    event_manager_objects_factory->create_listener_eval_task (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Get hold of the admin properties.
  TAO_Notify_AdminProperties* const admin_properties =
    this->event_manager_->admin_properties ();

  // Init the tasks
  this->dispatching_task_->init_task (admin_properties,
                                      &(this->qos_admin_));
  this->filter_eval_task_->init_task (admin_properties,
                                      &(this->qos_admin_));
}

// Implementation skeleton destructor
template <class SERVANT_TYPE>
TAO_Notify_ProxySupplier<SERVANT_TYPE>::~TAO_Notify_ProxySupplier (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  this->event_manager_->unregister_from_publication_updates (this
                                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // unsubscribe it to our current subscriptions.
  CosNotification::EventTypeSeq added (0);
  CosNotification::EventTypeSeq removed (this->subscription_list_.size ());

  this->subscription_list_.populate (removed);
  added.length (0);

  this->event_manager_->subscribe_for_events (this,
                                              added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  delete this->lock_;

  this->consumer_admin_->proxy_pushsupplier_destroyed (this->proxy_id_);

  consumer_admin_->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  TAO_Notify_EMO_Factory* event_manager_objects_factory =
    this->event_manager_->resource_factory ();

  event_manager_objects_factory->destroy_dispatching_task (this->dispatching_task_);
  event_manager_objects_factory->destroy_source_eval_task (this->filter_eval_task_);
}

template <class SERVANT_TYPE> CORBA::Boolean
TAO_Notify_ProxySupplier<SERVANT_TYPE>::evaluate_filter (TAO_Notify_Event &event, CORBA::Boolean eval_parent ACE_ENV_ARG_DECL)
{
  if (eval_parent == 1)
    {
      CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator =
        consumer_admin_->MyOperator (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      // Inter-filter group operator.

      CORBA::Boolean bval =
        this->consumer_admin_->get_filter_admin ().match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if ((bval == 1 && filter_operator == CosNotifyChannelAdmin::AND_OP) ||
          (bval == 0 && filter_operator == CosNotifyChannelAdmin::OR_OP))
        {
          bval = this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
          return bval;
        }
      else if (bval == 1 && filter_operator == CosNotifyChannelAdmin::OR_OP)
        {
          return 1;
        }
      else
        {
          return 0;
        }
    }
  else
    {
      int status = this->filter_admin_.match (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);
      return status;
    }
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::dispatch_event (TAO_Notify_Event &event ACE_ENV_ARG_DECL)
{
  if (this->is_connected_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"%t, %P",
                  "dispatch_event to disconnected proxy supplier from EC\n"));
      return;
    }

  if (this->is_suspended_ == 1)
    {
      ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                          CORBA::INTERNAL ());
      ACE_CHECK;

      this->event_list_.enqueue_tail (event.clone ());
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "Notify (%P|%t) - "
                              "dispatching event\n"));
      this->dispatch_event_i (event ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

template <class SERVANT_TYPE> TAO_Notify_Worker_Task*
TAO_Notify_ProxySupplier<SERVANT_TYPE>::event_dispatch_task (void)
{
  return this->dispatching_task_;
}

template <class SERVANT_TYPE> TAO_Notify_Worker_Task*
TAO_Notify_ProxySupplier<SERVANT_TYPE>::filter_eval_task (void)
{
  return this->filter_eval_task_;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::subscription_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  if (this->is_connected_ == 1)
    {
      this->event_manager_->subscribe_for_events (this,
                                                  added, removed ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    // simply update our subscription list.
    this->subscription_list_.insert_seq (added);
    this->subscription_list_.remove_seq (removed);
  }
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::on_connected (ACE_ENV_SINGLE_ARG_DECL)
{
  // Get hold of the admin properties.
  TAO_Notify_AdminProperties* const admin_properties =
    this->event_manager_->admin_properties ();

  TAO_Notify_Property_Long* const consumer_count =
    admin_properties->consumers ();

  if (admin_properties->max_consumers () != 0 &&
      consumer_count->value () >= admin_properties->max_consumers ())
    ACE_THROW (CORBA::IMP_LIMIT ()); // we've reached the limit of consumers connected.

  // register with CA
  this->consumer_admin_->register_listener (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CosNotification::EventTypeSeq added;

  CosNotification::EventTypeSeq removed (0);
  removed.length (0);

  // subscribe it to our current subscriptions.
  added.length (this->subscription_list_.size ());

  this->subscription_list_.populate (added);

  this->event_manager_->subscribe_for_events (this,
                                              added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager_->register_for_publication_updates (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  (*consumer_count)++;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::on_disconnected (ACE_ENV_SINGLE_ARG_DECL)
{
  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    if (this->is_connected_ == 0)
      return;

    this->is_connected_ = 0;
  }

  CosNotification::EventTypeSeq removed;

  CosNotification::EventTypeSeq added (0);
  added.length (0);

    // unsubscribe it to our current subscriptions.
  removed.length (this->subscription_list_.size ());

  this->subscription_list_.populate (removed);

  this->event_manager_->subscribe_for_events (this,
                                              added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->event_manager_->unregister_from_publication_updates (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // shutdown the tasks.

  this->dispatching_task_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->filter_eval_task_->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Get hold of the admin properties.
  TAO_Notify_AdminProperties* const admin_properties =
    this->event_manager_->admin_properties ();

  (*(admin_properties->consumers ()))--;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::suspend_connection (ACE_ENV_SINGLE_ARG_DECL_NOT_USED/*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ConnectionAlreadyInactive,
                   CosNotifyChannelAdmin::NotConnected
                   ))
{
  this->is_suspended_ = 1;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::resume_connection (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ConnectionAlreadyActive,
                   CosNotifyChannelAdmin::NotConnected
                   ))
{
  TAO_Notify_Event* event;

  this->is_suspended_ = 0;

  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    while (this->event_list_.dequeue_head (event) == 0)
      {
        this->dispatch_event_i (*event ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;

        delete event;
      }
  }
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::ConsumerAdmin_ptr
TAO_Notify_ProxySupplier<SERVANT_TYPE>::MyAdmin (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->consumer_admin_->get_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
}

template <class SERVANT_TYPE> CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ProxySupplier<SERVANT_TYPE>::priority_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::priority_filter (CosNotifyFilter::MappingFilter_ptr /*priority_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotifyFilter::MappingFilter_ptr
TAO_Notify_ProxySupplier<SERVANT_TYPE>::lifetime_filter (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyFilter::MappingFilter::_nil ());
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::lifetime_filter (CosNotifyFilter::MappingFilter_ptr /*lifetime_filter*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_Notify_ProxySupplier<SERVANT_TYPE>::obtain_offered_types (CosNotifyChannelAdmin::ObtainInfoMode mode ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
    CORBA::SystemException
  ))
{
  CosNotification::EventTypeSeq* event_type_seq = 0;

  if (mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_OFF ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      event_type_seq = this->event_manager_->obtain_offered_types ();
    }

  if (mode == CosNotifyChannelAdmin::NONE_NOW_UPDATES_ON ||
      mode == CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON)
    {
      // if updates are currently off, switch them on.
      if (this->updates_on_ == 0)
        this->event_manager_->register_for_publication_updates (this
                                                                ACE_ENV_ARG_PARAMETER);
    }
  else
    {
      // if updates are currently on, switch them off.
      if (this->updates_on_ == 1)
        this->event_manager_->unregister_from_publication_updates (this
                                                                   ACE_ENV_ARG_PARAMETER);
    }
  return event_type_seq;
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxySupplier<SERVANT_TYPE>::set_qos (
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
  if (this->dispatching_task_ != 0)
    {
      this->dispatching_task_->update_qos (this->qos_admin_);
    }
  if (this->filter_eval_task_ != 0)
    {
      this->filter_eval_task_->update_qos (this->qos_admin_);
    }
}


#endif /* TAO_NOTIFY_PROXYSUPPLIER_T_C */
