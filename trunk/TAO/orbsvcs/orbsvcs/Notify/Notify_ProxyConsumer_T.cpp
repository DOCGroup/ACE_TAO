// $Id$
#ifndef TAO_NOTIFY_PROXYCONSUMER_T_C
#define TAO_NOTIFY_PROXYCONSUMER_T_C

#include "Notify_ProxyConsumer_T.h"
#include "Notify_Event_Manager.h"
#include "Notify_SupplierAdmin_i.h"
#include "Notify_Factory.h"
#include "Notify_Channel_Objects_Factory.h"

ACE_RCSID(Notify, Notify_ProxyConsumer_T, "$Id$")

template <class SERVANT_TYPE>
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::TAO_Notify_ProxyConsumer (TAO_Notify_SupplierAdmin_i* supplier_admin)
 : supplier_admin_ (supplier_admin)
{
  event_manager_ = supplier_admin->get_event_manager ();
  supplier_admin_->_add_ref ();
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::init (CosNotifyChannelAdmin::ProxyID proxy_id, CORBA::Environment& ACE_TRY_ENV)
{
  this->proxy_id_ = proxy_id;

  TAO_Notify_CO_Factory* cof =
    TAO_Notify_Factory::get_channel_objects_factory ();

  this->lock_ = cof->create_proxy_consumer_lock (ACE_TRY_ENV);
}

// Implementation skeleton destructor
template <class SERVANT_TYPE>
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::~TAO_Notify_ProxyConsumer (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  this->event_manager_->unregister_from_subscription_updates (this,
                                                              ACE_TRY_ENV);
  delete this->lock_;

  this->supplier_admin_->proxy_pushconsumer_destroyed (this->proxy_id_);
  supplier_admin_->_remove_ref ();
}

template <class SERVANT_TYPE> CORBA::Boolean
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::evaluate_filter (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV)
{
  // check if it passes the parent filter.
  CORBA::Boolean bval =
    this->supplier_admin_->get_filter_admin ().match (event,
                                                      ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  if (supplier_admin_->MyOperator (ACE_TRY_ENV) == CosNotifyChannelAdmin::AND_OP)
    return (bval
            &&
            this->filter_admin_.match (event,
                                       ACE_TRY_ENV)
            );
      else
        return (bval
                ||
                this->filter_admin_.match (event,
                                           ACE_TRY_ENV)
                );
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::on_connected (CORBA::Environment &ACE_TRY_ENV)
{
  this->event_manager_->register_for_subscription_updates (this, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> void
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::offer_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  this->event_manager_->update_publication_list (added, removed, ACE_TRY_ENV);
}

template <class SERVANT_TYPE> CosNotification::EventTypeSeq*
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::obtain_subscription_types (CosNotifyChannelAdmin::ObtainInfoMode mode, CORBA::Environment &ACE_TRY_ENV)
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
        this->event_manager_->register_for_subscription_updates (this,
                                                                 ACE_TRY_ENV);
    }
  else
    {
      // if updates are currently on, switch them off.
      if (this->updates_on_ == 1)
        this->event_manager_->unregister_from_subscription_updates (this,
                                                                    ACE_TRY_ENV);
    }

  return event_type_seq;
}

template <class SERVANT_TYPE> CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_ProxyConsumer<SERVANT_TYPE>::MyAdmin (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->supplier_admin_->get_ref (ACE_TRY_ENV);
}

#endif /* TAO_NOTIFY_PROXYCONSUMER_T_C */
