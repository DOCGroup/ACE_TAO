// $Id$

#include "orbsvcs/Event/EC_Gateway_IIOP.h"
#include "orbsvcs/Event/ECG_Defaults.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"
#include "EC_Gateway_IIOP_Factory.h"
#include "ace/Dynamic_Service.h"

#include "ECG_ConsumerEC_Control.h"

ACE_RCSID(Event, EC_Gateway_IIOP, "$Id$")

TAO_EC_Gateway_IIOP::TAO_EC_Gateway_IIOP (void)
  :  busy_count_ (0),
     update_posted_ (0),
     supplier_info_ (0),
     consumer_info_ (0),
     consumer_ (this),
     consumer_is_active_ (0),
     supplier_ (this),
     supplier_is_active_ (0),
     ec_control_ (0),
     factory_ (0)
{
  if (this->factory_ == 0)
    {
      this->factory_ =
             ACE_Dynamic_Service<TAO_EC_Gateway_IIOP_Factory>::instance ("EC_Gateway_IIOP_Factory");

      if (this->factory_ == 0)
        {
          TAO_EC_Gateway_IIOP_Factory *f = 0;
          ACE_NEW (f,
                   TAO_EC_Gateway_IIOP_Factory);
          this->factory_ = f;
        }
    }
}

TAO_EC_Gateway_IIOP::~TAO_EC_Gateway_IIOP (void)
{
   delete ec_control_;
   ec_control_ = 0;
}

int
TAO_EC_Gateway_IIOP::init (RtecEventChannelAdmin::EventChannel_ptr supplier_ec,
                           RtecEventChannelAdmin::EventChannel_ptr consumer_ec
                           ACE_ENV_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  return this->init_i (supplier_ec, consumer_ec ACE_ENV_ARG_PARAMETER);
}

int
TAO_EC_Gateway_IIOP::init_i (RtecEventChannelAdmin::EventChannel_ptr supplier_ec,
                             RtecEventChannelAdmin::EventChannel_ptr consumer_ec
                             ACE_ENV_ARG_DECL_NOT_USED)
{
  if (CORBA::is_nil (this->supplier_ec_.in ()) && CORBA::is_nil (this->consumer_ec_.in ()))
  {
    this->supplier_ec_ =
      RtecEventChannelAdmin::EventChannel::_duplicate (supplier_ec);
    this->consumer_ec_ =
      RtecEventChannelAdmin::EventChannel::_duplicate (consumer_ec);

	if (ec_control_ == 0)
     {
        ec_control_ = factory_->create_consumerec_control(this);
        ec_control_->activate();
     }

    return 0;
  }
  else
    ACE_ERROR_RETURN ((LM_ERROR,
                       "TAO_EC_Gateway_IIOP - init_i "
                       "Supplier and consumer event channel reference "
                       "should be nil.\n"), -1);
}

void
TAO_EC_Gateway_IIOP::close (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  this->close_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_EC_Gateway_IIOP::cleanup_consumer_proxies (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  this->cleanup_consumer_proxies_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_EC_Gateway_IIOP::close_i (ACE_ENV_SINGLE_ARG_DECL)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));
  this->disconnect_consumer_proxies_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->disconnect_supplier_proxy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_EC_Gateway_IIOP::disconnect_consumer_proxies_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->consumer_proxy_map_.current_size () > 0)
    {
      for (Consumer_Map_Iterator j = this->consumer_proxy_map_.begin ();
           j != this->consumer_proxy_map_.end ();
           ++j)
        {
          RtecEventComm::PushConsumer_ptr consumer = (*j).int_id_;
          if (CORBA::is_nil (consumer))
            continue;
          ACE_TRY
            {
              consumer->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          ACE_CATCHANY
            {
            }
          ACE_ENDTRY;
          CORBA::release (consumer);
        }
      // Remove all the elements on the map.  Calling close() does not
      // work because the map is left in an inconsistent state.
      this->consumer_proxy_map_.open ();
    }

  if (!CORBA::is_nil (this->default_consumer_proxy_.in ()))
    {
      this->default_consumer_proxy_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->default_consumer_proxy_ =
        RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
    }
}

void
TAO_EC_Gateway_IIOP::disconnect_supplier_proxy_i (ACE_ENV_SINGLE_ARG_DECL)
{
  if (!CORBA::is_nil (this->supplier_proxy_.in ()))
    {
      this->supplier_proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->supplier_proxy_ =
        RtecEventChannelAdmin::ProxyPushSupplier::_nil ();
    }
}

