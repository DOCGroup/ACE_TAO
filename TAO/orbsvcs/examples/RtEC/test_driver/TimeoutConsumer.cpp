// $Id$

#include "TimeoutConsumer.h"

#include <sstream> //for ostringstream

#include "orbsvcs/Event_Utilities.h" //for ACE_ConsumerQOS_Factory
//#include "orbsvcs/Event_Service_Constants.h"
//#include "orbsvcs/RtecSchedulerC.h"
//#include "orbsvcs/RtecEventCommC.h"

ACE_RCSID(EC_Examples, Consumer, "$Id$")

TimeoutConsumer::TimeoutConsumer (Timeout_Observer* obs)
  : _observer(obs)
  , _consumer(this)
{
}

TimeoutConsumer::~TimeoutConsumer (void)
{
  // TODO this->disconnect() ???
}

RtecScheduler::handle_t 
TimeoutConsumer::get_RT_Info(void)
{
  return this->_rt_info;
}

void
TimeoutConsumer::connect (RtecScheduler::Scheduler_ptr scheduler,
                          const char *entry_prefix,
                          TimeBase::TimeT period,
                          RtecScheduler::Importance_t importance,
                          RtecScheduler::Criticality_t criticality,
                          RtecEventChannelAdmin::EventChannel_ptr ec
                          ACE_ENV_ARG_DECL)
{
  this->_scheduler = scheduler;
  
  //create consumer RT_Info
  std::ostringstream cons_entry_pt;
  cons_entry_pt << entry_prefix << " TimeoutConsumer"; //unique RT_Info entry point
  ACE_DEBUG((LM_DEBUG,"Creating %s\n",cons_entry_pt.str().c_str()));
  this->_rt_info = this->_scheduler->create (cons_entry_pt.str().c_str() ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  this->_scheduler->set (this->_rt_info,
			 criticality,
			 period, period, period,
			 period,
			 importance,
			 period,
			 0,
			 RtecScheduler::OPERATION
			 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Register as consumer of timeout events
  ACE_ConsumerQOS_Factory timeoutQOS;
  timeoutQOS.insert_time(ACE_ES_EVENT_INTERVAL_TIMEOUT /*??*/,
                         period, //TimeBase::TimeT
                         this->_rt_info);

  RtecEventChannelAdmin::ConsumerAdmin_var consumer_admin =
    ec->for_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->_supplier_proxy =
    consumer_admin->obtain_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  RtecEventComm::PushConsumer_var consumerv =
    this->_consumer._this (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->_supplier_proxy->connect_push_consumer (consumerv.in (),
                                                timeoutQOS.get_ConsumerQOS ()
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,"TimeoutConsumer connected as timeout consumer\n"));
  std::ostringstream prd;
  prd << period;
  ACE_DEBUG((LM_DEBUG,"\tTimeout period: %s\n",prd.str().c_str()));
}

void
TimeoutConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  //disconnect consumer
  if (! CORBA::is_nil (this->_supplier_proxy.in()))
    {
      this->_supplier_proxy->disconnect_push_supplier (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->_supplier_proxy = RtecEventChannelAdmin::ProxyPushSupplier::_nil();

      //Deactivate the servant
      PortableServer::POA_var poa =
        this->_consumer._default_POA(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
      PortableServer::ObjectId_var id =
        poa->servant_to_id (&this->_consumer ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      poa->deactivate_object(id.in() ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TimeoutConsumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  RtecScheduler::RT_Info* info = this->_scheduler->get(this->_rt_info ACE_ENV_ARG_PARAMETER);

  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"TimeoutConsumer %s (%P|%t) push but no events\n",info->entry_point.in()));
      return;
    }

  
  ACE_DEBUG((LM_DEBUG,"TimeoutConsumer %s (%P|%t) received %d events:\n",info->entry_point.in(),
             events.length()));
  for (size_t i=0; i<events.length(); ++i)
    {
      if (ACE_ES_EVENT_INTERVAL_TIMEOUT == events[i].header.type)
	{
	  ACE_DEBUG((LM_DEBUG,"TimeoutConsumer %s (%P|%t) received timeout event\n",info->entry_point.in()));
	  if (this->_observer != 0)
	    {
	      ACE_DEBUG((LM_DEBUG,"TimeoutConsumer %s (%P|%t) updating observer\n",info->entry_point.in()));
	      this->_observer->update();
	    }
	}
      else
	{
	  int prio = -1;
	  ACE_hthread_t handle;
	  ACE_Thread::self(handle);
	  ACE_Thread::getprio(handle,prio);
	  //ACE_thread_t tid = ACE_Thread::self();
	  ACE_DEBUG ((LM_DEBUG, "TimeoutConsumer %s @%d (%P|%t) we received event type %d\n",
		      info->entry_point.in(),prio,events[0].header.type));
	}
    }
}

void
TimeoutConsumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
