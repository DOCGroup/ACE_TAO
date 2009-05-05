#include "Worker_i.h"

Failure_Task::Failure_Task (CORBA::ORB_ptr orb,
                            long limit,
                            long & count)
  : condition_ (lock_),
    orb_ (CORBA::ORB::_duplicate (orb)),
    limit_ (limit),
    count_ (count),
    stop_ (false)
{
}

int
Failure_Task::svc (void)
{
  ACE_Guard <ACE_Thread_Mutex> guard (lock_);

  while (((limit_ == 0) || (count_ <= limit_)) && !stop_)
    {
      condition_.wait ();
    }

  orb_->shutdown (true);

  return 0;
}

void 
Failure_Task::signal (void)
{
  condition_.signal ();
}

void
Failure_Task::stop (void)
{
  stop_ = true;
  condition_.signal ();
}

Worker_i::Worker_i (CORBA::ORB_ptr orb,
                    PortableServer::POA_ptr poa,
                    const std::string & object_id,
		    StateSynchronizationAgent_ptr agent,
		    long invocations)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa)),
    object_id_ (object_id),
    agent_ (StateSynchronizationAgent::_duplicate (agent)),
    state_ (0),
    suicidal_count_ (invocations),
    task_ (orb_.in (), suicidal_count_, state_)
{
  timer_.calibrate ();
  task_.activate ();
}

void
Worker_i::run_method (CORBA::ULong prime_number,
                  CORBA::ULong kill)
{
  if (kill)
    ACE_OS::exit (1);
  
  ACE::is_prime (prime_number,
                 2,
                 prime_number / 2);
}

void
Worker_i::run_task (CORBA::Double execution_time,
                    bool ignore)
{
  /* do prime number calculation here */

  if (!ignore)
    {
      ++state_;

      agent_->state_changed (object_id_.c_str ());
    }

  task_.signal ();
}

void Worker_i::stop ()
{
  task_.stop ();
}

void
Worker_i::set_state (const CORBA::Any & state_value)
{
  // extract value to an intermediate long variable since it's not possible
  // to extract to a long & directly
  CORBA::Long value;

  if (state_value >>= value)
    state_ = value;
  else
    ACE_DEBUG ((LM_WARNING,
                "(%P|%t) Worker_i::set_state () "
                "could not extract state value from Any."));

  ACE_DEBUG ((LM_TRACE, "Worker_i::set_state (%d) called.\n", value));
}

CORBA::Any *
Worker_i::get_state ()
{
  // create new any object
  CORBA::Any_var state (new CORBA::Any);
  
  // create intermediate object with the value
  CORBA::Long value = state_;

  ACE_DEBUG ((LM_DEBUG, "(%P|%t) Worker_i::get_state returns %d.\n", value));

  // insert value into the any object
  *state <<= value;

  return state._retn ();
}

StateSynchronizationAgent_ptr
Worker_i::agent (void)
{
  return StateSynchronizationAgent::_duplicate (agent_.in ());
}
  
void
Worker_i::agent (StateSynchronizationAgent_ptr agent)
{
  agent_ = agent;
}

char *
Worker_i::object_id (void)
{
  return CORBA::string_dup (object_id_.c_str ());
}
  
void
Worker_i::object_id (const char * object_id)
{
  object_id_ = object_id;
}