void
TAO_EC_Gateway_IIOP::update_consumer (
    const RtecEventChannelAdmin::ConsumerQOS& c_qos
    ACE_ENV_ARG_DECL)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (c_qos.dependencies.length () == 0)
    return;

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->busy_count_ != 0)
    {
      this->update_posted_ = 1;
      this->c_qos_ = c_qos;
      return;
    }

  this->update_consumer_i (c_qos ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Gateway_IIOP::cleanup_consumer_proxies_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  if (this->consumer_proxy_map_.current_size () > 0)
    {
      for (Consumer_Map_Iterator j = this->consumer_proxy_map_.begin ();
           j != this->consumer_proxy_map_.end ();
           ++j)
        {
          RtecEventComm::PushConsumer_ptr consumer = (*j).int_id_;
          if (CORBA::is_nil (consumer))
            continue;

          CORBA::release (consumer);
        }
      // Remove all the elements on the map.  Calling close() does not
      // work because the map is left in an inconsistent state.
      this->consumer_proxy_map_.open ();
    }

  if (!CORBA::is_nil (this->default_consumer_proxy_.in ()))
    {
      this->default_consumer_proxy_ =
        RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
    }
}


void
TAO_EC_Gateway_IIOP::update_consumer_i (
    const RtecEventChannelAdmin::ConsumerQOS& c_qos
    ACE_ENV_ARG_DECL)
{
  this->close_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->consumer_ec_.in ())
      || CORBA::is_nil (this->supplier_ec_.in ()))
    return;

  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) update_consumer_i \n"));

  // = Connect as a supplier to the consumer EC
  RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
    this->consumer_ec_->for_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Change the RT_Info in the consumer QoS.
  // On the same loop we discover the subscriptions by event source,
  // and fill the consumer proxy map.
  RtecEventChannelAdmin::ConsumerQOS sub = c_qos;
  sub.is_gateway = 1;
  for (CORBA::ULong i = 0; i < sub.dependencies.length (); ++i)
    {
      sub.dependencies[i].rt_info = this->supplier_info_;

      RtecEventChannelAdmin::ProxyPushConsumer_ptr proxy = 0;
      const RtecEventComm::EventHeader &h =
        sub.dependencies[i].event.header;

      RtecEventComm::EventSourceID sid = h.source;

      //ACE_DEBUG ((LM_DEBUG,
      //            "ECG (%t)    trying (%d,%d)\n",
      //           sid, h.type));

      // Skip all subscriptions that do not require an specific source
      // id.
      if (sid == 0)
        continue;

      // Skip all the magic event types.
      if (0 < h.type && h.type < ACE_ES_EVENT_UNDEFINED)
        continue;

      if (this->consumer_proxy_map_.find (sid, proxy) == -1)
        {
          //ACE_DEBUG ((LM_DEBUG,
          //            "ECG (%t)    binding source %d\n",
          //            sid));
          proxy = supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;
          this->consumer_proxy_map_.bind (sid, proxy);
        }
    }
  //ACE_DEBUG ((LM_DEBUG,
  //            "ECG (%t)    consumer map computed (%d entries)\n",
  //            this->consumer_proxy_map_.current_size ()));

  if (this->consumer_proxy_map_.current_size () > 0)
    {
      this->supplier_is_active_ = 1;

      // Obtain a reference to our supplier personality...
      RtecEventComm::PushSupplier_var supplier_ref =
        this->supplier_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // For each subscription by source build the set of publications
      // (they may several, by type, for instance) and connect to the
      // consumer proxy.
      for (Consumer_Map_Iterator j = this->consumer_proxy_map_.begin ();
           j != this->consumer_proxy_map_.end ();
           ++j)
        {
          RtecEventChannelAdmin::SupplierQOS pub;
          pub.publications.length (sub.dependencies.length () + 1);
          pub.is_gateway = 1;

          int c = 0;

          RtecEventComm::EventSourceID sid = (*j).ext_id_;
          for (CORBA::ULong k = 0; k < sub.dependencies.length (); ++k)
            {
              const RtecEventComm::EventHeader& h =
                sub.dependencies[k].event.header;
              if (h.source != sid
                  || (0 < h.type
                      && h.type < ACE_ES_EVENT_UNDEFINED))
                continue;
              pub.publications[c].event.header = h;
              pub.publications[c].dependency_info.dependency_type =
                RtecBase::TWO_WAY_CALL;
              pub.publications[c].dependency_info.number_of_calls = 1;
              pub.publications[c].dependency_info.rt_info = this->consumer_info_;
              c++;
            }
          //ACE_DEBUG ((LM_DEBUG,
          //            "ECG (%t) supplier id %d has %d elements\n",
          //            sid, c));
          if (c == 0)
            continue;

          pub.publications.length (c);

          // ACE_DEBUG ((LM_DEBUG, "ECG (%P|%t) Gateway/Supplier "));
          // ACE_SupplierQOS_Factory::debug (pub);
          (*j).int_id_->connect_push_supplier (supplier_ref.in (),
                                               pub
                                               ACE_ENV_ARG_PARAMETER);
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
        RtecBase::TWO_WAY_CALL;
      pub.publications[c].dependency_info.number_of_calls = 1;
      pub.publications[c].dependency_info.rt_info = this->consumer_info_;
      c++;
    }

  if (c > 0)
    {
      this->supplier_is_active_ = 1;

      // Obtain a reference to our supplier personality...
      RtecEventComm::PushSupplier_var supplier_ref =
        this->supplier_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Obtain the consumer....
      this->default_consumer_proxy_ =
        supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      pub.publications.length (c);
      // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Supplier "));
      // ACE_SupplierQOS_Factory::debug (pub);
      this->default_consumer_proxy_->connect_push_supplier (supplier_ref.in (),
                                                            pub
                                                            ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }



  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    this->supplier_ec_->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_is_active_ = 1;
  RtecEventComm::PushConsumer_var consumer_ref =
    this->consumer_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // ACE_DEBUG ((LM_DEBUG, "ECG (%P|%t) Gateway/Consumer "));
  // ACE_ConsumerQOS_Factory::debug (sub);

  this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                sub
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

void
TAO_EC_Gateway_IIOP::update_supplier (
    const RtecEventChannelAdmin::SupplierQOS&
    ACE_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing...
}

void
TAO_EC_Gateway_IIOP::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // ACE_DEBUG ((LM_DEBUG,
  //             "ECG (%t): Supplier-consumer received "
  //            "disconnect from channel.\n"));
}

void
TAO_EC_Gateway_IIOP::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  // ACE_DEBUG ((LM_DEBUG,
  //            "ECG (%t): Supplier received "
  //            "disconnect from channel.\n"));
}

