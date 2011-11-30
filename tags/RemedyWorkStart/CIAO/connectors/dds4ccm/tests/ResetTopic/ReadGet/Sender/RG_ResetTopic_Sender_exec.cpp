// -*- C++ -*-
// $Id$

#include "RG_ResetTopic_Sender_exec.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

#define TOPIC_1 "ResetTopic1"
#define TOPIC_2 "ResetTopic2"


namespace CIAO_RG_ResetTopic_Sender_Impl
{

  /**
   * Timeout_Handler
   */

  Timeout_Handler::Timeout_Handler (Sender_exec_i &callback)
    : callback_ (callback)
  {
  }

  int
  Timeout_Handler::handle_timeout (const ACE_Time_Value &, const void *)
  {
    this->callback_.start_write ();
    return 0;
  }

  /**
   * Component Executor Implementation Class: Sender_exec_i
   */

  Sender_exec_i::Sender_exec_i (void)
    : iterations_ (0)
    , keys_ (0)
    , topic_name_ (TOPIC_2)
    , run_(0)
    , start_key_ (1)
    , start_iteration_ (1)
  {
  }

  Sender_exec_i::~Sender_exec_i (void)
  {
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

  void
  Sender_exec_i::start_write (void)
  {
    try
      {
        if (++this->run_ > 4)
          return;
        if (ACE_OS::strcmp (this->topic_name_.c_str(), TOPIC_1) == 0)
          this->topic_name_ = TOPIC_2;
        else
          this->topic_name_ = TOPIC_1;

        ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::start_write - "
                    "Setting topic name to <%C>\n", this->topic_name_.c_str()));

        ::RG_ResetTopic::RG_ResetTopicSampleConnector::Writer_var writer =
          this->ciao_context_->get_connection_info_write_data ();
        if (::CORBA::is_nil (writer.in ()))
            {
              ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::start_write - "
                                    "Unable to get writer interface\n"));
              throw ::CORBA::INTERNAL ();
            }
          ::CORBA::Object_var cmp = writer->_get_component ();
          if (::CORBA::is_nil (cmp.in ()))
            {
              ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::start_write - "
                                    "Unable to get component interface\n"));
              throw ::CORBA::INTERNAL ();
            }
        ::RG_ResetTopic::RG_ResetTopicSampleConnector::CCM_DDS_Event_var conn =
          ::RG_ResetTopic::RG_ResetTopicSampleConnector::CCM_DDS_Event::_narrow (cmp.in ());
        if (::CORBA::is_nil (conn.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Sender_exec_i::start_write - "
                                  "Unable to narrow connector interface\n"));
            throw ::CORBA::INTERNAL ();
          }

        conn->topic_name (::CORBA::string_dup (this->topic_name_.c_str()));

        ::CORBA::UShort key = 0;
        ::CORBA::UShort iter = 0;
        for (key = this->start_key_; key < this->run_ * this->keys_ + 1; ++key)
          {
            RG_ResetTopicSample sample;
            char tmp[7];
            ACE_OS::sprintf (tmp, "KEY_%d", key);
            sample.key = CORBA::string_dup(tmp);

            for (iter = this->start_iteration_; iter < this->run_ * this->iterations_ + 1; ++iter)
              {
                sample.iteration = iter;
                writer->write_one (sample, ::DDS::HANDLE_NIL);
                ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::start_event_test - "
                            "Sample written : key <%C> - iteration <%d>\n",
                            tmp, iter));
              }
          }
        this->start_key_ = key;
        this->start_iteration_ = iter;
        ACE_Time_Value tv (2, 0);
        ACE_OS::sleep (tv);
        ReaderStarter_var starter =
          this->ciao_context_->get_connection_reader_start ();
        if (!::CORBA::is_nil (starter.in ()))
          {
            ACE_DEBUG ((LM_DEBUG, "Sender_exec_i::start_event_test - "
                      "Inform the receiver that all samples were written\n"));
            starter->set_reader_properties (this->start_key_ - 1, this->start_iteration_ - 1);

            starter->start_read (::CORBA::string_dup(this->topic_name_.c_str()));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "Sender_exec_i::start_event_test - "
                      "ERROR: unable to get connection to the ReaderStarter\n"));
          }
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::start_event_test - "
                              "ERROR: Caught NonChangeable exception.\n"));
      }
    catch (const ::CORBA::Exception & e)
      {
        e._tao_print_exception("Sender_exec_i::start_event_test - ERROR");
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Sender_exec_i::start_event_test - "
                    "ERROR: Unexpected and unknown exception caught.\n"));
      }
  }


  // Component attributes and port operations.

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

  // Operations from Components::SessionComponent.

  void
  Sender_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::RG_ResetTopic::CCM_Sender_Context::_narrow (ctx);

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
     ACE_NEW_THROW_EX (this->to_handler_,
                       Timeout_Handler (*this),
                       ::CORBA::INTERNAL ());
     if (this->reactor ()->schedule_timer (
                 this->to_handler_,
                 0,
                 ACE_Time_Value (1, 0),
                 ACE_Time_Value (10, 0)) == -1)
       {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("Sender_exec_i::start : ")
                               ACE_TEXT ("Error scheduling timer")));
       }
  }

  void
  Sender_exec_i::ccm_passivate (void)
  {
    this->reactor()->cancel_timer(this->to_handler_);
  }

  void
  Sender_exec_i::ccm_remove (void)
  {
    delete this->to_handler_;
  }

  extern "C" SENDER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_RG_ResetTopic_Sender_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Sender_exec_i);

    return retval;
  }
}
