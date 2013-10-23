// -*- C++ -*-
// $Id$

#include "Hello_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_Hello_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (
   ::Hello::CCM_Receiver_Context_ptr ctx)
  : ciao_context_ (
     ::Hello::CCM_Receiver_Context::_duplicate (ctx)),
      get_rw_ (false),
      get_ro_ (false)
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
        Hello::InternalError ex (42, "Hello world");
        throw ex;
      }
    else
      {
        ACE_OS::sleep (ACE_OS::rand () % 2);
        answer = CORBA::string_dup ("This is my answer : Hi");
        return ACE_OS::rand () % 100;
      }
  }

  void
  MyFoo_exec_i::hello (::CORBA::Long_out answer)
  {
    ACE_OS::sleep (ACE_OS::rand () % 2);
    answer = ACE_OS::rand () % 100;
  }

  ::CORBA::Short
  MyFoo_exec_i::rw_attrib ()
  {
    if (this->get_rw_)
      {
        this->get_rw_ = false;
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
        Hello::InternalError ex (42, "Hello world");
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
   if (this->get_ro_)
      {
        this->get_ro_ = false;
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
    ACE_DEBUG ((LM_DEBUG,ACE_TEXT(" Receiver_exec_i::Receiver_exec_i \n")));

  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::Hello::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    if ( ::CORBA::is_nil (this->ciao_do_my_foo_.in ()))
      {
        MyFoo_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          MyFoo_exec_i (
            this->context_.in ()),
            ::Hello::CCM_MyFoo::_nil ());

          this->ciao_do_my_foo_ = tmp;
      }

    return
      ::Hello::CCM_MyFoo::_duplicate (
        this->ciao_do_my_foo_.in ());
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    ACE_DEBUG ((LM_DEBUG,ACE_TEXT(" Receiver_exec_i::set_session_context \n")));
    this->context_ = ::Hello::CCM_Receiver_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    ACE_DEBUG ((LM_DEBUG,ACE_TEXT(" Receiver_exec_i::configuration_complete \n")));
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ACE_DEBUG ((LM_DEBUG,ACE_TEXT(" Receiver_exec_i::ccm_activate \n")));

  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    ACE_DEBUG ((LM_DEBUG,ACE_TEXT(" Receiver_exec_i::ccm_passivate \n")));
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
