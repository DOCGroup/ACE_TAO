// -*- C++ -*-
// $Id$

#include "Getter_Test_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ciao/Logger/Log_Macros.h"
#include "ace/Date_Time.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_Getter_Test_Sender_Impl
{
  //============================================================
  // Pulse generator
  //============================================================

  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    // Notify the subscribers
    this->pulse_callback_.tick ();
    return 0;
  }

  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr ,
     const DDS::InconsistentTopicStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr ,
     const DDS::RequestedIncompatibleQosStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr ,
     const DDS::SampleRejectedStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr ,
     const DDS::OfferedDeadlineMissedStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr ,
     const DDS::OfferedIncompatibleQosStatus & )
  {
  }

  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind  status_kind)
  {
    if (status_kind == ::DDS::PUBLICATION_MATCHED_STATUS)
      this->callback_.start ();
  }


  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (10),
      keys_ (5),
      done_ (false),
      ccm_activated_ (false),
      last_iter_ (1)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
  }


  void
  Sender_exec_i::start_get_no_data (void)
  {
    this->invoker_->get_no_data ();
  }

  void
  Sender_exec_i::start (void)
  {
    //start can be called more than once...
    if (!this->done_ && this->ccm_activated_)
      {
        this->done_ = true;
        start_get_no_data ();
        if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                    this->ticker_,
                    0,
                    ACE_Time_Value (1, 0),
                    ACE_Time_Value (1, 0)) == -1)
          {
            CIAO_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                                  ACE_TEXT ("Error scheduling timer")));
          }
      }
  }

  void
  Sender_exec_i::tick (void)
  {
    if (this->last_iter_ < this->iterations_)
      {
        GetterTest *new_key = new GetterTest;
        new_key->key = CORBA::string_dup("KEY_1");
        this->invoker_->start_getting_data (
                        CORBA::string_dup("KEY_1"),
                        last_iter_);
        new_key->iteration = last_iter_;
        ACE_Time_Value tv (0, 500000);
        ACE_OS::sleep (tv);
        this->writer_->write_one (*new_key, ::DDS::HANDLE_NIL);
        CIAO_DEBUG ((LM_DEBUG, CLINFO ACE_TEXT ("Written key <%C> with <%d>\n"),
                    new_key->key.in (), last_iter_));
        ++last_iter_;
     }
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

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_info_out_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (*this);
  }

  void
  Sender_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Getter_Test::CCM_Sender_Context::_narrow (ctx);

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
        this->invoker_ = this->context_->get_connection_invoke_getter ();
        this->ccm_activated_ = true;
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught:");
        CIAO_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Sender_exec_i::ccm_activate: Exception caught\n")));
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, 
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
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
    delete this->ticker_;
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Getter_Test_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
