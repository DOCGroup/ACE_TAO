// $Id$

#include "SimpleMultipleUser_exec.h"
#include "ace/Log_Msg.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_Simple_SimpleMultipleUser_Impl
{
  //==================================================================
  // Component Executor Implementation Class:   SimpleMultipleUser_exec_i
  //==================================================================

  SimpleMultipleUser_exec_i::SimpleMultipleUser_exec_i (void)
  {
    ACE_TRACE ("SimpleMultipleUser_exec_i::SimpleMultipleUser_exec_i (void)");
  }

  SimpleMultipleUser_exec_i::~SimpleMultipleUser_exec_i (void)
  {
    ACE_TRACE ("SimpleMultipleUser_exec_i::~SimpleMultipleUser_exec_i (void)");
  }

  // Supported or inherited operations.

  // Attribute operations.

  // Port operations.

  // Operations from Components::SessionComponent

  void
  SimpleMultipleUser_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ACE_TRACE ("SimpleMultipleUser_exec_i::set_session_context ()");
    this->context_ =
      ::Simple::CCM_SimpleMultipleUser_Context::_narrow (ctx);

    if (CORBA::is_nil (this->context_.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }
  }

  void
  SimpleMultipleUser_exec_i::configuration_complete ()
  {
    ACE_TRACE ("SimpleMultipleUser_exec_i::configuration_complete ()");
    // Your code here.
  }

  void
  SimpleMultipleUser_exec_i::ccm_activate ()
  {
    ACE_TRACE ("SimpleMultipleUser_exec_i::ccm_activate ()");

    ::Simple::SimpleMultipleUser::trig_uses_mConnections_var conns =
        this->context_->get_connections_trig_uses_m ();

    ACE_DEBUG ((LM_NOTICE, "SimpleMultipleUser_exec_i::ccm_activate - "
                 "Got %u connections to my receptacle\n",
                 conns->length ()));

    for (CORBA::ULong i = 0; i < conns->length (); ++i)
      {
        try
          {
            ACE_DEBUG ((LM_NOTICE, "SimpleMultipleUser_exec_i::ccm_activate - "
                         "Invoking %u'th connection\n", i));
            ::Simple::Trigger_var trig (conns[i].objref);

            trig->hello ("Test succeeded, hello from SimpleMultipleUser_exec");
          }
        catch (const CORBA::Exception &ex)
          {
            ACE_ERROR ((LM_ERROR, "SimpleMultipleUser_exec_i::ccm_activate () - "
                         "Caught CORBA exception on %u'th reference, details follow:\n",
                         i));
            ex._tao_print_exception ("SimpleUser_exec_i::ccm_activate () - ");
          }
        catch (...)
          {
            ACE_ERROR ((LM_ERROR, "SimpleMultipleUser_exec_i::ccm_activate () - "
                         "Error: Caught unknown exception whilst invoking reference for port trig.\n"));
          }
      }

    // Your code here.
  }

  void
  SimpleMultipleUser_exec_i::ccm_passivate ()
  {
    ACE_TRACE ("SimpleMultipleUser_exec_i::ccm_passivate ()");
    // Your code here.
  }

  void
  SimpleMultipleUser_exec_i::ccm_remove ()
  {
    ACE_TRACE ("SimpleMultipleUser_exec_i::ccm_remove ()");
    //TODO: Enable this check again and resolve underlying issues.
/*
    ::Simple::SimpleMultipleUser::trig_uses_mConnections_var conns =
      this->context_->get_connections_trig_uses_m ();
    if (conns->length() != 0)
      {
        ACE_ERROR ((LM_ERROR, CLINFO "SimpleMultipleUser_exec_i::ccm_remove - "
                  "ERROR: Connections still exists on exit - <%u> %@\n",
                  conns->length(), this));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, CLINFO "SimpleMultipleUser_exec_i::ccm_remove - "
                  "OK: Connections do not exist on exit %@\n", this));
      }
*/
  }

  //==================================================================
  // Home Executor Implementation Class:   SimpleMultipleUserHome_exec_i
  //==================================================================

  SimpleMultipleUserHome_exec_i::SimpleMultipleUserHome_exec_i (void)
  {
    ACE_TRACE ("SimpleMultipleUserHome_exec_i::SimpleMultipleUserHome_exec_i (void)");
  }

  SimpleMultipleUserHome_exec_i::~SimpleMultipleUserHome_exec_i (void)
  {
    ACE_TRACE ("SimpleMultipleUserHome_exec_i::~SimpleMultipleUserHome_exec_i (void)");
  }

  // Supported or inherited operations.

  // Home operations.

  // Factory and finder operations.

  // Attribute operations.

  // Implicit operations.

  ::Components::EnterpriseComponent_ptr
  SimpleMultipleUserHome_exec_i::create ()
  {
    ACE_TRACE ("SimpleMultipleUserHome_exec_i::create ()");
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_THROW_EX (
      retval,
      SimpleMultipleUser_exec_i,
      ::CORBA::NO_MEMORY ());

    return retval;
  }

  extern "C" SIMPLEMULTIPLEUSER_EXEC_Export ::Components::HomeExecutorBase_ptr
  create_Simple_SimpleMultipleUserHome_Impl (void)
  {
    ACE_TRACE ("create_Simple_SimpleMultipleUserHome_Impl (void)");
    ::Components::HomeExecutorBase_ptr retval =
      ::Components::HomeExecutorBase::_nil ();

    ACE_NEW_RETURN (
      retval,
      SimpleMultipleUserHome_exec_i,
      ::Components::HomeExecutorBase::_nil ());

    return retval;
  }
}

