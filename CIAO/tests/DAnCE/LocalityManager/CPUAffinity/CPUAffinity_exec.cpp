// -*- C++ -*-
// $Id$

#include "CPUAffinity_exec.h"
#include "ace/Log_Msg.h"
#include "ace/os_include/os_sched.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

namespace CIAO_cpuaffinity_A_Impl
{
  //============================================================
  // Pulse generator
  //============================================================

  Component_exec_i::Component_exec_i  (void)
    : cpu_affinity_ (-1)
  {
  }

  Component_exec_i::~Component_exec_i  (void)
  {
  }

  // Port operations.
  void Component_exec_i::cpu_affinity (CORBA::Long tc_max)
  {
    this->cpu_affinity_ = tc_max;
  }

  CORBA::Long Component_exec_i::cpu_affinity (void)
  {
    return this->cpu_affinity_;
  }
  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::cpuaffinity::CCM_A_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  void
  Component_exec_i::ccm_activate (void)
  {
#if defined (ACE_HAS_SCHED_GETAFFINITY)
    if (ACE_OS::num_processors () < 2)
      {
        ACE_DEBUG ((LM_DEBUG, "This machine only has a single processor, aborting\n"));
        return;
      }

    cpu_set_t mask;
    CPU_ZERO (&mask);

    int retval = sched_getaffinity (0, sizeof (cpu_set_t), &mask);

    if (retval != 0)
      {
        ACE_ERROR ((LM_ERROR, "Error: Non-zero return value from sched_getaffinity %p\n"));
        return;
      }

    int z_set = CPU_ISSET (0, &mask);
    int o_set = CPU_ISSET (1, &mask);

    if (cpu_affinity_ == 0 &&
        (!z_set || o_set))
      {
        ACE_ERROR ((LM_ERROR, "Error: Expected to only be on processor zero.\n"));
        return;
      }

    if (cpu_affinity_ == 1 &&
        (z_set || !o_set))
      {
        ACE_ERROR ((LM_ERROR, "Error: Expected to only be on processor one.\n"));
      }

    if (cpu_affinity_ > 1)
      {
        ACE_ERROR ((LM_ERROR, "Error: Trying to test an affinity I don't support\n"));
      }
#endif
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
  }

  void
  Component_exec_i::ccm_remove (void)
  {
  }

  extern "C" COMMANDLINEPASSAGE_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i );

    return retval;
  }
}

