// $Id$

#include "Counting_Supplier.h"
#include "orbsvcs/Event_Utilities.h"

ACE_RCSID(EC_Tests, EC_Count_Supplier, "$Id$")

EC_Counting_Supplier::EC_Counting_Supplier (void)
  :  event_count (0),
     disconnect_count (0),
     consumer_adapter_ (this),
     event_source_ (-1),
     event_type_ (ACE_ES_EVENT_UNDEFINED)
{
}

void
EC_Counting_Supplier::activate (RtecEventChannelAdmin::ConsumerAdmin_ptr consumer_admin,
                    int milliseconds
                    ACE_ENV_ARG_DECL)
{
  RtecEventComm::PushConsumer_var consumer =
    this->consumer_adapter_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supplier_proxy_ =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Let's say that the execution time for event 2 is 1
  // milliseconds...
  ACE_Time_Value tv (0, milliseconds * 1000);
  TimeBase::TimeT time;
  ORBSVCS_Time::Time_Value_to_TimeT (time, tv);

  ACE_ConsumerQOS_Factory consumer_qos;
  consumer_qos.start_disjunction_group (1);
  consumer_qos.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                            time,
                            0);

  this->supplier_proxy_->connect_push_consumer (consumer.in (),
                                                consumer_qos.get_ConsumerQOS ()
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
EC_Counting_Supplier::deactivate (ACE_ENV_SINGLE_ARG_DECL)
{
  this->supplier_proxy_->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableServer::POA_var consumer_poa =
    this->consumer_adapter_._default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var consumer_id =
    consumer_poa->servant_to_id (&this->consumer_adapter_ ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  consumer_poa->deactivate_object (consumer_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
EC_Counting_Supplier::connect (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    int published_source,
    int published_type,
    int event_source,
    int event_type
    ACE_ENV_ARG_DECL)
{
  this->event_source_ = event_source;
  this->event_type_ = event_type;

  ACE_SupplierQOS_Factory supplier_qos;
  supplier_qos.insert (published_source,
                       published_type,
                       0, 1);
  this->connect (supplier_admin,
                 supplier_qos.get_SupplierQOS ()
                 ACE_ENV_ARG_PARAMETER);
}

void
EC_Counting_Supplier::connect (
    RtecEventChannelAdmin::SupplierAdmin_ptr supplier_admin,
    const RtecEventChannelAdmin::SupplierQOS &qos
    ACE_ENV_ARG_DECL)
{
  RtecEventComm::PushSupplier_var supplier =
    this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    {
      this->consumer_proxy_ =
        supplier_admin->obtain_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  this->consumer_proxy_->connect_push_supplier (supplier.in (),
                                                qos
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
EC_Counting_Supplier::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  if (!CORBA::is_nil (this->consumer_proxy_.in ()))
    {
      this->consumer_proxy_->disconnect_push_consumer (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  PortableServer::POA_var supplier_poa =
    this->_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
  PortableServer::ObjectId_var supplier_id =
    supplier_poa->servant_to_id (this ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  supplier_poa->deactivate_object (supplier_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

void
EC_Counting_Supplier::push (const RtecEventComm::EventSet&
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (this->consumer_proxy_.in ()))
    return;

  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.source = this->event_source_;
  event[0].header.type = this->event_type_;
  event[0].header.ttl = 1;

  this->consumer_proxy_->push (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->event_count++;
}

void
EC_Counting_Supplier::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
EC_Counting_Supplier::disconnect_push_supplier (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->disconnect_count++;
  this->consumer_proxy_ =
    RtecEventChannelAdmin::ProxyPushConsumer::_nil ();
}

// ****************************************************************

EC_Counting_Supplier_Task::
    EC_Counting_Supplier_Task (EC_Counting_Supplier *s)
  :  supplier_ (s),
     stop_flag_ (0),
     push_count_ (0)
{
}

int
EC_Counting_Supplier_Task::svc ()
{
  ACE_TRY_NEW_ENV
    {
      this->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

void
EC_Counting_Supplier_Task::stop (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  this->stop_flag_ = 1;
}

CORBA::ULong
EC_Counting_Supplier_Task::push_count (void)
{
  return this->push_count_;
}

void
EC_Counting_Supplier_Task::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->event_.length (1);

  int stop = 0;
  do {
    this->supplier_->push (this->event_ ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    // Sleep for a short time to avoid spinning...
    ACE_OS::sleep (0);

    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

    this->push_count_++;

    stop = this->stop_flag_;
  } while (stop == 0);
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_PushConsumer_Adapter<EC_Counting_Supplier>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_PushConsumer_Adapter<EC_Counting_Supplier>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
