// $Id$

#include "ace/Service_Config.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Event_Utilities.h"

#include "Dispatching_Modules.h"
#include "Memory_Pools.h"
#include "EC_Gateway.h"
#include "Module_Factory.h"
#include "Event_Channel.h"

// These are to save space.
#define WRITE_GUARD ACE_ES_WRITE_GUARD
#define READ_GUARD ACE_ES_READ_GUARD

#if !defined (__ACE_INLINE__)
#include "Event_Channel.i"
#endif /* __ACE_INLINE__ */

#include "tao/Timeprobe.h"

ACE_RCSID(Event, Event_Channel, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_Event_Channel_Timeprobe_Description[] =
{
  "Preemption_Priority - priority requested",
  "connected - priority obtained",
  "enter Push_Supplier_Proxy::push",
  "enter ES_Consumer_Module::push",
  "leave ES_Consumer_Module::push",
  "enter ACE_ES_Correlation_Module::push",
  "pushed to Correlation_Module",
  "push_source_type: Dispatch Module enqueuing",
  "ACE_ES_Consumer_Correlation::push, enter",
  "Consumer_Correlation::push, determine NO CORR.",
  "Consumer_Correlation::push, NO_CORR: alloc",
  "Consumer_Rep_Timeout::execute",
  "deliver to Subscription Module",
  "begin push_source_type",
  "end push_source_type",
  "deliver to Supplier Module (thru Supplier Proxy)",
  "connected - priority requested",
  "ES_Priority_Queue - start execute",
  "ES_Priority_Queue - end execute",
  "Consumer_Name - priority requested",
  "Consumer_Name - priority obtained",
  "deliver event to consumer proxy",
  "enter ACE_ES_Subscription_Module::push",
  "push_source_type"
};

enum
{
  // Timeprobe description table start key
  TAO_EVENT_CHANNEL_PREEMPTION_PRIORITY_PRIORITY_REQUESTED = 5100,
  TAO_EVENT_CHANNEL_CONNECTED_PRIORITY_OBTAINED,
  TAO_EVENT_CHANNEL_ENTER_PUSH_SUPPLIER_PROXY_PUSH,
  TAO_EVENT_CHANNEL_ENTER_ES_CONSUMER_MODULE_PUSH,
  TAO_EVENT_CHANNEL_LEAVE_ES_CONSUMER_MODULE_PUSH,
  TAO_EVENT_CHANNEL_ENTER_ACE_ES_CORRELATION_MODULE_PUSH,
  TAO_EVENT_CHANNEL_PUSHED_TO_CORRELATION_MODULE,
  TAO_EVENT_CHANNEL_PUSH_SOURCE_TYPE_DISPATCH_MODULE_ENQUEUING,
  TAO_EVENT_CHANNEL_ACE_ES_CONSUMER_CORRELATION_PUSH_ENTER,
  TAO_EVENT_CHANNEL_CONSUMER_CORRELATION_PUSH_DETERMINE_NO_CORR,
  TAO_EVENT_CHANNEL_CONSUMER_CORRELATION_PUSH_NO_CORR_ALLOC,
  TAO_EVENT_CHANNEL_CONSUMER_REP_TIMEOUT_EXECUTE,
  TAO_EVENT_CHANNEL_DELIVER_TO_SUBSCRIPTION_MODULE,
  TAO_EVENT_CHANNEL_BEGIN_PUSH_SOURCE_TYPE,
  TAO_EVENT_CHANNEL_END_PUSH_SOURCE_TYPE,
  TAO_EVENT_CHANNEL_DELIVER_TO_SUPPLIER_MODULE_THRU_SUPPLIER_PROXY,
  TAO_EVENT_CHANNEL_CONNECTED_PRIORITY_REQUESTED,
  TAO_EVENT_CHANNEL_ES_PRIORITY_QUEUE_START_EXECUTE,
  TAO_EVENT_CHANNEL_ES_PRIORITY_QUEUE_END_EXECUTE,
  TAO_EVENT_CHANNEL_CONSUMER_NAME_PRIORITY_REQUESTED,
  TAO_EVENT_CHANNEL_CONSUMER_NAME_PRIORITY_OBTAINED,
  TAO_EVENT_CHANNEL_DELIVER_EVENT_TO_CONSUMER_PROXY,
  TAO_EVENT_CHANNEL_ENTER_ACE_ES_SUBSCRIPTION_MODULE_PUSH,
  TAO_EVENT_CHANNEL_PUSH_SOURCE_TYPE
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_Event_Channel_Timeprobe_Description,
                                  TAO_EVENT_CHANNEL_PREEMPTION_PRIORITY_PRIORITY_REQUESTED);

#endif /* ACE_ENABLE_TIMEPROBES */

// ************************************************************

static RtecScheduler::OS_Priority
Preemption_Priority (RtecScheduler::handle_t rtinfo)
{
  RtecScheduler::OS_Priority thread_priority;
  RtecScheduler::Preemption_Subpriority subpriority;
  RtecScheduler::Preemption_Priority preemption_priority;

  TAO_TRY
    {
      ACE_TIMEPROBE (TAO_EVENT_CHANNEL_PREEMPTION_PRIORITY_PRIORITY_REQUESTED);
      ACE_Scheduler_Factory::server ()->priority
        (rtinfo,
         thread_priority,
         subpriority,
         preemption_priority,
         TAO_TRY_ENV);
      TAO_CHECK_ENV
      ACE_TIMEPROBE (TAO_EVENT_CHANNEL_CONNECTED_PRIORITY_OBTAINED);
    }
  TAO_CATCH (RtecScheduler::UNKNOWN_TASK, ex_ut)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "UNKNOWN_TASK %p.\n",
                         "Preemption_Priority"), 0);
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Unexpected exception %p.\n",
                         "Preemption_Priority"), 0);

    }
  TAO_ENDTRY;
  return preemption_priority;
}

static RtecScheduler::OS_Priority
IntervalToPriority (RtecScheduler::Time interval)
{
  for (int x=0; x < ACE_Scheduler_MAX_PRIORITIES; x++)
    if (interval <= ACE_Scheduler_Rates[x])
      return x;

  return ACE_Scheduler_MIN_PREEMPTION_PRIORITY;
}

// ************************************************************

class TAO_ORBSVCS_Export Shutdown_Consumer : public ACE_ES_Dispatch_Request
// = TITLE
//    Shutdown Consumer command
//
// = DESCRIPTION
//    This command object is sent through the system when a consumer
//    disconnects.  When the Dispatching Module dequeues this request,
//    it calls execute which execute calls back to the Consumer
//    Module.  At that point, the Consumer Module can tell the rest of
//    the system that the consumer has disconnected and delete the
//    consumer proxy.  This allows all events queued for the consumer
//    to be flushed to the consumer proxy (which will drop them).
//    Events can be queued in the ReactorEx (in a dispatch set), or in
//    the Dispatching Module.
{
public:
  // When executed, tells <consumer_module> that <consumer> has shut
  // down.
  Shutdown_Consumer (ACE_ES_Consumer_Module *consumer_module,
                     ACE_Push_Consumer_Proxy *consumer)
    : consumer_module_ (consumer_module)
    {
      consumer_ = consumer;

      // Set rt_info_ to the lowest priority rt_info in consumer_.
      // This is so the dispatching module can query us as a dispatch
      // request to get the appropriate preemption priority.
      ACE_ES_Dependency_Iterator iter (consumer->qos ().dependencies);
      while (iter.advance_dependency () == 0)
        {
          RtecEventComm::EventType &type = (*iter).event.type_;
          if (type != ACE_ES_GLOBAL_DESIGNATOR &&
              type != ACE_ES_CONJUNCTION_DESIGNATOR &&
              type != ACE_ES_DISJUNCTION_DESIGNATOR)
            {
              if (rt_info_ == 0 ||
                  ::Preemption_Priority ((*iter).rt_info) <
                  ::Preemption_Priority (rt_info_))
                rt_info_ = (*iter).rt_info;
            }
        }
    }

  // Report to the consumer module that consumer_ has shutdown.
  virtual int execute (u_long &command_action)
    {
      consumer_module_->shutdown_request (this);
      command_action = ACE_RT_Task_Command::RELEASE;
      return 0;
    }

  void *operator new (size_t /* nbytes */)
    { return ::new char[sizeof (Shutdown_Consumer)]; }

  void operator delete (void *buf)
    { ::delete [] ACE_static_cast(char*,buf); }

  // The module that we report to.
  ACE_ES_Consumer_Module *consumer_module_;
};

// ************************************************************

class TAO_ORBSVCS_Export Shutdown_Channel : public ACE_ES_Dispatch_Request
{
public:
  Shutdown_Channel (ACE_EventChannel *channel) :
    channel_ (channel) {}

  // Report to the consumer module that consumer_ has shutdown.
  virtual int execute (u_long &command_action)
    {
#if 0
      channel_->destroy_i ();
#endif
      command_action = ACE_RT_Task_Command::RELEASE;
      return 0;
    }

  void *operator new (size_t /* nbytes */)
    { return ::new char[sizeof (Shutdown_Channel)]; }

  void operator delete (void *buf)
    { ::delete [] ACE_static_cast(char*,buf); }

  ACE_EventChannel *channel_;
};

// ************************************************************

class TAO_ORBSVCS_Export Flush_Queue_ACT : public ACE_ES_Timer_ACT
// = TITLE
//    Flush Queue Asynchronous Completion Token
//
// = DESCRIPTION
//    Carries a single dispatch request through the ReactorEx.
//    Deletes itself when execute is called.
{
public:
  Flush_Queue_ACT (ACE_ES_Dispatch_Request *request,
                   ACE_ES_Dispatching_Module *dispatching_module) :
    request_ (request),
    dispatching_module_ (dispatching_module) { }

  virtual void execute (void)
    {
      TAO_TRY
        {
          ACE_ES_Dispatch_Request *request = request_;
          dispatching_module_->push (request, TAO_TRY_ENV);
          TAO_CHECK_ENV;
          delete this;
        }
      TAO_CATCHANY
        {
          ACE_ERROR ((LM_ERROR, "(%t) Flush_Queue_ACT::execute: "
                      "Unknown exception..\n"));
        }
      TAO_ENDTRY;
    }

  ACE_ES_Dispatch_Request *request_;
  ACE_ES_Dispatching_Module *dispatching_module_;
};

// ************************************************************

// Since this class is *defined* in the cpp file, the INLINE
// definitions must also be in the cpp file.   The should go here
// before any use of these methods.

ACE_INLINE int
ACE_ES_Priority_Timer::schedule_timer (RtecScheduler::handle_t rt_info,
                                       const ACE_ES_Timer_ACT *act,
                                       RtecScheduler::OS_Priority preemption_priority,
                                       const RtecScheduler::Time &delta,
                                       const RtecScheduler::Time &interval)
{
  if (rt_info != 0)
    {
      // Add the timer to the task's dependency list.
      RtecScheduler::handle_t timer_rtinfo =
        this->task_manager_->GetReactorTask (preemption_priority)->rt_info ();

      TAO_TRY
        {
          ACE_Scheduler_Factory::server()->add_dependency
            (rt_info, timer_rtinfo, 1, RtecScheduler::ONE_WAY_CALL, TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
      TAO_CATCHANY
        {
          ACE_ERROR ((LM_ERROR, "add dependency failed"));
        }
      TAO_ENDTRY;
    }

  // @@ We're losing resolution here.
  ACE_Time_Value tv_delta;
  ORBSVCS_Time::TimeT_to_Time_Value (tv_delta, delta);

  ACE_Time_Value tv_interval;
  ORBSVCS_Time::TimeT_to_Time_Value (tv_interval, interval);

  return this->task_manager_->GetReactorTask (preemption_priority)->
    get_reactor ().schedule_timer (this,
                                   (void *) act,
                                   tv_delta, tv_interval);
}

ACE_INLINE int
ACE_ES_Priority_Timer::cancel_timer (RtecScheduler::OS_Priority preemption_priority,
                                     int id, ACE_ES_Timer_ACT *&act)
{
  const void *vp;

  int result = this->task_manager_->
    GetReactorTask (preemption_priority)->
    get_reactor ().cancel_timer (id, &vp);

  if (result == 0)
    {
      ACE_ERROR ((LM_ERROR, "ACE_ES_Priority_Timer::cancel_timer: "
                  "Tried to cancel nonexistent timer.\n"));
      act = 0;
    }
  else
    act = (ACE_ES_Timer_ACT *) vp;

  return result;
}

// ************************************************************

ACE_ES_Event_Container::ACE_ES_Event_Container (void) :
  //  ACE_ES_Event (),
  ref_count_ (1)
{
}

ACE_ES_Event_Container::~ACE_ES_Event_Container (void)
{
}

ACE_ES_Event_Container::ACE_ES_Event_Container (const ACE_ES_Event_Container &ec)
 : RtecEventComm_Event (ec),
   ref_count_ (1)
{
}

ACE_ES_Event_Container::ACE_ES_Event_Container (const RtecEventComm_Event &e)
  : RtecEventComm_Event (e),
    ref_count_ (1)
{
}

ACE_ES_Event_Container *
ACE_ES_Event_Container::_duplicate (void)
{
  ref_count_++;
  return this;
}

void
ACE_ES_Event_Container::_release (void)
{
  if (--ref_count_ == 0)
    delete this;
}

int
ACE_ES_Event_Container::operator== (const ACE_ES_Event_Container &event)
{
  RtecEventComm::Event &event1 = (RtecEventComm::Event &) *this;
  RtecEventComm::Event &event2 = (RtecEventComm::Event &) event;
  return event1 == event2;
}

void *
ACE_ES_Event_Container::operator new (size_t nbytes)
{
  ACE_ASSERT (nbytes <= sizeof (ACE_ES_Event_Container));
  return ACE_ES_Memory_Pools::new_Event_Container ();
}

void
ACE_ES_Event_Container::operator delete (void *mem)
{
  ACE_ES_Memory_Pools::delete_Event_Container (mem);
}

void
ACE_ES_Event_Container::dump (void)
{
  ::dump_event ((RtecEventComm::Event &) *this);
}

// ************************************************************

ACE_Push_Supplier_Proxy::ACE_Push_Supplier_Proxy (ACE_ES_Supplier_Module *sm)
  : supplier_module_ (sm),
    push_supplier_ (0)
{
}

void
ACE_Push_Supplier_Proxy::connect_push_supplier (RtecEventComm::PushSupplier_ptr push_supplier,
                                                const RtecEventChannelAdmin::SupplierQOS &qos,
                                                CORBA::Environment &_env)
{
  if (this->connected ())
    TAO_THROW (RtecEventChannelAdmin::AlreadyConnected);

  this->push_supplier_ =
    RtecEventComm::PushSupplier::_duplicate(push_supplier);

  //ACE_DEBUG ((LM_DEBUG, "EC (%t) connect_push_supplier QOS is "));
  //ACE_SupplierQOS_Factory::debug (qos);

  // Copy by value.
  this->qos_ = qos;

  // ACE_SupplierQOS_Factory::debug (qos_);

  // @@ TODO: The SupplierQOS should have a more reasonable interface to
  // obtain the supplier_id(), BTW, a callback to push_supplier will
  // not work: it usually results in some form of dead-lock.
  this->source_id_ = qos_.publications[0].event.source_;

  supplier_module_->connected (this, _env);
}

void
ACE_Push_Supplier_Proxy::push (const RtecEventComm::EventSet &event,
                               CORBA::Environment &_env)
{
  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_ENTER_PUSH_SUPPLIER_PROXY_PUSH);

  ACE_hrtime_t ec_recv = ACE_OS::gethrtime ();
  for (CORBA::ULong i = 0; i < event.length (); ++i)
    {
      RtecEventComm::Event& ev =
        ACE_const_cast(RtecEventComm::Event&,event[i]);
      ORBSVCS_Time::hrtime_to_TimeT (ev.ec_recv_time_, ec_recv);
    }
  supplier_module_->push (this, event, _env);
}

void
ACE_Push_Supplier_Proxy::disconnect_push_consumer (CORBA::Environment &_env)
{
  ACE_TIMEPROBE_PRINT;
  if (this->connected ())
    {
      supplier_module_->disconnecting (this, _env);
      push_supplier_ = 0;
    }
}

void
ACE_Push_Supplier_Proxy::shutdown (void)
{
  TAO_TRY
    {
      push_supplier_->disconnect_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "ACE_Push_Supplier_Proxy::shutdown failed.\n"));
    }
  TAO_ENDTRY;
}

