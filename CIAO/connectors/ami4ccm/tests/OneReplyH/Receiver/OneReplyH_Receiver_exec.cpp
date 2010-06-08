// -*- C++ -*-
// $Id$

#include "OneReplyH_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"
 
namespace CIAO_OneReplyH_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (void)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  ::CORBA::Long
  MyFoo_exec_i::foo (const char * in_str,::CORBA::Long cmd,
                     ::CORBA::String_out answer)
  {
    if (ACE_OS::strlen (in_str) == 0)
      {
        OneReplyH::InternalError ex (cmd,"foo thrown by receiver");
        throw ex;
      }
    else
      {
        answer = CORBA::string_dup ("This is my answer : Hi");
        return cmd;
      }
  }

  void
  MyFoo_exec_i::bar (const char * in_str,
                         ::CORBA::Long cmd,
                         ::CORBA::String_out answer,
                         ::CORBA::Long_out l_cmd)
  {
    if (ACE_OS::strlen (in_str) == 0)
      {
        OneReplyH::InternalError ex (cmd,"bar thrown by receiver");
        throw ex;
      }
    else
      {
        answer = CORBA::string_alloc(20);
        sprintf(answer,"in_str was <%C>", in_str);
        l_cmd = cmd;
      }
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::OneReplyH::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::OneReplyH::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_OneReplyH_AMI_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_RETURN (
      retval,
      Receiver_exec_i,
      ::Components::EnterpriseComponent::_nil ());

    return retval;
  }
}
