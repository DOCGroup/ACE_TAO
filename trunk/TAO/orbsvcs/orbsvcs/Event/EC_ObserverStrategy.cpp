// $Id$

#include "EC_ObserverStrategy.h"
#include "EC_Event_Channel.h"
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
       RtecEventChannelAdmin::Observer_ptr,
       CORBA::Environment &ACE_TRY_ENV)
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
       RtecEventChannelAdmin::Observer_Handle,
       CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR,
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER))
{
  ACE_THROW (RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER());
}

void
TAO_EC_Null_ObserverStrategy::connected (TAO_EC_ProxyPushConsumer*,
                                         CORBA::Environment &)
{
}

void
TAO_EC_Null_ObserverStrategy::disconnected (TAO_EC_ProxyPushConsumer*,
                                            CORBA::Environment &)
{
}

void
TAO_EC_Null_ObserverStrategy::connected (TAO_EC_ProxyPushSupplier*,
                                         CORBA::Environment &)
{
}

void
TAO_EC_Null_ObserverStrategy::disconnected (TAO_EC_ProxyPushSupplier*,
                                            CORBA::Environment &)
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
       RtecEventChannelAdmin::Observer_ptr obs,
       CORBA::Environment &ACE_TRY_ENV)
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
  this->fill_qos (c_qos, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  obs->update_consumer (c_qos, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  RtecEventChannelAdmin::SupplierQOS s_qos;
  this->fill_qos (s_qos, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);
  obs->update_supplier (s_qos, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  return this->handle_generator_;
}


void
TAO_EC_Basic_ObserverStrategy::remove_observer (
       RtecEventChannelAdmin::Observer_Handle handle,
       CORBA::Environment &ACE_TRY_ENV)
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

void
TAO_EC_Basic_ObserverStrategy::connected (
    TAO_EC_ProxyPushConsumer *consumer,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (consumer->publications ().is_gateway)
    return;

  RtecEventChannelAdmin::SupplierQOS s_qos;
  this->fill_qos (s_qos, ACE_TRY_ENV);
  ACE_CHECK;

  int size = 0;
  ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var> copy;
  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
    ACE_CHECK;
    size = this->observers_.current_size ();
    RtecEventChannelAdmin::Observer_var *tmp;
    ACE_NEW (tmp, RtecEventChannelAdmin::Observer_var[size]);
    copy.reset (tmp);

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
  }

  for (int i = 0; i != size; ++i)
    {
      copy[i]->update_supplier (s_qos, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_Basic_ObserverStrategy::disconnected (
    TAO_EC_ProxyPushConsumer* consumer,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (consumer->publications ().is_gateway)
    return;

  RtecEventChannelAdmin::SupplierQOS s_qos;
  this->fill_qos (s_qos, ACE_TRY_ENV);
  ACE_CHECK;

  int size = 0;
  ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var> copy;
  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
    ACE_CHECK;
    size = this->observers_.current_size ();
    RtecEventChannelAdmin::Observer_var *tmp;
    ACE_NEW (tmp, RtecEventChannelAdmin::Observer_var[size]);
    copy.reset (tmp);

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
  }

  for (int i = 0; i != size; ++i)
    {
      copy[i]->update_supplier (s_qos, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_Basic_ObserverStrategy::connected (
    TAO_EC_ProxyPushSupplier* supplier,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (supplier->subscriptions ().is_gateway)
    return;

  RtecEventChannelAdmin::ConsumerQOS c_qos;
  this->fill_qos (c_qos, ACE_TRY_ENV);
  ACE_CHECK;

  int size = 0;
  ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var> copy;
  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
    ACE_CHECK;
    size = this->observers_.current_size ();
    RtecEventChannelAdmin::Observer_var *tmp;
    ACE_NEW (tmp, RtecEventChannelAdmin::Observer_var[size]);
    copy.reset (tmp);

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
  }

  for (int i = 0; i != size; ++i)
    {
      copy[i]->update_consumer (c_qos, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_Basic_ObserverStrategy::disconnected (
    TAO_EC_ProxyPushSupplier* supplier,
    CORBA::Environment &ACE_TRY_ENV)
{
  if (supplier->subscriptions ().is_gateway)
    return;

  RtecEventChannelAdmin::ConsumerQOS c_qos;
  this->fill_qos (c_qos, ACE_TRY_ENV);
  ACE_CHECK;

  int size = 0;
  ACE_Auto_Basic_Array_Ptr<RtecEventChannelAdmin::Observer_var> copy;
  {
    ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
    ACE_CHECK;
    size = this->observers_.current_size ();
    RtecEventChannelAdmin::Observer_var *tmp;
    ACE_NEW (tmp, RtecEventChannelAdmin::Observer_var[size]);
    copy.reset (tmp);

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
  }

  for (int i = 0; i != size; ++i)
    {
      copy[i]->update_consumer (c_qos, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_Basic_ObserverStrategy::fill_qos (
      RtecEventChannelAdmin::ConsumerQOS &qos,
      CORBA::Environment &ACE_TRY_ENV)
{
  Headers headers;

  TAO_EC_Accumulate_Supplier_Headers worker (headers);
  this->event_channel_->consumer_admin ()->for_each (&worker, ACE_TRY_ENV);
  ACE_CHECK;

  RtecEventChannelAdmin::DependencySet& dep = qos.dependencies;

  dep.length (headers.current_size () + 1);

  dep[0].event.header.type = ACE_ES_DISJUNCTION_DESIGNATOR;
  dep[0].event.header.source = 0;
  dep[0].event.header.creation_time = ORBSVCS_Time::zero ();
  dep[0].rt_info = 0;

  CORBA::ULong count = 1;
  HeadersIterator i (headers);
  for (i.first (); !i.is_done (); i.next ())
    {
      qos.dependencies[count++].event.header = *i.key ();
    }
}

void
TAO_EC_Basic_ObserverStrategy::fill_qos (
      RtecEventChannelAdmin::SupplierQOS &qos,
      CORBA::Environment &ACE_TRY_ENV)
{
  Headers headers;

  TAO_EC_Accumulate_Consumer_Headers worker (headers);
  this->event_channel_->supplier_admin ()->for_each (&worker,
                                                     ACE_TRY_ENV);
  ACE_CHECK;

  qos.publications.length (headers.current_size ());

  CORBA::ULong count = 0;
  HeadersIterator i (headers);
  for (i.first (); !i.is_done (); i.next ())
    {
      qos.publications[count++].event.header = *i.key ();
    }
}

// ****************************************************************

void
TAO_EC_Accumulate_Supplier_Headers::work (TAO_EC_ProxyPushSupplier *supplier,
                                          CORBA::Environment &)
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
TAO_EC_Accumulate_Consumer_Headers::work (TAO_EC_ProxyPushConsumer *consumer,
                                          CORBA::Environment &)
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
