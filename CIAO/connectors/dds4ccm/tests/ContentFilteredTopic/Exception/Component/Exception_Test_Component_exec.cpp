// -*- C++ -*-
//
// $Id$

#include "Exception_Test_Component_exec.h"

namespace CIAO_CFT_Exception_Test_CFTTestComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  Component_exec_i::Component_exec_i (void)
  {
  }

  Component_exec_i::~Component_exec_i (void)
  {
  }

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CFT_Exception_Test::CCM_CFTTestComponent_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_getter_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  void
  Component_exec_i::test_set_query_parameters ()
  {
    bool exception_caught = false;
    try
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::test_set_query_parameters: Test exception\n"));
        ::DDS::StringSeq parameters;
        parameters.length (2);
        parameters[0] = CORBA::string_dup ("2");
        parameters[1] = CORBA::string_dup ("43");

        this->filter_config_->set_filter_parameters (parameters);
      }
    catch (const ::CCM_DDS::InternalError&)
      {
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::test_set_query_parameters - "
                              "caught expected InternalErr exception.\n"));
        exception_caught = true;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::test_set_query_parameters: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_set_query_parameters - "
                              "Exception caught\n"));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_set_query_parameters - "
                              "caught unknown exception\n"));
      }
    if (!exception_caught)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_set_query_parameters - "
                              "Didn't catch the exprected exception\n"));
      }
  }

  void
  Component_exec_i::ccm_activate (void)
  {
    this->filter_config_ =
      this->context_->get_connection_getter_filter_config ();
    this->test_set_query_parameters ();
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
  }

  void
  Component_exec_i::ccm_remove (void)
  {
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CFTTest_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

