// -*- C++ -*-
// $Id$

#include "InterMulti_Receiver_exec.h"
#include "ace/OS_NS_unistd.h"

namespace CIAO_InterMulti_Receiver_Impl
{
  Atomic_UShort nr_of_received = 0;
  One_exec_i::One_exec_i (void)
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
    if (cmd != 1)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver ONE::foo,"
                              " origin not 1, but <%u>\n",
                              cmd));
      }
    else
      {
        ++nr_of_received;
      }
    answer = CORBA::string_dup ("answer foo one");
    return cmd;
  }

  Two_exec_i::Two_exec_i (void)
  {
  }

  Two_exec_i::~Two_exec_i (void)
  {
  }

  void
  Two_exec_i::bar (::CORBA::Long cmd,
                   ::CORBA::String_out answer)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver: two_bar,"
                          " cmd = <%u>\n",
                          cmd));
    if (cmd != 2)
      {
        ACE_ERROR ((LM_ERROR, "ERROR Receiver: TWO::bar,"
                              " origin not 2, but <%u>\n",
                              cmd));
      }
    else
      {
        ++nr_of_received;
      }
    answer = CORBA::string_dup ("answer bar two");
  }

  Three_exec_i::Three_exec_i (void)
  {
  }

  Three_exec_i::~Three_exec_i (void)
  {
  }

  void
  Three_exec_i::plus (::CORBA::Long cmd,
                     ::CORBA::String_out answer)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver: three_plus,"
                          " cmd = <%u>\n",
                          cmd));
    ++nr_of_received;
    answer = CORBA::string_dup ("answer plus three");
  }

  ::CORBA::Long
  Three_exec_i::foo (const char * in_str,::CORBA::Long cmd,
                     ::CORBA::String_out answer)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver: three_foo, in_str = %C"
                          " cmd = <%u>\n",
                          in_str, cmd));
    if (cmd != 3)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver THREE::foo,"
                              " origin not 3, but <%u>\n",
                              cmd));
      }
    else
      {
        ++nr_of_received;
      }
    answer = CORBA::string_dup ("answer foo three");
    return 3;
  }

  void
  Three_exec_i::bar (::CORBA::Long cmd,
                   ::CORBA::String_out answer)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver: three_bar,"
                          " cmd = <%u>\n",
                          cmd));
    if (cmd != 3)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver Three::bar,"
                              " origin not 3, but <%u>\n",
                              cmd));
      }
    else
      {
        ++nr_of_received;
      }
    answer = CORBA::string_dup ("answer bar three");
  }

  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  ::InterMulti::CCM_One_ptr
  Receiver_exec_i::get_do_my_one (void)
  {
    return new One_exec_i ();
  }

  ::InterMulti::CCM_Two_ptr
  Receiver_exec_i::get_do_my_two (void)
  {
    return new Two_exec_i ();
  }

  ::InterMulti::CCM_Three_ptr
  Receiver_exec_i::get_do_my_three (void)
  {
    return new Three_exec_i ();
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ = ::InterMulti::CCM_Receiver_Context::_narrow (ctx);

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
    if (nr_of_received.value() != 7)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: not received the expected number of"
                              "correct calls"
                              "Expected: 7, Received: %u.\n",
                              nr_of_received.value()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Receiver received the expected "
                              "number of correct calls. "
                              "Expected: 7, Received: %u.\n",
                              nr_of_received.value()));
      }
  }

  extern "C"  ::Components::EnterpriseComponent_ptr
  create_InterMulti_AMI_Receiver_Impl (void)
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