// ************************************************************

ACE_Push_Consumer_Proxy::ACE_Push_Consumer_Proxy (ACE_ES_Consumer_Module *cm)
  : push_consumer_ (0),
    consumer_module_ (cm)
{
}

ACE_Push_Consumer_Proxy::~ACE_Push_Consumer_Proxy (void)
{
}

void
ACE_Push_Consumer_Proxy::push (const RtecEventComm::EventSet &events,
                               CORBA::Environment &_env)
{
  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_DELIVER_EVENT_TO_CONSUMER_PROXY);

  if (CORBA::is_nil (push_consumer_.in ()))
    {
      ACE_DEBUG ((LM_DEBUG,
                  "EC (%t) Push to disconnected consumer %s\n",
                  ::ACE_ES_Consumer_Name (this->qos ())));
      // ACE_ES_DEBUG_ST (::dump_sequence (events));
      return;
    }

  TAO_TRY
    {
      push_consumer_->push (events, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (CORBA::SystemException, se)
    {
      ACE_ERROR ((LM_ERROR, "system exception.\n"));
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
ACE_Push_Consumer_Proxy::connect_push_consumer (RtecEventComm::PushConsumer_ptr push_consumer,
                                                const RtecEventChannelAdmin::ConsumerQOS &qos,
                                                CORBA::Environment &_env)
{
  if (this->connected ())
    TAO_THROW (RtecEventChannelAdmin::AlreadyConnected);

  this->push_consumer_ =
    RtecEventComm::PushConsumer::_duplicate(push_consumer);
  // @@ TODO Find out why are two duplicates needed...
  RtecEventComm::PushConsumer::_duplicate(push_consumer);

  //ACE_DEBUG ((LM_DEBUG, "EC (%t) connect_push_consumer QOS is "));
  //ACE_ConsumerQOS_Factory::debug (qos);

  // Copy by value.
  this->qos_ = qos;

  // ACE_ConsumerQOS_Factory::debug (qos_);

  this->consumer_module_->connected (this, _env);
}

void
ACE_Push_Consumer_Proxy::disconnect_push_supplier (CORBA::Environment &_env)
{
  ACE_TIMEPROBE_PRINT;
  this->push_consumer_ = 0;
  this->consumer_module_->disconnecting (this, _env);
}

void
ACE_Push_Consumer_Proxy::suspend_connection (CORBA::Environment &)
{
  correlation_.suspend ();
}

void
ACE_Push_Consumer_Proxy::resume_connection (CORBA::Environment &)
{
  correlation_.resume ();
}

void
ACE_Push_Consumer_Proxy::shutdown (void)
{
  TAO_TRY
    {
      this->push_consumer_->disconnect_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "ACE_Push_Consumer_Proxy::shutdown failed.\n"));
    }
  TAO_ENDTRY;
}

// ************************************************************

ACE_EventChannel::ACE_EventChannel (CORBA::Boolean activate_threads,
                                    u_long type,
                                    TAO_Module_Factory* factory)
  : rtu_manager_ (0),
    type_ (type),
    state_ (INITIAL_STATE),
    destroyed_ (0),
    own_factory_ (0),
    module_factory_ (factory)
{
  if (this->module_factory_ == 0)
    {
      this->own_factory_ = 1;
      ACE_NEW (this->module_factory_, TAO_Default_Module_Factory);
    }

  consumer_module_ =
    this->module_factory_->create_consumer_module (this);

  this->task_manager_ =
    this->module_factory_->create_task_manager (this);

  this->dispatching_module_ =
    this->module_factory_->create_dispatching_module(this);

  this->correlation_module_ =
    this->module_factory_->create_correlation_module (this);
  this->subscription_module_ =
    this->module_factory_->create_subscription_module (this);
  this->supplier_module_ =
    this->module_factory_->create_supplier_module (this);
  this->timer_ =
    this->module_factory_->create_timer_module (this);

  consumer_module_->open (dispatching_module_);
  dispatching_module_->open (consumer_module_, correlation_module_);
  correlation_module_->open (dispatching_module_, subscription_module_);
  subscription_module_->open (correlation_module_, supplier_module_);
  supplier_module_->open (subscription_module_);

  if (activate_threads)
    this->activate ();
}

ACE_EventChannel::~ACE_EventChannel (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "EC (%t) ACE_EventChannel deleting all modules.\n"));

  TAO_TRY
    {
      this->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_EventChannel::~ACE_EventChannel"));
    }
  TAO_ENDTRY;

  this->dispatching_module_->shutdown ();
  this->task_manager_->shutdown ();


  this->module_factory_->destroy_timer_module (this->timer_);
  this->module_factory_->destroy_supplier_module (this->supplier_module_);
  this->module_factory_->destroy_subscription_module (this->subscription_module_);
  this->module_factory_->destroy_correlation_module (this->correlation_module_);
  this->module_factory_->destroy_dispatching_module(this->dispatching_module_);
  this->module_factory_->destroy_task_manager (this->task_manager_);
  this->module_factory_->destroy_consumer_module (this->consumer_module_);

  if (this->own_factory_)
    delete this->module_factory_;
}

void
ACE_EventChannel::destroy (CORBA::Environment &)
{
  TAO_ORB_Core_instance ()->orb ()->shutdown ();

  ACE_ES_GUARD ace_mon (lock_);
  if (ace_mon.locked () == 0)
    ACE_ERROR ((LM_ERROR, "ACE_EventChannel::destroy"));

  if (destroyed_ != 0)
    return;

  destroyed_ = 1;
  ACE_DEBUG ((LM_DEBUG, "EC (%t) Event Channel shutting down.\n"));

  // Send a shutdown message through the modules.
  supplier_module_->shutdown ();

#if 0
  // Flush all messages in the channel.
  Shutdown_Channel *sc = new Shutdown_Channel (this);
  if (sc == 0)
    TAO_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

  // Create a wrapper around the dispatch request.
  Flush_Queue_ACT *act = new Flush_Queue_ACT (sc, dispatching_module_);
  if (act == 0)
    TAO_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

  // Set a 100ns timer.
  if (this->timer ()->schedule_timer (0, // no rt-info
                                      act,
                                      ACE_Scheduler_MIN_PREEMPTION_PRIORITY,
                                      100, // 10 usec delta
                                      0) == -1) // no interval
    {
      ACE_ERROR ((LM_ERROR, "%p queue_request failed.\n", "ACE_ES_Consumer_Module"));
      delete sc;
      delete act;
    }
#endif
}

void
ACE_EventChannel::activate (void)
{
  this->dispatching_module_->activate (THREADS_PER_DISPATCH_QUEUE);
  this->task_manager_->activate ();
}

void
ACE_EventChannel::shutdown (void)
{
  // @@ TODO: Find a portable way to shutdown the ORB, on Orbix we have
  // to call deactive_impl () on a CORBA::POA is that the portable
  // way?
  // With TAO we need access to the ORB (to call shutdown() on it).
  this->task_manager_->shutdown ();
  this->dispatching_module_->shutdown ();
}

void
ACE_EventChannel::report_connect (u_long event)
{
  ACE_ES_GUARD ace_mon (lock_);
  if (ace_mon.locked () == 0)
    ACE_ERROR ((LM_ERROR, "ACE_EventChannel::report_connect"));

  this->report_connect_i (event);
}

void
ACE_EventChannel::report_connect_i (u_long event)
{
  ACE_CLR_BITS (state_, event);
}

void
ACE_EventChannel::report_disconnect (u_long event)
{
  // No need to gtrab the lock is already take by our callers.
  ACE_ES_GUARD ace_mon (lock_);
  if (ace_mon.locked () == 0)
    ACE_ERROR ((LM_ERROR, "ACE_EventChannel::report_disconnect"));

  this->report_disconnect (event);
}

void
ACE_EventChannel::report_disconnect_i (u_long event)
{
  ACE_SET_BITS (state_, event);
  if (state_ == SHUTDOWN)
    ACE_DEBUG ((LM_DEBUG,
                "EC (%t) Event Channel has no consumers or suppliers.\n"));
}

void
ACE_EventChannel::add_gateway (TAO_EC_Gateway* gw,
                               CORBA::Environment& _env)
{
  this->gwys_.insert (gw);

  RtecEventChannelAdmin::ConsumerQOS c_qos;
  RtecEventChannelAdmin::SupplierQOS s_qos;

  this->consumer_module_->fill_qos (c_qos, s_qos);
  gw->update_consumer (c_qos, s_qos, _env);
  if (_env.exception () != 0) return;

  this->supplier_module_->fill_qos (c_qos, s_qos);
  gw->update_supplier (c_qos, s_qos, _env);
  if (_env.exception () != 0) return;
}

void
ACE_EventChannel::del_gateway (TAO_EC_Gateway* gw,
                               CORBA::Environment&)
{
  this->gwys_.remove (gw);
}

void
ACE_EventChannel::update_consumer_gwys (CORBA::Environment& _env)
{
  if (this->gwys_.is_empty ())
    return;

  ACE_DEBUG ((LM_DEBUG,
              "EC (%t) Event_Channel::update_consumer_gwys\n"));

  RtecEventChannelAdmin::ConsumerQOS c_qos;
  RtecEventChannelAdmin::SupplierQOS s_qos;
  this->consumer_module_->fill_qos (c_qos, s_qos);
  for (Gateway_Set_Iterator i = this->gwys_.begin ();
       i != this->gwys_.end ();
       ++i)
    {
      TAO_EC_Gateway* gw = *i;
      gw->update_consumer (c_qos, s_qos, _env);
      if (_env.exception () != 0) return;
    }
}

void
ACE_EventChannel::update_supplier_gwys (CORBA::Environment& _env)
{
  if (this->gwys_.is_empty ())
    return;

  ACE_DEBUG ((LM_DEBUG,
              "EC (%t) Event_Channel::update_supplier_gwys\n"));

  RtecEventChannelAdmin::ConsumerQOS c_qos;
  RtecEventChannelAdmin::SupplierQOS s_qos;
  this->supplier_module_->fill_qos (c_qos, s_qos);
  for (Gateway_Set_Iterator i = this->gwys_.begin ();
       i != this->gwys_.end ();
       ++i)
    {
      TAO_EC_Gateway* gw = *i;
      gw->update_supplier (c_qos, s_qos, _env);
      if (_env.exception () != 0) return;
    }
}

// ****************************************************************

ACE_ES_Disjunction_Group::~ACE_ES_Disjunction_Group (void)
{
}

ACE_ES_Conjunction_Group::~ACE_ES_Conjunction_Group (void)
{
}

// ************************************************************

ACE_ES_Subscription_Info::~ACE_ES_Subscription_Info (void)
{
  Subscriber_Map_Iterator iter (type_subscribers_);

  // Delete all type collections.
  for (Subscriber_Map_Entry *temp = 0;
       iter.next (temp) != 0;
       iter.advance ())
    {
      delete temp->int_id_;
    }
}

/*
void
ACE_ES_Subscription_Info::Type_Subscribers::operator=
(const ACE_ES_Subscription_Info::Type_Subscribers &rhs)
{
  ACE_ES_Subscription_Info::Subscriber_Set_Iterator iter (rhs.subscribers_);

  for (ACE_ES_Consumer_Rep **consumer = 0;
       iter.next (consumer) != 0;
       iter.advance ())
    {
      if (subscribers_.insert (consumer) != 0)
        ACE_ERROR ((LM_ERROR, "%p insert failed.\n",
                    "ACE_ES_Subscription_Info::Type_Subscribers::operator="));
    }

  // Pointer copy.
  dependency_info_ = rhs.dependency_info_;
}
*/

// Remove <consumer> from the consumer set in <type_map> set
// corresponding to <type>.
int
ACE_ES_Subscription_Info::remove (Subscriber_Map &type_map,
                                  ACE_ES_Consumer_Rep *consumer,
                                  RtecEventComm::EventType type)
{
  Type_Subscribers *subscribers;

  // Find the type set within the type collection.
  if (type_map.find (type, subscribers) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "EC (%t) Info::remove - not found %d\n", type));
      // type_map does not contain the type.
      return -1;
    }

  // Remove the consumer from the type set.
  if (subscribers->consumers_.remove (consumer) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p remove failed.\n",
                       "ACE_ES_Subscriber_Info::remove"), -1);
  // @@ Should probably remove the supplier from the consumers caller
  // list.

  // @@ Should we release here? consumer->_release ();

#if 0
  // If the set is empty, remove it from the type collection.
  // NOT!!!! In some cases the map is initialized to the types that a
  // certain supplier export; removing an entry from the map renders
  // that supplier unable to send that event type.
  // Before changing this ask me (coryan).
  if (subscribers->consumers_.size () == 0)
    {
      Type_Subscribers *removed_subscribers;
      if (type_map.unbind (type, removed_subscribers) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p unbind failed.\n",
                           "ACE_ES_Subscriber_Info::remove"), -1);

      // Sanity check.
      if (removed_subscribers != subscribers)
        ACE_ERROR_RETURN ((LM_ERROR, "ACE_ES_Subscriber_Info::remove: "
                           "removed wrong set!\n"), -1);

      // Free up the set removed.
      delete removed_subscribers;
    }
