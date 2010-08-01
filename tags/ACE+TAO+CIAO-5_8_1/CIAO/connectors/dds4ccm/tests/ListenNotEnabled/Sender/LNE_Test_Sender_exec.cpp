// -*- C++ -*-
// $Id$

#include "LNE_Test_Sender_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_LNE_Test_Sender_Impl
{
  //============================================================
  // WriteManyHandler
  //============================================================
  WriteManyHandler::WriteManyHandler (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteManyHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.write_many ();
    return 0;
  }


  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (10),
      keys_ (5)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::write_many (void)
  {
    ListenNotEnabledTestSeq write_many;
    write_many.length (this->keys_ * this->iterations_);
    for (CORBA::UShort key = 1; key < this->keys_ + 1; ++key)
      {
        ListenNotEnabledTest new_key;
        char tmp[7];
        ACE_OS::sprintf (tmp, "KEY_%d", key);
        new_key.key = CORBA::string_dup(tmp);
        for (CORBA::UShort iter = 1; iter < this->iterations_ + 1; ++iter)
          {
            new_key.iteration = iter;
            write_many[key + iter - 2] = new_key;
          }
      }

::LNE_Test::ListenNotEnabledTestConnector::Writer_var writer =
          this->context_->get_connection_info_write_data ();

    try
      {
        ACE_Time_Value tv (1, 0);
        ACE_OS::sleep (tv);
        writer->write_many (write_many);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("write_many : written <%u> samples\n"),
              write_many.length ()));
      }
    catch (const CCM_DDS::InternalError& ex)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Internal Error ")
                    ACE_TEXT ("while write many writer info: index <%d> - retval <%d>\n"),
                      ex.index, ex.error_code));
      }
  }


  void
  Sender_exec_i::start (void)
  {
    WriteManyHandler *rh = new WriteManyHandler (*this);
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (rh);
  }

  ::CORBA::UShort
  Sender_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Sender_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  ::CORBA::UShort
  Sender_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Sender_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::LNE_Test::CCM_Sender_Context::_narrow (ctx);

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
    try
      {
        start ();
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate: Exception caught\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate: Unknown exception caught\n")));
      }
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
  create_LNE_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
