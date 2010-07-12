// -*- C++ -*-
// $Id$

#include "Different_Test_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

#define ITERATIONS 15

namespace CIAO_Different_Test_Sender_Impl
{
  //============================================================
  // WriteHandler
  //============================================================
  WriteHandler::WriteHandler (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  WriteHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.start ();
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (ITERATIONS),
      keys_ (5)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::start (void)
  {
    for (CORBA::UShort iter_key = 1; iter_key < this->keys_ + 1; ++iter_key)
      {
        char key[7];
        QueryConditionTest new_key;
        ACE_OS::sprintf (key, "KEY_%d", iter_key);
        new_key.symbol = CORBA::string_dup(key);
        for (CORBA::UShort iter = 1;
            iter < this->iterations_ + 1;
            ++iter)
          {
            new_key.iteration = iter;
            ::Different_Test::QueryConditionTestConnector::Writer_var writer
              = this->context_->get_connection_info_write_data ();
            writer->write_one (new_key, ::DDS::HANDLE_NIL);
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Written key <%C> with <%d>\n"),
                        key, iter));
          }
      }
    ACE_OS::sleep (1);
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
      ::Different_Test::CCM_Sender_Context::_narrow (ctx);

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
        WriteHandler *wh = new WriteHandler (*this);
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (wh);
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: GET_CONNECTION_START_READER : Exception caught\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: GET_CONNECTION_START_READER : Unknown exception caught\n")));
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
  create_Different_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
