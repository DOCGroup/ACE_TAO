// $Id$

#include "orbsvcs/Event/EC_Gateway.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"

ACE_RCSID(Event, EC_Gateway, "$Id$")

TAO_EC_Gateway::TAO_EC_Gateway (void)
  :  handle_ (0)
{
}

TAO_EC_Gateway::~TAO_EC_Gateway (void)
{
}

void
TAO_EC_Gateway::observer_handle (RtecEventChannelAdmin::Observer_Handle h)
{
  this->handle_ = h;
}

RtecEventChannelAdmin::Observer_Handle
TAO_EC_Gateway::observer_handle (void) const
{
  return this->handle_;
}

// ****************************************************************

TAO_EC_Gateway_IIOP::TAO_EC_Gateway_IIOP (void)
  :  busy_count_ (0),
     update_posted_ (0),
     rmt_info_ (0),
     lcl_info_ (0),
     consumer_ (this),
     consumer_is_active_ (0),
     supplier_ (this),
     supplier_is_active_ (0)
{
}

TAO_EC_Gateway_IIOP::~TAO_EC_Gateway_IIOP (void)
{
}

void
TAO_EC_Gateway_IIOP::init (RtecEventChannelAdmin::EventChannel_ptr rmt_ec,
                           RtecEventChannelAdmin::EventChannel_ptr lcl_ec,
                           RtecScheduler::Scheduler_ptr rmt_sched,
                           RtecScheduler::Scheduler_ptr lcl_sched,
                           const char* lcl_name,
                           const char* rmt_name,
                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  if (!CORBA::is_nil (this->rmt_ec_.in ()))
    return;

  this->rmt_ec_ =
    RtecEventChannelAdmin::EventChannel::_duplicate (rmt_ec);
  this->lcl_ec_ =
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  if (!CORBA::is_nil (rmt_sched))
    {
      this->rmt_info_ =
        rmt_sched->create (rmt_name, ACE_TRY_ENV);
      ACE_CHECK;

      // @@ TODO Many things are hard-coded in the RT_Info here.

      // The worst case execution time is far less than 500 usecs, but
      // that is a safe estimate....
      ACE_Time_Value tv (0, 500);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      rmt_sched->set (this->rmt_info_,
                      RtecScheduler::VERY_HIGH_CRITICALITY,
                      time, time, time,
                      25000 * 10,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      time,
                      0,
                      RtecScheduler::OPERATION,
                      ACE_TRY_ENV);
      ACE_CHECK;
    }

  if (!CORBA::is_nil (lcl_sched))
    {
      this->lcl_info_ =
        lcl_sched->create (lcl_name, ACE_TRY_ENV);
      ACE_CHECK;

      ACE_Time_Value tv (0, 500);
      TimeBase::TimeT time;
      ORBSVCS_Time::Time_Value_to_TimeT (time, tv);
      lcl_sched->set (this->lcl_info_,
                      RtecScheduler::VERY_HIGH_CRITICALITY,
                      time, time, time,
                      25000 * 10,
                      RtecScheduler::VERY_LOW_IMPORTANCE,
                      time,
                      1,
                      RtecScheduler::REMOTE_DEPENDANT,
                      ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
TAO_EC_Gateway_IIOP::close (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  this->close_i (ACE_TRY_ENV);
}


void
TAO_EC_Gateway_IIOP::close_i (CORBA::Environment &ACE_TRY_ENV)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));

  if (this->consumer_proxy_map_.current_size () > 0)
    {
      for (Consumer_Map_Iterator j = this->consumer_proxy_map_.begin ();
           j != this->consumer_proxy_map_.end ();
           ++j)
        {
          RtecEventComm::PushConsumer_ptr consumer = (*j).int_id_;
          if (CORBA::is_nil (consumer))
            continue;
          consumer->disconnect_push_consumer (ACE_TRY_ENV);
          CORBA::release (consumer);
          ACE_CHECK;
        }
      this->consumer_proxy_map_.close ();
    }

  if (!CORBA::is_nil (this->default_consumer_proxy_.in ()))
    {
      this->default_consumer_proxy_->disconnect_push_consumer (ACE_TRY_ENV);
      ACE_CHECK;

      this->default_consumer_proxy_ =
        RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
    }

  if (!CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_->disconnect_push_supplier (ACE_TRY_ENV);
      ACE_CHECK;

      this->supplier_proxy_ =
        RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
    }
}

