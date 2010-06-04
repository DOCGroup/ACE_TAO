// -*- C++ -*-
//
// $Id$

#include "CFT_Test_Component_exec.h"

#define QUERY "( (y > %0) AND (y < %1) )"

namespace CIAO_CFT_Test_CFTTestComponent_Impl
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
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::CFT_Test::CCM_CFTTestComponent_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_two_dim_get_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_three_dim_get_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  bool
  Component_exec_i::test_two_dimensional_filter (void)
  {
    try
      {
        CCM_DDS::QueryFilter filter;
        filter.expression = CORBA::string_dup (QUERY);
        filter.parameters.length (2);
        filter.parameters[0] = CORBA::string_dup ("5");
        filter.parameters[1] = CORBA::string_dup ("5");
        this->two_reader_->filter (filter);
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_two_dimensional_filter - "
                              "caught InternalError exception: retval <%u>\n",
                              ex.error_code));
        return false;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::test_two_dimensional_filter: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_two_dimensional_filter - "
                              "Exception caught\n"));
        return false;
      }
    return true;
  }

  bool
  Component_exec_i::test_three_dimensional_filter (void)
  {
    try
      {
        CCM_DDS::QueryFilter filter;
        filter.expression = CORBA::string_dup (QUERY);
        filter.parameters.length (2);
        filter.parameters[0] = CORBA::string_dup ("5");
        filter.parameters[1] = CORBA::string_dup ("5");
        this->three_reader_->filter (filter);
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_three_dimensional_filter - "
                              "caught InternalError exception: retval <%u>\n",
                              ex.error_code));
        return false;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Receiver_exec_i::test_three_dimensional_filter: ");
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver_exec_i::test_three_dimensional_filter - "
                              "Exception caught\n"));
        return false;
      }
    return true;
  }

  void
  Component_exec_i::ccm_activate (void)
  {
     this->two_reader_ = this->context_->get_connection_two_dim_get_data();
     this->three_reader_ = this->context_->get_connection_three_dim_get_data();
     if (this->test_two_dimensional_filter () &&
         this->test_three_dimensional_filter ())
      {
        ACE_DEBUG ((LM_DEBUG, "Component_exec_i::ccm_activate - "
                              "OK : Test succeeded. Two "
                              "ContentfilteredTopics can "
                              "be created.\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Component_exec_i::ccm_activate - "
                              "ERROR : Test failed. Probably two "
                              "ContentfilteredTopics "
                              "with the same name were created.\n"));
      }
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

