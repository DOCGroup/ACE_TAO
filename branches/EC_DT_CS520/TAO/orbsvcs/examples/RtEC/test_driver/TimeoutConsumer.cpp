// $Id$

#include "TimeoutConsumer.h"

#include <sstream> //for ostringstream

#include "orbsvcs/Event_Utilities.h" //for ACE_ConsumerQOS_Factory
//#include "orbsvcs/RtecSchedulerC.h"
//#include "orbsvcs/RtecEventCommC.h"
//REACTOR CHANGE
#include "Timer_Event_Handler.h"
#include "orbsvcs/Event_Service_Constants.h" //for ORBSVCS_Time
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

ACE_RCSID(EC_Examples, Consumer, "$Id$")

TimeoutConsumer::TimeoutConsumer (Timeout_Observer* obs)
  : _observer(obs)
  , _consumer(this)
  //REACTOR CHANGE
  , _handler(0)
  //REACTOR CHANGE END
{
}

TimeoutConsumer::~TimeoutConsumer (void)
{
  this->disconnect();
}
/*
RtecScheduler::handle_t
TimeoutConsumer::get_RT_Info(void)
{
  return this->_rt_info;
}
*/
void
TimeoutConsumer::connect (RtecScheduler::Scheduler_ptr scheduler,
                          const char *entry_prefix,
                          TimeBase::TimeT period,
                          RtecScheduler::Importance_t importance,
                          RtecScheduler::Criticality_t criticality,
                          RtecEventChannelAdmin::EventChannel_ptr ec
                          ACE_ENV_ARG_DECL)
{
  /*
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
  timeoutQOS.insert_time(ACE_ES_EVENT_INTERVAL_TIMEOUT,
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
  */

  //REACTOR CHANGE
  ACE_UNUSED_ARG(entry_prefix);
  ACE_UNUSED_ARG(importance);
  ACE_UNUSED_ARG(criticality);
  ACE_UNUSED_ARG(scheduler);
  ACE_UNUSED_ARG(ec);

  this->entry_pt << entry_prefix << " TimeoutConsumer"; //unique RT_Info entry point
/*
  ACE_DEBUG((LM_DEBUG,"Creating %s\n",this->entry_pt.str().c_str()));
*/

  this->_handler = new Timer_Event_Handler(this);
  ACE_Time_Value interval;
  ORBSVCS_Time::TimeT_to_Time_Value(interval,period);
  TAO_ORB_Core *core = ec->orb_core();
  ACE_Reactor *reactor = core->reactor();
  //ACE_Reactor *reactor = ACE_Reactor::instance();
  this->_timer_id = reactor->schedule_timer(this->_handler,
                                           0,
                                           interval,
                                           interval);
  //REACTOR CHANGE END
}

void
TimeoutConsumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
{
  //disconnect consumer
  if (! CORBA::is_nil (this->_supplier_proxy.in()))
    {
      //REACTOR CHANGE
      /*
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
      */
      //REACTOR CHANGE END
    }

  //REACTOR CHANGE
  //ACE_Reactor *reactor = ACE_Reactor::instance();
  ACE_Reactor *reactor = this->_supplier_proxy->orb_core()->reactor();
  reactor->cancel_timer(this->_timer_id);
  delete this->_handler;
  this->_handler = 0;
  //REACTOR CHANGE END
}

void
TimeoutConsumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  /*
  RtecScheduler::RT_Info* info = this->_scheduler->get(this->_rt_info ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  */

  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,"TimeoutConsumer %s (%P|%t) push but no events\n",this->entry_pt.str().c_str()));
      return;
    }


/*
  ACE_DEBUG((LM_DEBUG,"TimeoutConsumer %s (%P|%t) received %d events:\n",this->entry_pt.str().c_str(),
            events.length()));
*/
  for (size_t i=0; i<events.length(); ++i)
    {
      if (ACE_ES_EVENT_INTERVAL_TIMEOUT == events[i].header.type)
        {
          //ACE_DEBUG((LM_DEBUG,"TimeoutConsumer %s (%P|%t) received timeout event\n",this->entry_pt.str().c_str()));
          if (this->_observer != 0)
            {
              ACE_DEBUG((LM_DEBUG,"TimeoutConsumer %s (%P|%t) updating observer\n",this->entry_pt.str().c_str()));
              this->_observer->update();
            }
        }
      else
        {
          int prio = -1;
          ACE_hthread_t handle;
          ACE_Thread::self(handle);
          ACE_Thread::getprio(handle,prio);
          ACE_thread_t tid = ACE_Thread::self();
          ACE_DEBUG ((LM_DEBUG, "TimeoutConsumer %s @%d (%P|%t) we received event type %d\n",
                      this->entry_pt.str().c_str(),prio,events[0].header.type));
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
