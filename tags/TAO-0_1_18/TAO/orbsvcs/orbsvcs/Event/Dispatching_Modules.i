/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_ES_Dispatch_Request::ACE_ES_Dispatch_Request (void) :
  priority_ (0),
  rt_info_ (0),
  dispatching_module_ (0),
  use_single_event_ (0),
  consumer_ (0),
  single_event_ (),
  event_set_ ()
{
}

ACE_INLINE
ACE_ES_Dispatch_Request::~ACE_ES_Dispatch_Request (void)
{
}

ACE_INLINE
ACE_ES_Dispatch_Request::ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer, 
						  RtecScheduler::handle_t rt_info) :
  priority_ (0),
  rt_info_ (rt_info),
  dispatching_module_ (0),
  use_single_event_ (0),
  consumer_ (consumer),
  single_event_ (),
  event_set_ ()
{
}

ACE_INLINE
ACE_ES_Dispatch_Request::ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
						  const Event_Set &event_set,
						  RtecScheduler::handle_t rt_info) :
  priority_ (0),
  rt_info_ (rt_info),
  dispatching_module_ (0),
  use_single_event_ (0),
  consumer_ (consumer),
  single_event_ (),
  event_set_ (event_set)
{
}

ACE_INLINE
ACE_ES_Dispatch_Request::ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
						  ACE_ES_Event_Container *event,
						  RtecScheduler::handle_t rt_info) :
  priority_ (0),
  rt_info_ (rt_info),
  dispatching_module_ (0),
  use_single_event_ (0),
  consumer_ (consumer),
  single_event_ (),
  event_set_ ()
{
  event_set_ += event;
}

ACE_INLINE
ACE_ES_Dispatch_Request::ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
						  const RtecEventComm::Time &time,
						  RtecScheduler::handle_t rt_info) :
  priority_ (0),
  rt_info_ (rt_info),
  dispatching_module_ (0),
  use_single_event_ (1),
  consumer_ (consumer),
  single_event_ (),
  event_set_ ()
{
  single_event_.creation_time_ = time;
  single_event_.type_ = ACE_ES_EVENT_TIMEOUT;
}

ACE_INLINE void
ACE_ES_Dispatch_Request::set (ACE_ES_Dispatching_Base *dispatching_module,
			      RtecScheduler::OS_Priority preemption_priority,
			      RtecScheduler::Sub_Priority sub_priority)
{
  dispatching_module_ = dispatching_module;
  priority_ = preemption_priority;
  ACE_Message_Block::msg_priority (sub_priority);
}

ACE_INLINE ACE_Push_Consumer_Proxy *
ACE_ES_Dispatch_Request::consumer (void) const
{
  return consumer_;
}

ACE_INLINE const ACE_ES_Dispatch_Request::Event_Set &
ACE_ES_Dispatch_Request::event_set (void) const
{
  return event_set_;
}

ACE_INLINE ACE_ES_Dispatch_Request::Event_Set &
ACE_ES_Dispatch_Request::event_set (void)
{
  return event_set_;
}

ACE_INLINE CORBA::ULong
ACE_ES_Dispatch_Request::number_of_events (void) const
{
  if (use_single_event_)
    return 1;
  else
    return event_set_.length ();
}

ACE_INLINE RtecScheduler::OS_Priority
ACE_ES_Dispatch_Request::priority (void)
{
  return priority_;
}

ACE_INLINE RtecScheduler::handle_t
ACE_ES_Dispatch_Request::rt_info (void)
{
  return rt_info_;
}

// ************************************************************

ACE_INLINE
ACE_ES_Dispatching_Base::ACE_ES_Dispatching_Base (ACE_EventChannel *channel)
  : channel_ (channel),
    thr_count_ (0),
    up_ (0),
    down_ (0)
{
}

ACE_INLINE void
ACE_ES_Dispatching_Base::open (ACE_ES_Consumer_Module *up,
			       ACE_ES_Correlation_Module *down)
{
  up_ = up;
  down_ = down;
  // 1 == 2.
}

ACE_INLINE void 
ACE_ES_Dispatching_Base::connected (ACE_Push_Consumer_Proxy *consumer,
				    CORBA::Environment &_env)
{
  down_->connected (consumer, _env);
}

ACE_INLINE void 
ACE_ES_Dispatching_Base::disconnected (ACE_Push_Consumer_Proxy *consumer)
{
  // Do nothing.
  ACE_UNUSED_ARG (consumer);
}

ACE_INLINE void
ACE_ES_Dispatching_Base::disconnecting (ACE_Push_Consumer_Proxy *consumer,
					CORBA::Environment &_env)
{
  down_->disconnecting (consumer, _env);
}

ACE_INLINE void
ACE_ES_Dispatching_Base::dispatch_queue_closed (ACE_ES_Dispatch_Queue *q)
{
  ACE_UNUSED_ARG (q);
}

// Just forward the request.  This is basically a hook for the RTU
// stuff. 
ACE_INLINE int
ACE_ES_Dispatching_Base::dispatch_event (ACE_ES_Dispatch_Request *request,
					 u_long &command_action)
{
  ACE_TIMEPROBE ("  decode the event");

  TAO_TRY
    {
      // Forward the request.
      up_->push (request, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "ACE_ES_Dispatching_Base::dispatch_event unknown exception.\n"));
    }
  TAO_ENDTRY;

  // Tell our caller to release the request.
  command_action = ACE_RT_Task_Command::RELEASE;

  // Return zero so our calling thread does not exit.
  return 0;
}

ACE_INLINE void
ACE_ES_Dispatching_Base::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) ACE_ES_Dispatching_Base module shutting down.\n"));
}

// ************************************************************

#if defined (ACE_WIN32)
ACE_INLINE
ACE_ES_ReactorEx_NS::ACE_ES_ReactorEx_NS (ACE_Event_Handler *eh)
  : ACE_Notification_Strategy (eh, ACE_Event_Handler::NULL_MASK)
{
}

ACE_INLINE int
ACE_ES_ReactorEx_NS::open (void)
{
  return ACE_Task_Manager::instance ()->
    GetReactorTask (0)->get_reactor ().register_handler (eh_, event_.handle ());
}

ACE_INLINE void
ACE_ES_ReactorEx_NS::shutdown (void)
{
  ACE_Task_Manager::instance ()->GetReactorTask (0)->
    get_reactor ().remove_handler (eh_, ACE_Event_Handler::DONT_CALL);
}

ACE_INLINE int 
ACE_ES_ReactorEx_NS::notify (void)
{
  return event_.signal ();
}

ACE_INLINE int 
ACE_ES_ReactorEx_NS::notify (ACE_Event_Handler *eh,
			       ACE_Reactor_Mask mask)
{
  return event_.signal ();
}

#else /* !defined (ACE_WIN32) */
// This class is only necessary on non-win32 platforms.
ACE_INLINE
ACE_ES_Reactor_NS::ACE_ES_Reactor_NS (ACE_Event_Handler *eh)
  : ACE_Reactor_Notification_Strategy (&(ACE_Task_Manager::instance ()->
				       GetReactorTask (0)->get_reactor ()),
				       eh, ACE_Event_Handler::READ_MASK)
{
}

ACE_INLINE int
ACE_ES_Reactor_NS::open (void)
{
  return 0;
}

ACE_INLINE void
ACE_ES_Reactor_NS::shutdown (void)
{
}

#endif /* ACE_WIN32 */

// ************************************************************
