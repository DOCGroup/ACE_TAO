// $Id$

#include "EC_ObserverStrategy.h"
#include "EC_Event_Channel_Base.h"
#include "EC_ProxySupplier.h"
#include "EC_ProxyConsumer.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "ace/Synch.h"
#include "ace/Auto_Ptr.h"

#if ! defined (__ACE_INLINE__)
#include "EC_ObserverStrategy.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_ObserverStrategy, "$Id$")

TAO_EC_ObserverStrategy::~TAO_EC_ObserverStrategy (void)
{
}

// ****************************************************************

RtecEventChannelAdmin::Observer_Handle
TAO_EC_Null_ObserverStrategy::append_observer (
       RtecEventChannelAdmin::Observer_ptr
       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
{
  ACE_THROW_RETURN (
      RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER(),
      0);
}

void
TAO_EC_Null_ObserverStrategy::remove_observer (
       RtecEventChannelAdmin::Observer_Handle
       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  ACE_THROW (RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER());
}

void
TAO_EC_Null_ObserverStrategy::connected (TAO_EC_ProxyPushConsumer*
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Null_ObserverStrategy::disconnected (TAO_EC_ProxyPushConsumer*
                                            ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Null_ObserverStrategy::connected (TAO_EC_ProxyPushSupplier*
                                         ACE_ENV_ARG_DECL_NOT_USED)
{
}

void
TAO_EC_Null_ObserverStrategy::disconnected (TAO_EC_ProxyPushSupplier*
                                            ACE_ENV_ARG_DECL_NOT_USED)
{
}

// ****************************************************************

TAO_EC_Basic_ObserverStrategy::~TAO_EC_Basic_ObserverStrategy (void)
{
  delete this->lock_;
  this->lock_ = 0;
}

RtecEventChannelAdmin::Observer_Handle
TAO_EC_Basic_ObserverStrategy::append_observer (
       RtecEventChannelAdmin::Observer_ptr obs
       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER))
{
  {
    ACE_GUARD_THROW_EX (
        ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
    ACE_CHECK_RETURN (0);

    this->handle_generator_++;
    Observer_Entry entry (this->handle_generator_,
                          RtecEventChannelAdmin::Observer::_duplicate (obs));

    if (this->observers_.bind (entry.handle, entry) == -1)
      ACE_THROW_RETURN (
          RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER(),
          0);
  }

  RtecEventChannelAdmin::ConsumerQOS c_qos;
  this->fill_qos (c_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  obs->update_consumer (c_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  RtecEventChannelAdmin::SupplierQOS s_qos;
  this->fill_qos (s_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);
  obs->update_supplier (s_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->handle_generator_;
}


void
TAO_EC_Basic_ObserverStrategy::remove_observer (
       RtecEventChannelAdmin::Observer_Handle handle
       ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
  ACE_CHECK;

  if (this->observers_.unbind (handle) == -1)
    ACE_THROW (
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER());
}

int
TAO_EC_Basic_ObserverStrategy::create_observer_list (
                                RtecEventChannelAdmin::Observer_var *&lst
                                ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                 RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
  ACE_CHECK_RETURN (0);

  int size = ACE_static_cast (int, this->observers_.current_size ());
  RtecEventChannelAdmin::Observer_var *tmp;
  ACE_NEW_RETURN (tmp,
                  RtecEventChannelAdmin::Observer_var[size],
                  0);
  ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var> copy (tmp);

  Observer_Map_Iterator end = this->observers_.end ();
  int j = 0;
  for (Observer_Map_Iterator i  = this->observers_.begin ();
       i != end;
       ++i)
    {
      Observer_Entry& entry = (*i).int_id_;
      copy[j++] =
        RtecEventChannelAdmin::Observer::_duplicate (entry.observer.in ());
    }

  lst = copy.release ();
  return size;
}

void
TAO_EC_Basic_ObserverStrategy::connected (
    TAO_EC_ProxyPushConsumer *consumer
    ACE_ENV_ARG_DECL)
{
  this->supplier_qos_update (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Basic_ObserverStrategy::disconnected (
    TAO_EC_ProxyPushConsumer* consumer
    ACE_ENV_ARG_DECL)
{
  this->supplier_qos_update (consumer ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Basic_ObserverStrategy::supplier_qos_update (
                                        TAO_EC_ProxyPushConsumer *consumer
                                        ACE_ENV_ARG_DECL)
{
  if (consumer->publications ().is_gateway)
    return;

  RtecEventChannelAdmin::SupplierQOS s_qos;
  this->fill_qos (s_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::Observer_var *tmp = 0;
  int size = this->create_observer_list (tmp ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var> copy (tmp);

  for (int i = 0; i != size; ++i)
    {
      copy[i]->update_supplier (s_qos ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_EC_Basic_ObserverStrategy::connected (
    TAO_EC_ProxyPushSupplier* supplier
    ACE_ENV_ARG_DECL)
{
  this->consumer_qos_update (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Basic_ObserverStrategy::disconnected (
    TAO_EC_ProxyPushSupplier* supplier
    ACE_ENV_ARG_DECL)
{
  this->consumer_qos_update (supplier ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Basic_ObserverStrategy::consumer_qos_update (
                                        TAO_EC_ProxyPushSupplier *supplier
                                        ACE_ENV_ARG_DECL)
{
  if (supplier->subscriptions ().is_gateway)
    return;

  RtecEventChannelAdmin::ConsumerQOS c_qos;
  this->fill_qos (c_qos ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::Observer_var *tmp = 0;
  int size = this->create_observer_list (tmp ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var> copy (tmp);

  for (int i = 0; i != size; ++i)
    {
      copy[i]->update_consumer (c_qos ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_EC_Basic_ObserverStrategy::fill_qos (
      RtecEventChannelAdmin::ConsumerQOS &qos
      ACE_ENV_ARG_DECL)
{
  Headers headers;

  TAO_EC_Accumulate_Supplier_Headers worker (headers);
  this->event_channel_->for_each_consumer (&worker
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventChannelAdmin::DependencySet& dep = qos.dependencies;

  dep.length (ACE_static_cast (CORBA::ULong, headers.current_size () + 1));

  dep[0].event.header.type = ACE_ES_DISJUNCTION_DESIGNATOR;
  dep[0].event.header.source = ACE_static_cast (CORBA::ULong,
                                                headers.current_size ());
  dep[0].event.header.creation_time = ORBSVCS_Time::zero ();
  dep[0].rt_info = 0;

  CORBA::ULong count = 1;
  for (HeadersIterator i = headers.begin (); i != headers.end (); ++i)
    {
      dep[count++].event.header = (*i).key ();
    }
}

void
TAO_EC_Basic_ObserverStrategy::fill_qos (
      RtecEventChannelAdmin::SupplierQOS &qos
      ACE_ENV_ARG_DECL)
{
  Headers headers;

  TAO_EC_Accumulate_Consumer_Headers worker (headers);
  this->event_channel_->for_each_supplier (&worker
                                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  qos.publications.length (ACE_static_cast (CORBA::ULong,
                                            headers.current_size ()));

  CORBA::ULong count = 0;
  for (HeadersIterator i = headers.begin (); i != headers.end (); ++i)
    {
      qos.publications[count++].event.header = (*i).key ();
    }
}

// ****************************************************************

void
TAO_EC_Accumulate_Supplier_Headers::work (TAO_EC_ProxyPushSupplier *supplier
                                          ACE_ENV_ARG_DECL_NOT_USED)
{
  const RtecEventChannelAdmin::ConsumerQOS& sub =
    supplier->subscriptions ();
  if (sub.is_gateway)
    return;
  for (CORBA::ULong j = 0; j < sub.dependencies.length (); ++j)
    {
      const RtecEventComm::Event& event =
        sub.dependencies[j].event;
      RtecEventComm::EventType type = event.header.type;

      if (0 < type && type < ACE_ES_EVENT_UNDEFINED)
        continue;
      this->headers_.insert (event.header, 1);
    }
}

// ****************************************************************

void
TAO_EC_Accumulate_Consumer_Headers::work (TAO_EC_ProxyPushConsumer *consumer
                                          ACE_ENV_ARG_DECL_NOT_USED)
{
  const RtecEventChannelAdmin::SupplierQOS& pub =
    consumer->publications ();
  if (pub.is_gateway)
    return;
  for (CORBA::ULong j = 0; j < pub.publications.length (); ++j)
    {
      const RtecEventComm::Event& event =
        pub.publications[j].event;
      RtecEventComm::EventType type = event.header.type;

      if (0 < type && type < ACE_ES_EVENT_UNDEFINED)
        continue;
      this->headers_.insert (event.header, 1);
    }
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Map_Manager<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>;
template class ACE_Map_Iterator<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>;
template class ACE_Map_Entry<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry>;

template class ACE_RB_Tree<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator_Base<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>;
template class ACE_RB_Tree_Reverse_Iterator<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<RtecEventComm::EventHeader,int>;

template class ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Map_Manager<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry>

#pragma instantiate ACE_RB_Tree<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator_Base<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Reverse_Iterator<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<RtecEventComm::EventHeader,int>

#pragma instantiate ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
