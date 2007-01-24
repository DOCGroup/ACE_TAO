// $Id$

#include "RoundTrip_exec.h"
#include "CIAO_common.h"

//-- ACE Scheduling Params
#include "ace/OS_NS_errno.h"

//=================================================================

namespace CIDL_RoundTrip_Impl
{
  RoundTrip_exec_i::RoundTrip_exec_i ()
  {

  }

  RoundTrip_exec_i::~RoundTrip_exec_i ()
  {
  }

  NodeAppTest::CCM_LatencyTest_ptr
  RoundTrip_exec_i::get_facet_1 (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return new LatencyTest_Impl;
  }

  NodeAppTest::CCM_LatencyTest_ptr
  RoundTrip_exec_i::get_facet_2 (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return new LatencyTest_Impl;
  }

  CORBA::Long
  RoundTrip_exec_i::cube_long (CORBA::Long data
                               ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return data * data * data;
  }

  // Operations from Components::SessionComponent
  void
  RoundTrip_exec_i::set_session_context (Components::SessionContext_ptr
                                         ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    if (CIAO::debug_level () > 0)
      {
        ACE_DEBUG ((LM_DEBUG,
                    "RoundTrip_exec_i::set_session_context\n"));
      }
  }

  void
  RoundTrip_exec_i::ccm_activate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    if (CIAO::debug_level () > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "RoundTrip_exec_i::ccm_activate\n"));
      }

  }

  void
  RoundTrip_exec_i::ccm_passivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    if (CIAO::debug_level () > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "RoundTrip_exec_i::ccm_passivate\n"));
      }
  }

  void
  RoundTrip_exec_i::ccm_remove (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    if (CIAO::debug_level () > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "RoundTrip_exec_i::ccm_remove\n"));
      }
  }

  void
  RoundTrip_exec_i::ciao_preactivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    if (CIAO::debug_level () > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "RoundTrip_exec_i::ciao_preactivate\n"));
      }

  }

  void
  RoundTrip_exec_i::ciao_postactivate (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    if (CIAO::debug_level () > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "RoundTrip_exec_i::ciao_postactivate\n"));
      }
  }

  RoundTripHome_exec_i::RoundTripHome_exec_i ()
  {
  }

  RoundTripHome_exec_i::~RoundTripHome_exec_i ()
  {
  }

  ::Components::EnterpriseComponent_ptr
  RoundTripHome_exec_i::create (void)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Components::CCMException))
  {
    return new RoundTrip_exec_i;
  }

  // Implementation for the probided inferface.
  CORBA::Long
  LatencyTest_Impl::cube_long (CORBA::Long data
		                           ACE_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
  {
    return data*data*data;
  }

  extern "C" NODEAPPTEST_ROUNDTRIP_EXEC_Export
  ::Components::HomeExecutorBase_ptr
  createRoundTripHome_Impl (void)
  {
    if (CIAO::debug_level () > 0)
      {
        ACE_DEBUG ((LM_DEBUG, "Creating Round_trip impl\n"));
      }

    return new RoundTripHome_exec_i ();
  }
}