#endif /* 0 */

  return 0;
}


int
ACE_ES_Subscription_Info::remove (SourceID_Map &source_subscribers,
                                  ACE_ES_Consumer_Rep *consumer,
                                  RtecEventComm::EventSourceID sid)
{
  Subscriber_Set *subscribers;

  // Find the subscribers of <sid>.
  if (source_subscribers.find (sid, subscribers) == -1)
    // does not contain the <sid>.
    return -1;

  // Remove the consumer from the subscriber set.
  if (subscribers->remove (consumer) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p remove failed.\n",
                       "ACE_ES_Subscriber_Info::remove"), -1);

  // @@ Should we release here? consumer->_release ();

  // @@ Should probably remove the supplier from the consumers caller
  // list.

  // If the set is empty, remove it from the type collection.
#if 0
  if (subscribers->size () == 0)
    {
      Subscriber_Set *removed_subscribers;
      if (source_subscribers.unbind (sid, removed_subscribers) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p unbind failed.\n",
                           "ACE_ES_Subscriber_Info::remove"), -1);

      // Sanity check.
      if (removed_subscribers != subscribers)
        ACE_ERROR_RETURN ((LM_ERROR, "ACE_ES_Subscriber_Info::remove: "
                           "removed wrong set!\n"), -1);

      // Free up the set removed.
      delete removed_subscribers;
    }
#endif /* 0 */

  return 0;
}


void
ACE_ES_Subscription_Info::append_subscribers (Subscriber_Set &dest,
                                              Subscriber_Set &src)
{
  Subscriber_Set_Iterator src_iter (src);

  // Iterate through the source set.  Add each source proxy to the
  // destination set.
  for (ACE_ES_Consumer_Rep **proxy = 0;
       src_iter.next (proxy) != 0;
       src_iter.advance ())
    {
      if (dest.insert (*proxy) == -1)
        ACE_ERROR ((LM_ERROR, "%p: insert failed.\n", "append_subscribers"));
    }
}

int
ACE_ES_Subscription_Info::insert_or_allocate (SourceID_Map &sid_map,
                                              ACE_ES_Consumer_Rep *consumer,
                                              RtecEventComm::EventSourceID sid)
{
  Subscriber_Set *subscribers;

  if (sid_map.find (sid, subscribers) == -1)
    {
      // If the correct type set does not exist, make one with a null
      // dependency info (since there is no supplier of this event).
      subscribers = new Subscriber_Set;

      if (sid_map.bind (sid, subscribers) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p bind failed.\n",
                      "ACE_ES_Subscription_Info::insert_or_allocate"));
          delete subscribers;
          return -1;
        }
    }

  // 0 and 1 are success for insert.
  if (subscribers->insert (consumer) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p insert failed.\n",
                       "ACE_ES_Subscription_Info::insert_or_allocate"),
                      -1);

  consumer->_duplicate ();
  return 0;
}

int
ACE_ES_Subscription_Info::insert_or_allocate (Subscriber_Map &type_map,
                                              ACE_ES_Consumer_Rep *consumer,
                                              RtecEventComm::EventType type)
{
  Type_Subscribers *subscribers;

  if (type_map.find (type, subscribers) == -1)
    {
      // If the correct type set does not exist, make one with a null
      // dependency info (since there is no supplier of this event).
      subscribers = new Type_Subscribers (0);

      if (type_map.bind (type, subscribers) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p bind failed.\n",
                      "ACE_ES_Subscription_Info::insert_or_allocate"));
          delete subscribers;
          return -1;
        }
    }

  if (subscribers->consumers_.insert (consumer) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p insert failed.\n",
                  "ACE_ES_Subscription_Info::insert_or_allocate"));
    }

  consumer->_duplicate ();
  return 0;
}

int
ACE_ES_Subscription_Info::insert_or_fail (Subscriber_Map &type_map,
                                          ACE_ES_Consumer_Rep *consumer,
                                          RtecEventComm::EventType type,
                                          RtecScheduler::Dependency_Info *&dependency)
{
  Type_Subscribers *subscribers;

  // Get the subscriber set for <type>.
  if (type_map.find (type, subscribers) == -1)
    return -1;

  // Pass back the description of the method generating <type>.
  dependency = subscribers->dependency_info_;

  // Insert the new consumer into the subscriber set.
  if (subscribers->consumers_.insert (consumer) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p insert failed.\n",
                         "ACE_ES_Subscription_Info::insert_or_fail"),
                        -1);
    }

  consumer->_duplicate ();
  return 0;
}

// ************************************************************

ACE_ES_Consumer_Module::ACE_ES_Consumer_Module (ACE_EventChannel* channel)
  :  lock_ (),
     all_consumers_ (),
     channel_ (channel),
     down_ (0)
{
}

void
ACE_ES_Consumer_Module::open (ACE_ES_Dispatching_Module *down)
{
  down_ = down;
}

void
ACE_ES_Consumer_Module::connected (ACE_Push_Consumer_Proxy *consumer,
                                   CORBA::Environment &_env)
{
  // ACE_DEBUG ((LM_DEBUG,
  //             "EC (%t) Consumer_Module - connecting consumer %x\n",
  //  consumer));

  this->channel_->report_connect (ACE_EventChannel::CONSUMER);
  this->down_->connected (consumer, _env);
  if (_env.exception () != 0) return;
  if (!consumer->qos ().is_gateway)
    this->channel_->update_consumer_gwys (_env);
}

void
ACE_ES_Consumer_Module::shutdown_request (ACE_ES_Dispatch_Request *request)
{
  TAO_TRY
    {
      Shutdown_Consumer *sc = (Shutdown_Consumer *) request;

      // Tell everyone else that the consumer is disconnected.  This means
      // that *nothing* is left in the system for the consumer, so
      // everyone can free up any resources.
      this->down_->disconnected (sc->consumer ());

      // ACE_DEBUG ((LM_DEBUG,
      //             "EC (%t) Consumer_Module - remove consumer %x\n",
      //  sc->consumer ()));

      CORBA::Boolean dont_update = sc->consumer ()->qos ().is_gateway;
      // Delete the consumer proxy.
      delete sc->consumer ();

      if (!dont_update)
        this->channel_->update_consumer_gwys (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_ES_GUARD ace_mon (lock_);
      if (ace_mon.locked () == 0)
        return;

      // Tell the channel that we may need to shut down.
      if (all_consumers_.size () <= 0)
        {
          // ACE_DEBUG ((LM_DEBUG,
          //             "EC (%t) No more consumers connected.\n"));
          channel_->report_disconnect_i (ACE_EventChannel::CONSUMER);
        }
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Consumer_Module::shutdown_request");
    }
  TAO_ENDTRY;
}

void
ACE_ES_Consumer_Module::shutdown (void)
{
  Consumers copy;

  {
    ACE_ES_GUARD ace_mon (lock_);
    if (ace_mon.locked () == 0)
      goto DONE;

    if (all_consumers_.size () == 0)
      goto DONE;

    // Make a copy so that the consumers can disconnect without the
    // lock being held.
    copy = all_consumers_;
  }

  // This scope is just to thwart the compiler.  It was complaining
  // about the above goto's bypassing variable initializations.  Yadda
  // yadda.
  {
    Consumer_Iterator iter (copy);

    CORBA::Environment env;

    for (ACE_Push_Consumer_Proxy **proxy = 0;
         iter.next (proxy) != 0;
         iter.advance ())
      {
        (*proxy)->shutdown ();
        // @@ Cannnot use CORBA::release (*proxy), since it is a servant.
        delete *proxy;

        // Remove the consumer from our list.
        {
          ACE_ES_GUARD ace_mon (lock_);
          if (ace_mon.locked () == 0)
            ACE_ERROR ((LM_ERROR, "%p Failed to acquire lock.\n", "ACE_ES_Consumer_Module::shutdown"));

          if (all_consumers_.remove (*proxy) == -1)
            ACE_ERROR ((LM_ERROR, "%p Failed to remove consumer.\n", "ACE_ES_Consumer_Module::shutdown"));
        }
      }
  }

DONE:
  channel_->shutdown ();
}

void
ACE_ES_Consumer_Module::disconnecting (ACE_Push_Consumer_Proxy *consumer,
                                       CORBA::Environment &_env)
{
  {
    ACE_ES_GUARD ace_mon (lock_);
    if (ace_mon.locked () == 0)
      TAO_THROW (RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR);

    if (all_consumers_.remove (consumer) == -1)
      TAO_THROW (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR);
  }

  // Tell everyone else that the consumer is disconnecting.  This
  // allows them to remove the consumer from any subscription lists
  // etc.  However, messages may still be queued in the ReactorEx or
  // in the Dispatching Module for this consumer, so no queues or
  // proxies can be deleted just yet.
  down_->disconnecting (consumer, _env);

  // Send a shutdown message through the system.  When this is
  // dispatched, the consumer proxy will be deleted.  <request> is
  // queued in the Priority_Timer at <priority> level.  It will be
  // scheduled for dispatching in 1 nanosecond.  This gives components
  // a hook into the first queueing point in the channel.

  // Create a shutdown message.  When this is dispatched, it will
  // delete the proxy.
  Shutdown_Consumer *sc = new Shutdown_Consumer (this, consumer);
  if (sc == 0)
    TAO_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

  // Create a wrapper around the dispatch request.
  Flush_Queue_ACT *act = new Flush_Queue_ACT (sc, channel_->dispatching_module_);
  if (act == 0)
    TAO_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

  // ACE_DEBUG ((LM_DEBUG, "EC (%t) initiating consumer disconnect.\n"));

  // Set a 100ns timer.
  TimeBase::TimeT ns100;
  ORBSVCS_Time::hrtime_to_TimeT (ns100, 100);
  if (channel_->timer ()->schedule_timer (0, // no rt_info
                                          act,
                                          // ::Preemption_Priority (consumer->qos ().rt_info_),
                                          ACE_Scheduler_MIN_PREEMPTION_PRIORITY,
                                          ns100,
                                          ORBSVCS_Time::zero) == -1)
    {
      ACE_ERROR ((LM_ERROR, "%p queue_request failed.\n", "ACE_ES_Consumer_Module"));
      delete sc;
      delete act;
    }
}

// This method executes in the same thread of control that will hand
// the event set to the consumer (or it's proxy).  A network proxy may
// copy the event set to the network buffer.  An active client may
// copy the event set to be queued.  Or a same address-space consumer
// can read the set we allocated off the stack.
void
ACE_ES_Consumer_Module::push (const ACE_ES_Dispatch_Request *request,
                              CORBA::Environment &_env)
{
  // ACE_DEBUG ((LM_DEBUG, "EC (%t) Consumer_Module::push\n"));

  ACE_FUNCTION_TIMEPROBE (TAO_EVENT_CHANNEL_ENTER_ES_CONSUMER_MODULE_PUSH);
  // We'll create a temporary event set with the size of the incoming
  // request.
  RtecEventComm::EventSet event_set (request->number_of_events ());
  request->make_copy (event_set);

  // Forward the event set.
  ACE_hrtime_t ec_send = ACE_OS::gethrtime ();
  for (CORBA::ULong i = 0; i < event_set.length (); ++i)
    {
      RtecEventComm::Event& ev =
        ACE_const_cast(RtecEventComm::Event&,event_set[i]);
      ORBSVCS_Time::hrtime_to_TimeT (ev.ec_send_time_, ec_send);
    }
  request->consumer ()->push (event_set, _env);
}

RtecEventChannelAdmin::ProxyPushSupplier_ptr
ACE_ES_Consumer_Module::obtain_push_supplier (CORBA::Environment &_env)
{
  ACE_Push_Consumer_Proxy *new_consumer = new ACE_Push_Consumer_Proxy (this);

  // Get a new supplier proxy object.
  if (new_consumer == 0)
    {
      ACE_ERROR ((LM_ERROR, "ACE_EventChannel"
                  "::obtain_push_supplier failed.\n"));
      TAO_THROW_RETURN (CORBA::NO_MEMORY (CORBA::COMPLETED_NO), 0);
    }

  {
    ACE_ES_GUARD ace_mon (lock_);
    if (ace_mon.locked () == 0)
      {
        delete new_consumer;
        TAO_THROW_RETURN (RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR, 0);
      }

    if (all_consumers_.insert (new_consumer) == -1)
      ACE_ERROR ((LM_ERROR, "ACE_ES_Consumer_Module insert failed.\n"));
  }

  // Return the CORBA object reference to the new supplier proxy.
  return new_consumer->get_ref (_env);
}

void
ACE_ES_Consumer_Module::fill_qos (RtecEventChannelAdmin::ConsumerQOS& c_qos,
                                  RtecEventChannelAdmin::SupplierQOS& s_qos)
{
  ACE_GUARD (ACE_ES_MUTEX, ace_mon, this->lock_);

  c_qos.is_gateway = CORBA::B_TRUE;
  s_qos.is_gateway = CORBA::B_TRUE;

  int count = 0;
  {
    for (Consumer_Iterator i = this->all_consumers_.begin ();
         i != this->all_consumers_.end ();
         ++i)
      {
        ACE_Push_Consumer_Proxy *c = *i;

        if (c->qos ().is_gateway)
          continue;

        count += c->qos ().dependencies.length ();
      }
  }

  RtecEventChannelAdmin::DependencySet& dep = c_qos.dependencies;
  RtecEventChannelAdmin::PublicationSet& pub = s_qos.publications;

  dep.length (count + 1);
  pub.length (count);

  CORBA::ULong cc = 0;
  CORBA::ULong sc = 0;
  dep[cc].event.type_ = ACE_ES_DISJUNCTION_DESIGNATOR;
  dep[cc].event.source_ = 0;
  dep[cc].event.creation_time_ = ORBSVCS_Time::zero;
  dep[cc].rt_info = 0;
  cc++;

  for (Consumer_Iterator i = this->all_consumers_.begin ();
       i != this->all_consumers_.end ();
       ++i)
    {
      ACE_Push_Consumer_Proxy *c = *i;

      if (c->qos ().is_gateway)
        continue;

      CORBA::ULong count = c->qos ().dependencies.length ();
      for (CORBA::ULong j = 0; j < count; ++j)
        {
          RtecEventComm::Event& event =
            c->qos ().dependencies[j].event;

          RtecEventComm::EventType type = event.type_;
          if (type <= ACE_ES_EVENT_UNDEFINED)
            continue;

          // Only type and source dependencies are relevant, notice
          // that we turn conjunctions into disjunctions because
          // correlations could be satisfied by events coming from
          // several remote ECs.
          if (type <= ACE_ES_EVENT_UNDEFINED)
            continue;

          // If the dependency is already there we don't add it.
          CORBA::ULong k;
          for (k = 0; k < cc; ++k)
            {
              if (dep[k].event.type_ == event.type_
                  && dep[k].event.source_ == event.source_)
                break;
            }
          if (k == cc)
            {
              dep[cc].event.type_ = event.type_;
              dep[cc].event.source_ = event.source_;
              dep[cc].event.creation_time_ = ORBSVCS_Time::zero;
              // The RT_Info is filled up later.
              dep[cc].rt_info = 0;
              cc++;

              pub[sc].event.type_ = event.type_;
              pub[sc].event.source_ = event.source_;
              pub[sc].event.creation_time_ = ORBSVCS_Time::zero;
              pub[sc].dependency_info.dependency_type =
                RtecScheduler::TWO_WAY_CALL;
              pub[sc].dependency_info.number_of_calls = 1;
              pub[sc].dependency_info.rt_info = 0;
              sc++;
            }
        }
    }
  dep.length (cc);
  pub.length (sc);
}

// ************************************************************

ACE_ES_Correlation_Module::ACE_ES_Correlation_Module (ACE_EventChannel *channel)
  : channel_ (channel),
    up_ (0),
    subscription_module_ (0)
{
}

void
ACE_ES_Correlation_Module::open (ACE_ES_Dispatching_Module *up,
                                 ACE_ES_Subscription_Module *sm)
{
  up_ = up;
  subscription_module_ = sm;
}

void
ACE_ES_Correlation_Module::connected (ACE_Push_Consumer_Proxy *consumer,
                                      CORBA::Environment &_env)
{
  // Initialize the consumer correlation filter.
  if (consumer->correlation ().connected (consumer, this) == -1)
    TAO_THROW (RtecEventChannelAdmin::EventChannel::CORRELATION_ERROR);
}

void
ACE_ES_Correlation_Module::disconnecting (ACE_Push_Consumer_Proxy *consumer,
                                          CORBA::Environment &)
{
  if (consumer->correlation ().disconnecting () == -1)
    ACE_ERROR ((LM_ERROR, "ACE_ES_Correlation_Module::disconnecting failed.\n"));
}

int
ACE_ES_Correlation_Module::subscribe (ACE_ES_Consumer_Rep *consumer)
{
  return subscription_module_->subscribe (consumer);
}

int
ACE_ES_Correlation_Module::unsubscribe (ACE_ES_Consumer_Rep *cr)
{
  return subscription_module_->unsubscribe (cr);
}

void
ACE_ES_Correlation_Module::push (ACE_ES_Consumer_Rep *consumer,
                                 ACE_ES_Event_Container *event,
                                 CORBA::Environment &_env)
{
  // ACE_DEBUG ((LM_DEBUG, "EC (%t) Correlation_Module::push\n"));

  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_ENTER_ACE_ES_CORRELATION_MODULE_PUSH);
  ACE_ES_Dispatch_Request *request =
    consumer->correlation ()->push (consumer, event);
  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_PUSHED_TO_CORRELATION_MODULE);

  // If request == 0, then the event was queued for later.  Otherwise,
  // we need to push the event now.
  if (request != 0)
    up_->push (request, _env);

  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_PUSH_SOURCE_TYPE_DISPATCH_MODULE_ENQUEUING);
}

