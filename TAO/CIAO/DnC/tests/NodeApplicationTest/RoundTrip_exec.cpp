// $Id$

#include "RoundTrip_exec.h"
#include "CIAO_common.h"

//-- ACE Scheduling Params
#include "ace/OS_NS_errno.h"

//=================================================================

MyImpl::RoundTrip_exec_i::RoundTrip_exec_i ()
{

}

MyImpl::RoundTrip_exec_i::~RoundTrip_exec_i ()
{
}

/*
::NodeAppTest::CCM_LatencyTest *
MyImpl::RoundTrip_exec_i::get_latency (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return ::NodeAppTest::CCM_LatencyTest::_duplicate (this);
}
*/

/* Main method that invokes the octet_sequence */
CORBA::Long
MyImpl::RoundTrip_exec_i::cube_long (CORBA::Long data)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return data*data*data;
}

// Operations from Components::SessionComponent
void
MyImpl::RoundTrip_exec_i::set_session_context (Components::SessionContext_ptr 
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::set_session_context\n"));
  ACE_CHECK;

}

void
MyImpl::RoundTrip_exec_i::ccm_activate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::ccm_activate\n"));

}

void
MyImpl::RoundTrip_exec_i::ccm_passivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::ccm_passivate\n"));
}

void
MyImpl::RoundTrip_exec_i::ccm_remove (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::ccm_remove\n"));
}

void
MyImpl::RoundTrip_exec_i::ciao_preactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::ciao_preactivate\n"));

}

void
MyImpl::RoundTrip_exec_i::ciao_postactivate (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   Components::CCMException))
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "MyImpl::RoundTrip_exec_i::ciao_postactivate\n"));
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


extern "C" NODEAPPTEST_ROUNDTRIP_EXEC_Export
::Components::HomeExecutorBase_ptr
createRoundTripHome_Impl (void)
{
  if (CIAO::debug_level () > 0)
    ACE_DEBUG ((LM_DEBUG, "Creating Round_trip impl\n"));

  return new MyImpl::RoundTripHome_exec_i ();
}
