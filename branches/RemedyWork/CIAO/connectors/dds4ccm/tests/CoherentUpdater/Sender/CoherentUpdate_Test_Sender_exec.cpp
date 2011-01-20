// -*- C++ -*-
// $Id$

#include "CoherentUpdate_Test_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"
#include "ace/Log_Msg.h"

#define SAMPLE_KEY_NAME "KEY_1"

namespace CIAO_CoherentUpdate_Test_Sender_Impl
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
  // Facet Executor Implementation Class: restart_updater_exec_i
  //============================================================

  restart_updater_exec_i::restart_updater_exec_i (
        ::CoherentUpdate_Test::CCM_Sender_Context_ptr ctx,
        Sender_exec_i & callback)
    : ciao_context_ (
        ::CoherentUpdate_Test::CCM_Sender_Context::_duplicate (ctx)),
      callback_ (callback)
  {
  }

  restart_updater_exec_i::~restart_updater_exec_i (void)
  {
  }

  // Operations from ::CoherentUpdateRestarter

  void
  restart_updater_exec_i::restart_update (void)
  {
    this->callback_.restart ();
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (3),
      run_ (1),
      total_iter (0),
      wh_ (0)
  {
    ACE_NEW_THROW_EX (this->wh_,
                      WriteHandler (*this),
                      CORBA::INTERNAL ());
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    delete this->wh_;
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

  ::CCM_CoherentUpdateRestarter_ptr
  Sender_exec_i::get_restart_updater (void)
  {
    if ( ::CORBA::is_nil (this->ciao_restart_updater_.in ()))
      {
        restart_updater_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          restart_updater_exec_i (
            this->ciao_context_.in (),
            *this),
            ::CCM_CoherentUpdateRestarter::_nil ());

          this->ciao_restart_updater_ = tmp;
      }

    return
      ::CCM_CoherentUpdateRestarter::_duplicate (
        this->ciao_restart_updater_.in ());
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
      ::CoherentUpdate_Test::CCM_Sender_Context::_narrow (ctx);

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
        ::CoherentUpdate_Test::Updater_var updater =
          this->ciao_context_->get_connection_info_update_data ();
        updater->is_coherent_write (true);

        // First create an instance for consecutive updating.
        CoherentUpdateTest new_key;
        new_key.symbol = CORBA::string_dup(SAMPLE_KEY_NAME);
        new_key.iteration = 0;
        updater->create_one (new_key);

        this->reactor ()->notify (this->wh_);
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

  void
  Sender_exec_i::restart (void)
  {
    ++this->run_;
    delete this->wh_;
    ACE_NEW_THROW_EX (this->wh_,
                      WriteHandler (*this),
                      CORBA::INTERNAL ());
    this->reactor ()->notify (this->wh_);
  }

  void
  Sender_exec_i::start (void)
  {
    ::CoherentUpdateStarter_var starter =
        this->ciao_context_->get_connection_start_reader ();

    if (! ::CORBA::is_nil (starter.in ()))
      {
        starter->set_reader_properties (this->iterations_);
      }
    else
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Unable to start the reader\n")));
      }

    ACE_DEBUG ((LM_DEBUG, "Start run <%d> with <%u> iterations\n",
                          this->run_,
                          this->iterations ()));

    CoherentUpdateTestSeq update_many_seq;
    update_many_seq.length (this->iterations_);
    for (int i = 1; i < this->iterations_ + 1; ++i)
      {
        CoherentUpdateTest new_key;
        new_key.symbol = CORBA::string_dup(SAMPLE_KEY_NAME);
        new_key.iteration = ++total_iter;
        update_many_seq[i-1] = new_key;
      }

    ::CoherentUpdate_Test::Updater_var updater =
        this->ciao_context_->get_connection_info_update_data ();
    updater->update_many (update_many_seq);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Written <%u> keys uptil now\n"),
                total_iter));
    ACE_OS::sleep (2);
    starter->start_read (this->run_);
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_CoherentUpdate_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
