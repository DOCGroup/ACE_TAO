// $Id$

#include "Notify_SupplierAdmin_i.h"
#include "Notify_ProxyPushConsumer_i.h"
#include "Notify_StructuredProxyPushConsumer_i.h"
#include "Notify_SequenceProxyPushConsumer_i.h"
#include "Notify_EventChannel_i.h"
#include "Notify_Channel_Objects_Factory.h"
#include "Notify_POA_Factory.h"
#include "Notify_Factory.h"
#include "Notify_Event_Manager.h"

#include "tao/debug.h"

#include "ace/Auto_Ptr.h"

ACE_RCSID(Notify, Notify_SupplierAdmin_i, "$Id$")

// Implementation skeleton constructor
TAO_Notify_SupplierAdmin_i::TAO_Notify_SupplierAdmin_i (TAO_Notify_EventChannel_i* event_channel)
  :event_channel_ (event_channel),
   channel_objects_factory_ (0),
   poa_factory_ (0),
   lock_ (0),
   destory_child_POAs_ (0)
{
  event_channel_->_add_ref (); // we don't want our parent to go away!
}

// Implementation skeleton destructor
TAO_Notify_SupplierAdmin_i::~TAO_Notify_SupplierAdmin_i ()
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,"in SA dtor\n"));
  // Cleanup all resources..

  delete this->lock_;

  this->event_channel_->supplier_admin_destroyed (this->my_id_);
  event_channel_->_remove_ref ();
}

void
TAO_Notify_SupplierAdmin_i::proxy_pushconsumer_destroyed (CosNotifyChannelAdmin::ProxyID proxyID)
{
  this->proxy_pushconsumer_ids_.put (proxyID); // This id is no longer in use.
}

