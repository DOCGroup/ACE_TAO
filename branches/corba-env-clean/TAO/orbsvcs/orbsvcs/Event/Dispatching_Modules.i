/* -*- C++ -*- */
// $Id$

ACE_INLINE
ACE_ES_Dispatch_Request::ACE_ES_Dispatch_Request (void)
  : priority_ (0),
    rt_info_ (0),
    dispatching_module_ (0),
    use_single_event_ (0),
    consumer_ (0),
    event_set_ (1)
{
}

ACE_INLINE
ACE_ES_Dispatch_Request::~ACE_ES_Dispatch_Request (void)
{
}

ACE_INLINE
ACE_ES_Dispatch_Request::
ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                         RtecScheduler::handle_t rt_info)
  : priority_ (0),
    rt_info_ (rt_info),
    dispatching_module_ (0),
    use_single_event_ (0),
    consumer_ (consumer),
    event_set_ (1)
{
}

ACE_INLINE
ACE_ES_Dispatch_Request::
ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                         const TAO_EC_Event_Array &event_set,
                         RtecScheduler::handle_t rt_info)
  : priority_ (0),
    rt_info_ (rt_info),
    dispatching_module_ (0),
    use_single_event_ (0),
    consumer_ (consumer),
    single_event_ (),
    event_set_ (event_set)
{
}

#if 0
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
#endif

ACE_INLINE
ACE_ES_Dispatch_Request::
ACE_ES_Dispatch_Request (ACE_Push_Consumer_Proxy *consumer,
                         const RtecEventComm::Time &time,
                         RtecScheduler::handle_t rt_info)
  : priority_ (0),
    rt_info_ (rt_info),
    dispatching_module_ (0),
    use_single_event_ (1),
    consumer_ (consumer),
    event_set_ (1)
{
  // @@ TODO this could be more efficient
  RtecEventComm::Event tmp;
  tmp.header.creation_time = time;
  tmp.header.type = ACE_ES_EVENT_TIMEOUT;
  TAO_EC_Event_Set* set =
    TAO_EC_Event_Set::_create (tmp);
  this->single_event_ = TAO_EC_Event (set, set->length ());

  TAO_EC_Event_Set::_release (set);
}

ACE_INLINE void
ACE_ES_Dispatch_Request::set (ACE_ES_Dispatching_Base *dispatching_module,
                              RtecScheduler::OS_Priority preemption_priority,
                              RtecScheduler::Preemption_Subpriority_t sub_priority)
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

ACE_INLINE const TAO_EC_Event_Array&
ACE_ES_Dispatch_Request::event_set (void) const
{
  return event_set_;
}

ACE_INLINE TAO_EC_Event_Array&
ACE_ES_Dispatch_Request::event_set (void)
{
  return event_set_;
}

ACE_INLINE CORBA::ULong
ACE_ES_Dispatch_Request::number_of_events (void) const
{
  if (this->use_single_event_)
    return 1;
  else
    return this->event_set_.size ();
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
ACE_ES_Dispatching_Base::connected (ACE_Push_Consumer_Proxy *consumer
                                    TAO_ENV_ARG_DECL)
{
  down_->connected (consumer TAO_ENV_ARG_PARAMETER);
}

ACE_INLINE void
ACE_ES_Dispatching_Base::disconnected (ACE_Push_Consumer_Proxy *consumer)
{
  // Do nothing.
  ACE_UNUSED_ARG (consumer);
}

ACE_INLINE void
ACE_ES_Dispatching_Base::disconnecting (ACE_Push_Consumer_Proxy *consumer
                                        TAO_ENV_ARG_DECL)
{
  down_->disconnecting (consumer TAO_ENV_ARG_PARAMETER);
}

ACE_INLINE void
ACE_ES_Dispatching_Base::dispatch_queue_closed (ACE_ES_Dispatch_Queue *q)
{
  ACE_UNUSED_ARG (q);
}

// ************************************************************

#if defined (ACE_WIN32)
ACE_INLINE
ACE_ES_ReactorEx_NS::ACE_ES_ReactorEx_NS (ACE_Event_Handler *eh,
                                          TAO_EC_Timer_Module* tm)
  : ACE_Notification_Strategy (eh, ACE_Event_Handler::NULL_MASK),
    timer_module_ (tm)
{
}

ACE_INLINE int
ACE_ES_ReactorEx_NS::open (void)
{
  return this->timer_module_->register_handler (0,
                                                eh_,
                                                event_.handle ());
}

ACE_INLINE void
ACE_ES_ReactorEx_NS::shutdown (void)
{
// @@ TODO: Fix this.
//  this->timer_module_->remove_handler (0,
//                                     eh_,
//                                     ACE_Event_Handler::DONT_CALL);
}

ACE_INLINE int
ACE_ES_ReactorEx_NS::notify (void)
{
  return this->event_.signal ();
}

ACE_INLINE int
ACE_ES_ReactorEx_NS::notify (ACE_Event_Handler *,
                             ACE_Reactor_Mask)
{
  return this->event_.signal ();
}

#else /* !defined (ACE_WIN32) */
// This class is only necessary on non-win32 platforms.
ACE_INLINE
ACE_ES_Reactor_NS::ACE_ES_Reactor_NS (ACE_Event_Handler *eh,
                                      TAO_EC_Timer_Module *tm)
  : ACE_Reactor_Notification_Strategy (tm->reactor (0),
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
