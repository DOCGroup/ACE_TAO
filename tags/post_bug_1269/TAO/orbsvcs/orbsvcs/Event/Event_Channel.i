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

ACE_INLINE const RtecEventChannelAdmin::SupplierQOS&
ACE_Push_Supplier_Proxy::qos (void) const
{
  return this->qos_;
}

// **************************************************

ACE_INLINE RtecEventChannelAdmin::ConsumerQOS &
ACE_Push_Consumer_Proxy::qos (void)
{
  return qos_;
}

ACE_INLINE int
ACE_Push_Consumer_Proxy::connected (void)
{
  return !CORBA::is_nil(push_consumer_.in ());
}

ACE_INLINE ACE_ES_Consumer_Correlation &
ACE_Push_Consumer_Proxy::correlation (void)
{
  return correlation_;
}

ACE_INLINE RtecEventChannelAdmin::ConsumerAdmin_ptr
ACE_ES_Consumer_Module::get_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE RtecEventChannelAdmin::SupplierAdmin_ptr
ACE_ES_Supplier_Module::get_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// **************************************************

ACE_INLINE RtecEventChannelAdmin::EventChannel_ptr
ACE_EventChannel::get_ref (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE RtecEventChannelAdmin::SupplierAdmin_ptr
ACE_EventChannel::for_suppliers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return supplier_module_->get_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE RtecEventChannelAdmin::ConsumerAdmin_ptr
ACE_EventChannel::for_consumers (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return consumer_module_->get_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
}

ACE_INLINE TAO_EC_Timer_Module*
ACE_EventChannel::timer_module (void) const
{
  return this->timer_module_;
}

// ************************************************************

// Makes a temporary Event_var and appends it to the <dest>.
ACE_INLINE void
operator += (TAO_EC_Event_Array &dest,
             const TAO_EC_Event &item)
{
  int length = dest.size ();
  dest.size (length + 1);
  dest[length] = item;
}

ACE_INLINE int
operator == (const RtecEventComm::Event &event1,
             const RtecEventComm::Event &event2)
{
  // Check if the sources are equal.  0 is a wildcard.
  if ((event1.header.source != 0)
      && (event2.header.source != 0)
      && (event1.header.source != event2.header.source))
    return 0;

  // Check if the types are equal.  ACE_ES_EVENT_ANY is a wildcard.
  if ((event1.header.type != ACE_ES_EVENT_ANY) &&
      (event2.header.type != ACE_ES_EVENT_ANY) &&
      (event1.header.type != event2.header.type))
    return 0;

  return 1;
}

ACE_INLINE int
operator != (const TAO_EC_Event& rhs, const TAO_EC_Event& lhs)
{
  if (rhs.empty () && lhs.empty ())
    return 1;
  if (rhs.empty () || lhs.empty ())
    return 0;
  return ! (rhs.event () == lhs.event ());
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
ACE_ES_Disjunction_Group::add_events (TAO_EC_Event_Array *outbox,
                                      TAO_EC_Event_Array *,
                                      u_long &)
{
  // Append the act.
  if (!this->act_.empty ())
    *outbox += this->act_;
}

ACE_INLINE void
ACE_ES_Disjunction_Group::set_act (RtecEventComm::Event &act)
{
  TAO_EC_Event_Set* set = TAO_EC_Event_Set::_create (act);
  if (set == 0)
    {
      // @@ throw an exception...
      ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Disjunction_Group::set_act"));
      return;
    }

  this->act_ = TAO_EC_Event (set, 0);
  TAO_EC_Event_Set::_release (set);
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
      // This routine was dead-code, but I'll leave it here until I
      // find out what it is supposed to do.
      ACE_ERROR ((LM_WARNING,
                  "EC (%t) RTU_Manager::should_preempt - obsolete\n"));

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
ACE_ES_Consumer_Rep_Timeout::ACE_ES_Consumer_Rep_Timeout (void)
  : timer_id_ (0),
    preemption_priority_ (ACE_Scheduler_MIN_PREEMPTION_PRIORITY),
    timeout_event_ ()
{
}

ACE_INLINE void
ACE_ES_Consumer_Rep_Timeout::init (ACE_ES_Consumer_Correlation *correlation,
                                   RtecEventChannelAdmin::Dependency &dep)
{
  TAO_EC_Event_Set *temp = TAO_EC_Event_Set::_create (dep.event);
  // @@ TODO throw an exception
  if (temp == 0)
    {
      ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_ES_Consumer_Rep_Timeout::init"));
      return;
    }

  this->timeout_event_ = TAO_EC_Event (temp, 0);
  TAO_EC_Event_Set::_release (temp);

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

      switch (rep_[x].event.header.type)
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
ACE_ES_Conjunction_Group::add_events (TAO_EC_Event_Array *outbox,
                                      TAO_EC_Event_Array *pending_events,
                                      u_long &pending_flags)
{
  // Append the act first.
  if (!this->act_.empty ())
    *outbox += this->act_;

  u_long fv = forward_value_;
  int x = 0;
  while (fv > 0)
    {
      // If this type_id is part of the correlation, then append each
      // event pending to the outbox.
      if (ACE_BIT_ENABLED (forward_value_, ACE_INT2BIT[x]))
        {
          // Step through each of the pending events.
          TAO_EC_Event_Array &pending = pending_events[x];

          size_t outbox_end = 0;
          if (outbox != 0)
            {
              outbox_end = outbox->size ();
              outbox->size (outbox_end + pending.size ());
            }
          for (CORBA::ULong i = 0; i < pending.size (); ++i)
            {
              if (pending[i].empty ())
                continue;

              // Add the pending event to the outbox.
              if (outbox != 0)
                outbox->set (pending[i], outbox_end++);

              // Remove the event from the pending events array.
              pending[i].clear ();
            }

          // Reset the array length.
          pending.size (0);
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

ACE_INLINE int
ACE_EventChannel::cancel_timer (RtecScheduler::OS_Priority preemption_priority,
                                int id,
                                ACE_Command_Base *&act)
{
    return this->timer_module ()->cancel_timer (preemption_priority,
                                                id,
                                                act);
}

ACE_INLINE RtecScheduler::Scheduler_ptr
ACE_EventChannel::scheduler (void)
{
  return RtecScheduler::Scheduler::_duplicate (this->scheduler_.in ());
}

// ************************************************************
