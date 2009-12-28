// -*- C++ -*-
// $Id$

#include "Hello_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Hello_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (void) : get_rw_ (false), get_ro_ (false)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  ::CORBA::Long
  MyFoo_exec_i::foo (const char * in_str, ::CORBA::String_out answer)
  {
    if (ACE_OS::strlen (in_str) == 0)
      {
        printf ("Receiver (FOO) :\tEMPTY string received -> throw internal excep with id <42> and errorstring <Hello World>\n");
        Hello::InternalError ex (42, "Hello world");
        throw ex;
      }
    else
      {
        printf ("Receiver (FOO) :\tReceived string <%s>\n", in_str);
        ACE_OS::sleep (ACE_OS::rand () % 2);
        answer = CORBA::string_dup ("This is my answer : Hi");
        return ACE_OS::rand () % 100;
      }
  }

  void
  MyFoo_exec_i::hello (::CORBA::Long_out answer)
  {
    printf ("Receiver (HELLO) :\tReceived request\n");
    ACE_OS::sleep (ACE_OS::rand () % 2);
    answer = ACE_OS::rand () % 100;
  }

  ::CORBA::Short
  MyFoo_exec_i::rw_attrib ()
  {
    printf ("Receiver (RW_ATTRIB) :\tReceived request\n");
    if (this->get_rw_)
      {
        this->get_rw_ = false;
        printf ("Receiver (FOO) :\tSecond get rw_attrib received -> throw internal excep with id <42> and errorstring <Hello World>\n");
        Hello::InternalError ex (42, "Hello world");
        throw ex;
      }
    else
      {
        ACE_OS::sleep (ACE_OS::rand () % 2);
        this->get_rw_ = true;
      }
    return ACE_OS::rand () % 100;
  }

  void
  MyFoo_exec_i::rw_attrib (::CORBA::Short new_value)
  {
    if (new_value == 0)
      {
        printf ("Receiver (RW_ATTRIB) :\tnew_value == 0 -> throw internal excep with id <42> and errorstring <Hello World>\n");
        Hello::InternalError ex (42, "Hello world");
        throw ex;
      }
    else
      {
        printf ("Receiver (RW_ATTRIB) :\tSet <%d>\n", new_value);
        ACE_OS::sleep (ACE_OS::rand () % 2);
      }
  }

  CORBA::Short
  MyFoo_exec_i::ro_attrib ()
  {
    printf ("Receiver (RO_ATTRIB) :\tReceived request\n");
    if (this->get_ro_)
      {
        this->get_ro_ = false;
        printf ("Receiver (FOO) :\tSecond get ro_attrib received -> throw internal excep with id <42> and errorstring <Hello World>\n");
        Hello::InternalError ex (42, "Hello world");
        throw ex;
      }
    else
      {
        ACE_OS::sleep (ACE_OS::rand () % 2);
        this->get_ro_ = true;
      }
    return ACE_OS::rand () % 100;
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::Hello::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::Hello::CCM_Receiver_Context::_narrow (ctx);

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
  create_Hello_AMI_Receiver_Impl (void)
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