// Must check consumer->qos ().use_timeout () before calling this.
// This method is supposed properly schedule a timer with respect to
// the consumer's priority AND the correlation that should receive the
// timeout event.
int
ACE_ES_Correlation_Module::schedule_timeout (ACE_ES_Consumer_Rep_Timeout *consumer)
{
  RtecEventComm::Time &interval =
    consumer->dependency ()->event.creation_time_;
  RtecEventComm::Time &delay =
    consumer->dependency ()->event.creation_time_;

  // Store the preemption priority so we can cancel the correct timer.
  // The priority values may change during the process lifetime (e.g.,
  // after the scheduler has been run).
  consumer->preemption_priority (::IntervalToPriority (interval));

  // ACE_DEBUG ((LM_DEBUG,
  // "EC (%t) Adding timer at preemption %d, rate = (%d,%d)\n",
  // consumer->preemption_priority (),
  // interval.low, interval.high));

  // Register the timer.
  int id = channel_->timer ()->schedule_timer (consumer->dependency ()->rt_info,
                                               consumer,
                                               consumer->preemption_priority (),
                                               delay, interval);

  // Store the timer id for canceling.
  consumer->timer_id (id);

  if (id == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p schedule timer failed.\n",
                       "ACE_ES_Correlation_Module::schedule_timeout"), -1);

  return 0;
}

// Must check consumer->qos ().timeout_ before calling this.
int
ACE_ES_Correlation_Module::cancel_timeout (ACE_ES_Consumer_Rep_Timeout *consumer)
{
  // Cancel the timer from the Priority Timer.
  ACE_ES_Timer_ACT *act;
  channel_->timer ()->cancel_timer (consumer->preemption_priority (),
                                    consumer->timer_id (),
                                    act);

  ACE_ASSERT (consumer == act);

  // Free up the Timer ACT.
  //  delete act;

  return 0;
}


int
ACE_ES_Correlation_Module::reschedule_timeout (ACE_ES_Consumer_Rep_Timeout *consumer)
{
  if (this->cancel_timeout (consumer) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n", "ACE_ES_Disjunction_Group::reschedule_deadline"), -1);
  else
    {
      RtecEventComm::Time &interval =
        consumer->dependency ()->event.creation_time_;
      RtecEventComm::Time &delay =
        consumer->dependency ()->event.creation_time_;

      // Store the preemption priority so we can cancel the correct timer.
      // The priority values may change during the process lifetime (e.g.,
      // after the scheduler has been run).
      consumer->preemption_priority (::IntervalToPriority (interval));

      // Register the timer.
      int id = channel_->timer ()->schedule_timer (0, // Do not pass an RT_Info.
                                                   consumer,
                                                   consumer->preemption_priority (),
                                                   delay, interval);

      // Store the timer id for canceling.
      consumer->timer_id (id);

      if (id == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "%p schedule timer failed.\n",
                           "ACE_ES_Correlation_Module::reschedule_timeout"), -1);

      return 0;
    }
}

void
ACE_ES_Correlation_Module::shutdown (void)
{
  // Perhaps this should call disconnecting on all the consumers?
  // We'll opt to just forward this message for now.
  up_->shutdown ();
}

// ************************************************************

ACE_ES_Consumer_Correlation::ACE_ES_Consumer_Correlation (void) :
  correlation_module_ (0),
  type_id_index_ (0),
  channel_ (0),
  qos_ (),
  pending_events_ (0),
  lock_ (),
  consumer_ (0),
  pending_flags_ (0),
  consumer_reps_ (0),
  n_consumer_reps_ (0),
  timer_reps_ (0),
  n_timer_reps_ (0),
  conjunction_groups_ (0),
  n_conjunction_groups_ (0),
  disjunction_groups_ (0),
  n_disjunction_groups_ (0),
  connected_ (0)
{
}

ACE_ES_Consumer_Correlation::~ACE_ES_Consumer_Correlation (void)
{
  delete [] timer_reps_;
  for (int i = 0; i < this->n_consumer_reps_; ++i)
    {
      ACE_ES_Consumer_Rep *r = this->consumer_reps_[i];
      if (r != 0)
        {
          this->correlation_module_->unsubscribe (r);
          r->_release ();
        }
    }
  delete [] consumer_reps_;
  delete [] conjunction_groups_;
  delete [] disjunction_groups_;
  delete [] pending_events_;
}

void
ACE_ES_Consumer_Correlation::disconnect_push_supplier (CORBA::Environment &_env)
{
  connected_ = 0;
}

int
ACE_ES_Consumer_Correlation::allocate_correlation_resources (ACE_ES_Dependency_Iterator &iter)
{
  n_conjunction_groups_ = iter.n_conjunctions ();
  if (n_conjunction_groups_ > 0)
    {
      conjunction_groups_ = new ACE_ES_Conjunction_Group[n_conjunction_groups_];
      if (conjunction_groups_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                           "ACE_ES_Consumer_Correlation::"
                           "allocate_correlation_resources"), -1);
      for (int n=0; n < n_conjunction_groups_; n++)
        conjunction_groups_[n].set_correlation_module (correlation_module_);
    }

  n_disjunction_groups_ = iter.n_disjunctions ();
  if (n_disjunction_groups_ > 0)
    {
      disjunction_groups_ = new ACE_ES_Disjunction_Group[n_disjunction_groups_];
      if (disjunction_groups_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                           "ACE_ES_Consumer_Correlation::"
                           "allocate_correlation_resources"), -1);
      for (int n=0; n < n_disjunction_groups_; n++)
        disjunction_groups_[n].set_correlation_module (correlation_module_);
    }

  n_consumer_reps_ = iter.n_events ();
  if (n_consumer_reps_ > 0)
    {
      // This allocates more than is needed if there are repeats:
      // (A+B)|(B+C).  We allocate these individually so that they can
      // be deleted individually.

      typedef ACE_ES_Consumer_Rep *reparray;
      consumer_reps_ = new reparray[n_consumer_reps_];

      for (int cr = 0; cr < n_consumer_reps_; cr++)
        {
          consumer_reps_[cr] = new ACE_ES_Consumer_Rep;
          if (consumer_reps_[cr] == 0)
            ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                               "ACE_ES_Consumer_Correlation::"
                               "allocate_correlation_resources"), -1);
        }
    }

  n_timer_reps_ = iter.n_timeouts ();
  if (n_timer_reps_ > 0)
    {
      timer_reps_ = new ACE_ES_Consumer_Rep_Timeout[n_timer_reps_];
      if (timer_reps_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                           "ACE_ES_Consumer_Correlation::"
                           "allocate_correlation_resources"), -1);
    }

  // This allocates more than is needed.
  pending_events_ = new Event_Set[n_consumer_reps_ + n_timer_reps_];
  if (pending_events_ == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                       "ACE_ES_Consumer_Correlation::"
                       "allocate_correlation_resources"), -1);

  return 0;
}

// We don't need synchronization until after we've been connected and
// subscribed to events.
int
ACE_ES_Consumer_Correlation::connected (ACE_Push_Consumer_Proxy *consumer,
                                        ACE_ES_Correlation_Module *correlation_module)
{
  correlation_module_ = correlation_module;
  consumer_ = consumer;

  //  for (CORBA_Types::ULong index=0; index < consumer->qos ().dependencies_.length (); index++)
  //    consumer->qos ().dependencies_[index].event.dump ();

  ACE_ES_Dependency_Iterator iter (consumer->qos ().dependencies);
  iter.parse ();
  if (this->allocate_correlation_resources (iter) == -1)
    return -1;


  int cgroup_index = -1;
  int dgroup_index = -1;
  int crep_index = 0;
  int trep_index = 0;
  RtecEventComm::EventType group_type = 0;

  while (iter.advance_dependency () == 0)
    {
      // Keep track of how many conjunction and disjunction groups are
      // registered.  Update the index pointers so that the helper
      // functions can update the appropriate group objects.
      switch ((*iter).event.type_)
        {
        case ACE_ES_CONJUNCTION_DESIGNATOR:
          cgroup_index++;
          ACE_ASSERT (cgroup_index < n_conjunction_groups_);
          group_type = ACE_ES_CONJUNCTION_DESIGNATOR;
          continue;

        case ACE_ES_DISJUNCTION_DESIGNATOR:
          dgroup_index++;
          ACE_ASSERT (dgroup_index < n_disjunction_groups_);
          group_type = ACE_ES_DISJUNCTION_DESIGNATOR;
          continue;

        case ACE_ES_GLOBAL_DESIGNATOR:
          group_type = ACE_ES_GLOBAL_DESIGNATOR;
          continue;

          // These Delegate to the appropriate registration method.
#if 0
          // @@ TODO rt_info_ is a handle_t now, does checking against
          // 0  still make sense?
          // Check for a null rt_info.
          if ((*iter).rt_info_ == 0)
            {
              ACE_ERROR ((LM_ERROR, "Found a ConsumerQOS::dependencies[].rt_info_ == 0.\n"));
              continue;
            }
#endif /* 0 */

        case ACE_ES_EVENT_TIMEOUT:
          // For backwards compatibility.
        case ACE_ES_EVENT_DEADLINE_TIMEOUT:
          if (this->register_deadline_timeout (*iter,
                                               group_type,
                                               cgroup_index,
                                               dgroup_index,
                                               trep_index) == -1)
            return -1;
          break;

        case ACE_ES_EVENT_INTERVAL_TIMEOUT:
          if (this->register_interval_timeout (*iter,
                                               group_type,
                                               cgroup_index,
                                               dgroup_index,
                                               trep_index) == -1)
            return -1;
          break;

        case ACE_ES_EVENT_ACT:
          // Store the ACT in the current conjunction or disjunction
          // group.
          switch (group_type)
            {
            case ACE_ES_CONJUNCTION_DESIGNATOR:
              conjunction_groups_[cgroup_index].set_act ((*iter).event);
              break;
            case ACE_ES_DISJUNCTION_DESIGNATOR:
              disjunction_groups_[cgroup_index].set_act ((*iter).event);
              break;
            case ACE_ES_GLOBAL_DESIGNATOR:
            default:
              ACE_ERROR ((LM_ERROR, "Warning: ACTs not implemented for Global.\n"));
            }
          break;

        default:
          // Non-timer event subscription.
          if (this->register_event (*iter,
                                    group_type,
                                    cgroup_index,
                                    dgroup_index,
                                    crep_index) == -1)
            return -1;
          break;
        }
    }

  // We may not use all of the consumer reps if there are repeats:
  // (A+B)|(B+C).  Must update n_consumer_reps_ so we don't try to
  // unsubscribe a blank rep during disconnect.
  if (crep_index < n_consumer_reps_)
    n_consumer_reps_ = crep_index;

  return 0;
}

int
ACE_ES_Consumer_Correlation::register_deadline_timeout (RtecEventChannelAdmin::Dependency &dependency,
                                                        RtecEventComm::EventType group_type,
                                                        int cgindex,
                                                        int dgindex,
                                                        int &trep_index)
{
  // new_timeout will be returned as an ACT.  When executed, it will
  // forward *iter.event_ to the consumer.
  ACE_ES_Consumer_Rep_Timeout *new_timeout = &timer_reps_[trep_index++];
  new_timeout->init (this, dependency);
  new_timeout->correlation_type (ACE_ES_Consumer_Rep::DEADLINE_TIMEOUT);
  // Deadline timers do not need type ids.

  switch (group_type)
    {
    case ACE_ES_CONJUNCTION_DESIGNATOR:
      // Reps keep pointers back to the groups that they're deadlines for.
      new_timeout->add_disjunction_group (conjunction_groups_[cgindex]);
      // Groups keep references to the deadline timers for rescheduling.
      if (conjunction_groups_[cgindex].set_deadline_timeout (new_timeout) == -1)
        return -1;
      break;

    case ACE_ES_DISJUNCTION_DESIGNATOR:
      new_timeout->add_disjunction_group (disjunction_groups_[dgindex]);
      if (disjunction_groups_[dgindex].set_deadline_timeout (new_timeout) == -1)
        return -1;
      break;

    case ACE_ES_GLOBAL_DESIGNATOR:
      ACE_ERROR_RETURN ((LM_ERROR, "No global deadline timeouts, yet!\n"), -1);
    }

  return 0;
}

