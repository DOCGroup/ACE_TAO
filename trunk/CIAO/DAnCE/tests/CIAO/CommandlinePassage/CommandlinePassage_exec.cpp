// -*- C++ -*-
// $Id$

#include "CommandlinePassage_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

namespace CIAO_command_line_test_A_Impl
{
  //============================================================
  // Pulse generator
  //============================================================

  Component_exec_i::Component_exec_i  (void)
    : tc_max_ (3)
  {
  }

  Component_exec_i::~Component_exec_i  (void)
  {
  }

  // Port operations.
  void Component_exec_i::tc_max (CORBA::Long tc_max)
  {
    this->tc_max_ = tc_max;
  }

  CORBA::Long Component_exec_i::tc_max (void)
  {
    return this->tc_max_;
  }
  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::command_line_test::CCM_A_Context::_narrow (ctx);

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
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
  }

  void
  Component_exec_i::ccm_remove (void)
  {
    try
      {
        long tc =
          this->context_->get_CCM_object ()->_get_orb ()->orb_core ()->lane_resources ().transport_cache ().current_size ();
        CIAO_DEBUG ((LM_ERROR, ACE_TEXT ("Transport cache : set <%d>, current_size <%d>\n"),
              this->tc_max_, tc));
        if (this->tc_max_ != tc)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Commanline parameter is not passed properly\n")));
          }
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Exception caught while checking transport cache.\n")));
      }
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

