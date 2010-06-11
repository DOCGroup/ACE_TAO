// -*- C++ -*-
// $Id$

#include "ReadGet_Test_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

#define ITERATIONS 10

namespace CIAO_ReadGet_Test_Sender_Impl
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
  // Restarter_exec_i
  //============================================================
  Restarter_exec_i::Restarter_exec_i (Sender_exec_i & callback)
    : callback_ (callback)
  {
  }

  Restarter_exec_i::~Restarter_exec_i (void)
  {
  }

  void
  Restarter_exec_i::restart_write ()
  {
    this->callback_.restart ();
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (ITERATIONS),
      keys_ (5),
      run_ (1),
      wh_ (0)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::restart (void)
  {
    ++this->run_;
    delete this->wh_;
    ACE_NEW_THROW_EX (this->wh_,
                      WriteHandler (*this),
                      CORBA::INTERNAL ());
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (this->wh_);
  }

  void
  Sender_exec_i::start (void)
  {
    if (! ::CORBA::is_nil (this->starter_))
      {
        this->starter_->set_reader_properties (this->keys_, this->iterations_);
      }
    else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unable to start the reader\n")));
      }
    if (this->run_ > 1)
      {
        for (CORBA::UShort iter_key = 1; iter_key < this->keys_ + 1; ++iter_key)
          {
            char key[7];
            QueryConditionTest new_key;
            ACE_OS::sprintf (key, "KEY_%d", iter_key);
            new_key.symbol = CORBA::string_dup(key);
            for (CORBA::UShort iter = ((this->run_ - 2) * this->iterations_) + 1;
                iter < this->run_ * this->iterations_ + 1;
                ++iter)
              {
                new_key.iteration = iter;
                this->writer_->write_one (new_key, ::DDS::HANDLE_NIL);
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Written key <%C> with <%d>\n"),
                            key, iter));
              }
          }
        ACE_OS::sleep (1);
      }
    this->starter_->start_read (this->run_);
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

  ::CCM_ReadGetRestarter_ptr
  Sender_exec_i::get_restart_writer (void)
  {
    return new Restarter_exec_i (*this);
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::ReadGet_Test::CCM_Sender_Context::_narrow (ctx);

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
        this->writer_ = this->context_->get_connection_info_write_data ();
        this->starter_ = this->context_->get_connection_start_reader ();
        ACE_NEW_THROW_EX (this->wh_,
                          WriteHandler (*this),
                          CORBA::INTERNAL ());
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (this->wh_);
      }
    catch (const CORBA::Exception& ex)
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
    delete this->wh_;
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_ReadGet_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