int
ACE_ES_Consumer_Correlation::register_interval_timeout (RtecEventChannelAdmin::Dependency &dependency,
                                                        RtecEventComm::EventType group_type,
                                                        int cgindex,
                                                        int /* dgindex */,
                                                        int &trep_index)
{
  // new_timeout will be returned as an ACT.  When executed, it will
  // forward *iter.event_ to the consumer.
  ACE_ES_Consumer_Rep_Timeout *new_timeout = &timer_reps_[trep_index++];
  new_timeout->init (this, dependency);
  new_timeout->type_id (this->new_type_id ());

  switch (group_type)
    {
    case ACE_ES_CONJUNCTION_DESIGNATOR:
      // If it's a conjunction, then we need to perform correlations
      // on the timeout.
      new_timeout->correlation_type (ACE_ES_Consumer_Rep::CORRELATE);
      conjunction_groups_[cgindex].add_type (new_timeout->type_id ());
      break;

    case ACE_ES_DISJUNCTION_DESIGNATOR:
    case ACE_ES_GLOBAL_DESIGNATOR:
      new_timeout->correlation_type (ACE_ES_Consumer_Rep::NO_CORRELATION);
      break;
    }

  // Schedule the timeout.
  if (correlation_module_->schedule_timeout (new_timeout) == -1)
    return -1;
  else
    return 0;
}

// Search <creps> for a rep matching <dependency>.  If one is not
// found, allocate one.  All returned reps should have the appropriate
// type_id set.
ACE_ES_Consumer_Rep *
ACE_ES_Consumer_Correlation::get_consumer_rep (RtecEventChannelAdmin::Dependency &dependency,
                                               int &crep_index)
{
  ACE_ES_Consumer_Rep *rep = 0;

  // Step through all existing consumer reps.
  for (int x=0; x < crep_index; x++)
    {
      RtecEventComm::Event& e = consumer_reps_[x]->dependency ()->event;
      // If <dependency> matches any previously subscribed consumer
      // reps, we'll reuse it.
      if (e.type_ == dependency.event.type_
          && e.source_ == dependency.event.source_ )
        {
          rep = consumer_reps_[x];
          break;
        }
    }

  // Check if we didn't find it.
  if (rep == 0)
    {
      if (crep_index >= n_consumer_reps_)
        ACE_ERROR_RETURN ((LM_ERROR, "Too many event registrations.\n"), 0);
      // Allocate a new rep and set its type id.
      rep = consumer_reps_[crep_index];
      crep_index++;
      rep->init (this, dependency);
      rep->type_id (this->new_type_id ());
    }

  return rep;
}

int
ACE_ES_Consumer_Correlation::new_type_id (void)
{
  int type_id = type_id_index_;
  if (++type_id_index_ >= ACE_ES_MAX_SUBSCRIPTIONS)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_ES_MAX_SUBSCRIPTIONS exceeded.\n"),0);
  else
    return type_id;
}

int
ACE_ES_Consumer_Correlation::register_event (RtecEventChannelAdmin::Dependency &dependency,
                                             RtecEventComm::EventType group_type,
                                             int cgindex,
                                             int dgindex,
                                             int &crep_index)
{
  // These are stored in the subscription module data structures.
  ACE_ES_Consumer_Rep *consumer_rep = this->get_consumer_rep (dependency, crep_index);

  if (consumer_rep == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                       "ACE_ES_Consumer_Correlation::register_event"), -1);

  switch (group_type)
    {
    case ACE_ES_CONJUNCTION_DESIGNATOR:
      // If it's a conjunction, then we need to perform correlations
      // on the object.  Otherwise, NO_CORRELATION is set by default.
      consumer_rep->correlation_type (ACE_ES_Consumer_Rep::CORRELATE);
      conjunction_groups_[cgindex].add_type (consumer_rep->type_id ());
      break;

    case ACE_ES_DISJUNCTION_DESIGNATOR:
      consumer_rep->add_disjunction_group (disjunction_groups_[dgindex]);
      break;

    case ACE_ES_GLOBAL_DESIGNATOR:
      ACE_ERROR ((LM_ERROR, "ACE_ES_Consumer_Correlation::register_event: "
                  "ACE_ES_GLOBAL_DESIGNATOR not implemented.\n"));
      break;
    }

  // Subscribe the consumer_rep to the suppliers.
  if (correlation_module_->subscribe (consumer_rep) == -1)
    return -1;
  else
    return 0;
}

int
ACE_ES_Consumer_Correlation::disconnecting (void)
{
  // If we were forwarding events, disconnect as a supplier.
  if (connected_)
    {
      CORBA::Environment env;
      channel_->disconnect_push_consumer (env);
      if (env.exception () != 0)
        ACE_ERROR ((LM_ERROR, "ACE_ES_Consumer_Correlation::disconnecting failed.\n"));
    }

  for (int j = 0; j < this->n_timer_reps_; ++j)
    this->correlation_module_->cancel_timeout (&timer_reps_[j]);

  for (int i = 0; i < this->n_consumer_reps_; ++i)
    {
      ACE_ES_Consumer_Rep *r = this->consumer_reps_[i];
      if (r != 0)
        {
          r->disconnect ();
        }
    }

  return 0;
}

ACE_ES_Dispatch_Request *
ACE_ES_Consumer_Correlation::push (ACE_ES_Consumer_Rep *cr,
                                   ACE_ES_Event_Container *event)
{
  // ACE_DEBUG ((LM_DEBUG, "EC (%t) Consumer_Correlation_Module::push\n"));

  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_ACE_ES_CONSUMER_CORRELATION_PUSH_ENTER);

  // Check if this event needs any correlating, or if it should just
  // be forwarded real fast-like.
  switch (cr->correlation_type ())
    {
    case ACE_ES_Consumer_Rep::NO_CORRELATION:
      {
        // Calls reschedule on all disjunction groups it belongs to.
        cr->reschedule_deadlines ();

        ACE_TIMEPROBE (TAO_EVENT_CHANNEL_CONSUMER_CORRELATION_PUSH_DETERMINE_NO_CORR);
        ACE_ES_Dispatch_Request *request =
          new ACE_ES_Dispatch_Request (consumer_, event,
                                       cr->dependency ()->rt_info);
        ACE_TIMEPROBE (TAO_EVENT_CHANNEL_CONSUMER_CORRELATION_PUSH_NO_CORR_ALLOC);

        if (request == 0)
          ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                             "ACE_ES_Consumer_Correlation::push"), 0);

        return request;
      }

    case ACE_ES_Consumer_Rep::CORRELATE:
      return this->correlate (cr, event);

    case ACE_ES_Consumer_Rep::DEADLINE_TIMEOUT:
      {
        ACE_ES_Dispatch_Request *request =
          new ACE_ES_Dispatch_Request (consumer_, cr->dependency ()->rt_info);

        if (request == 0)
          ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                             "ACE_ES_Consumer_Correlation::push"), 0);

        // Add the deadline timeout to the outbox.
        request->event_set () += event;

        // Add any pending events to the outbox.
        cr->top_group ()->add_events (&(request->event_set ()),
                                      pending_events_, pending_flags_);

        return request;
      }

    default:
      ACE_ERROR_RETURN ((LM_ERROR, "ACE_ES_Consumer_Correlation::push:"
                         " unknown correlation type\n"), 0);
    }
}

// @@ If we're just event forwarding, then no pending_events_ need to
// be kept!  I'll add this optimization later.
ACE_ES_Dispatch_Request *
ACE_ES_Consumer_Correlation::correlate (ACE_ES_Consumer_Rep *cr,
                                        ACE_ES_Event_Container *event)
{
   // If the consumer has specified correlation criteria, then we must
   // first acquire the mutex.
   ACE_ES_GUARD ace_mon (lock_);
   if (ace_mon.locked () == 0)
     ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                        "ACE_ES_Consumer_Correlation::push"), 0);

   // Add the new event to the pending events.
   pending_events_[cr->type_id ()] += event;

   // Set the bit corresponding to the arrived event.
   // This should be pending_flags_->event_arrived (index);
   ACE_SET_BITS (pending_flags_, ACE_INT2BIT[cr->type_id ()]);

   ACE_ES_Dispatch_Request *request = 0;
   Event_Set *outbox = 0;
   // Since add_events changes pending_flags_, we need to keep this
   // for all iterations through the conjunction groups.
   u_long freeze_pending_flags = pending_flags_;

   for (int x=0; x < n_conjunction_groups_; x++)
     {
       if (conjunction_groups_[x].should_forward (freeze_pending_flags))
         {
           // If there is a deadline timer for this conjunction group,
           // this will reschedule them.
           conjunction_groups_[x].reschedule_deadline ();

           // First time in, allocate the new dispatch request.
           if (request == 0)
             {
               request =
                 new ACE_ES_Dispatch_Request (consumer_,
                                              cr->dependency ()->rt_info);
               if (request == 0)
                 ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                                    "ACE_ES_Consumer_Correlation::correlate"), 0);
               outbox = &(request->event_set ());
             }

           // Add each of the pending events for this correlation to
           // the outgoing dispatch request.  If outbox == 0, then
           // this will just clear any pending events.
           conjunction_groups_[x].add_events (outbox,
                                              pending_events_,
                                              pending_flags_);
         }
     }

   return request;
}


// ************************************************************

ACE_ES_Consumer_Rep::~ACE_ES_Consumer_Rep (void)
{
}

void
ACE_ES_Consumer_Rep::execute (void)
{
  ACE_ERROR ((LM_ERROR, "Warning!  ACE_ES_Consumer_Rep::execute called.\n"));
}

// ************************************************************

void
ACE_ES_Consumer_Rep_Timeout::execute (void)
{
  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_CONSUMER_REP_TIMEOUT_EXECUTE);
  if (this->receiving_events ())
    {
      CORBA::Environment __env;
      ACE_Time_Value tv = ACE_OS::gettimeofday ();
      ORBSVCS_Time::Time_Value_to_TimeT
        (timeout_event_->creation_time_, tv);
      correlation_->correlation_module_->push (this, timeout_event_, __env);
      if (__env.exception () != 0)
        ACE_ERROR ((LM_ERROR, "ACE_ES_Consumer_Rep_Timeout::execute: unexpected exception.\n"));
    }
}

// ************************************************************

ACE_ES_Subscription_Module::ACE_ES_Subscription_Module (ACE_EventChannel *channel)
  : channel_ (channel),
    up_ (0),
    down_ (0)
{
}

void
ACE_ES_Subscription_Module::open (ACE_ES_Correlation_Module *up,
                                  ACE_ES_Supplier_Module *down)
{
  // Brilliant.
  up_ = up;
  down_ = down;
}

ACE_ES_Subscription_Module::~ACE_ES_Subscription_Module (void)
{
}

// When a supplier connects, we step through each of its
// publications.  For each event type published, we allocate a set in
// the suppliers type collection.  Then we build a subscribers list
// starting with any consumers having a type-based subscription in the
// global type collection.
void
ACE_ES_Subscription_Module::connected (ACE_Push_Supplier_Proxy *supplier,
                                       CORBA::Environment &_env)
{
  RtecEventComm::EventSourceID sid = 0;
  // We will record the source_id for later usage.
 {
    ACE_ES_WGUARD ace_mon (lock_);
    if (ace_mon.locked () == 0)
      TAO_THROW (RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR);

    if (all_suppliers_.insert (supplier) == -1)
      ACE_ERROR ((LM_ERROR, "ACE_ES_Subscription_Module insert failed.\n"));

    // For every type that this supplier generates, bind a new
    // Type_Subscribers to the type in the supplier proxy's type
    // collection.
    RtecEventChannelAdmin::PublicationSet &publications =
      supplier->qos ().publications;

    sid = publications[0].event.source_;
    for (CORBA::ULong index=0; index < publications.length (); index++)
      {
        // Check to make sure an RT_Info was specified.
#if 0
        // @@ TODO: We should check if rt_info is a valid handle_t.
        if (publications[index].dependency_info_.rt_info.value() == 0)
          {
            ACE_ERROR ((LM_ERROR, "Found a SupplierQOS::dependency_info_.rt_info_ == 0\n"));
            continue;
          }
#endif

        RtecEventComm::EventType event_type =
          publications[index].event.type_;

        // @@ TODO we should throw something Check to make sure a type
        // was specified.
        if (event_type == ACE_ES_EVENT_ANY)
          {
            ACE_ERROR ((LM_ERROR, "ACE_ES_Subscription_Module::connected: "
                        "source is publishing ACE_ES_EVENT_ANY.\n"));
            continue;
          }

        // Make a new set for the proxy.  Include the dependency
        // info describing the RT_Method that generates this event.
        // This object will hold all the consumers that subscribe to
        // this publication.
        ACE_ES_Subscription_Info::Type_Subscribers *new_subscribers =
          new ACE_ES_Subscription_Info::Type_Subscribers (&(publications[index].dependency_info));

        if (new_subscribers == 0)
          {
            ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Subscription_Module::connected"));
            return;
          }

        // Check the global type collection for consumers that register
        // before suppliers.
        ACE_ES_Subscription_Info::Type_Subscribers *existing_subscribers;
        if (type_subscribers_.find (event_type, existing_subscribers) != -1)
          {
            // Iterate through existing subscribers.
            ACE_ES_Subscription_Info::Subscriber_Set_Iterator iter (existing_subscribers->consumers_);

            for (ACE_ES_Consumer_Rep **proxy = 0;
                 iter.next (proxy) != 0;
                 iter.advance ())
              {
                // Each existing subscriber will get appended to the
                // new subscribers list.  Dependencies are updated.

                // @@ TODO: Handle exceptions.
                ACE_Scheduler_Factory::server()->add_dependency
                  ((*proxy)->dependency()->rt_info,
                   new_subscribers->dependency_info_->rt_info,
                   new_subscribers->dependency_info_->number_of_calls,
                   RtecScheduler::ONE_WAY_CALL,
                   _env);
                if (_env.exception () != 0)
                  return;
                // @@ TODO use the TAO_TRY macros.

                if (new_subscribers->consumers_.insert (*proxy) == -1)
                  {
                    ACE_ERROR ((LM_ERROR,
                                "%p: add_dependency/insert failed.\n",
                                "ACE_ES_Subscription_Module::connected"));
                    continue;
                  }
                (*proxy)->_duplicate ();
              }
          }
#if 0
        else
          {
            //ACE_DEBUG ((LM_DEBUG,
            //          "EC (%t) No consumers for type %d\n", event_type));
          }
#endif

        // Put the new subscribers for this event type in the supplier
        // proxy's type map.
        if (supplier->subscription_info ().type_subscribers_.
            bind (event_type, new_subscribers) != 0)
          {
            // This may occur with a double bind, I think.
            ACE_ERROR ((LM_ERROR, "%p can't initialize type.\n",
                        "ACE_ES_Subscription_Module::connected"));
            delete new_subscribers;
            continue;
          }
      }
  } // release lock

  // Reregister any consumers that tried to subscribe before this
  // supplier connected.
  // NOTE: We used to call back the supplier here (using
  // supplier->source_id()), this is ineffective and leads to all kind
  // of dead-locks (the supplier is blocked and waiting for us).
  // We use the information on the publications to get the source_id.
  this->reregister_consumers (sid);
}