void
TAO_Notify_SupplierAdmin_i::init (CosNotifyChannelAdmin::AdminID my_id,
                                  CosNotifyChannelAdmin::InterFilterGroupOperator filter_operator,
                                  PortableServer::POA_ptr my_POA
                                  ACE_ENV_ARG_DECL)
{
  this->my_POA_ = PortableServer::POA::_duplicate (my_POA);

  this->channel_objects_factory_ =
    TAO_Notify_Factory::get_channel_objects_factory ();

  this->poa_factory_ = TAO_Notify_Factory::get_poa_factory ();

  this->lock_ = this->channel_objects_factory_->
    create_supplier_admin_lock (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->proxy_pushconsumer_POA_ = this->poa_factory_->
    create_proxy_pushconsumer_POA (this->my_POA_.in (), my_id
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  my_id_ = my_id;
  filter_operator_ = filter_operator;
}

CosNotifyChannelAdmin::SupplierAdmin_ptr
TAO_Notify_SupplierAdmin_i::get_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  // @@ Pradeep: if you get a chance to quantify this stuff you will
  // notice that this is a very expensive operation, you may want to
  // cache the result if it is invoked very often...

  CORBA::Object_var obj = this->poa_factory_->
    servant_to_reference (this->my_POA_.in (), this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::SupplierAdmin::_nil ());

  return CosNotifyChannelAdmin::SupplierAdmin::_narrow (obj.in ());
}

// @@ Pradeep: it is possible that you want methods like this
// inlined...

TAO_Notify_Event_Manager*
TAO_Notify_SupplierAdmin_i::get_event_manager (void)
{
  return this->event_channel_->get_event_manager ();
}

TAO_Notify_FilterAdmin_i&
TAO_Notify_SupplierAdmin_i::get_filter_admin (void)
{
  return this->filter_admin_;
}

void
TAO_Notify_SupplierAdmin_i::deactivate_proxy_pushconsumer (PortableServer::Servant servant ACE_ENV_ARG_DECL)
{
  this->poa_factory_->
    deactivate_object (servant, this->proxy_pushconsumer_POA_.in ()
                       ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::AdminID
TAO_Notify_SupplierAdmin_i::MyID (ACE_ENV_SINGLE_ARG_DECL_NOT_USED/*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return my_id_;
}

CosNotifyChannelAdmin::EventChannel_ptr
TAO_Notify_SupplierAdmin_i::MyChannel (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return event_channel_->get_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::InterFilterGroupOperator
TAO_Notify_SupplierAdmin_i::MyOperator (ACE_ENV_SINGLE_ARG_DECL_NOT_USED/*ACE_ENV_SINGLE_ARG_PARAMETER*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return filter_operator_;
}

CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_SupplierAdmin_i::push_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  return this->proxy_pushconsumer_ids_.get_sequence (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin_i::get_proxy_consumer (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::ProxyNotFound
                   ))
{
  CORBA::Object_var obj =
    this->poa_factory_->id_to_reference (proxy_id,
                                         proxy_pushconsumer_POA_.in ()
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());

  return CosNotifyChannelAdmin::ProxyConsumer::_narrow (obj.in ());
}

CORBA::Object_ptr
TAO_Notify_SupplierAdmin_i::obtain_sequence_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  TAO_Notify_SequenceProxyPushConsumer_i* seq_proxy_pushconsumer =
    this->channel_objects_factory_->create_seq_proxy_pushconsumer (this
                                                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushconsumer_var (seq_proxy_pushconsumer);

  seq_proxy_pushconsumer->init (proxy_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->poa_factory_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushconsumer_POA_.in (),
                             seq_proxy_pushconsumer
                             ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_Notify_SupplierAdmin_i::obtain_struct_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  TAO_Notify_StructuredProxyPushConsumer_i* struct_proxy_pushconsumer =
    this->channel_objects_factory_->create_struct_proxy_pushconsumer (this
                                                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushconsumer_var (struct_proxy_pushconsumer);

  struct_proxy_pushconsumer->init (proxy_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->poa_factory_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushconsumer_POA_.in (),
                             struct_proxy_pushconsumer
                             ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_Notify_SupplierAdmin_i::obtain_proxy_pushconsumer_i (CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  TAO_Notify_ProxyPushConsumer_i* proxy_pushconsumer =
    this->channel_objects_factory_->create_proxy_pushconsumer (this
                                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ServantBase_var proxy_pushconsumer_var (proxy_pushconsumer);

  proxy_pushconsumer->init (proxy_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  return this->poa_factory_->
    activate_object_with_id (proxy_id,
                             this->proxy_pushconsumer_POA_.in (),
                             proxy_pushconsumer
                             ACE_ENV_ARG_PARAMETER);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_notification_push_consumer (CosNotifyChannelAdmin::ClientType ctype, CosNotifyChannelAdmin::ProxyID_out proxy_id ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  CORBA::Object_var obj;

  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());

    proxy_id = this->proxy_pushconsumer_ids_.get ();
  }

  switch (ctype)
    {
    case CosNotifyChannelAdmin::ANY_EVENT:
      {
        obj = this->obtain_proxy_pushconsumer_i (proxy_id ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());
      }
      break;

    case CosNotifyChannelAdmin::STRUCTURED_EVENT:
      {
        obj = this->obtain_struct_proxy_pushconsumer_i (proxy_id
                                                        ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());
      }
      break;

    case CosNotifyChannelAdmin::SEQUENCE_EVENT:
      {
        obj = this->obtain_sequence_proxy_pushconsumer_i (proxy_id
                                                          ACE_ENV_ARG_PARAMETER);
        ACE_CHECK_RETURN (CosNotifyChannelAdmin::ProxyConsumer::_nil ());
      }
      break;

    default:
      ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                        CosNotifyChannelAdmin::ProxyConsumer::_nil ());
    }

  this->proxy_pushconsumer_ids_.next (); // commit the current id.
  return CosNotifyChannelAdmin::ProxyConsumer::_narrow (obj.in ()
                                                        ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->destory_child_POAs_ = 1;
  if (this->destory_child_POAs_ == 1)
    this->poa_factory_->destroy_POA (this->proxy_pushconsumer_POA_.in ()
                                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // deactivate ourselves
  this->poa_factory_->deactivate_object (this, this->my_POA_.in ()
                                         ACE_ENV_ARG_PARAMETER);
}

CosNotification::QoSProperties*
TAO_Notify_SupplierAdmin_i::get_qos (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->qos_admin_.get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin_i::set_qos (const CosNotification::QoSProperties & qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.set_qos (qos ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin_i::validate_qos (const CosNotification::QoSProperties & required_qos, CosNotification::NamedPropertyRangeSeq_out available_qos ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotification::UnsupportedQoS
                   ))
{
  this->qos_admin_.validate_qos (required_qos, available_qos ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin_i::offer_change (const CosNotification::EventTypeSeq & added, const CosNotification::EventTypeSeq & removed ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  this->get_event_manager ()->update_publication_list (added,
                                                       removed ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::FilterID
TAO_Notify_SupplierAdmin_i::add_filter (CosNotifyFilter::Filter_ptr new_filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.add_filter (new_filter ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin_i::remove_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  this->filter_admin_.remove_filter (filter ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::Filter_ptr
TAO_Notify_SupplierAdmin_i::get_filter (CosNotifyFilter::FilterID filter ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyFilter::FilterNotFound
                   ))
{
  return this->filter_admin_.get_filter (filter ACE_ENV_ARG_PARAMETER);
}

CosNotifyFilter::FilterIDSeq*
TAO_Notify_SupplierAdmin_i::get_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  return this->filter_admin_.get_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_Notify_SupplierAdmin_i::remove_all_filters (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  this->filter_admin_.remove_all_filters (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CosEventChannelAdmin::ProxyPushConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_push_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  TAO_Notify_CosEC_ProxyPushConsumer_i* cosec_proxy;

  ACE_NEW_THROW_EX (cosec_proxy,
                    TAO_Notify_CosEC_ProxyPushConsumer_i (this),
                    CORBA::NO_MEMORY ());

  PortableServer::ServantBase_var proxy_var (cosec_proxy);

  cosec_proxy->init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushConsumer::_nil ());

  PortableServer::POA_var def_poa =
    this->event_channel_->get_default_POA ();

  CORBA::Object_var obj = this->poa_factory_->
    activate_object (def_poa.in (), cosec_proxy ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosEventChannelAdmin::ProxyPushConsumer::_nil ());

  return CosEventChannelAdmin::ProxyPushConsumer::_narrow (obj.in ());
}

// = Pull Methods.
CosNotifyChannelAdmin::ProxyIDSeq*
TAO_Notify_SupplierAdmin_i::pull_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // @@ Pradeep: there is no rush to implement pull, but look at the
  // code in the new CosEC, we may need to start thinking about pull,
  // and how can we reduce duplicated code for both pull and push
  // models.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);
}

CosNotifyChannelAdmin::ProxyConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_notification_pull_consumer (CosNotifyChannelAdmin::ClientType /*ctype*/, CosNotifyChannelAdmin::ProxyID_out /*proxy_id*/ ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyChannelAdmin::AdminLimitExceeded
                   ))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosNotifyChannelAdmin::ProxyConsumer::_nil ());
}

CosEventChannelAdmin::ProxyPullConsumer_ptr
TAO_Notify_SupplierAdmin_i::obtain_pull_consumer (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // @@ Pradeep: you may want to group all the Pull methods together,
  // it would be easier to identify them or read the code that way.
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    CosEventChannelAdmin::ProxyPullConsumer::_nil ());
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ProxyID, CosNotifyChannelAdmin::ProxyIDSeq>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate TAO_Notify_ID_Pool_Ex<CosNotifyChannelAdmin::ProxyID, CosNotifyChannelAdmin::ProxyIDSeq>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
