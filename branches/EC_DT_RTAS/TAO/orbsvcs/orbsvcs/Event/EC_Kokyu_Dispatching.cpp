// $Id$

#include "EC_Kokyu_Dispatching.h"
#include "EC_Event_Channel_Base.h"
#include "EC_ProxySupplier.h"
#include "EC_QOS_Info.h"

#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/RtecSchedulerC.h"
#include "tao/ORB_Constants.h"

#include "ace/Sched_Params.h"
#include "ace/Malloc_Allocator.h"

#include "Kokyu/Kokyu.h"

#if     ! defined (__ACE_INLINE__)
#include "EC_Kokyu_Dispatching.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Kokyu_Dispatching, "$Id$")

TAO_EC_Kokyu_Dispatching::TAO_EC_Kokyu_Dispatching (TAO_EC_Event_Channel_Base *ec, int sched_policy, int sched_scope)
  :allocator_ (0),
   dispatcher_ (0),
   lanes_setup_ (0),
   disp_sched_policy_ (sched_policy),
   disp_sched_scope_ (sched_scope)
{
  CORBA::Object_var tmp = ec->scheduler ();
  this->scheduler_ = RtecScheduler::Scheduler::_narrow (tmp.in ());

  //@@VS - need to revisit this - should be some other allocator
  if (this->allocator_ == 0)
    {
      this->allocator_ = ACE_Allocator::instance ();
    }
}

void
TAO_EC_Kokyu_Dispatching::activate (void)
{
  if (!lanes_setup_)
    setup_lanes ();

  this->dispatcher_->activate ();

  //ACE_DEBUG ((LM_DEBUG, "Kokyu dispatcher activated\n"));
}

void
TAO_EC_Kokyu_Dispatching::setup_lanes (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  // Query the scheduler togetConfig_Infos
  RtecScheduler::Config_Info_Set_var configs;
  ACE_TRY
    {
      this->scheduler_->get_config_infos(configs.out());
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // Ignore exceptions..
    }
  ACE_ENDTRY;

  //might be no Config_Infos in the set (if none passed to scheduler_)

  // Convert RtecScheduler::Config_Info_Set     to Kokyu::ConfigInfoSet
  // OK to assume exact correspondence betwen Config_Info and ConfigInfo?
  Kokyu::ConfigInfoSet kconfigs(configs->length());
  for(CORBA::ULong i=0; i<configs->length(); ++i) {
        kconfigs[i].preemption_priority_ = configs[i].preemption_priority;
        kconfigs[i].thread_priority_ = configs[i].thread_priority;
        switch (configs[i].dispatching_type) {
                case RtecScheduler::STATIC_DISPATCHING:
                        kconfigs[i].dispatching_type_ = Kokyu::FIFO_DISPATCHING;
                        break;
                case RtecScheduler::DEADLINE_DISPATCHING:
                        kconfigs[i].dispatching_type_ = Kokyu::DEADLINE_DISPATCHING;
                        break;
                case RtecScheduler::LAXITY_DISPATCHING:
                        kconfigs[i].dispatching_type_ = Kokyu::LAXITY_DISPATCHING;
                        break;
        }
  }

  Kokyu::Dispatcher_Attributes attrs;
  attrs.config_info_set_ = kconfigs;
  attrs.sched_policy (disp_sched_policy_);
  attrs.sched_scope (disp_sched_scope_);

  // Create Kokyu::Dispatcher using factory
  Kokyu::Dispatcher_Auto_Ptr
    tmp(Kokyu::Dispatcher_Factory::create_dispatcher(attrs));
  this->dispatcher_ = tmp;
  this->lanes_setup_ = 1;

  //ACE_DEBUG ((LM_DEBUG, "Kokyu dispatcher setup\n"));
}

void
TAO_EC_Kokyu_Dispatching::shutdown (void)
{
  this->dispatcher_->shutdown();
}

void
TAO_EC_Kokyu_Dispatching::push (TAO_EC_ProxyPushSupplier* proxy,
                                RtecEventComm::PushConsumer_ptr consumer,
                                const RtecEventComm::EventSet& event,
                                TAO_EC_QOS_Info&     qos_info
                                ACE_ENV_ARG_DECL)
{
  RtecEventComm::EventSet event_copy = event;
  this->push_nocopy (proxy, consumer, event_copy, qos_info ACE_ENV_ARG_PARAMETER);
}