void
TAO_EC_Gateway_IIOP::update_consumer (
    const RtecEventChannelAdmin::ConsumerQOS& c_qos,
    CORBA::Environment& ACE_TRY_ENV)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (c_qos.dependencies.length () <= 1)
    return;

  ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->busy_count_ != 0)
    {
      this->update_posted_ = 1;
      this->c_qos_ = c_qos;
      return;
    }

  this->update_consumer_i (c_qos, ACE_TRY_ENV);
}

void
TAO_EC_Gateway_IIOP::update_consumer_i (
    const RtecEventChannelAdmin::ConsumerQOS& c_qos,
    CORBA::Environment& ACE_TRY_ENV)
{
  this->close_i (ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (this->lcl_ec_.in ())
      || CORBA::is_nil (this->rmt_ec_.in ()))
    return;

  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Open gateway\n"));

  // = Connect as a supplier to the local EC
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->lcl_ec_->for_suppliers (ACE_TRY_ENV);
  ACE_CHECK;

  // Change the RT_Info in the consumer QoS.
  // On the same loop we discover the subscriptions by event source,
  // and fill the consumer proxy map.
  RtecEventChannelAdmin::ConsumerQOS sub = c_qos;
  sub.is_gateway = 1;
  for (CORBA::ULong i = 0; i < sub.dependencies.length (); ++i)
    {
      sub.dependencies[i].rt_info = this->rmt_info_;

      RtecEventChannelAdmin::ProxyPushConsumer_ptr proxy = 0;
      RtecEventComm::EventSourceID sid =
        sub.dependencies[i].event.header.source;
      if (sid != 0
          && this->consumer_proxy_map_.find (sid, proxy) == -1)
        {
          proxy = supplier_admin->obtain_push_consumer (ACE_TRY_ENV);
          ACE_CHECK;
          this->consumer_proxy_map_.bind (sid, proxy);
        }
    }


  if (this->consumer_proxy_map_.current_size () > 0)
    {
      this->supplier_is_active_ = 1;

      // Obtain a reference to our supplier personality...
      RtecEventComm::PushSupplier_var supplier_ref =
        this->supplier_._this (ACE_TRY_ENV);
      ACE_CHECK;

      // For each subscription by source build the set of publications
      // (they may several, by type, for instance) and connect to the
      // consumer proxy.
      for (Consumer_Map_Iterator j = this->consumer_proxy_map_.begin ();
           j != this->consumer_proxy_map_.end ();
           ++j)
        {
          RtecEventChannelAdmin::SupplierQOS pub;
          pub.publications.length (sub.dependencies.length ());
          pub.is_gateway = 1;
          int c = 0;

          RtecEventComm::EventSourceID sid = (*j).ext_id_;
          for (CORBA::ULong k = 0; k < sub.dependencies.length (); ++k)
            {
              const RtecEventComm::EventHeader& h =
                sub.dependencies[k].event.header;
              if (h.source != sid
                  || (1 <= h.type
                      && h.type < ACE_ES_EVENT_UNDEFINED))
                continue;
              pub.publications[c].event.header = h;
              pub.publications[c].dependency_info.dependency_type =
                RtecScheduler::TWO_WAY_CALL;
              pub.publications[c].dependency_info.number_of_calls = 1;
              pub.publications[c].dependency_info.rt_info = this->lcl_info_;
              c++;
            }
          if (c == 0)
            continue;

          pub.publications.length (c);
          (*j).int_id_->connect_push_supplier (supplier_ref.in (),
                                               pub,
                                               ACE_TRY_ENV);
          ACE_CHECK;
        }
    }

  // Also build the subscriptions that are *not* by source and connect
  // to the default consumer proxy.
  RtecEventChannelAdmin::SupplierQOS pub;
  pub.publications.length (sub.dependencies.length () - 1);
  pub.is_gateway = 1;
  int c = 0;
  for (CORBA::ULong k = 0; k < sub.dependencies.length (); ++k)
    {
      const RtecEventComm::EventHeader& h =
        sub.dependencies[k].event.header;
      RtecEventComm::EventSourceID sid = h.source;
      if (sid != 0
          || (0 <= h.type
              && h.type < ACE_ES_EVENT_UNDEFINED))
        continue;
      pub.publications[c].event.header = h;
      pub.publications[c].event.header.creation_time = ORBSVCS_Time::zero ();
      pub.publications[c].dependency_info.dependency_type =
        RtecScheduler::TWO_WAY_CALL;
      pub.publications[c].dependency_info.number_of_calls = 1;
      pub.publications[c].dependency_info.rt_info = this->lcl_info_;
      c++;
    }

  if (c > 0)
    {
      this->supplier_is_active_ = 1;

      // Obtain a reference to our supplier personality...
      RtecEventComm::PushSupplier_var supplier_ref =
        this->supplier_._this (ACE_TRY_ENV);
      ACE_CHECK;

      // Obtain the consumer....
      this->default_consumer_proxy_ =
        supplier_admin->obtain_push_consumer (ACE_TRY_ENV);
      ACE_CHECK;

      pub.publications.length (c);
      this->default_consumer_proxy_->connect_push_supplier (supplier_ref.in (),
                                                            pub,
                                                            ACE_TRY_ENV);
      ACE_CHECK;
    }


  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Supplier "));
  // ACE_SupplierQOS_Factory::debug (pub);

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->rmt_ec_->for_consumers (ACE_TRY_ENV);
  ACE_CHECK;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_is_active_ = 1;
  RtecEventComm::PushConsumer_var consumer_ref =
    this->consumer_._this (ACE_TRY_ENV);
  ACE_CHECK;

  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Consumer "));
  // ACE_ConsumerQOS_Factory::debug (sub);

  this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                sub,
                                                ACE_TRY_ENV);
  ACE_CHECK;

}

