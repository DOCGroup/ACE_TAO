// -*- C++ -*-
// $Id$

#include "ExceptionT_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_ExceptionT_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (void) : get_rw_ (true), get_ro_ (true)
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
        ExceptionT::InternalError ex  (42, "Rec.Exc.foo",
                                       "thrown by receiver");
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
    //always an exception is thrown
    ExceptionT::InternalError ex (42, "Rec.Exc.hello",
                                  "thrown by receiver");
    throw ex;
  }

  ::CORBA::Short
  MyFoo_exec_i::rw_attrib ()
  {
     // this->get_rw_ is always true, so always an exception is thrown.
     if (this->get_rw_)
       {
          ExceptionT::InternalError ex (42, "Rec.Exc.get_rw_attrib",
                                            "thrown by receiver");
          throw ex;
       }
     else
       {
         return ACE_OS::rand () % 100;
       }
  }

  void
  MyFoo_exec_i::rw_attrib (::CORBA::Short new_value)
  {
    if (new_value == 0)
      {
        //throw internal excep
        ExceptionT::InternalError ex (42, "Rec.Exc.set_rw_attrib",
                                          "thrown by receiver");
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
     // this->get_ro_ is always true, so always an exception is thrown.
    if (this->get_ro_)
      {
        ExceptionT::InternalError ex (42, "Rec.Exc.ro_attrib",
                                          "thrown by receiver");
        throw ex;
      }
    else
      {
        ACE_OS::sleep (ACE_OS::rand () % 2);
      }
    return ACE_OS::rand () % 100;
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::ExceptionT::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::ExceptionT::CCM_Receiver_Context::_narrow (ctx);

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
  create_ExceptionT_AMI_Receiver_Impl (void)
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
