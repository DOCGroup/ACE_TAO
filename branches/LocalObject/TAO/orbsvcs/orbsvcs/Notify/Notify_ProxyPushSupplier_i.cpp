//$Id$

#include "Notify_ProxyPushSupplier_i.h"
#include "Notify_Event_Manager.h"
#include "Notify_ConsumerAdmin_i.h"

TAO_Notify_ProxyPushSupplier_i::TAO_Notify_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager)
  :TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier> (consumeradmin, resource_manager),
  notify_style_consumer_ (0)
{
  // No-Op.
}

// Implementation skeleton destructor
TAO_Notify_ProxyPushSupplier_i::~TAO_Notify_ProxyPushSupplier_i (void)
{
  // No-Op.
}

void
TAO_Notify_ProxyPushSupplier_i::cleanup_i (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>::cleanup_i (ACE_TRY_ENV);

  this->cosec_push_consumer_ = CosEventComm::PushConsumer::_nil ();
  this->notify_push_consumer_ = CosNotifyComm::PushConsumer::_nil ();
}

void
TAO_Notify_ProxyPushSupplier_i::dispatch_event_i (TAO_Notify_Event &event, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      event.do_push (this->cosec_push_consumer_.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_ProxyPushSupplier_i::dispatch_update_i (CosNotification::EventTypeSeq added, CosNotification::EventTypeSeq removed, CORBA::Environment &ACE_TRY_ENV)
{
  ACE_TRY
    {
      this->notify_push_consumer_->offer_change (added, removed, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHALL
    {
      // misbehaving client, ignore for now.
    }
  ACE_ENDTRY;
}

void
TAO_Notify_ProxyPushSupplier_i::connect_any_push_consumer (CosEventComm::PushConsumer_ptr push_consumer, CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventChannelAdmin::AlreadyConnected,
                   CosEventChannelAdmin::TypeError
  ))
{
  if (CORBA::is_nil (push_consumer))
    ACE_THROW (CosEventChannelAdmin::TypeError ());
  else if (this->is_connected_ == 1)
    ACE_THROW (CosEventChannelAdmin::AlreadyConnected ());
  else
    {
      this->cosec_push_consumer_ =
        CosEventComm::PushConsumer::_duplicate (push_consumer);

      this->notify_push_consumer_ =
        CosNotifyComm::PushConsumer::_narrow (push_consumer, ACE_TRY_ENV);
      ACE_CHECK;

      if (!CORBA::is_nil (this->notify_push_consumer_.in ()))
        {
          CosNotifyComm::PushConsumer::
            _duplicate (this->notify_push_consumer_.in ());
          this->notify_style_consumer_ = 1;
        }
    }

  ACE_TRY
    {
      this->on_connected (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      this->is_connected_ = 1;
    }
 ACE_CATCHALL
   {
     this->cosec_push_consumer_ = CosNotifyComm::PushConsumer::_nil ();
     this->notify_push_consumer_ = CosNotifyComm::PushConsumer::_nil ();
     ACE_RETHROW;
   }
 ACE_ENDTRY;
}

void
TAO_Notify_ProxyPushSupplier_i::disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->is_destroyed_ = 1;

  // ask our parent to deactivate us.
  this->myadmin_->
    deactivate_proxy_pushsupplier (this, ACE_TRY_ENV);

  this->cleanup_i (ACE_TRY_ENV);
}

// = TAO_Notify_CosEC_ProxyPushSupplier_i

TAO_Notify_CosEC_ProxyPushSupplier_i::TAO_Notify_CosEC_ProxyPushSupplier_i (TAO_Notify_ConsumerAdmin_i* consumeradmin, TAO_Notify_Resource_Manager* resource_manager)
  :notify_proxy_ (consumeradmin, resource_manager)
{
  // No-Op.
}

TAO_Notify_CosEC_ProxyPushSupplier_i::~TAO_Notify_CosEC_ProxyPushSupplier_i (void)
{
}

void
TAO_Notify_CosEC_ProxyPushSupplier_i::init (CORBA::Environment &ACE_TRY_ENV)
{
  this->notify_proxy_.init (0, ACE_TRY_ENV);
}

void
TAO_Notify_CosEC_ProxyPushSupplier_i::disconnect_push_supplier (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->notify_proxy_.disconnect_push_supplier (ACE_TRY_ENV);
}

void
TAO_Notify_CosEC_ProxyPushSupplier_i::connect_push_consumer(CosEventComm::PushConsumer_ptr push_consumer, CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     CosEventChannelAdmin::AlreadyConnected,
                     CosEventChannelAdmin::TypeError))
{
  this->notify_proxy_.connect_any_push_consumer (push_consumer, ACE_TRY_ENV);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>;
template class TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::ProxyPushSupplier>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate TAO_Notify_ProxySupplier<POA_CosNotifyChannelAdmin::ProxyPushSupplier>
#pragma instantiate TAO_Notify_Proxy<POA_CosNotifyChannelAdmin::ProxyPushSupplier>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Unbounded_Queue<TAO_Notify_Event*>;
template class ACE_Node<TAO_Notify_Event*>;
template class ACE_Unbounded_Queue_Iterator <TAO_Notify_Event*>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Unbounded_Queue<TAO_Notify_Event*>
#pragma instantiate ACE_Node<TAO_Notify_Event*>
#pragma instantiate ACE_Unbounded_Queue_Iterator <TAO_Notify_Event*>;

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
