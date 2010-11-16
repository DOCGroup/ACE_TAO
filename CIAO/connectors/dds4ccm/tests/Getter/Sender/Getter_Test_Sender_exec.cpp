// -*- C++ -*-
// $Id$

#include "Getter_Test_Sender_exec.h"
#include "ace/Log_Msg.h"
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

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

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
    ::DDS::Entity_ptr ,
    ::DDS::StatusKind  status_kind)
  {
    if (status_kind == ::DDS::PUBLICATION_MATCHED_STATUS)
      {
        this->callback_.start ();
      }
  }

  //============================================================
  // Sender_exec_i
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
  Sender_exec_i::start_timeout_tests (void)
  {
    GetInvoker_var invoker =
      this->context_->get_connection_invoke_getter ();

    invoker->start_timeout_get_one ();
    invoker->start_timeout_get_many ();
  }

  void
  Sender_exec_i::write_many (void)
  {
    ::Getter_Test::GetterTestConnector::Writer_var writer =
      this->context_->get_connection_info_write_data ();
    GetInvoker_var invoker =
      this->context_->get_connection_invoke_getter ();

    GetterTestSeq write_many (this->keys_ * this->iterations_);
    write_many.length (this->keys_ * this->iterations_);
    for (CORBA::UShort key = 1; key < this->keys_ + 1; ++key)
      {
        GetterTest new_key;
        char tmp[7];
        ACE_OS::sprintf (tmp, "KEY_%d", key);
        new_key.key = CORBA::string_dup(tmp);
        for (CORBA::UShort iter = 1; iter < this->iterations_ + 1; ++iter)
          {
            new_key.iteration = iter;
            write_many[key + iter - 2] = new_key;
          }
      }
    try
      {
        invoker->start_get_many (this->keys_, this->iterations_);
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
    //start can be called more than once...
    if (!this->done_ && this->ccm_activated_)
      {
        this->done_ = true;
        start_timeout_tests ();
        if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                    this->ticker_,
                    0,
                    ACE_Time_Value (0, 500000),
                    ACE_Time_Value (0, 500000)) == -1)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                                  ACE_TEXT ("Error scheduling timer")));
          }
      }
  }

  void
  Sender_exec_i::tick (void)
  {
    ::Getter_Test::GetterTestConnector::Writer_var writer =
      this->context_->get_connection_info_write_data ();
    ::Getter_Test::GetterFixedConnector::Writer_var fixed =
      this->context_->get_connection_info_fixed_data ();
    ::GetInvoker_var invoker =
      this->context_->get_connection_invoke_getter ();

    if (this->last_iter_ <= this->iterations_)
      {
        GetterFixed  fixed_key;
        GetterTest *new_key = new GetterTest;
        new_key->key = CORBA::string_dup("KEY_1");
        fixed_key.key = 1;
        invoker->start_get_one ("KEY_1", 1, last_iter_);
        new_key->iteration = last_iter_;
        fixed_key.iteration = last_iter_;

        ACE_Time_Value tv (0, 50000);
        ACE_OS::sleep (tv);

        writer->write_one (*new_key, ::DDS::HANDLE_NIL);
        fixed->write_one (fixed_key, ::DDS::HANDLE_NIL);
        ACE_DEBUG ((LM_DEBUG, "Written keys <%C> and <%u> with <%d>\n",
                    new_key->key.in (), fixed_key.key, last_iter_));

        ++last_iter_;
     }
   else
    {
      write_many ();
      this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
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
    this->context_ = ::Getter_Test::CCM_Sender_Context::_narrow (ctx);

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
        this->ccm_activated_ = true;
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
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
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
