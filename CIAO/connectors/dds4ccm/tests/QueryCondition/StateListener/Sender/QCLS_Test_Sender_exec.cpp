// -*- C++ -*-
// $Id$

#include "QCLS_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "ace/Date_Time.h"

#include "ace/Reactor.h"

namespace CIAO_QCLS_Test_Sender_Impl
{
  //============================================================
  // StartHandler
  //============================================================
  StartHandler::StartHandler(Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  StartHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.run ();
    return 0;
  }

  //============================================================
  // UpdateTicker
  //============================================================
  UpdateTicker::UpdateTicker (Sender_exec_i &callback)
    : callback_ (callback),
      last_iter_ (0)
  {
  }

  int
  UpdateTicker::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    this->callback_.update_one (++this->last_iter_);
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================
  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (10),
      keys_ (5)
  {
    this->ticker_ = new UpdateTicker (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }

  void
  Sender_exec_i::update_one (CORBA::UShort iter)
  {
    if (iter <= this->iterations ())
      {
        for (CORBA::UShort i = 1; i < this->keys () + 1; ++i)
          {
            char key[7];
            ACE_OS::sprintf (key, "KEY_%d", i);
            QueryConditionListenStateTest update_key;
            update_key.key = CORBA::string_dup (key);
            update_key.iteration = iter;
            this->updater_->update_one (update_key, ::DDS::HANDLE_NIL);
            ACE_DEBUG ((LM_DEBUG, "Updated key <%C> - <%u>\n",
                          key, iter));

          }
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::update_one - "
                              "Finished updating. Cancel Timer.\n"));
        this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (
                this->ticker_);
      }
  }

  void
  Sender_exec_i::run ()
  {

    //first create the instances for consecutive updating.
    for (CORBA::UShort i = 1; i < this->keys () + 1; ++i)
      {
        char key[7];
        ACE_OS::sprintf (key, "KEY_%d", i);
        QueryConditionListenStateTest new_key;
        new_key.key = CORBA::string_dup(key);
        new_key.iteration = 0;
        this->updater_->create_one (new_key);
      }

   if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (5, 50000),
                ACE_Time_Value (0, 50000)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                              ACE_TEXT ("Error scheduling timer")));
      }
  }

  void
  Sender_exec_i::start (void)
  {
    StartHandler *rh = new StartHandler (*this);
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
      ::QCLS_Test::CCM_Sender_Context::_narrow (ctx);

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
        this->updater_ = this->context_->get_connection_info_update_data ();
        start ();
      }
    catch (const CORBA::Exception& ex)
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
  create_QCLS_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
