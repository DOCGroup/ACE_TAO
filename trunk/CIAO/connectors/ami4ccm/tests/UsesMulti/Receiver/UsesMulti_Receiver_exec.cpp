// -*- C++ -*-
// $Id$

#include "UsesMulti_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_UsesMulti_Receiver_Impl
{
  One_exec_i::One_exec_i (Atomic_UShort &nr_of_received)
    : nr_of_received_(nr_of_received)
  {
  }

  One_exec_i::~One_exec_i (void)
  {
  }

  ::CORBA::Long
  One_exec_i::foo (const char * in_str, ::CORBA::Long cmd,
                   ::CORBA::String_out answer)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver: one_foo, in_str = %C"
                          " cmd = <%u>\n",
                          in_str, cmd));  
    if ((cmd < 0) || (cmd > 2)) 
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver ONE::foo,"
                              " origin not between 0 and 3, but <%u>\n",
                              cmd));  
      }
    else
      {
        nr_of_received_++;
      }
    answer = CORBA::string_dup (in_str);
   
    return cmd;
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::UsesMulti::CCM_One_ptr
  Receiver_exec_i::get_do_my_one (void)
  {
    return new One_exec_i (nr_of_received_);
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::UsesMulti::CCM_Receiver_Context::_narrow (ctx);

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
    if (nr_of_received_.value() != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver didn't receive the expected "
                              "number of correct calls.\n"
                              "Expected: 2, Received: %u.\n",
                              nr_of_received_.value()));  
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Receiver received the expected "
                              "number of correct calls.\n"
                              "Expected: 2, Received: %u.\n",
                              nr_of_received_.value()));  
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_UsesMulti_AMI_Receiver_Impl (void)
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
