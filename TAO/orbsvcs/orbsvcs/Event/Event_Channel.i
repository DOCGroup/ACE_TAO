/* -*- C++ -*- */
// $Id$

const unsigned int ACE_INT2BIT[32] =
{
  1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192,
  16384, 32768, 65536, 131072, 262144, 524288, 1048576, 2097152,
  4194304, 8388608, 16777216, 33554432, 67108864, 134217728,
  268435456, 536870912, 1073741824, 2147483648u,
};

// **************************************************

ACE_INLINE RtecEventChannelAdmin::ProxyPushConsumer_ptr
ACE_Push_Supplier_Proxy::get_ref (void)
{
  CORBA::Environment env;
  return this->_this (env);
}

ACE_INLINE int
ACE_Push_Supplier_Proxy::connected (void)
{
  return !CORBA::is_nil((CORBA::Object*) push_supplier_);
}

ACE_INLINE ACE_ES_Subscription_Info &
ACE_Push_Supplier_Proxy::subscription_info (void)
{
  return subscription_info_;
}

ACE_INLINE RtecEventChannelAdmin::SupplierQOS &
ACE_Push_Supplier_Proxy::qos (void)
{
  return qos_;
}

ACE_INLINE int
ACE_Push_Supplier_Proxy::operator== (const RtecEventComm::EventSourceID rhs)
{
  // Pointer comparison is fine for now.
  return (source_id_ == rhs);
}

ACE_INLINE RtecEventComm::EventSourceID
ACE_Push_Supplier_Proxy::source_id (void)
{
  return source_id_;
}

// **************************************************

ACE_INLINE RtecEventChannelAdmin::ProxyPushSupplier_ptr
ACE_Push_Consumer_Proxy::get_ref (void)
{
  CORBA::Environment env;
  return this->_this (env);
}

ACE_INLINE RtecEventChannelAdmin::ConsumerQOS &
ACE_Push_Consumer_Proxy::qos (void)
{
  return qos_;
}

ACE_INLINE int
ACE_Push_Consumer_Proxy::connected (void)
{
  return !CORBA::is_nil(push_consumer_);
}

ACE_INLINE ACE_ES_Consumer_Correlation &
ACE_Push_Consumer_Proxy::correlation (void)
{
  return correlation_;
}

ACE_INLINE RtecEventChannelAdmin::ConsumerAdmin_ptr
ACE_ES_Consumer_Module::get_ref (void)
{
  CORBA::Environment env;
  return this->_this (env);
}

ACE_INLINE RtecEventChannelAdmin::SupplierAdmin_ptr
ACE_ES_Supplier_Module::get_ref (void)
{
  CORBA::Environment env;
  return this->_this (env);
}

// **************************************************

ACE_INLINE RtecEventChannelAdmin::EventChannel_ptr
ACE_EventChannel::get_ref (void)
{
  CORBA::Environment env;
  return this->_this (env);
}

ACE_INLINE RtecEventChannelAdmin::SupplierAdmin_ptr
ACE_EventChannel::for_suppliers (CORBA::Environment &)
{
  return supplier_module_->get_ref ();
}

ACE_INLINE RtecEventChannelAdmin::ConsumerAdmin_ptr
ACE_EventChannel::for_consumers (CORBA::Environment &)
{
  return consumer_module_->get_ref ();
}

ACE_INLINE ACE_ES_Priority_Timer *
ACE_EventChannel::timer (void)
{
  return timer_;
}

// ************************************************************

// Makes a temporary Event_var and appends it to the <dest>.
ACE_INLINE void
operator += (ACE_CORBA_Sequence<ACE_ES_Event_Container_var> &dest,
	     ACE_ES_Event_Container *item)
{
  int length = dest.length ();
  dest.length (length + 1);
  dest[length] = item;
}

/*
// Makes a temporary Event_var and appends it to the <dest>.
ACE_INLINE void
operator += (ACE_CORBA_Sequence<RtecEventComm::Event_var> &dest,
	     RtecEventComm::Event *item)
{
  //  RtecEventComm::Event_var event (item);
  int length = dest.length ();
  dest.length (length + 1);
  dest[length] = item;
}
*/

ACE_INLINE int
operator == (const RtecEventComm::Event &event1,
	     const RtecEventComm::Event &event2)
{
  // Check if the sources are equal.  0 is a wildcard.
  if ((event1.source_ != 0)
      && (event2.source_ != 0)
      && (event1.source_ != event2.source_))
    return 0;

  // Check if the types are equal.  ACE_ES_EVENT_ANY is a wildcard.
  if ((event1.type_ != ACE_ES_EVENT_ANY) &&
      (event2.type_ != ACE_ES_EVENT_ANY) &&
      (event1.type_ != event2.type_))
    return 0;

  return 1;
}

