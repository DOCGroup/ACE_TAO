// $Id$
 
#include "RoundTrip_exec.h"

//-- ACE Scheduling Params
#include "ace/Sched_Params.h"

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
          ACE_DEBUG ((LM_DEBUG,
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
MyImpl::RoundTrip_exec_i::get_latency (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::Benchmark::CCM_LatencyTest::_duplicate (this);
}

/* Main method that invokes the octet_sequence */
CORBA::Long
MyImpl::RoundTrip_exec_i::makeCall (CORBA::Long data)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return data*data*data;
  
}

// Operations from Components::SessionComponent
void
MyImpl::RoundTrip_exec_i::set_session_context (Components::SessionContext_ptr ctx
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::set_session_context\n"));
  //cout << "MyImpl::RoundTrip_exec_i::set_session_context\n" << endl;

  this->context_ =
    Benchmark::CCM_RoundTrip_Context::_narrow (ctx
                                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (CORBA::is_nil (this->context_.in ()))
    ACE_THROW (CORBA::INTERNAL ());
  // Urm, we actually discard exceptions thown from this operation.
}

void
MyImpl::RoundTrip_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::ccm_activate\n"));
  //cout << "MyImpl::RoundTrip_exec_i::ccm_activate\n";

  // Starting method!
  //set_priority();
}

void
MyImpl::RoundTrip_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::ccm_passivate\n"));
}

void
MyImpl::RoundTrip_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::ccm_remove\n"));
}


MyImpl::RoundTripHome_exec_i::RoundTripHome_exec_i ()
{
}

MyImpl::RoundTripHome_exec_i::~RoundTripHome_exec_i ()
{
}

::Components::EnterpriseComponent_ptr
MyImpl::RoundTripHome_exec_i::create (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  return new MyImpl::RoundTrip_exec_i ();
}


extern "C" ROUNDTRIP_EXEC_Export ::Components::HomeExecutorBase_ptr
createRoundTripHome_Impl (void)
{
  //cout << "Creating Round_trip impl" << endl;
  return new MyImpl::RoundTripHome_exec_i ();
}
