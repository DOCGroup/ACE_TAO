// $Id$
#include "RG_LateBinding_Receiver_impl.h"
#include "ace/Reactor.h"

namespace CIAO_RG_LateBinding_Receiver_Impl
{
  /**
   * Timeout_Handler
   */

  Timeout_Handler::Timeout_Handler (RG_LateBinding_Receiver_impl &callback)
    : callback_ (callback)
  {
  }

  int
  Timeout_Handler::handle_timeout (const ACE_Time_Value &, const void *arg)
  {
    unsigned long assingment = reinterpret_cast<unsigned long> (arg);
    if (assingment == 0)
      {
        this->callback_.test_exception ();
        this->callback_.set_topic_name_reader ();
        this->callback_.start_reading ();
      }
    else if (assingment == 1)
      {
        this->callback_.set_topic_name_getter ();
        this->callback_.start_getting ();
      }
    return 0;
  }

  /**
   * RG_LateBinding_Receiver_impl
   */
  RG_LateBinding_Receiver_impl::RG_LateBinding_Receiver_impl (
      ::RG_LateBinding::CCM_Receiver_Context_ptr ctx,
      ::CORBA::UShort expected)
    : ciao_context_ (
        ::RG_LateBinding::CCM_Receiver_Context::_duplicate (ctx))
    , expected_ (expected)
  {
    ACE_NEW_THROW_EX (this->to_handler_,
                      Timeout_Handler (*this),
                      ::CORBA::INTERNAL ());
  }

  RG_LateBinding_Receiver_impl::~RG_LateBinding_Receiver_impl ()
  {
    delete this->to_handler_;
  }