// ************************************************************

ACE_INLINE
ACE_ES_ACT::ACE_ES_ACT (void)
  : has_act_ (0)
{
}

// ************************************************************

ACE_INLINE
ACE_ES_Disjunction_Group::ACE_ES_Disjunction_Group (void) :
  act_ (),
  deadline_timer_rep_ (0),
  correlation_module_ (0)
{
}

ACE_INLINE void
ACE_ES_Disjunction_Group::set_correlation_module (ACE_ES_Correlation_Module *cm)
{
  correlation_module_ = cm;
}

ACE_INLINE void
ACE_ES_Disjunction_Group::reschedule_deadline (void)
{
  if (deadline_timer_rep_ != 0)
    {
      if (correlation_module_->reschedule_timeout (deadline_timer_rep_) == -1)
	ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Disjunction_Group::reschedule_deadline"));
    }
}

ACE_INLINE int
ACE_ES_Disjunction_Group::set_deadline_timeout (ACE_ES_Consumer_Rep_Timeout *cr)
{
  deadline_timer_rep_ = cr;
  // Schedule the timeout.
  if (correlation_module_->schedule_timeout (deadline_timer_rep_) == -1)
    return -1;
  else
    return 0;
}

ACE_INLINE void
ACE_ES_Disjunction_Group::add_events (Event_Set *outbox,
				      Event_Set *pending_events,
				      u_long &pending_flags)
{
  ACE_UNUSED_ARG (pending_events);
  ACE_UNUSED_ARG (pending_flags);

  // Append the act.
  if (act_ != 0)
    *outbox += act_;
}

ACE_INLINE void
ACE_ES_Disjunction_Group::set_act (RtecEventComm::Event &act)
{
  ACE_ES_Event_Container *temp = new ACE_ES_Event_Container (act);
  if (temp == 0)
    {
      ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Disjunction_Group::set_act"));
      return;
    }

  act_ = temp;
  temp->_release ();
}

// ************************************************************

ACE_INLINE
ACE_ES_Consumer_Rep::ACE_ES_Consumer_Rep (void) :
  disconnected_ (0),
  suspended_ (0),
  correlation_type_ (ACE_ES_Consumer_Rep::NO_CORRELATION),
  dependency_ (0),
  correlation_ (0),
  type_id_ (0),
  disjunction_group_ (0),
  ref_count_ (1)
{
}

ACE_INLINE void
ACE_ES_Consumer_Rep::init (ACE_ES_Consumer_Correlation *correlation,
			   RtecEventChannelAdmin::Dependency& dependency)
{
  dependency_ = &dependency;
  correlation_ = correlation;
}

ACE_INLINE RtecEventChannelAdmin::Dependency*
ACE_ES_Consumer_Rep::dependency (void)
{
  return dependency_;
}

ACE_INLINE int
ACE_ES_Consumer_Rep::type_id (void)
{
  return type_id_;
}

ACE_INLINE void
ACE_ES_Consumer_Rep::type_id (int id)
{
  type_id_ = id;
}

ACE_INLINE ACE_ES_Consumer_Correlation *
ACE_ES_Consumer_Rep::correlation (void)
{
  return correlation_;
}

ACE_INLINE u_long
ACE_ES_Consumer_Rep::correlation_type (void)
{
  return correlation_type_;
}

ACE_INLINE void
ACE_ES_Consumer_Rep::correlation_type (u_long ct)
{
  correlation_type_ = ct;
}

ACE_INLINE int
ACE_ES_Consumer_Rep::add_disjunction_group (ACE_ES_Disjunction_Group &dg)
{
  if (disjunction_group_ != 0)
    ACE_ERROR ((LM_ERROR, "ACE_ES_Consumer_Rep::add_disjunction_group: "
		"disjunction_group already set!\n"));
  disjunction_group_ = &dg;
  return 0;
}

ACE_INLINE ACE_ES_Disjunction_Group *
ACE_ES_Consumer_Rep::top_group (void)
{
  return disjunction_group_;
}

ACE_INLINE void
ACE_ES_Consumer_Rep::reschedule_deadlines (void)
{
  if (disjunction_group_ != 0)
    disjunction_group_->reschedule_deadline ();
}

ACE_INLINE int
ACE_ES_Consumer_Rep::receiving_events (void)
{
  return suspended_ == 0 && disconnected_ == 0;
}