// Step through each of the source_subscribers looking for consumers
// that registered for <source> before <source> connected.
void
ACE_ES_Subscription_Module::reregister_consumers (RtecEventComm::EventSourceID source_id)
{
  ACE_ES_Subscription_Info::Subscriber_Set *subscribers = 0;
  if (source_subscribers_.find (source_id, subscribers) == -1)
    // Not found.
    return;

  ACE_ES_Subscription_Info::Subscriber_Set_Iterator iter (*subscribers);

  // Try to reregister all consumers.
  for (ACE_ES_Consumer_Rep **consumer = 0;
       iter.next (consumer) != 0;
       iter.advance ())
    if (this->subscribe (*consumer) == -1)
      ACE_ERROR ((LM_ERROR, "%p.\n" "ACE_ES_Subscription_Module::reregister_consumers"));
}


void
ACE_ES_Subscription_Module::disconnecting (ACE_Push_Supplier_Proxy *supplier,
                                           CORBA::Environment &_env)
{
  ACE_ES_WGUARD ace_mon (lock_);
  if (ace_mon.locked () == 0)
    TAO_THROW (RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR);

  if (all_suppliers_.remove (supplier) == -1)
    TAO_THROW (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR);

  // Remove all consumers from the supplier's source-based subscription lists.
  ACE_ES_Subscription_Info::Subscriber_Set_Iterator source_iterator
    (supplier->subscription_info ().source_subscribers_);

  for (ACE_ES_Consumer_Rep **consumer;
       source_iterator.next (consumer) != 0;
       source_iterator.advance ())
    {
      (*consumer)->_release ();
    }

  // Get the subscriber list for each type.
  ACE_ES_Subscription_Info::Subscriber_Map_Iterator type_map_iterator
    (supplier->subscription_info ().type_subscribers_);

  for (ACE_ES_Subscription_Info::Subscriber_Map_Entry *entry;
       type_map_iterator.next (entry) != 0;
       type_map_iterator.advance ())
    {
      // Remove all consumers from the supplier's source-based
      // subscription lists.
      ACE_ES_Subscription_Info::Subscriber_Set_Iterator type_iterator
        (entry->int_id_->consumers_);

      for (ACE_ES_Consumer_Rep **c;
           type_iterator.next (c) != 0;
           type_iterator.advance ())
        {
          (*c)->_release ();
        }
    }
}

int
ACE_ES_Subscription_Module::subscribe_all (ACE_ES_Consumer_Rep *)
{
  ACE_ERROR_RETURN ((LM_ERROR, "Consumer tried to register for all"
                     "events!  This is not implemented.\n"), -1);
}

// Forward <events> to all consumers subscribed to <source> only.
int
ACE_ES_Subscription_Module::push_source (ACE_Push_Supplier_Proxy *source,
                                         ACE_ES_Event_Container *event,
                                         CORBA::Environment &_env)
{
  // ACE_DEBUG ((LM_DEBUG, "EC (%t) Subscription_Module::push_source\n"));

  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_ENTER_ACE_ES_SUBSCRIPTION_MODULE_PUSH);
  // If there are now source-based subscribers for this supplier,
  // return.
  if (source->subscription_info ().source_subscribers_.size () == 0)
    return 0;

  ACE_ES_Subscription_Info::Subscriber_Set &set =
    source->subscription_info ().source_subscribers_;

  // List of consumers that need to be disconnected.
  ACE_ES_CRSet disconnect_list;

  {
    // Acquire a read lock.
    ACE_ES_RGUARD ace_mon (source->subscription_info ().lock_);
    if (ace_mon.locked () == 0)
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_ES_Subscription_Module::push_source.\n"), -1);

    ACE_ES_Subscription_Info::Subscriber_Set_Iterator iter (set);

    // Iterate through all subscribers.
    for (ACE_ES_Consumer_Rep **consumer = 0;
         iter.next (consumer) != 0;
         iter.advance ())
      {
        // Only push the event if the consumer is not suspended
        // and not disconnected.
        if ((*consumer)->receiving_events ())
          {
            up_->push (*consumer, event, _env);
            if (_env.exception () != 0) return -1;
          }
        // If the consumer has disconnected, schedule it for
        // disconnection.  We can not modify our list now.  It
        // would mess up the iterator.
        if ((*consumer)->disconnected ())
          disconnect_list.insert (*consumer);
      }
    // Release the read lock.
  }

  // If there are consumers scheduled for disconnect, acquire a write
  // lock and disconnect them.
  if (disconnect_list.size () != 0)
    {
      ACE_ES_WGUARD ace_mon (source->subscription_info ().lock_);
      if (ace_mon.locked () == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ACE_ES_Subscription_Module::push_source.\n"), -1);

      ACE_ES_CRSet_Iterator iter (disconnect_list.data (),
                                  disconnect_list.size ());

      // Iterate through the disconnecting consumers.
      for (ACE_ES_Consumer_Rep **consumer = 0;
           iter.next (consumer) != 0;
           iter.advance ())
        {
          // Remove the consumer from subscriber list.
          if (set.remove (*consumer) == -1)
            ACE_ERROR ((LM_ERROR, "%p remove failed.\n",
                        "ACE_ES_Subscription_Module::push_source.\n"));
          else
            // Decrement the consumer rep's reference count.
            (*consumer)->_release ();
        }
    }

  return 0;
}

// 1. figure out why we're going through the subscription module,
// instead of just passing through.
// 2. where is lock_?  Is there only one per module!?

int
ACE_ES_Subscription_Module::push_source_type (ACE_Push_Supplier_Proxy *source,
                                              ACE_ES_Event_Container *event,
                                              CORBA::Environment& _env)
{
  // ACE_DEBUG ((LM_DEBUG,
  // "EC (%t) Subscription_Module::push_source_type: \n"));

  // Step through each event in the set.  For each event type, find
  // the corresponding set in the type collection.  Push the single
  // event to each consumer in the set.

  ACE_ES_Subscription_Info::Subscriber_Map &supplier_map =
    source->subscription_info ().type_subscribers_;

  ACE_ES_CRSet disconnect_list;

  ACE_ES_Subscription_Info::Subscriber_Set *set;

  {
    ACE_ES_RGUARD ace_mon (source->subscription_info ().lock_);
    if (ace_mon.locked () == 0)
    {
      ACE_TIMEPROBE (TAO_EVENT_CHANNEL_PUSH_SOURCE_TYPE);
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_ES_Subscription_Module::"
                         "push_source_type.\n"), -1);
    }

    ACE_ES_Subscription_Info::Type_Subscribers *subscribers;

    if (supplier_map.current_size () == 0)
      {
        ACE_TIMEPROBE (TAO_EVENT_CHANNEL_PUSH_SOURCE_TYPE);
        // ACE_DEBUG ((LM_DEBUG, "EC (%t) Subscription_Module::"
        // "push_source_type - empty supplier map\n"));
        return 0;
      }

    if (supplier_map.find (event->type_, subscribers) == -1)
      {
        ACE_DEBUG ((LM_ERROR,
                    "EC (%t) ACE_ES_Subscription_Module::push_source_type"
                    " Warning: event type %d not registered.\n",
                    event->type_));
        ACE_TIMEPROBE (TAO_EVENT_CHANNEL_PUSH_SOURCE_TYPE);
        return 0; // continue anyway
      }

    if (subscribers->consumers_.size () == 0)
      {
        // ACE_DEBUG ((LM_DEBUG, "EC (%t) Subscription_Module::"
        // "push_source_type - empty consumer set for %d\n",
        // event->type_));
        ACE_TIMEPROBE (TAO_EVENT_CHANNEL_PUSH_SOURCE_TYPE);
        return 0;
      }

    set = &subscribers->consumers_;

    // We've found the set of consumers subscribed to this type
    // of event from this supplier.  Forward the event to each.
    ACE_ES_Subscription_Info::Subscriber_Set_Iterator iter (*set);

    for (ACE_ES_Consumer_Rep **consumer = 0;
         iter.next (consumer) != 0;
         iter.advance ())
      {
        if ((*consumer)->receiving_events ())
          {
            up_->push (*consumer, event, _env);
            if (_env.exception () != 0) return -1;
          }
        if ((*consumer)->disconnected ())
          disconnect_list.insert (*consumer);
      }
  }

  // Acquire a write lock and remove all disconnected consumers.
  if (disconnect_list.size () != 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "EC (%t) Subscription_Module::push_source_type"
                  " - disconnecting consumers\n"));
      ACE_ES_WGUARD ace_mon (source->subscription_info ().lock_);
      if (ace_mon.locked () == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ACE_ES_Subscription_Module::"
                           "push_source.\n"), -1);

      ACE_ES_CRSet_Iterator iter (disconnect_list.data (),
                                  disconnect_list.size ());

      for (ACE_ES_Consumer_Rep **consumer = 0;
           iter.next (consumer) != 0;
           iter.advance ())
        {
          if (set->remove (*consumer) == -1)
            ACE_ERROR ((LM_ERROR, "%p remove failed.\n",
                        "ACE_ES_Subscription_Module::push_source.\n"));
          else
            (*consumer)->_release ();
          ACE_DEBUG ((LM_DEBUG, "EC (%t) Subscription_Module::"
                      "push_source_type - consumer %x removed\n",
                      *consumer));
        }
    }

  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_PUSH_SOURCE_TYPE);
  return 0;
}

int
ACE_ES_Subscription_Module::subscribe_source (ACE_ES_Consumer_Rep *consumer,
                                              RtecEventComm::EventSourceID source)
{
  // Step through all Supplier Proxies looking for a match to
  // -supplier-.  Add the -consumer- to the correct supplier proxy.
  Supplier_Iterator iter (all_suppliers_);

  int success = -1;

  for (ACE_Push_Supplier_Proxy **proxy = 0;
       iter.next (proxy) != 0;
       iter.advance ())
    {
      // Operator == checks if <proxy> is a proxy for <supplier>.
      if ((**proxy) == source)
        {
          ACE_ES_WGUARD mon ((*proxy)->subscription_info ().lock_);

          ACE_ES_Subscription_Info::Subscriber_Set &set =
            (*proxy)->subscription_info ().source_subscribers_;

          // Insert the consumer to the supplier's subscription set for
          // the type.
          int insert_result = set.insert (consumer);
          switch (insert_result)
            {
            case -1:
              // Error.
              ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                                 "Subscription Module::subscribe_source"), -1);
            case 0:
            default:
              {
                // Increment the consumer rep's reference count.
                consumer->_duplicate ();
                // Success.
                // Add each of the supplier's dependency infos to the
                // consumer's dependency list.
                ACE_ES_Subscription_Info::Subscriber_Map_Iterator iter2
                  ((*proxy)->subscription_info ().type_subscribers_);

                // Delete all type collections.
                for (ACE_ES_Subscription_Info::Subscriber_Map_Entry *temp = 0;
                     iter2.next (temp) != 0;
                     iter2.advance ())
                  {
                    TAO_TRY
                      {
                        ACE_Scheduler_Factory::server()->add_dependency
                          (consumer->dependency()->rt_info,
                           temp->int_id_->dependency_info_->rt_info,
                           temp->int_id_->dependency_info_->number_of_calls,
                           RtecScheduler::ONE_WAY_CALL,
                           TAO_TRY_ENV);
                        TAO_CHECK_ENV;

                      }
                    TAO_CATCHANY
                      {
                        TAO_TRY_ENV.print_exception ("error adding dependency");
                        return -1;
                      }
                    TAO_ENDTRY;
                  }
              }

            case 1:
              // Already there.
              success = 0;
              break;
            }
        }
    }

  // Add the consumer to the global source subscribers list.
  // @@ TODO This seems to require that source IDs be unique, i.e. any
  // new supplier with the same ID will be ignored.
  if (success == -1)
    return ACE_ES_Subscription_Info::insert_or_allocate (source_subscribers_,
                                                         consumer,
                                                         source);
  else
    return success;
}

// Step through all Supplier Proxies.  For each proxy, if it generates
// <type>, add <consumer> to its subscription info.
int
ACE_ES_Subscription_Module::subscribe_type (ACE_ES_Consumer_Rep *consumer,
                                            RtecEventComm::EventType type)
{
  // ACE_DEBUG ((LM_DEBUG,
  // "EC (%t) Subscription_Module::subscribe_type - %d\n", type));

  // First insert <consumer> into the global type collection set
  // corresponding to <type>.  The type collection will only be used
  // when suppliers register late.
  if (ACE_ES_Subscription_Info::insert_or_allocate (type_subscribers_,
                                                    consumer, type) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Subscription_Module - insert_or_allocate failed\n"),
                        -1);
    }

  Supplier_Iterator iter (all_suppliers_);

  for (ACE_Push_Supplier_Proxy **proxy = 0;
       iter.next (proxy) != 0;
       iter.advance ())
    {
      ACE_ES_WGUARD mon ((*proxy)->subscription_info ().lock_);
      // Insert the consumer to the supplier's subscription set for
      // the type.  If the supplier does not publish this type, the
      // operation will fail.  If this succeeds, dependency_info will
      // be added to the consumer.
      RtecScheduler::Dependency_Info *dependency_info;
      if (ACE_ES_Subscription_Info::insert_or_fail
          ((*proxy)->subscription_info ().type_subscribers_,
           consumer, type, dependency_info) == 0)
        {
          // Success.  Add the supplier dependency info to the
          // consumer's dependency list.
          // @@ TODO handle exceptions.
          TAO_TRY
            {
              ACE_Scheduler_Factory::server()->add_dependency
                (consumer->dependency ()->rt_info,
                 dependency_info->rt_info,
                 dependency_info->number_of_calls,
                 RtecScheduler::ONE_WAY_CALL,
                 TAO_TRY_ENV);
              TAO_CHECK_ENV;
            }
          TAO_CATCHANY
            {
              ACE_ERROR ((LM_ERROR, "Subscription_Module::subscribe_type:"
                          " add_dependency failed.\n"));
              return -1;
            }
          TAO_ENDTRY;
        }
    }

  return 0;
}

