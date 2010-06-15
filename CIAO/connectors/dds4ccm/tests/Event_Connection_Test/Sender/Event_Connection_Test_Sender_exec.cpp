// -*- C++ -*-
// $Id$

#include "Event_Connection_Test_Sender_exec.h"

namespace CIAO_Event_Connection_Test_Sender_Impl
{
  //============================================================
  // Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : writer_ok_ (false),
      writer_dds_datawriter_ok_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Event_Connection_Test::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    ::Event_ConnectionConnector::Writer_var writer =
        this->context_->get_connection_info_write_data ();
    if (! ::CORBA::is_nil (writer.in ()))
      {
        this->writer_ok_ = true;
      }
    DDS::DataWriter_var writer_dds_datawriter =
        this->context_->get_connection_info_write_dds_entity ();
    if (! ::CORBA::is_nil (writer_dds_datawriter.in ()))
      {
        this->writer_dds_datawriter_ok_ = true;
      }
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    if (!this->writer_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Sender : get writer failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender : Get writer passed\n")));
      }
    if (!this->writer_dds_datawriter_ok_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Sender : get dds writer failed\n")));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender : Get dds writer passed\n")));
      }
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Event_Connection_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