  void
  RG_LateBinding_Receiver_impl::check_samples (
    const char * test,
    const RG_LateBindingTestSeq& samples,
    const ::CORBA::UShort& expected)
  {
    if (samples.length () != expected)
      {
        ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::check_samples - "
                    "ERROR: Unexpected number of %C samples received: "
                    "expected <%d> - received <%u>\n",
                    test,
                    expected,
                    samples.length ()));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::check_samples - "
                    "%C Samples found: <%u>\n",
                    test,
                    samples.length ()));
      }
    for (::CORBA::ULong i = 0;
         i < samples.length ();
         ++i)
      {
        ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::check_samples - "
                    "Sample %C: sample <%d> - key <%C> - iteration <%d>\n",
                    test,
                    i,
                    samples[i].key.in (),
                    samples[i].iteration));
      }
  }

  void
  RG_LateBinding_Receiver_impl::test_exception (void)
  {
    try
      {
        ::RG_LateBinding::RG_LateBindingTestConnector::Reader_var reader =
          this->ciao_context_->get_connection_info_read_data ();
        if (::CORBA::is_nil (reader.in ()))
          {
            ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::test_exception - "
                        "ERROR: Unable to get writer interface from the "
                        "CIAO context\n"));
            return;
          }

        RG_LateBindingTest sample;
        ::CCM_DDS::ReadInfo readinfo;
        sample.key = CORBA::string_dup ("KEY_1");
        reader->read_one_last (sample,
                               readinfo,
                               ::DDS::HANDLE_NIL);
        ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::test_exception - "
                    "ERROR: No exception caught before topic name has been set\n"));
      }
    catch (const ::CORBA::BAD_INV_ORDER &e)
      {
        ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::test_exception - "
                    "Expected BAD_INV_ORDER thrown.\n"));
      }
    catch (const CORBA::Exception &e)
      {
        e._tao_print_exception("RG_LateBinding_Receiver_impl::test_exception - "
                               "ERROR: Unexpected exception");
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::test_exception - "
                    "ERROR: expected and unknown exception caught\n"));
      }
  }

  void
  RG_LateBinding_Receiver_impl::start_reading (void)
  {
    ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::start_reading - "
                "Start reading\n"));
    ::RG_LateBinding::RG_LateBindingTestConnector::Reader_var reader =
      this->ciao_context_->get_connection_info_read_data ();
    try
      {
        RG_LateBindingTestSeq samples;
        ::CCM_DDS::ReadInfoSeq readinfo_seq;
        reader->read_all (samples, readinfo_seq);
        this->check_samples ("read", samples, this->expected_);
      }
    catch (const CORBA::Exception &e)
      {
        e._tao_print_exception("RG_LateBinding_Receiver_impl::start_reading - "
                               "ERROR: Unexpected exception");
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::start_reading - "
                    "ERROR: Caught unknow exception\n"));
      }
  }

  void
  RG_LateBinding_Receiver_impl::start_getting (void)
  {
    try
      {
        ::RG_LateBinding::RG_LateBindingTestConnector::Getter_var getter =
          this->ciao_context_->get_connection_info_get_fresh_data ();
        if (::CORBA::is_nil (getter.in ()))
          {
            ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::start_getting - "
                        "ERROR: Unable to get writer interface from the "
                        "CIAO context\n"));
            return;
          }
        DDS::Duration_t to;
        to.sec = 1;
        to.nanosec = 0;
        getter->time_out (to);

        RG_LateBindingTestSeq samples;
        ::CCM_DDS::ReadInfoSeq readinfos;
        getter->get_many (samples, readinfos);
        this->check_samples ("get", samples, 1);
      }
    catch (const CORBA::Exception &e)
      {
        e._tao_print_exception("RG_LateBinding_Receiver_impl::start_getting - "
                               "ERROR: Unexpected exception");
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::start_getting - "
                    "ERROR: expected and unknown exception caught\n"));
      }
  }

  void
  RG_LateBinding_Receiver_impl::set_topic_name_reader ()
  {
    ::RG_LateBinding::RG_LateBindingTestConnector::Reader_var reader =
      this->ciao_context_->get_connection_info_read_data ();
    if (::CORBA::is_nil (reader.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::start - "
                                "Unable to get reader interface\n"));
          throw ::CORBA::INTERNAL ();
        }
    ::CORBA::Object_var cmp = reader->_get_component ();
    if (::CORBA::is_nil (cmp.in ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::start - "
                              "Unable to get component interface\n"));
        throw ::CORBA::INTERNAL ();
      }
    ::RG_LateBinding::RG_LateBindingTestConnector::CCM_DDS_State_var conn =
      ::RG_LateBinding::RG_LateBindingTestConnector::CCM_DDS_State::_narrow (cmp.in ());
    if (::CORBA::is_nil (conn.in ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::start - "
                              "Unable to narrow connector interface\n"));
        throw ::CORBA::INTERNAL ();
      }
    conn->topic_name ("LateBindingTopic");
  }

  void
  RG_LateBinding_Receiver_impl::set_topic_name_getter ()
  {
    ::RG_LateBinding::RG_LateBindingTestConnector::Getter_var getter =
      this->ciao_context_->get_connection_info_get_fresh_data ();
    if (::CORBA::is_nil (getter.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::start - "
                                "Unable to get getter interface\n"));
          throw ::CORBA::INTERNAL ();
        }
      ::CORBA::Object_var cmp = getter->_get_component ();
      if (::CORBA::is_nil (cmp.in ()))
        {
          ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::start - "
                                "Unable to get component interface\n"));
          throw ::CORBA::INTERNAL ();
        }
    ::RG_LateBinding::RG_LateBindingTestConnector::CCM_DDS_Event_var conn =
      ::RG_LateBinding::RG_LateBindingTestConnector::CCM_DDS_Event::_narrow (cmp.in ());
    if (::CORBA::is_nil (conn.in ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::start - "
                              "Unable to narrow connector interface\n"));
        throw ::CORBA::INTERNAL ();
      }
    conn->topic_name ("LateBindingTopic");
  }

  void
  RG_LateBinding_Receiver_impl::start (
    ACE_Reactor *reactor)
  {
    if (reactor->schedule_timer (
               this->to_handler_,
               reinterpret_cast<const void *> (0),
               ACE_Time_Value (8, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::start - "
                              "Error scheduling timer"));
      }
    if (reactor->schedule_timer (
               this->to_handler_,
               reinterpret_cast<const void *> (1),
               ACE_Time_Value (10, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::start - "
                              "Error scheduling timer"));
      }
  }
}
