//
// $Id$
//

#include "orbsvcs/Event/EC_Gateway.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Time_Utilities.h"

TAO_EC_Gateway::~TAO_EC_Gateway (void)
{
}

// ****************************************************************

TAO_EC_Gateway_IIOP::TAO_EC_Gateway_IIOP (void)
  :  consumer_ (this),
     supplier_ (this)
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
			   CORBA::Environment &_env)
{
  this->rmt_ec_ = 
    RtecEventChannelAdmin::EventChannel::_duplicate (rmt_ec);
  this->lcl_ec_ = 
    RtecEventChannelAdmin::EventChannel::_duplicate (lcl_ec);

  this->rmt_info_ = 
    rmt_sched->create (rmt_name, _env);
  if (_env.exception () != 0) return;

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
		  _env);
  if (_env.exception () != 0) return;

  this->lcl_info_ =
    lcl_sched->create (lcl_name, _env);
  if (_env.exception () != 0) return;

  lcl_sched->set (this->lcl_info_,
		  RtecScheduler::VERY_HIGH_CRITICALITY,
		  time, time, time,
		  25000 * 10,
		  RtecScheduler::VERY_LOW_IMPORTANCE,
		  time,
		  1,
		  RtecScheduler::OPERATION,
		  _env);
  if (_env.exception () != 0) return;

}

void
TAO_EC_Gateway_IIOP::open (const RtecEventChannelAdmin::ConsumerQOS& sub,
			   const RtecEventChannelAdmin::SupplierQOS& pub,
			   CORBA::Environment &_env)
{
  TAO_TRY
    {
      // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Open gateway\n"));
      if (CORBA::is_nil (this->lcl_ec_.in ())
	  || CORBA::is_nil (this->rmt_ec_.in ()))
	return;

      // = Connect as a supplier to the local EC
      RtecEventChannelAdmin::SupplierAdmin_var supplier_admin =
        this->lcl_ec_->for_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumer_proxy_ =
        supplier_admin->obtain_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushSupplier_var supplier_ref =
        this->supplier_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      //ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Supplier "));
      //ACE_SupplierQOS_Factory::debug (pub);

      this->consumer_proxy_->connect_push_supplier (supplier_ref.in (),
                                                    pub,
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
        this->rmt_ec_->for_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->supplier_proxy_ =
        consumer_admin->obtain_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushConsumer_var consumer_ref =
        this->consumer_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      //ACE_DEBUG ((LM_DEBUG, "ECG (%t) Gateway/Consumer "));
      //ACE_ConsumerQOS_Factory::debug (sub);

      this->supplier_proxy_->connect_push_consumer (consumer_ref.in (),
                                                    sub,
                                                    TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("TAO_EC_Gateway_IIOP::open");
      this->consumer_proxy_ = 0;
      this->supplier_proxy_ = 0;
      
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
TAO_EC_Gateway_IIOP::close (CORBA::Environment &env)
{
  // ACE_DEBUG ((LM_DEBUG, "ECG (%t) Closing gateway\n"));
  if (CORBA::is_nil (this->consumer_proxy_.in ())
      || CORBA::is_nil (this->supplier_proxy_.in ()))
    return;

  this->consumer_proxy_->disconnect_push_consumer (env);
  if (env.exception () != 0) return;
  this->consumer_proxy_ = 0;

  this->supplier_proxy_->disconnect_push_supplier (env);
  if (env.exception () != 0) return;
  this->supplier_proxy_ = 0;
}

void
TAO_EC_Gateway_IIOP::update_consumer (RtecEventChannelAdmin::ConsumerQOS& sub,
				      RtecEventChannelAdmin::SupplierQOS& pub,
				      CORBA::Environment& env)
{
  this->close (env);
  if (env.exception () != 0) return;

  if (sub.dependencies.length () == 0
      || pub.publications.length () == 0)
    return;

  for (CORBA::ULong i = 0; i < sub.dependencies.length (); ++i)
    {
      sub.dependencies[i].rt_info = this->rmt_info_;
    }

  for (CORBA::ULong j = 0; j < pub.publications.length (); ++j)
    {
      pub.publications[j].dependency_info.rt_info = this->lcl_info_;
    }

  this->open (sub, pub, env);
}

void
TAO_EC_Gateway_IIOP::update_supplier (RtecEventChannelAdmin::ConsumerQOS& sub,
				      RtecEventChannelAdmin::SupplierQOS& pub,
				      CORBA::Environment& env)
{
  this->close (env);
  if (env.exception () != 0) return;

  for (CORBA::ULong i = 0; i < sub.dependencies.length (); ++i)
    {
      sub.dependencies[i].rt_info = this->rmt_info_;
    }

  for (CORBA::ULong j = 0; j < pub.publications.length (); ++j)
    {
      pub.publications[j].dependency_info.rt_info = this->lcl_info_;
    }

  this->open (sub, pub, env);
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
			   CORBA::Environment & _env)
{
  //ACE_DEBUG ((LM_DEBUG, "TAO_EC_Gateway_IIOP::push - "));

  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  //ACE_DEBUG ((LM_DEBUG, "ECP: %d event(s) - ", events.length ()));

  // @@ TODO, there is an extra data copy here, we should do the event
  // modification without it and only compact the necessary events.
  int count = 0;
  RtecEventComm::EventSet out (events.length ());
  for (u_int i = 0; i < events.length (); ++i)
    {
      //ACE_DEBUG ((LM_DEBUG, "type = %d ", events[i].type_));
      if (events[i].ttl_ > 0)
        {
          count++;
          out.length (count);
          out[count - 1] = events[i];
          out[count - 1].ttl_--;
        }
    }
  //ACE_DEBUG ((LM_DEBUG, "count = %d\n", count));

  if (count > 0)
    {
      this->consumer_proxy_->push (out, _env);
    }
}

int
TAO_EC_Gateway_IIOP::shutdown (CORBA::Environment& _env)
{
  this->close (_env);
  if (_env.exception () == 0) return -1;

  this->lcl_ec_ = 0;
  this->rmt_ec_ = 0;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_PushConsumer_Adapter<TAO_EC_Gateway_IIOP>;
template class ACE_PushSupplier_Adapter<TAO_EC_Gateway_IIOP>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_PushConsumer_Adapter<TAO_EC_Gateway_IIOP>
#pragma instantiate ACE_PushSupplier_Adapter<TAO_EC_Gateway_IIOP>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
