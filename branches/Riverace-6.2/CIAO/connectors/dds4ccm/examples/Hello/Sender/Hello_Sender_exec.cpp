// -*- C++ -*-
// $Id$

#include "Hello_Sender_exec.h"
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/Date_Time.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Reactor.h"

namespace CIAO_Hello_Sender_Impl
{

  // TAO_IDL - Generated from
  // be/be_visitor_component/facet_exs.cpp:75
  //============================================================
  // Facet Executor Implementation Class: connector_status_exec_i
  //============================================================

  connector_status_exec_i::connector_status_exec_i (
        ::Hello::CCM_Sender_Context_ptr ctx,
        Atomic_Boolean &ready_to_start)
    : ciao_context_ (
        ::Hello::CCM_Sender_Context::_duplicate (ctx)),
      ready_to_start_(ready_to_start)
  {
  }

  connector_status_exec_i::~connector_status_exec_i (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("~connector_status_exec_i\n")));
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener

  void
  connector_status_exec_i::on_inconsistent_topic (
    ::DDS::Topic_ptr /* the_topic */,
    const ::DDS::InconsistentTopicStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  connector_status_exec_i::on_requested_incompatible_qos (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedIncompatibleQosStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  connector_status_exec_i::on_sample_rejected (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleRejectedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  connector_status_exec_i::on_offered_deadline_missed (
    ::DDS::DataWriter_ptr /* the_writer */,
    const ::DDS::OfferedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  connector_status_exec_i::on_offered_incompatible_qos (
    ::DDS::DataWriter_ptr /* the_writer */,
    const ::DDS::OfferedIncompatibleQosStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  connector_status_exec_i::on_unexpected_status (
    ::DDS::Entity_ptr /* the_entity */,
    ::DDS::StatusKind status_kind)
  {
    if (status_kind == DDS::PUBLICATION_MATCHED_STATUS)
      {
        // be aware that when only the sender runs, ready_to_start will never
        // be true.
        this->ready_to_start_ = true;
      }
  }

  //============================================================
  // Pulse generator
  //============================================================

  pulse_Generator::pulse_Generator (Sender_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  pulse_Generator::~pulse_Generator ()
  {
  }

  int
  pulse_Generator::handle_timeout (const ACE_Time_Value &,
                                   const void *)
  {
    // Notify the subscribers
    this->pulse_callback_.tick ();
    return 0;
  }

  //============================================================
  // Component Executor Implementation Class: Sender_exec_i
  //============================================================

  Sender_exec_i::Sender_exec_i (void)
    : rate_ (1),
      iteration_ (0),
      iterations_ (1000),
      log_time_ (false),
      msg_ ("Hello World!"),
      ready_to_start_(false)
  {
    this->ticker_ = new pulse_Generator (*this);
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
    delete this->ticker_;
  }

  // Supported operations and attributes.

  // Component attributes and port operations.

  ::CORBA::ULong
  Sender_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Sender_exec_i::rate (
    ::CORBA::ULong rate)
  {
    if (rate == 0)
      {
        rate = 1;
      }
    else
      {
        this->rate_ = rate;
      }
  }

  ::CORBA::ULong
  Sender_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Sender_exec_i::iterations (
    ::CORBA::ULong iterations)
  {
    this->iterations_ = iterations;
  }

  char *
  Sender_exec_i::message (void)
  {
    return CORBA::string_dup (this->msg_.c_str());
  }

  void
  Sender_exec_i::message (
    const char * message)
  {
    this->msg_ = message;
  }

  ::CORBA::Boolean
  Sender_exec_i::log_time (void)
  {
    return this->log_time_;
  }

  void
  Sender_exec_i::log_time (
    ::CORBA::Boolean log_time)
  {
    this->log_time_ = log_time;
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Sender_exec_i::get_connector_status (void)
  {
    if ( ::CORBA::is_nil (this->ciao_connector_status_.in ()))
      {
        connector_status_exec_i *tmp = 0;
        ACE_NEW_RETURN (
          tmp,
          connector_status_exec_i (
            this->ciao_context_.in (),
            this->ready_to_start_),
          ::CCM_DDS::CCM_ConnectorStatusListener::_nil ());

        this->ciao_connector_status_ = tmp;
      }

    return
      ::CCM_DDS::CCM_ConnectorStatusListener::_duplicate (
        this->ciao_connector_status_.in ());
  }

  ACE_CString
  Sender_exec_i::create_message (const ACE_CString &msg)
  {
    if (!this->log_time_)
      return msg;
    char timestamp[16];
    ACE_Date_Time now;
    ACE_OS::sprintf (timestamp,
                     "%02d.%06d",
                     static_cast<int> (now.second()),
                     static_cast<int> (now.microsec ()));
    ACE_CString ret (timestamp);
    ret = ret + " " + msg;
    return ret.c_str ();
  }

  void
  Sender_exec_i::tick ()
  {
    // Start writing after DataWriter find first DataReader that matched the
    // Topic It is still possible that other Readers aren't yet ready to
    // receive data, for that case in the profile the durability is set to
    // TRANSIENT_DURABILITY_QOS, so each Reader should receive each message.
    if(this->ready_to_start_.value())
      {
        if (this->iteration_ < this->iterations_)
          {
            Hello::Writer_var writer =
              this->ciao_context_->get_connection_info_in_data ();
            if (! ::CORBA::is_nil (writer.in ()))
              {
                DDSHello new_msg;
                ACE_CString msg = create_message (this->msg_);
                new_msg.hello = msg.c_str ();
                new_msg.iterator = ++this->iteration_;
                writer->write_one (new_msg, ::DDS::HANDLE_NIL);
                ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::tick - "
                                      "Written sample: <%C> - <%u>\n",
                                      msg.c_str (),
                                      new_msg.iterator));
              }
          }
        else
          {
            // We are done
            this->stop ();
          }
       }
  }

  void
  Sender_exec_i::start (void)
  {
    ACE_Reactor* reactor = 0;

    ::CORBA::Object_var ccm_object = this->ciao_context_->get_CCM_object();
    if (!::CORBA::is_nil (ccm_object.in ()))
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (!::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }

    if (reactor)
      {
        // calculate the interval time
        long const usec = 1000000 / this->rate_;
        if (reactor->schedule_timer (
                    this->ticker_,
                    0,
                    ACE_Time_Value (3, usec),
                    ACE_Time_Value (0, usec)) == -1)
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                                ACE_TEXT ("Error scheduling timer")));
        }
      }
    else
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::stop (void)
  {
    ACE_Reactor* reactor = 0;

    ::CORBA::Object_var ccm_object = this->ciao_context_->get_CCM_object();
    if (!::CORBA::is_nil (ccm_object.in ()))
      {
        ::CORBA::ORB_var orb = ccm_object->_get_orb ();
        if (!::CORBA::is_nil (orb.in ()))
          {
            reactor = orb->orb_core ()->reactor ();
          }
      }

    if (reactor)
      {
        reactor->cancel_timer (this->ticker_);
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Sender_exec_i::stop : Timer canceled.\n")));
      }
    else
      {
        throw ::CORBA::INTERNAL ();
      }

    if (!this->ready_to_start_.value())
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::stop - ")
                              ACE_TEXT ("Sender never got ready to start\n")));
      }

  }


  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::Hello::CCM_Sender_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Sender_exec_i::configuration_complete (void)
  {
    /* Your code here. */
  }

  void
  Sender_exec_i::ccm_activate (void)
  {
    this->start ();
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->stop ();
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    /* Your code here. */
  }

  extern "C" HELLO_SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
