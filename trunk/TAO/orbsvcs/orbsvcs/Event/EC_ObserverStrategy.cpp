// $Id$

#include "EC_ObserverStrategy.h"
#include "EC_Event_Channel.h"
#include "EC_ProxySupplier.h"
#include "EC_ProxyConsumer.h"
#include "EC_ConsumerAdmin.h"
#include "EC_SupplierAdmin.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "ace/Synch.h"

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
       RtecEventChannelAdmin::Observer_ptr obs,
       CORBA::Environment &ACE_TRY_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_APPEND_OBSERVER))
{
  ACE_THROW_RETURN (
      RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER(),
      0);
}

void
TAO_EC_Null_ObserverStrategy::remove_observer (
       RtecEventChannelAdmin::Observer_Handle handle,
       CORBA::Environment &ACE_TRY_ENV)
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_REMOVE_OBSERVER))
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
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_APPEND_OBSERVER))
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
  ACE_CHECK_RETURN (0);

  this->handle_generator_++;
  Observer_Entry entry (this->handle_generator_,
                        RtecEventChannelAdmin::Observer::_duplicate (obs));

  if (this->observers_.bind (entry.handle, entry) == -1)
    ACE_THROW_RETURN (
        RtecEventChannelAdmin::EventChannel::CANT_APPEND_OBSERVER(),
        0);

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
    TAO_THROW_SPEC ((CORBA::SystemException,
                     RtecEventChannel::EventChannel::SYNCHRONIZATION_ERROR,
                     RtecEventChannel::EventChannel::CANT_REMOVE_OBSERVER))
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
      RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR());
  ACE_CHECK;

  if (this->observers_.unbind (handle) == -1)
    ACE_THROW (
        RtecEventChannelAdmin::EventChannel::CANT_REMOVE_OBSERVER());
}

void
TAO_EC_Basic_ObserverStrategy::connected (TAO_EC_ProxyPushConsumer*,
                                         CORBA::Environment &)
{
}

void
TAO_EC_Basic_ObserverStrategy::disconnected (TAO_EC_ProxyPushConsumer*,
                                            CORBA::Environment &)
{
}

void
TAO_EC_Basic_ObserverStrategy::connected (TAO_EC_ProxyPushSupplier*,
                                         CORBA::Environment &)
{
}

void
TAO_EC_Basic_ObserverStrategy::disconnected (TAO_EC_ProxyPushSupplier*,
                                            CORBA::Environment &)
{
}

void
TAO_EC_Basic_ObserverStrategy::fill_qos (
      RtecEventChannelAdmin::ConsumerQOS &qos,
      CORBA::Environment &ACE_TRY_ENV)
{
  Headers headers;

  {
    ACE_GUARD_THROW_EX (TAO_EC_ConsumerAdmin::Busy_Lock,
        ace_mon, this->event_channel_->consumer_admin ()->busy_lock (),
        RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR ());
    ACE_CHECK;

    TAO_EC_ConsumerAdmin::SupplierSetIterator end =
      this->event_channel_->consumer_admin ()->end ();
    for (TAO_EC_ConsumerAdmin::SupplierSetIterator i =
           this->event_channel_->consumer_admin ()->begin ();
         i != end;
         ++i)
      {
        TAO_EC_ProxyPushSupplier* supplier = *i;

        const RtecEventChannelAdmin::ConsumerQOS& sub =
          supplier->subscriptions ();
        if (sub.is_gateway)
          continue;
        for (CORBA::ULong j = 0; j < sub.dependencies.length (); ++j)
          {
            const RtecEventComm::Event& event =
              sub.dependencies[j].event;
            RtecEventComm::EventType type = event.header.type;

            if (0 <= type && type <= ACE_ES_EVENT_UNDEFINED)
              continue;
            headers.insert (event.header, 1);
          }
      }
  }
  CORBA::ULong count = 1;
  HeadersIterator i (headers);
  for (i.first (); !i.is_done (); i.next ())
    {
      count++;
    }

  RtecEventChannelAdmin::DependencySet& dep = qos.dependencies;

  dep.length (count + 1);

  dep[0].event.header.type = ACE_ES_DISJUNCTION_DESIGNATOR;
  dep[0].event.header.source = 0;
  dep[0].event.header.creation_time = ORBSVCS_Time::zero ();
  dep[0].rt_info = 0;
  count = 1;
  for (i.first (); !i.is_done (); i.next ())
    {
      qos.dependencies[count++].event.header = *i.key ();
    }
}

void
TAO_EC_Basic_ObserverStrategy::fill_qos (
      RtecEventChannelAdmin::SupplierQOS &qos,
      CORBA::Environment &)
{
  Headers headers;

  {
    // @@ TODO locking in the consumer admin?
    TAO_EC_SupplierAdmin::ConsumerSetIterator end =
      this->event_channel_->supplier_admin ()->end ();
    for (TAO_EC_SupplierAdmin::ConsumerSetIterator i =
           this->event_channel_->supplier_admin ()->begin ();
         i != end;
         ++i)
      {
        TAO_EC_ProxyPushConsumer* consumer = *i;
        const RtecEventChannelAdmin::SupplierQOS& pub =
          consumer->publications ();
        if (pub.is_gateway)
          continue;
        for (CORBA::ULong j = 0; j < pub.publications.length (); ++j)
          {
            const RtecEventComm::Event& event =
              pub.publications[j].event;
            RtecEventComm::EventType type = event.header.type;

            if (0 <= type && type <= ACE_ES_EVENT_UNDEFINED)
              continue;
            headers.insert (event.header, 1);
          }
      }
  }
  CORBA::ULong count = 0;
  HeadersIterator i (headers);
  for (i.first (); !i.is_done (); i.next ())
    {
      count++;
    }
  qos.publications.length (count);
  count = 0;
  for (i.first (); !i.is_done (); i.next ())
    {
      qos.publications[count++].event.header = *i.key ();
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Map_Manager<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>;
template class ACE_Map_Iterator<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>;
template class ACE_Map_Entry<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry>;

template class ACE_RB_Tree<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>;
template class ACE_RB_Tree_Iterator<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>;
template class ACE_RB_Tree_Node<RtecEventComm::EventHeader,int>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Map_Manager<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Entry<RtecEventChannelAdmin::Observer_Handle,TAO_EC_Basic_ObserverStrategy::Observer_Entry>

#pragma instantiate ACE_RB_Tree<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Iterator<RtecEventComm::EventHeader,int,TAO_EC_Basic_ObserverStrategy::Header_Compare,ACE_Null_Mutex>
#pragma instantiate ACE_RB_Tree_Node<RtecEventComm::EventHeader,int>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
