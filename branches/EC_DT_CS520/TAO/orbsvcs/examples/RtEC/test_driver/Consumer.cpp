// $Id$

#include "Consumer.h"
#include "cpuload.h"

#include <sstream> //for ostringstream

#include "ace/Thread.h"
#include "orbsvcs/Event_Utilities.h" //for ACE_Supplier/ConsumerQOS_Factory
#include "orbsvcs/RtecSchedulerC.h"

ACE_RCSID(EC_Examples, Consumer, "$Id$")

Consumer::Consumer (void)
  : _consumer(this)
  , _consumer_id(-1)
{
}

Consumer::~Consumer(void)
{
}

void
Consumer::connect (RtecScheduler::Scheduler_ptr scheduler,
                   const char *entry_prefix,
                   int consumer_id, //unique identifier
                   long event_type,
                   RtecEventChannelAdmin::EventChannel_ptr ec
                   ACE_ENV_ARG_DECL)
{
  this->connect_impl(false,
                     scheduler,
                     entry_prefix,
                     consumer_id,
                     event_type,
                     0, //period; ignored
                     RtecScheduler::VERY_LOW_IMPORTANCE, //ignored
                     RtecScheduler::VERY_LOW_CRITICALITY, //ignored
                     ec
                     ACE_ENV_ARG_PARAMETER);
}

void
Consumer::connect (RtecScheduler::Scheduler_ptr scheduler,
                   const char *entry_prefix,
                   int consumer_id, //unique identifier
                   long event_type,
                   TimeBase::TimeT period,
                   RtecScheduler::Importance_t importance,
                   RtecScheduler::Criticality_t criticality,
                   RtecEventChannelAdmin::EventChannel_ptr ec
                   ACE_ENV_ARG_DECL)
{
  this->connect_impl(true,
                     scheduler,
                     entry_prefix,
                     consumer_id,
                     event_type,
                     period,
                     importance,
                     criticality,
                     ec
                     ACE_ENV_ARG_PARAMETER);
}

void
Consumer::connect_impl (bool set_rtinfo, //true if should set RT_Info
                        RtecScheduler::Scheduler_ptr scheduler,
                        const char *entry_prefix,
                        int consumer_id, //unique identifier
                        long event_type,
                        TimeBase::TimeT period,
                        RtecScheduler::Importance_t importance,
                        RtecScheduler::Criticality_t criticality,
                        RtecEventChannelAdmin::EventChannel_ptr ec
                        ACE_ENV_ARG_DECL)
{
  ACE_UNUSED_ARG(period);
  this->_consumer_id = consumer_id;

  //create consumer RT_Info
  std::ostringstream cons_entry_pt;
  cons_entry_pt << entry_prefix; //unique RT_Info entry point
  ACE_DEBUG((LM_DEBUG,"Creating %s\n",cons_entry_pt.str().c_str()));
  ACE_DEBUG((LM_DEBUG,"\timportance: %d\tcriticality: %d\n",importance,criticality));
  RtecScheduler::handle_t rt_info = scheduler->create (cons_entry_pt.str().c_str()
                                                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  //TODO: How get period if !set_rtinfo?
  if (set_rtinfo)
    {
      //ORBSVCS_Time::TimeT_to_Time_Value(this->_work_time,period);
      //this->_work_time *= 0.5; //only work for half the period
      //TODO: How much work should we do?
      this->_work_time.set(1,0); //based on DT test, work is 1 second long

      //ignore period, since it will propagate from Supplier
      RtecScheduler::Period_t p = 0;//period;

      ACE_Time_Value tv (0,0);
      TimeBase::TimeT tmp;
      ORBSVCS_Time::Time_Value_to_TimeT (tmp, tv);
      scheduler->set (rt_info,
                      criticality,
                      tmp,tmp,tmp,
                      p,
                      importance,
                      tmp,
                      0,
                      RtecScheduler::OPERATION
                      ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
      ACE_DEBUG((LM_DEBUG,"Set Consumer %d RT_Info\n",this->_consumer_id));
      ACE_DEBUG((LM_DEBUG,"\tcriticality: %d\n",criticality));
      ACE_DEBUG((LM_DEBUG,"\tTimes (worst,typical,cached): %d, %d, %d\n",tmp,tmp,tmp));
      ACE_DEBUG((LM_DEBUG,"\tperiod: %d\n",p));
      ACE_DEBUG((LM_DEBUG,"\timportance: %d\n",importance));
      ACE_DEBUG((LM_DEBUG,"\tquantum: %d\n",tmp));
    } else
    {
      ACE_DEBUG((LM_DEBUG,"NOT Set Consumer %d RT_Info\n",this->_consumer_id));
    }

  // Register as consumer of appropriate event type
  ACE_ConsumerQOS_Factory consQoS;
  consQoS.insert_type(event_type,
                      rt_info);

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
                                                consQoS.get_ConsumerQOS ()
                                                ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG((LM_DEBUG,"Consumer %d connected\n",this->_consumer_id));
  ACE_DEBUG((LM_DEBUG,"\tEvent type: %d\n",event_type));

  //calibrate
  //TODO: calibrate seems to cause Consumer's connect_push_consumer() to hang!
  //this->_work.calibrate();
}

void
Consumer::disconnect (ACE_ENV_SINGLE_ARG_DECL)
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
      ACE_DEBUG((LM_DEBUG,"Consumer %d disconnected\n",this->_consumer_id));
    } else
    {
      ACE_DEBUG((LM_DEBUG,"Cannot disconnect; Consumer %d not connected!\n",this->_consumer_id));
    }
}

void
Consumer::push (const RtecEventComm::EventSet& events
                ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (events.length () == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Consumer (%P|%t) no events\n"));
      return;
    }
  int prio = -1;
  ACE_hthread_t handle;
  ACE_Thread::self(handle);
  ACE_Thread::getprio(handle,prio);
  //ACE_thread_t tid = ACE_Thread::self();
  ACE_DEBUG ((LM_DEBUG, "Consumer #%d @%d (%P|%t) we received event type %d\n",
              this->_consumer_id,prio,events[0].header.type));

  //@BT INSTRUMENT with event ID: EVENT_WORK_START Measure time
  //when work triggered by event starts.

  //do work
  //what happens when run w/o calibratin?
  timeval load = (timeval) this->_work_time;
  this->_work.run(load);

  //@BT INSTRUMENT with event ID: EVENT_WORK_END Measure time when
  //work triggered by event finishes.
}

void
Consumer::disconnect_push_consumer (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