int
ACE_ES_Subscription_Module::subscribe_source_type (ACE_ES_Consumer_Rep *consumer,
                                                   RtecEventComm::EventSourceID source,
                                                   RtecEventComm::EventType type)
{
  // Step through all Supplier Proxies looking for a match to
  // <supplier>.  Once we find one, find the correct set for the
  // specified type.  Add the <consumer> to that set.
  Supplier_Iterator iter (all_suppliers_);

  int success = -1;

  for (ACE_Push_Supplier_Proxy **proxy = 0;
       iter.next (proxy) != 0;
       iter.advance ())
    {
      ACE_ES_WGUARD mon ((*proxy)->subscription_info ().lock_);

      if ((**proxy) == source)
        {
          // Insert the consumer to the supplier's subscription set for
          // the type.
          RtecScheduler::Dependency_Info *dependency_info;
          int insert_result = ACE_ES_Subscription_Info::insert_or_fail
            ((*proxy)->subscription_info().type_subscribers_,
             consumer, type, dependency_info);

          switch (insert_result)
            {
            case -1:
              // Error.
              ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                                 "Subscription Module::subscribe_source_type"), -1);
            case 0:
            default:
              {
                // Success.
                // Add the supplier to the consumer's dependency list.
                // @@ TODO handle exceptions.
                TAO_TRY
                  {
                    ACE_Scheduler_Factory::server()->add_dependency
                      (consumer->dependency ()->rt_info,
                       dependency_info->rt_info,
                       dependency_info->number_of_calls,
                       RtecScheduler::ONE_WAY_CALL,
                       TAO_TRY_ENV);
                    TAO_CHECK_ENV;
                  }
                TAO_CATCHANY
                  {
                    ACE_ERROR_RETURN ((LM_ERROR, "Subscription_Module::subscribe_source_type:"
                                       " add_dependency failed.\n"),
                                      -1);
                  }
                TAO_ENDTRY;
              }
              /* FALLTHROUGH */
            case 1:
              success = 0;

              // Already there.
              break;
            }

        }
    }

  // If we failed to find a source, insert this consumer in the
  // global source subscriber list.
  // @@ TODO This seems to require that the supplier IDs be unique.
  if (success == -1)
    return ACE_ES_Subscription_Info::insert_or_allocate (source_subscribers_,
                                                         consumer,
                                                         source);

  return success;
}

// <consumer> contains information for one type of subscription.
// Delegate to the appropriate method for subscription.
int
ACE_ES_Subscription_Module::subscribe (ACE_ES_Consumer_Rep *consumer)
{
  // We could have finer granularity by putting RGUARDs in some of the
  // subscribe methods.
  ACE_ES_WGUARD ace_mon (lock_);
  if (ace_mon.locked () == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                       "ACE_ES_Subscription_Module::subscribe"), -1);

  int result = 0;
  RtecEventComm::Event &event = consumer->dependency ()->event;

  if (event.source_ == 0)
    // Not source-based subscription.
    {
      if (event.type_ == ACE_ES_EVENT_ANY)
        result = this->subscribe_all (consumer);
      else
        result = this->subscribe_type (consumer, event.type_);
    }
  else
    // Source-based subscription.
    {
      if (event.type_ == ACE_ES_EVENT_ANY)
        result = this->subscribe_source (consumer, event.source_);
      else
        result = this->subscribe_source_type (consumer,
                                              event.source_,
                                              event.type_);
    }

  return result;
}

int
ACE_ES_Subscription_Module::unsubscribe (ACE_ES_Consumer_Rep *consumer)
{
  // We could have finer granularity by putting RGUARDs in some of the
  // unsubscribe methods.
  ACE_ES_WGUARD ace_mon (lock_);
  if (ace_mon.locked () == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                       "ACE_ES_Subscription_Module::unsubscribe"), -1);

  RtecEventComm::Event &event = consumer->dependency ()->event;

  if (event.type_ != ACE_ES_EVENT_ANY)
    {
      // Remove the consumer from the global type-based subscription list.
      ACE_ES_Subscription_Info::remove (type_subscribers_,
                                        consumer,
                                        event.type_);
    }
  else
    // Remove the consumer from the global source-based subscription list.
    ACE_ES_Subscription_Info::remove (source_subscribers_,
                                      consumer,
                                      event.source_);

#if 0
  // @@ TODO This code was removed and I'm (coryan) adding it again
  // because it does seem necessary, the only explanation for its
  // removal is this comment:
  //
  // /*
  //
  //  This old code manually removed the consumer from the subscription
  // lists.  Now we do lazy removal.
  // */
  //
  // I quite not follow what was going on.
  //
  // [CORYAN]: The mistery seems resolved: the code was here to remove
  // the consumer proxy from the supplier sets, but the "new" strategy
  // was to remove them as the supplier tries to push events through
  // them, unfortunately that code was broken and did not remove the
  // objects at all, hence the apparent need to re-instate this code.
  // Bottom line: it seems the code is not needed after all.

  int result = 0;

  if (event.source_ == 0)
    {
      if (event.type_ == ACE_ES_EVENT_ANY)
        result = this->unsubscribe_all (consumer);
      else
        result = this->unsubscribe_type (consumer, event.type_);
    }
  else
    {
      if (event.type_ == ACE_ES_EVENT_ANY)
        result = this->unsubscribe_source (consumer, event.source_);
      else
        result = this->unsubscribe_source_type (consumer, event.source_, event.type_);
    }
  return result;
#else
  return 0;
#endif /* 0 */
}

int
ACE_ES_Subscription_Module::unsubscribe_all (ACE_ES_Consumer_Rep *)
{
  return 0;
}

int
ACE_ES_Subscription_Module::unsubscribe_type (ACE_ES_Consumer_Rep *consumer,
                                              RtecEventComm::EventType type)
{
  // Step through all Supplier Proxies trying to remove the
  // consumer-type pair.  ACE_ES_Subscription_Info::remove will fail
  // if the supplier does not generate <type>, but that's ok.
  Supplier_Iterator iter (all_suppliers_);

  for (ACE_Push_Supplier_Proxy **proxy = 0;
       iter.next (proxy) != 0;
       iter.advance ())
    {
      ACE_ES_WGUARD mon ((*proxy)->subscription_info ().lock_);

      // This remove will be harmless if the supplier does not
      // generate <type>.
      ACE_ES_Subscription_Info::remove ((*proxy)->subscription_info ().type_subscribers_,
                                        consumer, type);
    }

  return 0;
}

int
ACE_ES_Subscription_Module::unsubscribe_source (ACE_ES_Consumer_Rep *consumer,
                                                RtecEventComm::EventSourceID source)
{
  Supplier_Iterator iter (all_suppliers_);

  for (ACE_Push_Supplier_Proxy **proxy = 0;
       iter.next (proxy) != 0;
       iter.advance ())
    {
      ACE_ES_WGUARD mon ((*proxy)->subscription_info ().lock_);

      if ((**proxy) == source)
        {
          ACE_ES_Subscription_Info::Subscriber_Set &set =
            (*proxy)->subscription_info ().source_subscribers_;
          if (set.remove (consumer) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                               "Subscription Module::unsubscribe_source"), -1);
          consumer->_release ();
        }
    }

  return 0;
}

int
ACE_ES_Subscription_Module::unsubscribe_source_type (ACE_ES_Consumer_Rep *consumer,
                                                     RtecEventComm::EventSourceID source,
                                                     RtecEventComm::EventType type)

{
  Supplier_Iterator iter (all_suppliers_);

  // Step through all supplier proxies looking for a match to the
  // consumer's event.source_.  This is the same as unsubscribe_type,
  // only we can check the source first.
  for (ACE_Push_Supplier_Proxy **proxy = 0;
       iter.next (proxy) != 0;
       iter.advance ())
    // If the proxy matches the source id we're looking for, try to
    // remove <consumer> from the proxy's <event.type_> set.
    if ((**proxy) == source)
      {
        ACE_ES_WGUARD mon ((*proxy)->subscription_info ().lock_);

        // Continue in spite of errors.
        ACE_ES_Subscription_Info::remove ((*proxy)->subscription_info ().type_subscribers_,
                                          consumer, type);
      }

  return 0;
}

void
ACE_ES_Subscription_Module::push (ACE_Push_Supplier_Proxy *source,
                                  ACE_ES_Event_Container *event,
                                  CORBA::Environment &_env)
{
  // ACE_DEBUG ((LM_DEBUG, "EC (%t) Subscription_Module::push\n"));

  ACE_TIMEPROBE (TAO_EVENT_CHANNEL_DELIVER_TO_SUBSCRIPTION_MODULE);
  // These are all inline function calls.
  if (this->push_source (source, event, _env) == -1)
    return;

  {
    ACE_FUNCTION_TIMEPROBE (TAO_EVENT_CHANNEL_BEGIN_PUSH_SOURCE_TYPE);

    if (this->push_source_type (source, event, _env) == -1)
      return;
  }
}

void
ACE_ES_Subscription_Module::shutdown (void)
{
  ACE_ES_WGUARD ace_mon (lock_);
  if (ace_mon.locked () == 0)
    ACE_ERROR ((LM_ERROR, "%p.\n",
                "ACE_ES_Subscription_Module::shutdown"));

  // Remove all type_subscribers_ and source_subscribers_.

  ACE_ES_Subscription_Info::Subscriber_Map_Iterator type_iter (type_subscribers_);
  for (ACE_ES_Subscription_Info::Subscriber_Map_Entry *entry = 0;
       type_iter.next (entry) != 0;
       type_iter.advance ())
    {
      ACE_ES_Subscription_Info::Subscriber_Set_Iterator ts_iter (entry->int_id_->consumers_);

      for (ACE_ES_Consumer_Rep **consumer = 0;
           ts_iter.next (consumer) != 0;
           ts_iter.advance ())
        (*consumer)->_release ();

      delete entry->int_id_;
    }

  ACE_ES_Subscription_Info::SourceID_Map_Iterator source_iter (source_subscribers_);

  for (ACE_ES_Subscription_Info::SourceID_Map_Entry *entry2;
       source_iter.next (entry2) != 0;
       source_iter.advance ())
    {
      ACE_ES_Subscription_Info::Subscriber_Set_Iterator ss_iter (*entry2->int_id_);

      for (ACE_ES_Consumer_Rep **consumer = 0;
           ss_iter.next (consumer) != 0;
           ss_iter.advance ())
        (*consumer)->_release ();

      delete entry2->int_id_;
    }

  // We don't need to do anything to all_suppliers_ since the supplier
  // module should have disconnected all suppliers.  To be more
  // independent from the supplier module, this method should iterate
  // through all suppliers and call this->disconnecting.
  up_->shutdown ();
}

// ************************************************************

ACE_ES_Supplier_Module::ACE_ES_Supplier_Module (ACE_EventChannel *channel) :
  all_suppliers_ (),
  lock_ (),
  up_ (0),
  channel_ (channel)
{
}

void
ACE_ES_Supplier_Module::open (ACE_ES_Subscription_Module *up)
{
  // There is the theory of the Mobius, a twist, in the fabric of
  // space, where time becomes a loop, where time becomes a loop.
  up_ = up;
}

void
ACE_ES_Supplier_Module::connected (ACE_Push_Supplier_Proxy *supplier,
                                   CORBA::Environment &_env)
{
  channel_->report_connect (ACE_EventChannel::SUPPLIER);
  up_->connected (supplier, _env);
  if (!supplier->qos ().is_gateway)
    this->channel_->update_supplier_gwys (_env);
}

void
ACE_ES_Supplier_Module::disconnecting (ACE_Push_Supplier_Proxy *supplier,
                                       CORBA::Environment &_env)
{
  ACE_ES_GUARD ace_mon (lock_);
  if (ace_mon.locked () == 0)
    TAO_THROW (RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR);

  if (all_suppliers_.remove (supplier) == -1)
    TAO_THROW (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR);

  up_->disconnecting (supplier, _env);

  if (all_suppliers_.size () <= 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "EC (%t) No more suppliers connected.\n"));
      channel_->report_disconnect_i (ACE_EventChannel::SUPPLIER);
    }

  CORBA::Boolean dont_update = supplier->qos ().is_gateway;

  // @@ TODO It would seem
  // IMHO this release is broken: supplier is a parameter, we never
  // actually increased its reference count, so we shouldn't decrease
  // it.
  // CORBA::release (supplier);

  if (!dont_update)
    this->channel_->update_supplier_gwys (_env);
}

void
ACE_ES_Supplier_Module::shutdown (void)
{
  Suppliers copy;

  {
    ACE_ES_GUARD ace_mon (lock_);
    if (ace_mon.locked () == 0)
      return;

    copy = all_suppliers_;
  }

  if (copy.size () > 0)
    {
      Supplier_Iterator iter (copy);

      CORBA::Environment env;

      for (ACE_Push_Supplier_Proxy **proxy = 0;
           iter.next (proxy) != 0;
           iter.advance ())
        {
          (*proxy)->shutdown ();
          this->disconnecting (*proxy, env);
        }
    }

  up_->shutdown ();
}

RtecEventChannelAdmin::ProxyPushConsumer_ptr
ACE_ES_Supplier_Module::obtain_push_consumer (CORBA::Environment &_env)
{
  ACE_Push_Supplier_Proxy *new_supplier = new ACE_Push_Supplier_Proxy (this);

  if (new_supplier == 0)
    TAO_THROW_RETURN (CORBA::NO_MEMORY (CORBA::COMPLETED_NO), 0);

  {
    ACE_ES_GUARD ace_mon (lock_);
    if (ace_mon.locked () == 0)
      {
        delete new_supplier;
        TAO_THROW_RETURN (RtecEventChannelAdmin::EventChannel::SYNCHRONIZATION_ERROR, 0);
      }

    if (all_suppliers_.insert (new_supplier) == -1)
      ACE_ERROR ((LM_ERROR, "ACE_ES_Supplier_Module insert failed.\n"));
  }

  return new_supplier->get_ref (_env);
}