ACE_INLINE void
ACE_ES_Consumer_Rep::suspend (void)
{
  suspended_ = 1;
}

ACE_INLINE void
ACE_ES_Consumer_Rep::resume (void)
{
  suspended_ = 0;
}

ACE_INLINE void
ACE_ES_Consumer_Rep::_duplicate (void)
{
  // This is atomic.
  ref_count_++;
}

ACE_INLINE void
ACE_ES_Consumer_Rep::_release (void)
{
  // This is atomic.  rc is because we want to avoid Atomic_Op's
  // operator==.  Don't change this code unless you think you're more
  // studly than ACE_Atomic_Op.
  int rc = --ref_count_;

  if (rc == 0)
    delete this;
}

ACE_INLINE int
ACE_ES_Consumer_Rep::disconnected (void)
{
  return disconnected_;
}

ACE_INLINE void
ACE_ES_Consumer_Rep::disconnect (void)
{
  disconnected_ = 1;
}

// ************************************************************

ACE_INLINE ACE_RTU_Manager *
ACE_EventChannel::rtu_manager (void)
{
  return rtu_manager_;
}

ACE_INLINE
ACE_RTU_Manager::ACE_RTU_Manager (int active)
  : active_ (active),
    should_preempt_ (0),
    not_done_ (0),
    priority_ (ACE_Scheduler_MIN_PREEMPTION_PRIORITY)
{
}

ACE_INLINE int
ACE_RTU_Manager::should_preempt (void)
{
  if (!active_)
    return 0;
  else
    {
      // Expire any timers.  Am I evil for putting this here?
      ACE_Time_Value tv;
      if (ACE_Task_Manager::instance ()->
	  GetReactorTask (0)->get_reactor ().handle_events (&tv) == -1)
	ACE_ERROR ((LM_ERROR, "%p.\n",
		    "ACE_RTU_Manager::should_preempt"));

      int should_preempt = should_preempt_;
      should_preempt_ = 0;
      return should_preempt;
    }
}

ACE_INLINE void
ACE_RTU_Manager::should_preempt (int s)
{
  should_preempt_ = s;
}

ACE_INLINE int
ACE_RTU_Manager::not_done (void)
{
  int not_done = not_done_;
  not_done_ = 0;
  return not_done;
}

ACE_INLINE void
ACE_RTU_Manager::not_done (int nd)
{
  not_done_ = nd;
}

ACE_INLINE RtecScheduler::OS_Priority
ACE_RTU_Manager::priority (void)
{
  return priority_;
}

ACE_INLINE void
ACE_RTU_Manager::priority (RtecScheduler::OS_Priority p)
{
  priority_ = p;
}

// ************************************************************

ACE_INLINE
ACE_ES_Consumer_Rep_Timeout::ACE_ES_Consumer_Rep_Timeout (void) :
  timer_id_ (0),
  preemption_priority_ (ACE_Scheduler_MIN_PREEMPTION_PRIORITY),
  timeout_event_ ()
{
}

ACE_INLINE void
ACE_ES_Consumer_Rep_Timeout::init (ACE_ES_Consumer_Correlation *correlation,
				   RtecEventChannelAdmin::Dependency &dep)
{
  ACE_ES_Event_Container *temp = new ACE_ES_Event_Container (dep.event);
  if (temp == 0)
    {
      ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Consumer_Rep_Timeout::init"));
      return;
    }

  timeout_event_ = temp;
  temp->_release ();

  ACE_ES_Consumer_Rep::init (correlation, dep);
}

ACE_INLINE int
ACE_ES_Consumer_Rep_Timeout::timer_id (void)
{
  return timer_id_;
}

ACE_INLINE void
ACE_ES_Consumer_Rep_Timeout::timer_id (int id)
{
  timer_id_ = id;
}

ACE_INLINE RtecScheduler::OS_Priority
ACE_ES_Consumer_Rep_Timeout::preemption_priority (void)
{
  return preemption_priority_;
}

ACE_INLINE void
ACE_ES_Consumer_Rep_Timeout::preemption_priority (RtecScheduler::OS_Priority p)
{
  preemption_priority_ = p;
}

// ************************************************************

ACE_INLINE void
ACE_ES_Consumer_Correlation::suspend (void)
{
  for (int x=0; x < n_timer_reps_; x++)
    timer_reps_[x].suspend ();

  for (int y=0; y < n_consumer_reps_; y++)
    consumer_reps_[y]->suspend ();
}