void
TAO_EC_Gateway_IIOP::update_supplier (
    const RtecEventChannelAdmin::SupplierQOS&,
    CORBA::Environment&)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing...
}

void
TAO_EC_Gateway_IIOP::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "ECG (%t): Supplier-consumer received "
              "disconnect from channel.\n"));
}

void
TAO_EC_Gateway_IIOP::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "ECG (%t): Supplier received "
              "disconnect from channel.\n"));
}

void
TAO_EC_Gateway_IIOP::push (const RtecEventComm::EventSet &events,
                           CORBA::Environment & ACE_TRY_ENV)
{
  //ACE_DEBUG ((LM_DEBUG, "TAO_EC_Gateway_IIOP::push - "));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

    this->busy_count_++;
  }

  // ACE_DEBUG ((LM_DEBUG, "ECG: %d event(s)\n", events.length ()));

  // @@ TODO, there is an extra data copy here, we should do the event
  // modification without it and only compact the necessary events.
  RtecEventComm::EventSet out (1);
  out.length (1);
  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      if (events[i].header.ttl == 0)
        continue;

      RtecEventChannelAdmin::ProxyPushConsumer_ptr proxy = 0;
      RtecEventComm::EventSourceID sid = events[i].header.source;
      if (sid == 0
          || this->consumer_proxy_map_.find (sid, proxy) == -1)
        {
          // If the source is not in our map we have to use the
          // default consumer proxy.
          proxy = this->default_consumer_proxy_.in ();
        }

      if (CORBA::is_nil (proxy))
        continue;

      out[0] = events[i];
      out[0].header.ttl--;

      proxy->push (out, ACE_TRY_ENV);
      ACE_CHECK;
    }

  {
    ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);

    this->busy_count_--;

    if (this->busy_count_ == 0 && this->update_posted_ != 0)
      {
        this->update_posted_ = 0;
        this->update_consumer_i (this->c_qos_, ACE_TRY_ENV);
      }
  }

}

int
TAO_EC_Gateway_IIOP::shutdown (CORBA::Environment& ACE_TRY_ENV)
{
  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  this->close_i (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (this->supplier_is_active_)
    {
      PortableServer::POA_var poa =
        this->supplier_._default_POA (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->supplier_, ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      this->supplier_is_active_ = 0;
    }

  if (this->consumer_is_active_)
    {
      PortableServer::POA_var poa =
        this->consumer_._default_POA (ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->consumer_, ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      poa->deactivate_object (id.in (), ACE_TRY_ENV);
      ACE_CHECK_RETURN (-1);
      this->consumer_is_active_ = 0;
    }

  this->lcl_ec_ =
    RtecEventChannelAdmin::EventChannel::_nil ();
  this->rmt_ec_ =
    RtecEventChannelAdmin::EventChannel::_nil ();

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_PushConsumer_Adapter<TAO_EC_Gateway_IIOP>;
template class ACE_PushSupplier_Adapter<TAO_EC_Gateway_IIOP>;
template class ACE_Map_Entry<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr>;
template class ACE_Map_Manager<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex>;
template class ACE_Map_Iterator<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_PushConsumer_Adapter<TAO_EC_Gateway_IIOP>
#pragma instantiate ACE_PushSupplier_Adapter<TAO_EC_Gateway_IIOP>

#pragma instantiate ACE_Map_Entry<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr>
#pragma instantiate ACE_Map_Manager<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<RtecEventComm::EventSourceID,RtecEventChannelAdmin::ProxyPushConsumer_ptr,ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