void
TAO_EC_Gateway_IIOP::push (const RtecEventComm::EventSet &events
                           ACE_ENV_ARG_DECL)
{
  // ACE_DEBUG ((LM_DEBUG, "TAO_EC_Gateway_IIOP::push (%P|%t) - \n"));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

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

      // ACE_DEBUG ((LM_DEBUG, "ECG: event sent to proxy\n"));
      this->push_to_consumer(proxy, out ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

    this->busy_count_--;

    if (this->busy_count_ == 0 && this->update_posted_ != 0)
      {
        this->update_posted_ = 0;
        this->update_consumer_i (this->c_qos_ ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
  }
}

void
TAO_EC_Gateway_IIOP::push_to_consumer (
    RtecEventChannelAdmin::ProxyPushConsumer_ptr consumer,
    const RtecEventComm::EventSet& event
    ACE_ENV_ARG_DECL)
{
  ACE_TRY
    {
      consumer->push (event ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::OBJECT_NOT_EXIST, not_used)
    {
      ec_control_->event_channel_not_exist (this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ec_control_->system_exception (this,
                                     sysex
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Shouldn't happen.
    }
  ACE_ENDTRY;
}

int
TAO_EC_Gateway_IIOP::shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  ec_control_->shutdown();

  this->close_i (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (this->supplier_is_active_)
    {
      PortableServer::POA_var poa =
        this->supplier_._default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->supplier_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      this->supplier_is_active_ = 0;
    }

  if (this->consumer_is_active_)
    {
      PortableServer::POA_var poa =
        this->consumer_._default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->consumer_ ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      poa->deactivate_object (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
      this->consumer_is_active_ = 0;
    }

  this->cleanup_consumer_ec_i ();
  this->cleanup_supplier_ec_i ();

  return 0;
}

int
TAO_EC_Gateway_IIOP::cleanup_consumer_ec (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  this->cleanup_consumer_ec_i ();

  return 0;
}

int
TAO_EC_Gateway_IIOP::cleanup_supplier_ec (void)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, -1);

  this->cleanup_supplier_ec_i ();

  return 0;
}

void
TAO_EC_Gateway_IIOP::cleanup_consumer_ec_i (void)
{
  this->consumer_ec_ =
    RtecEventChannelAdmin::EventChannel::_nil ();
}

void
TAO_EC_Gateway_IIOP::cleanup_supplier_ec_i (void)
{
  this->supplier_ec_ =
    RtecEventChannelAdmin::EventChannel::_nil ();
}

CORBA::Boolean
TAO_EC_Gateway_IIOP::is_consumer_ec_connected_i (void) const
{
  return !CORBA::is_nil (this->consumer_ec_.in ());
}

CORBA::Boolean
TAO_EC_Gateway_IIOP::consumer_ec_non_existent (
      CORBA::Boolean_out disconnected
      ACE_ENV_ARG_DECL)
{
  CORBA::Object_var consumer_ec;
  {
    ACE_GUARD_THROW_EX (
        TAO_SYNCH_MUTEX, ace_mon, this->lock_,
        CORBA::INTERNAL ());
    ACE_CHECK_RETURN (0);

    disconnected = 0;
    if (this->is_consumer_ec_connected_i () == 0)
      {
        disconnected = 1;
        return 0;
      }

    consumer_ec = CORBA::Object::_duplicate (this->consumer_ec_.in ());
  }

#if (TAO_HAS_MINIMUM_CORBA == 0)
  return consumer_ec->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
#else
  return 0;
#endif /* TAO_HAS_MINIMUM_CORBA */
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

