// -*- C++ -*-
// $Id$

#include "ThreeComp_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_ThreeComp_Receiver_Impl
{
  MyFoo_exec_i::MyFoo_exec_i (Atomic_UShort &nr_of_received)
  : nr_of_received_(nr_of_received)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  CORBA::Long
  MyFoo_exec_i::foo (::CORBA::Long cmd,
                       ::CORBA::String_out answer)
  {
    ACE_OS::sleep(2);
    CORBA::Long ret = 0;
    ++nr_of_received_;
    if (cmd == 10)
      {
        ret = 1;
      }
    if (cmd == 20)
      {
        ret = 2;
      }
    answer = CORBA::string_dup ("Hi from receiver.");
    return ret;
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::ThreeComp::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i (nr_of_received_);
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::ThreeComp::CCM_Receiver_Context::_narrow (ctx);

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
    nr_of_received_ = 0;
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    // each Receiver has to receive 2 requests.
      if (nr_of_received_.value() != 2)
        {
          ACE_ERROR ((LM_ERROR,
                     "ERROR: Receiver received wrong number of messages"
                     " ( %u/2)!\n",
                     nr_of_received_.value() ));
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "OK: This Receiver works as expected.\n"));
        }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_ThreeComp_AMI_Receiver_Impl (void)
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
