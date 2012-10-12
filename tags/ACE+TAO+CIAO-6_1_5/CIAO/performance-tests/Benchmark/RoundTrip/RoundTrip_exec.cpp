// $Id$

#include "RoundTrip_exec.h"

//-- ACE Scheduling Params
#include "ace/Sched_Params.h"
#include "ace/OS_NS_errno.h"
#include "ace/Log_Msg.h"

//=================================================================

void
set_priority ()
{
  int priority =
    (ACE_Sched_Params::priority_min (ACE_SCHED_FIFO)
     + ACE_Sched_Params::priority_max (ACE_SCHED_FIFO)) / 2;
  // Enable FIFO scheduling, e.g., RT scheduling class on Solaris.

  if (ACE_OS::sched_params (ACE_Sched_Params (ACE_SCHED_FIFO,
                                              priority,
                                              ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_WARNING,
                      "client (%P|%t): user is not superuser, "
                      "test runs in time-shared class\n"));
        }
      else
        ACE_ERROR ((LM_ERROR,
                    "client (%P|%t): sched_params failed\n"));
    }
}



MyImpl::RoundTrip_exec_i::RoundTrip_exec_i ()
{

}

MyImpl::RoundTrip_exec_i::~RoundTrip_exec_i ()
{
}

::Benchmark::CCM_LatencyTest *
MyImpl::RoundTrip_exec_i::get_latency ()
{
  return ::Benchmark::CCM_LatencyTest::_duplicate (this);
}

/* Main method that invokes the octet_sequence */
CORBA::Long
MyImpl::RoundTrip_exec_i::makeCall (CORBA::Long data)
{
  return data * data * data;
}

// Operations from Components::SessionComponent
void
MyImpl::RoundTrip_exec_i::set_session_context (
    Components::SessionContext_ptr ctx)
{
  ACE_DEBUG ((LM_TRACE, "MyImpl::RoundTrip_exec_i::set_session_context\n"));

  this->context_ =
    Benchmark::CCM_RoundTrip_Context::_narrow (ctx);

  if (CORBA::is_nil (this->context_.in ()))
    throw CORBA::INTERNAL ();
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::RoundTrip_exec_i::configuration_complete ()
{
}

void
MyImpl::RoundTrip_exec_i::ccm_activate ()
{
  ACE_DEBUG ((LM_TRACE, "MyImpl::RoundTrip_exec_i::ccm_activate\n"));
  //cout << "MyImpl::RoundTrip_exec_i::ccm_activate\n";

  // Starting method!
  //set_priority();
}

void
MyImpl::RoundTrip_exec_i::ccm_passivate ()
{
  ACE_DEBUG ((LM_TRACE, "MyImpl::RoundTrip_exec_i::ccm_passivate\n"));
}

void
MyImpl::RoundTrip_exec_i::ccm_remove ()
{
  ACE_DEBUG ((LM_INFO, "MyImpl::RoundTrip_exec_i::ccm_remove\n"));
}


MyImpl::RoundTripHome_exec_i::RoundTripHome_exec_i ()
{
}

MyImpl::RoundTripHome_exec_i::~RoundTripHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
MyImpl::RoundTripHome_exec_i::create ()
{
  return new MyImpl::RoundTrip_exec_i;
}


extern "C" ROUNDTRIP_EXEC_Export ::Components::HomeExecutorBase_ptr
createRoundTripHome_Impl (void)
{
  //cout << "Creating Round_trip impl" << endl;
  return new MyImpl::RoundTripHome_exec_i ();
}
