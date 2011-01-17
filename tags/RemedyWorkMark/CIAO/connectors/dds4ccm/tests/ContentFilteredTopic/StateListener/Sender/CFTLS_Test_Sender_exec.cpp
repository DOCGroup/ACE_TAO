// -*- C++ -*-
// $Id$

#include "CFTLS_Test_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"
#include "ace/Date_Time.h"

namespace CIAO_CFTLS_Test_Sender_Impl
{
  /**
   * StartHandler
   */
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

  /**
   * UpdateTicker
   */
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


  /**
   * Component Executor Implementation Class: Sender_exec_i
   */

  Sender_exec_i::Sender_exec_i (void)
    : keys_ (5)
      , iterations_ (10)
  {
    ACE_NEW_THROW_EX (this->ticker_,
                      UpdateTicker (*this),
                      CORBA::INTERNAL ());
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    delete this->ticker_;
  }

  void
  Sender_exec_i::update_one (CORBA::UShort iter)
  {
    ::CFTLS_Test::ContentFilteredTopicListenStateTestConnector::Updater_var
      updater = this->ciao_context_->get_connection_info_update_data ();
    if (iter <= this->iterations ())
      {
        for (CORBA::UShort i = 1; i < this->keys () + 1; ++i)
          {
            char key[7];
            ACE_OS::sprintf (key, "KEY_%d", i);
            ContentFilteredTopicListenStateTest update_key;
            update_key.key = CORBA::string_dup (key);
            update_key.iteration = iter;
            updater->update_one (update_key, ::DDS::HANDLE_NIL);
            ACE_DEBUG ((LM_DEBUG, "Updated key <%C> - <%u>\n",
                          key, iter));

          }
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::update_one - "
                              "Finished updating. Cancel Timer.\n"));
        this->reactor ()->cancel_timer (this->ticker_);
      }
  }

  void
  Sender_exec_i::run ()
  {
    ::CFTLS_Test::ContentFilteredTopicListenStateTestConnector::Updater_var
      updater = this->ciao_context_->get_connection_info_update_data ();

    // First create the instances for consecutive updating.
    for (CORBA::UShort i = 1; i < this->keys () + 1; ++i)
      {
        char key[7];
        ACE_OS::sprintf (key, "KEY_%d", i);
        ContentFilteredTopicListenStateTest new_key;
        new_key.key = CORBA::string_dup(key);
        new_key.iteration = 0;
        updater->create_one (new_key);
      }

   if (this->reactor ()->schedule_timer (
                this->ticker_,
                0,
                ACE_Time_Value (5, 50000),
                ACE_Time_Value (0, 50000)) == -1)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::run : ")
                              ACE_TEXT ("Error scheduling timer")));
      }
  }

  void
  Sender_exec_i::start (void)
  {
    StartHandler *rh = new StartHandler (*this);
    this->reactor ()->notify (rh);
  }

  // Supported operations and attributes.
  ACE_Reactor*
  Sender_exec_i::reactor (void)
  {
    ACE_Reactor* reactor = 0;
    ::CORBA::Object_var ccm_object =
      this->ciao_context_->get_CCM_object();
    if (! ::CORBA::is_nil (ccm_object.in ()))
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (! ::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }
    if (reactor == 0)
      {
        throw ::CORBA::INTERNAL ();
      }
    return reactor;
  }

  // Component attributes and port operations.

  ::CORBA::UShort
  Sender_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Sender_exec_i::keys (
    const ::CORBA::UShort keys)
  {
    this->keys_ = keys;
  }

  ::CORBA::UShort
  Sender_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Sender_exec_i::iterations (
    const ::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
  }

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::CFTLS_Test::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
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
  create_CFTLS_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
