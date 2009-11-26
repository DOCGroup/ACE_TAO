// -*- C++ -*-
// $Id$

#include "Get_Connection_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Get_Connection_Test_Sender_Impl
{
  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : updater_ok_ (false),
      writer_ok_ (false),
      updater_dds_datawriter_ok_ (false),
      writer_dds_datawriter_ok_ (false)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  // Supported operations and attributes.

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Get_Connection_Test::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    this->writer_ = 
        this->context_->get_connection_info_write_data ();
    if (!CORBA::is_nil (this->writer_))
      {
        this->writer_ok_ = true;
      }
    this->updater_ = 
        this->context_->get_connection_info_update_data ();
    if (!CORBA::is_nil (this->updater_))
      {
        this->updater_ok_ = true;
      }
    this->updater_dds_datawriter_ = 
        this->context_->get_connection_info_update_dds_entity ();
    if (!CORBA::is_nil (this->updater_dds_datawriter_))
      {
        this->updater_dds_datawriter_ok_ = true;
      }
    this->writer_dds_datawriter_ = 
        this->context_->get_connection_info_write_dds_entity ();
    if (!CORBA::is_nil (this->writer_dds_datawriter_))
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
    if (!this->updater_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Sender : get updater failed\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Sender : Get updater passed\n")));
      }
    if (!this->writer_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Sender : get writer failed\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Sender : Get writer passed\n")));
      }
    if (!this->updater_dds_datawriter_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Sender : get dds updater failed\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Sender : Get dds updater passed\n")));
      }
    if (!this->writer_dds_datawriter_ok_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Sender : get dds writer failed\n")));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Sender : Get dds writer passed\n")));
      }
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Get_Connection_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