void
TAO_EC_Kokyu_Dispatching::push_nocopy (TAO_EC_ProxyPushSupplier* proxy,
                                       RtecEventComm::PushConsumer_ptr consumer,
                                       RtecEventComm::EventSet& event,
                                       TAO_EC_QOS_Info& qos_info
                                       ACE_ENV_ARG_DECL)
{
  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_no_copy() ENTER\n"));

  if (this->dispatcher_.get () == 0)
    this->setup_lanes ();

  void* buf =
    this->allocator_->malloc (sizeof (TAO_EC_Kokyu_Push_Command ));

  if (buf == 0)
    ACE_THROW (CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                 CORBA::COMPLETED_NO));

  // Create Dispatch_Command
  TAO_EC_Kokyu_Push_Command *cmd =
    new (buf) TAO_EC_Kokyu_Push_Command (proxy,
                                         consumer,
                                         event, this->allocator_);

  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_nocopy() RT_Info handle: %i\n",qos_info.rt_info));
  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_nocopy() Event type: %i\n",event[0].header.type));

  // Convert TAO_EC_QOS_Info to QoSDescriptor
  RtecScheduler::RT_Info *rt_info =
    this->scheduler_->get(qos_info.rt_info);

  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_nocopy() RT_Info period: %i00nsec\n",rt_info->period));

  Kokyu::QoSDescriptor qosd;
  qosd.preemption_priority_ = rt_info->preemption_priority;
  //RT_Info period is in 100nsec, so convert to msec and set
  long msec_period = rt_info->period/10000; //1/10000 msec per nsec
  qosd.period_.msec(msec_period);
  qosd.deadline_.msec(msec_period); //assume deadline same as period
  qosd.deadline_ += ACE_OS::gettimeofday(); //deadline should be absolute!
  if (event.length() > 0) {
    //Not sure what the task_id is for, but we'll treat it as the event
    //type.  Unfortunately, we can only handle one type, but I don't
    //think the EC handles more than one anyway, so it should be OK.
    qosd.task_id_ = event[0].header.type;
  } //else we have nothing to set the task_id_ from; hopefully the consumer ignores empty event sets!
  ORBSVCS_Time::TimeT_to_Time_Value (qosd.execution_time_,
                                     rt_info->worst_case_execution_time);
  //Ugh, trivial and ugly conversion between RtecScheduler and Kokyu enums
  switch (rt_info->importance)
    {
    case RtecScheduler::VERY_LOW_IMPORTANCE:
      qosd.importance_ = Kokyu::VERY_LOW_IMPORTANCE;
      break;
    case RtecScheduler::LOW_IMPORTANCE:
      qosd.importance_ = Kokyu::LOW_IMPORTANCE;
      break;
    case RtecScheduler::MEDIUM_IMPORTANCE:
      qosd.importance_ = Kokyu::MEDIUM_IMPORTANCE;
      break;
    case RtecScheduler::HIGH_IMPORTANCE:
      qosd.importance_ = Kokyu::HIGH_IMPORTANCE;
      break;
    case RtecScheduler::VERY_HIGH_IMPORTANCE:
      qosd.importance_ = Kokyu::VERY_HIGH_IMPORTANCE;
      break;
    }

  //set event deadlines!
  for(CORBA::ULong i=0; i<event.length(); ++i)
    {
      ORBSVCS_Time::Time_Value_to_TimeT(event[i].header.deadline,
                                        qosd.deadline_);
      //ACE_DEBUG((LM_DEBUG,"EC_Kokyu_Dispatching (%P|%t) push_nocopy() Event[%d] deadline is %i\n",i,event[i].header.deadline));
    }

  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_nocopy() QoSDescriptor period: %isec %iusec\n",qosd.period_.sec(),qosd.period_.usec()));
  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_nocopy() QoSDescriptor deadline: %isec %iusec\n",qosd.deadline_.sec(),qosd.deadline_.usec()));

  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_no_copy() before dispatcher->dispatch()\n"));
  this->dispatcher_->dispatch(cmd,qosd);
  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_no_copy() after dispatcher->dispatch()\n"));

  //ACE_DEBUG((LM_DEBUG, "EC_Kokyu_Dispatching (%P|%t) push_no_copy() LEAVE\n"));
}

// ****************************************************************

TAO_EC_Kokyu_Shutdown_Command::~TAO_EC_Kokyu_Shutdown_Command(void)
{
}

int
TAO_EC_Kokyu_Shutdown_Command::execute(void)
{
  return -1;
}

// ****************************************************************

TAO_EC_Kokyu_Push_Command::~TAO_EC_Kokyu_Push_Command(void)
{
  this->proxy_->_decr_refcnt ();
}

int
TAO_EC_Kokyu_Push_Command::execute ()
{
  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      //ACE_DEBUG ((LM_DEBUG,
      //            "(%t) Command object executed.\n"));

      this->proxy_->push_to_consumer (this->consumer_.in (),
                                      this->event_
                                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)
template class ACE_Array<Kokyu::ConfigInfo>;
template class ACE_Array_Base<Kokyu::ConfigInfo>;
#  endif /* __GNUC__ */
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if (! defined (__GNUC__)) || (__GNUC__ > 2) || \
(__GNUC__ == 2 && defined (__GNUC_MINOR__) && __GNUC_MINOR__ >= 8)
#pragma instantiate ACE_Array<Kokyu::ConfigInfo>
#pragma instantiate ACE_Array_Base<Kokyu::ConfigInfo>;
#  endif /* __GNUC__ */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
