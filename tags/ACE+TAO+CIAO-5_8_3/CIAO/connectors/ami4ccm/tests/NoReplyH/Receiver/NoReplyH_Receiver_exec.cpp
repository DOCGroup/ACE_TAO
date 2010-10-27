// -*- C++ -*-
// $Id$

#include "NoReplyH_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_NoReplyH_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (Atomic_UShort &calls)
    : calls_(calls)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  ::CORBA::Long
  MyFoo_exec_i::foo (const char * in_str, ::CORBA::String_out answer)
  {
    ++this->calls_;
    if (ACE_OS::strlen (in_str) == 0)
      {
        NoReplyH::InternalError ex (42, "Hello world");
        throw ex;
      }
    else
      {
        ACE_OS::sleep (ACE_OS::rand () % 2);
        answer = CORBA::string_dup ("This is my answer : Hi");
        return 1;
      }
  }

  void
  MyFoo_exec_i::hello (::CORBA::Long_out answer)
  {
    ++this->calls_;
    ACE_OS::sleep (ACE_OS::rand () % 2);
    answer = 4;
  }

  ::CORBA::Short
  MyFoo_exec_i::rw_attrib ()
  {
    ++this->calls_;
    ACE_OS::sleep (ACE_OS::rand () % 2);
    return 2;
  }

  void
  MyFoo_exec_i::rw_attrib (::CORBA::Short new_value)
  {
    ++this->calls_;
    if (new_value == 0)
      {
        NoReplyH::InternalError ex (42, "Hello world");
        throw ex;
      }
    else
      {
        ACE_OS::sleep (ACE_OS::rand () % 2);
      }
  }

  CORBA::Short
  MyFoo_exec_i::ro_attrib ()
  {
    ++this->calls_;
    return 3;
  }

  Receiver_exec_i::Receiver_exec_i (void)
  :calls_(0)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::NoReplyH::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i (this->calls_);
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::NoReplyH::CCM_Receiver_Context::_narrow (ctx);

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
    if (this->calls_.value() != 10)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: not received the expected number of"
                              "calls"
                              "Expected: 10, Received: %u.\n",
                              this->calls_.value()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Receiver received the expected "
                              "number of calls. "
                              "Expected: 10, Received: %u.\n",
                              this->calls_.value()));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_NoReplyH_AMI_Receiver_Impl (void)
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
