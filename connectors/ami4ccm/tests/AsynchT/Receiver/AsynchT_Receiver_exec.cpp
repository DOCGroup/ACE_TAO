// -*- C++ -*-
// $Id$

#include "AsynchT_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_AsynchT_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (void)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  ::CORBA::Long
  MyFoo_exec_i::foo (const char * /*in_str*/, ::CORBA::Long cmd,
                     ::CORBA::String_out answer)
  {
    if (cmd == 1)
      {
        ACE_OS::sleep(2);
      }
    answer = CORBA::string_dup ("Hi from receiver.");
    return cmd;
  }

  void
  MyFoo_exec_i::bar (::CORBA::Long cmd,
                     ::CORBA::Long_out l_cmd)
  {
    ACE_OS::sleep(2);
    l_cmd = cmd;
  }

 ::CORBA::Short
  MyFoo_exec_i::rw_attrib ()
  {
     ACE_OS::sleep (ACE_OS::rand () % 2);
     return 3;
  }

  void
  MyFoo_exec_i::rw_attrib (::CORBA::Short /*new_value*/)
  {
     ACE_OS::sleep (ACE_OS::rand () % 2);
  }

  CORBA::Short
  MyFoo_exec_i::ro_attrib ()
  {
    ACE_OS::sleep (2);
    return 4;
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::AsynchT::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::AsynchT::CCM_Receiver_Context::_narrow (ctx);

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
  create_AsynchT_AMI_Receiver_Impl (void)
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