void
ACE_ES_Supplier_Module::push (ACE_Push_Supplier_Proxy *proxy,
                              const RtecEventComm::EventSet &event,
                              CORBA::Environment &_env)
{
  // ACE_DEBUG ((LM_DEBUG, "EC (%t) Supplier_Module::push\n"));
  TAO_TRY
    {
      for (CORBA::ULong i = 0; i < event.length(); ++i)
        {
          ACE_ES_Event_Container *temp =
            new ACE_ES_Event_Container (event[i]);
          //RtecEventComm::Event *temp = new RtecEventComm::Event (event);

          if (temp == 0)
            TAO_THROW (CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

          // This will guarantee that release gets called when we exit
          // the scope.
          ACE_ES_Event_Container_var event_copy (temp);
          temp->_release ();
          ACE_TIMEPROBE (TAO_EVENT_CHANNEL_DELIVER_TO_SUPPLIER_MODULE_THRU_SUPPLIER_PROXY);
          up_->push (proxy, event_copy, TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }
    }
  TAO_CATCH (RtecEventChannelAdmin::TypeError, t)
    {
      ACE_ERROR ((LM_ERROR, "%p Type Error.\n",
                  "ACE_ES_Supplier_Module::push"));
      TAO_RETHROW;
    }
  TAO_CATCH (CORBA::NO_MEMORY, e)
    {
      ACE_ERROR ((LM_ERROR, "%p No Memory.\n",
                  "ACE_ES_Supplier_Module::push"));
      TAO_RETHROW;
    }
  TAO_CATCH (CORBA::SystemException, e)
    {
      ACE_ERROR ((LM_ERROR, "%p CORBA System Exception.\n",
                  "ACE_ES_Supplier_Module::push"));
      TAO_RETHROW;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "ACE_ES_Supplier_Module::push: "
                  "Unknown exception.\n"));
      TAO_RETHROW;
    }
  TAO_ENDTRY;
}

void
ACE_ES_Supplier_Module::fill_qos (RtecEventChannelAdmin::ConsumerQOS& c_qos,
                                  RtecEventChannelAdmin::SupplierQOS& s_qos)
{
  ACE_GUARD (ACE_ES_MUTEX, ace_mon, this->lock_);

  c_qos.is_gateway = CORBA::B_TRUE;
  s_qos.is_gateway = CORBA::B_TRUE;

  int count = 0;
  {
    for (Supplier_Iterator i = this->all_suppliers_.begin ();
         i != this->all_suppliers_.end ();
         ++i)
      {
        ACE_Push_Supplier_Proxy *s = *i;

        if (s->qos ().is_gateway)
          continue;

        count += s->qos ().publications.length ();
      }
  }

  RtecEventChannelAdmin::DependencySet& dep = c_qos.dependencies;
  RtecEventChannelAdmin::PublicationSet& pub = s_qos.publications;

  dep.length (count + 1);
  pub.length (count);

  CORBA::ULong cc = 0;
  CORBA::ULong sc = 0;
  dep[cc].event.type_ = ACE_ES_DISJUNCTION_DESIGNATOR;
  dep[cc].event.source_ = 0;
  dep[cc].event.creation_time_ = ORBSVCS_Time::zero;
  dep[cc].rt_info = 0;
  cc++;

  for (Supplier_Iterator i = this->all_suppliers_.begin ();
       i != this->all_suppliers_.end ();
       ++i)
    {
      ACE_Push_Supplier_Proxy *s = *i;

      if (s->qos ().is_gateway)
        continue;

      CORBA::ULong count = s->qos ().publications.length ();
      for (CORBA::ULong j = 0; j < count; ++j)
        {
          RtecEventComm::Event& event =
            s->qos ().publications[j].event;

          RtecEventComm::EventType type = event.type_;
          if (type <= ACE_ES_EVENT_UNDEFINED)
            continue;

          // Only type and source dependencies are relevant, notice
          // that we turn conjunctions into disjunctions because
          // correlations could be satisfied by events coming from
          // several remote ECs.
          if (type <= ACE_ES_EVENT_UNDEFINED)
            continue;

          // If the dependency is already there we don't add it.
          CORBA::ULong k;
          for (k = 0; k < sc; ++k)
            {
              if (pub[k].event.type_ == event.type_
                  && pub[k].event.source_ == event.source_)
                break;
            }
          if (k == sc)
            {
              dep[cc].event.type_ = event.type_;
              dep[cc].event.source_ = event.source_;
              dep[cc].event.creation_time_ = ORBSVCS_Time::zero;
              // The RT_Info is filled up later.
              dep[cc].rt_info = 0;
              cc++;

              pub[sc].event.type_ = event.type_;
              pub[sc].event.source_ = event.source_;
              pub[sc].event.creation_time_ = ORBSVCS_Time::zero;
              pub[sc].dependency_info.dependency_type =
                RtecScheduler::TWO_WAY_CALL;
              pub[sc].dependency_info.number_of_calls = 1;
              pub[sc].dependency_info.rt_info = 0;
              sc++;
            }
        }
    }
  dep.length (cc);
  pub.length (sc);
}

// ************************************************************

ACE_ES_Priority_Timer::ACE_ES_Priority_Timer (ACE_Task_Manager* tm)
  : task_manager_ (tm)
{
}

int
ACE_ES_Priority_Timer::connected (RtecScheduler::handle_t rt_info)
{
  RtecScheduler::OS_Priority thread_priority;
  RtecScheduler::Preemption_Subpriority subpriority;
  RtecScheduler::Preemption_Priority preemption_priority;

  TAO_TRY
    {
      ACE_TIMEPROBE (TAO_EVENT_CHANNEL_CONNECTED_PRIORITY_REQUESTED);
      ACE_Scheduler_Factory::server ()->priority
        (rt_info, thread_priority,
         subpriority, preemption_priority, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      ACE_TIMEPROBE (TAO_EVENT_CHANNEL_CONNECTED_PRIORITY_OBTAINED);
#if 0
      ACE_ERROR_RETURN ((LM_ERROR, "%p RtecScheduler::Scheduler::priority failed.\n",
                         "ACE_ES_Priority_Timer::connected"), -1);
#endif /* 0 */
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%p RtecScheduler::Scheduler::priority failed.\n",
                         "ACE_ES_Priority_Timer::connected"), -1);
    }
  TAO_ENDTRY;

  // Just make sure the ORB allocates resources for this priority.
  if (this->task_manager_->GetReactorTask (preemption_priority) == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "%p.\n",
                       "ACE_ES_Priority_Timer::connected"), -1);

  return 0;
}

int
ACE_ES_Priority_Timer::handle_timeout (const ACE_Time_Value &,
                                       const void *vp)
{
  ACE_ES_Timer_ACT *act = (ACE_ES_Timer_ACT *) vp;

  if (act == 0)
    ACE_ERROR_RETURN ((LM_ERROR, "ACE_ES_Priority_Timer::handle_timeout: "
                       "received act == 0!!!.\n"), 0);

  {
    ACE_FUNCTION_TIMEPROBE (TAO_EVENT_CHANNEL_ES_PRIORITY_QUEUE_START_EXECUTE);

    act->execute ();
  }

  return 0;
}

// ************************************************************

const char *
ACE_ES_Consumer_Name (const RtecEventChannelAdmin::ConsumerQOS &qos)
{
  // The first dependency should designate a correlation group.

  TAO_TRY
    {
      ACE_FUNCTION_TIMEPROBE (TAO_EVENT_CHANNEL_CONSUMER_NAME_PRIORITY_REQUESTED);
      RtecScheduler::RT_Info* rt_info = ACE_Scheduler_Factory::server ()->get
        (qos.dependencies[1].rt_info, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      return rt_info->entry_point;
    }
  TAO_CATCHANY
    {
      return "no-name";
    }
  TAO_ENDTRY;
  return "no-name";
}

// ************************************************************

void
dump_event (const RtecEventComm::Event &event)
{
  ACE_DEBUG ((LM_DEBUG, "source_ = %d "
              "type_ = %d "
              "time_ = %u.\n",
              (void*)event.source_,
              event.type_,
              // The divide-by-1 is for ACE_U_LongLong support.
              ORBSVCS_Time::to_hrtime (event.creation_time_) / 1));
}

// ************************************************************

#if defined(ACE_ES_LACKS_ORB)
void
dump_sequence (const ACE_CORBA_Sequence<ACE_ES_Event> &seq)
{
  for (CORBA::ULong index=0; index < seq.length (); index++)
    ::dump_event (seq[index]);
}
#endif /* ACE_ES_LACKS_ORB */

// ************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Atomic_Op<ACE_ES_MUTEX, int>;
template class ACE_Map_Entry<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT>;
template class ACE_Map_Entry<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT>;
template class ACE_Map_Manager<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT, ACE_ES_Subscription_Info::SYNCH>;
template class ACE_Map_Manager<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT, ACE_ES_Subscription_Info::SYNCH>;
template class ACE_Map_Iterator_Base<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT, ACE_ES_Subscription_Info::SYNCH>;
template class ACE_Map_Iterator_Base<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT, ACE_ES_Subscription_Info::SYNCH>;
template class ACE_Map_Iterator<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT, ACE_ES_Subscription_Info::SYNCH>;
template class ACE_Map_Reverse_Iterator<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT, ACE_ES_Subscription_Info::SYNCH>;
template class ACE_Map_Iterator<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT, ACE_ES_Subscription_Info::SYNCH>;
template class ACE_Map_Reverse_Iterator<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT, ACE_ES_Subscription_Info::SYNCH>;
template class ACE_Node<ACE_ES_Consumer_Rep *>;
template class ACE_Node<ACE_Push_Consumer_Proxy *>;
template class ACE_Node<ACE_Push_Supplier_Proxy *>;
template class ACE_Unbounded_Set<ACE_ES_Consumer_Rep *>;
template class ACE_Unbounded_Set<ACE_Push_Consumer_Proxy *>;
template class ACE_Unbounded_Set<ACE_Push_Supplier_Proxy *>;
template class ACE_Unbounded_Set_Iterator<ACE_ES_Consumer_Rep *>;
template class ACE_Unbounded_Set_Iterator<ACE_Push_Consumer_Proxy *>;
template class ACE_Unbounded_Set_Iterator<ACE_Push_Supplier_Proxy *>;

// For ACE_ES_Event_Container_Allocator.
template class ACE_Cached_Allocator<ACE_ES_Event_Container_Chunk, ACE_MEMORY_POOL_MUTEX>;
template class ACE_Cached_Allocator<ACE_ES_Event_Container_Chunk, ACE_SYNCH_MUTEX>;
template class ACE_Cached_Allocator<ACE_ES_Dispatch_Request_Chunk, ACE_MEMORY_POOL_MUTEX>;
template class ACE_Cached_Mem_Pool_Node<ACE_ES_Event_Container_Chunk>;
template class ACE_Cached_Mem_Pool_Node<ACE_ES_Dispatch_Request_Chunk>;
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Event_Container_Chunk>, ACE_Null_Mutex>;
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Event_Container_Chunk>, ACE_Thread_Mutex>;
template class ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Dispatch_Request_Chunk>, ACE_Null_Mutex>;
template class ACE_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Event_Container_Chunk> >;
template class ACE_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Dispatch_Request_Chunk> >;
template class ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>;

template class ACE_ES_Array_Iterator<ACE_ES_Consumer_Rep *>;
template class ACE_ES_Simple_Array<ACE_ES_Consumer_Rep *, 100>;

template class ACE_CORBA_var<ACE_ES_Event_Container>;

template class ACE_Node<TAO_EC_Gateway*>;
template class ACE_Unbounded_Set<TAO_EC_Gateway*>;
template class ACE_Unbounded_Set_Iterator<TAO_EC_Gateway*>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Atomic_Op<ACE_ES_MUTEX, int>
#pragma instantiate ACE_Map_Entry<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT>
#pragma instantiate ACE_Map_Entry<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT>
#pragma instantiate ACE_Map_Manager<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT, ACE_ES_Subscription_Info::SYNCH>
#pragma instantiate ACE_Map_Manager<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT, ACE_ES_Subscription_Info::SYNCH>
#pragma instantiate ACE_Map_Iterator_Base<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT, ACE_ES_Subscription_Info::SYNCH>
#pragma instantiate ACE_Map_Iterator_Base<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT, ACE_ES_Subscription_Info::SYNCH>
#pragma instantiate ACE_Map_Iterator<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT, ACE_ES_Subscription_Info::SYNCH>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_ES_Subscription_Info::EXT, ACE_ES_Subscription_Info::INT, ACE_ES_Subscription_Info::SYNCH>
#pragma instantiate ACE_Map_Iterator<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT, ACE_ES_Subscription_Info::SYNCH>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_ES_Subscription_Info::sEXT, ACE_ES_Subscription_Info::sINT, ACE_ES_Subscription_Info::SYNCH>
#pragma instantiate ACE_Node<ACE_ES_Consumer_Rep *>
#pragma instantiate ACE_Node<ACE_Push_Consumer_Proxy *>
#pragma instantiate ACE_Node<ACE_Push_Supplier_Proxy *>
#pragma instantiate ACE_Unbounded_Set<ACE_ES_Consumer_Rep *>
#pragma instantiate ACE_Unbounded_Set<ACE_Push_Consumer_Proxy *>
#pragma instantiate ACE_Unbounded_Set<ACE_Push_Supplier_Proxy *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_ES_Consumer_Rep *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Push_Consumer_Proxy *>
#pragma instantiate ACE_Unbounded_Set_Iterator<ACE_Push_Supplier_Proxy *>
#pragma instantiate ACE_Cached_Allocator<ACE_ES_Event_Container_Chunk, ACE_MEMORY_POOL_MUTEX>
#pragma instantiate ACE_Cached_Allocator<ACE_ES_Event_Container_Chunk, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Cached_Allocator<ACE_ES_Dispatch_Request_Chunk, ACE_MEMORY_POOL_MUTEX>
#pragma instantiate ACE_Cached_Mem_Pool_Node<ACE_ES_Event_Container_Chunk>
#pragma instantiate ACE_Cached_Mem_Pool_Node<ACE_ES_Dispatch_Request_Chunk>
#pragma instantiate ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Event_Container_Chunk>, ACE_Null_Mutex>
#pragma instantiate ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Event_Container_Chunk>, ACE_Thread_Mutex>
#pragma instantiate ACE_Locked_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Dispatch_Request_Chunk>, ACE_Null_Mutex>
#pragma instantiate ACE_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Event_Container_Chunk> >
#pragma instantiate ACE_Free_List<ACE_Cached_Mem_Pool_Node<ACE_ES_Dispatch_Request_Chunk> >
#pragma instantiate ACE_Malloc<ACE_LOCAL_MEMORY_POOL, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_ES_Array_Iterator<ACE_ES_Consumer_Rep *>
#pragma instantiate ACE_ES_Simple_Array<ACE_ES_Consumer_Rep *, 100>
#pragma instantiate ACE_CORBA_var<ACE_ES_Event_Container>

#pragma instantiate ACE_Node<TAO_EC_Gateway*>
#pragma instantiate ACE_Unbounded_Set<TAO_EC_Gateway*>
#pragma instantiate ACE_Unbounded_Set_Iterator<TAO_EC_Gateway*>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
