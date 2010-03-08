// -*- C++ -*-
// $Id$

#include "DNM_Sender_exec.h"
#include "Base/DNM_BaseC.h"

namespace CIAO_DNM_Test_Sender_Impl
{
  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::DNM_Test::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    this->writer_ = this->context_->get_connection_info_write_data ();

    ::X::Y::Z::DNM_Test_Struct * dnm =
        new ::X::Y::Z::DNM_Test_Struct;
    dnm->key = CORBA::string_dup ("KEY_1");
    dnm->iter = 10;
    this-writer_->write_one (*dnm, ::DDS::HANDLE_NIL);
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoherentWrite_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