ACE_INLINE void
ACE_ES_Consumer_Correlation::resume (void)
{
  for (int x=0; x < n_timer_reps_; x++)
    timer_reps_[x].resume ();

  for (int y=0; y < n_consumer_reps_; y++)
    consumer_reps_[y]->resume ();
}

// ************************************************************

ACE_INLINE
ACE_ES_Dependency_Iterator::ACE_ES_Dependency_Iterator (RtecEventChannelAdmin::DependencySet &rep) :
  rt_info_ (0),
  rep_ (rep),
  index_ (-1),
  n_conjunctions_ (0),
  n_disjunctions_ (0),
  n_timeouts_ (0),
  n_events_ (0)
{
}

ACE_INLINE int
ACE_ES_Dependency_Iterator::advance_dependency (void)
{
  index_++;
  if ((CORBA::ULong) index_ >= rep_.length ())
    return -1;
  else
    return 0;
}

ACE_INLINE RtecEventChannelAdmin::Dependency &
ACE_ES_Dependency_Iterator::operator *(void)
{
  return rep_[index_];
}

ACE_INLINE int
ACE_ES_Dependency_Iterator::parse (void)
{
  for (CORBA::ULong x = 0; x < rep_.length (); x++)
    {
      if (rt_info_ == 0)
	rt_info_ = rep_[x].rt_info;

      switch (rep_[x].event.type_)
	{
	case ACE_ES_CONJUNCTION_DESIGNATOR:
	  n_conjunctions_++;
	  break;

	case ACE_ES_DISJUNCTION_DESIGNATOR:
	  n_disjunctions_++;
	  break;

	case ACE_ES_EVENT_TIMEOUT:
	case ACE_ES_EVENT_INTERVAL_TIMEOUT:
	case ACE_ES_EVENT_DEADLINE_TIMEOUT:
	  n_timeouts_++;
	  break;

	default:
	  n_events_++;
	  break;
	}
    }

  return 0;
}

ACE_INLINE int
ACE_ES_Dependency_Iterator::n_conjunctions (void)
{
  return n_conjunctions_;
}

ACE_INLINE int
ACE_ES_Dependency_Iterator::n_disjunctions (void)
{
  return n_disjunctions_;
}

ACE_INLINE int
ACE_ES_Dependency_Iterator::n_timeouts (void)
{
  return n_timeouts_;
}

ACE_INLINE int
ACE_ES_Dependency_Iterator::n_events (void)
{
  return n_events_;
}

ACE_INLINE RtecScheduler::handle_t
ACE_ES_Dependency_Iterator::first_rt_info (void)
{
  return rt_info_;
}

// ************************************************************

ACE_INLINE
ACE_ES_Conjunction_Group::ACE_ES_Conjunction_Group (void) :
  forward_value_ (0)
{
}

ACE_INLINE int
ACE_ES_Conjunction_Group::add_type (int type_id)
{
  ACE_SET_BITS (forward_value_, ACE_INT2BIT[type_id]);
  return 0;
}

/*
  // Set length bits.
  for (int x=0; x < length; x++)
  {
  forward_value_ <<= 1;
  forward_value_ |= 1;
  }
  */

ACE_INLINE int
ACE_ES_Conjunction_Group::should_forward (u_long pending_flags)
{
  if ((forward_value_ & pending_flags) == forward_value_)
    return 1;
  else
    return 0;
}

ACE_INLINE void
ACE_ES_Conjunction_Group::add_events (Event_Set *outbox,
				      Event_Set *pending_events,
				      u_long &pending_flags)
{
  // Append the act first.
  if (act_ != 0)
    *outbox += act_;

  u_long fv = forward_value_;
  int x = 0;
  while (fv > 0)
    {
      // If this type_id is part of the correlation, then append each
      // event pending to the outbox.
      if (ACE_BIT_ENABLED (forward_value_, ACE_INT2BIT[x]))
	{
	  // Step through each of the pending events.
	  Event_Set &pending = pending_events[x];
	  for (CORBA::ULong y=0; y < pending.length (); y++)
	    {
	      // Add the pending event to the outbox.
	      if (outbox != 0)
		*outbox += pending[y];
	      // Remove the event from the pending events array.
	      pending[y] = 0;
	    }

	  // Reset the array length.
	  pending.length (0);
	  // Since we just emptied the events for this type, clear the
	  // x^th bit in pending flags.
	  ACE_CLR_BITS (pending_flags, ACE_INT2BIT[x]);
	  // Clear the x^th bit in fv.
	  ACE_CLR_BITS (fv, ACE_INT2BIT[x]);
	}

      x++;
    }
}

// ************************************************************
