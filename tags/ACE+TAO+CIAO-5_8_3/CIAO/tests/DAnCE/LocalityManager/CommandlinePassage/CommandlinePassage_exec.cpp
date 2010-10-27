// -*- C++ -*-
// $Id$

#include "CommandlinePassage_exec.h"
#include "ace/Log_Msg.h"
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
    ::CORBA::ORB_var orb;

    ::CORBA::Object_var ccm_object = this->context_->get_CCM_object();
    if (!::CORBA::is_nil (ccm_object.in ()))
      {
        orb = ccm_object->_get_orb ();
      }

    try
      {
        long tc =
          orb->orb_core ()->lane_resources ().transport_cache ().total_size ();
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Transport cache : set <%d>, current_size <%d>\n"),
              this->tc_max_, tc));
        if (this->tc_max_ != tc)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Commandline parameter isn't not passed properly ")
                                   ACE_TEXT ("should be <%d>, is <%d>\n"),
                                   this->tc_max_, tc));
          }
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Exception caught while checking transport cache.\n")));
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

