// -*- C++ -*-
// $Id$

#include "ComponentC_exec.h"
#include "ace/Log_Msg.h"

namespace CIAO_connector_test_C_Impl
{
  //============================================================
  // Pulse generator
  //============================================================

  ComponentC_exec_i::ComponentC_exec_i  (void)
    : topic_name_c_ (""),
      topic_name_b_ (""),
      topic_name_a_ (""),
      topic_name_c_has_been_set_ (false),
      topic_name_b_has_been_set_ (false),
      topic_name_a_has_been_set_ (false)
  {
  }

  ComponentC_exec_i::~ComponentC_exec_i  (void)
  {
  }

  // Port operations.
  void
  ComponentC_exec_i::topic_name_c (const char * topic_name)
  {
    this->topic_name_c_ = topic_name;
    this->topic_name_c_has_been_set_ = true;
  }
  char *
  ComponentC_exec_i::topic_name_c (void)
  {
    return CORBA::string_dup (this->topic_name_c_.in ());
  }

  void
  ComponentC_exec_i::topic_name_b (const char * topic_name)
  {
    this->topic_name_b_ = topic_name;
    this->topic_name_b_has_been_set_ = true;
  }
  char *
  ComponentC_exec_i::topic_name_b (void)
  {
    return CORBA::string_dup (this->topic_name_b_.in ());
  }

  void
  ComponentC_exec_i::topic_name_a (const char * topic_name)
  {
    this->topic_name_a_ = topic_name;
    this->topic_name_a_has_been_set_ = true;
  }
  char *
  ComponentC_exec_i::topic_name_a (void)
  {
    return CORBA::string_dup (this->topic_name_a_.in ());
  }
  // Operations from Components::SessionComponent.
  void
  ComponentC_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::connector_test::CCM_C_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  ComponentC_exec_i::configuration_complete (void)
  {
  }

  void
  ComponentC_exec_i::ccm_activate (void)
  {
  }

  void
  ComponentC_exec_i::ccm_passivate (void)
  {
  }

  void
  ComponentC_exec_i::ccm_remove (void)
  {
    if (!this->topic_name_c_has_been_set_)
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Topic name C has not been set\n")));
    if (!this->topic_name_b_has_been_set_)
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Topic name B has not been set\n")));
    if (!this->topic_name_a_has_been_set_)
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR : Topic name A has not been set\n")));
  }

  extern "C" INHERITED_COMPONENTS_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ComponentC_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      ComponentC_exec_i );

    return retval;
  }
}

