// $Id$
#include "RG_LateBinding_Receiver_impl.h"
#include "tao/ORB_Core.h"
#include "ace/Reactor.h"

namespace CIAO_RG_LateBinding_Receiver_Impl
{

  /**
   * Check last
   */
  LastSampleChecker::LastSampleChecker (RG_LateBinding_Receiver_impl &callback,
                                        const ::CORBA::UShort &iterations)
    : callback_ (callback)
      , iterations_ (iterations)
  {
  }

  LastSampleChecker::~LastSampleChecker ()
  {
  }

  int
  LastSampleChecker::handle_timeout (const ACE_Time_Value &, const void *)
  {
    ACE_DEBUG ((LM_DEBUG, "Checking if last sample "
                          "is available in DDS...\n"));
    if (this->callback_.check_last ())
      {
        this->callback_.start_read ();
      }
    return 0;
  }

  /**
   * RG_LateBinding_Receiver_impl
   */
  RG_LateBinding_Receiver_impl::RG_LateBinding_Receiver_impl (
      ::RG_LateBinding::CCM_Receiver_Context_ptr ctx,
      const ::CORBA::UShort & iterations,
      const ::CORBA::UShort & keys)
    : ciao_context_ (
        ::RG_LateBinding::CCM_Receiver_Context::_duplicate (ctx))
    , iterations_ (iterations)
    , keys_ (keys)
  {
  }

  RG_LateBinding_Receiver_impl::~RG_LateBinding_Receiver_impl ()
  {
    delete this->checker_;
  }

  ACE_Reactor*
  RG_LateBinding_Receiver_impl::reactor (void)
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

  bool
  RG_LateBinding_Receiver_impl::check_last (void)
  {
    ::RG_LateBinding::RG_LateBindingTestConnector::Reader_var reader =
      this->ciao_context_->get_connection_info_read_data ();
    try
      {
        RG_LateBindingTest datum;
        ::CCM_DDS::ReadInfo readinfo;
        char key[10];
        ACE_OS::sprintf (key, "KEY_%d", this->keys_);
        datum.key = CORBA::string_dup (key);
        reader->read_one_last (
                datum,
                readinfo,
                ::DDS::HANDLE_NIL);
        ACE_DEBUG ((LM_DEBUG, "Receiver_exec_i::check_last - "
                              "last iteration <%02d> - <%02d>\n",
                               datum.iteration,
                               this->iterations_));
        return datum.iteration >= this->iterations_;
      }
    catch (const ::CCM_DDS::InternalError &)
      {
      }
    catch (const ::CCM_DDS::NonExistent &)
      {
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::check_last: "
                              "ERROR: Unexpected exception caught\n"));
      }
    return false;
  }

  void
  RG_LateBinding_Receiver_impl::start (void)
  {
    this->test_exception ();
    this->set_topic_name_reader ();

    ACE_NEW_THROW_EX (this->checker_,
                      LastSampleChecker (*this,
                                         this->iterations_),
                      ::CORBA::NO_MEMORY ());
    if (this->reactor ()->schedule_timer (this->checker_,
                                          0,
                                          ACE_Time_Value (1, 0),
                                          ACE_Time_Value (1, 0)) == -1)
      {
        ACE_ERROR ((LM_ERROR, "Receiver_exec_i::schedule_timer - "
                  "ERROR: Error while starting LastSampleChecker\n"));
      }
  }

  void
  RG_LateBinding_Receiver_impl::start_read ()
  {
    if (this->checker_)
      {
        this->reactor ()->cancel_timer (this->checker_);
      }
    this->start_reading ();
    this->set_topic_name_getter ();
    this->start_getting ();
    this->set_topic_name_reader (true);
    this->set_topic_name_getter (true);
  }

  void
  RG_LateBinding_Receiver_impl::check_samples (
    const char * test,
    const RG_LateBindingTestSeq& samples,
    const ::CORBA::UShort& expected)
  {
    bool error = samples.length () != expected;
    if (ACE_OS::strcmp (test, "get") == 0)
      {
        error = !(samples.length () > 0);
      }
    if (error)
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
        if (! ::CORBA::is_nil (this->ciao_context_.in ()))
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
        else
          {
            ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::test_exception - "
                        "ERROR: CIAO context seems to be NIL\n"));
          }
      }
    catch (const ::CORBA::BAD_INV_ORDER &)
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
        this->check_samples ("read", samples, this->iterations_ * this->keys_);
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
        this->check_samples ("get", samples);
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
  RG_LateBinding_Receiver_impl::set_topic_name_reader (bool test_non_changeable)
  {
    try
      {
        ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::set_topic_name_reader - "
                    "Setting topic name\n"));
        ::RG_LateBinding::RG_LateBindingTestConnector::Reader_var reader =
          this->ciao_context_->get_connection_info_read_data ();
        if (::CORBA::is_nil (reader.in ()))
            {
              ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::set_topic_name_reader - "
                                    "Unable to get reader interface\n"));
              throw ::CORBA::INTERNAL ();
            }
        ::CORBA::Object_var cmp = reader->_get_component ();
        if (::CORBA::is_nil (cmp.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::set_topic_name_reader - "
                                  "Unable to get component interface\n"));
            throw ::CORBA::INTERNAL ();
          }
        ::RG_LateBinding::RG_LateBindingTestConnector::CCM_DDS_State_var conn =
          ::RG_LateBinding::RG_LateBindingTestConnector::CCM_DDS_State::_narrow (cmp.in ());
        if (::CORBA::is_nil (conn.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::set_topic_name_reader - "
                                  "Unable to narrow connector interface\n"));
            throw ::CORBA::INTERNAL ();
          }
        if (test_non_changeable)
          {
            conn->topic_name ("LateBindingTopicSecondTime");
            ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::set_topic_name_reader - "
                        "ERROR: No NonChangeable exception thrown.\n"));
          }
        else
          {
            conn->topic_name ("LateBindingTopic");
          }
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        if (test_non_changeable)
          {
            ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::set_topic_name_reader - "
                        "Expected NonChangeable caught.\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::set_topic_name_reader - "
                        "Caught NonChangeable exception.\n"));
          }
      }
  }

  void
  RG_LateBinding_Receiver_impl::set_topic_name_getter (bool test_non_changeable)
  {
    try
      {
        ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::set_topic_name_getter - "
                    "Setting topic name\n"));
        ::RG_LateBinding::RG_LateBindingTestConnector::Getter_var getter =
          this->ciao_context_->get_connection_info_get_fresh_data ();
        if (::CORBA::is_nil (getter.in ()))
            {
              ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::set_topic_name_getter - "
                                    "Unable to get getter interface\n"));
              throw ::CORBA::INTERNAL ();
            }
          ::CORBA::Object_var cmp = getter->_get_component ();
          if (::CORBA::is_nil (cmp.in ()))
            {
              ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::set_topic_name_getter - "
                                    "Unable to get component interface\n"));
              throw ::CORBA::INTERNAL ();
            }
        ::RG_LateBinding::RG_LateBindingTestConnector::CCM_DDS_Event_var conn =
          ::RG_LateBinding::RG_LateBindingTestConnector::CCM_DDS_Event::_narrow (cmp.in ());
        if (::CORBA::is_nil (conn.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR: RG_LateBinding_Receiver_impl::set_topic_name_getter - "
                                  "Unable to narrow connector interface\n"));
            throw ::CORBA::INTERNAL ();
          }
        if (test_non_changeable)
          {
            conn->topic_name ("LateBindingTopicSecondTime");
            ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::set_topic_name_getter - "
                        "ERROR: No NonChangeable exception thrown.\n"));
          }
        else
          {
            conn->topic_name ("LateBindingTopic");
          }
      }
    catch (const ::CCM_DDS::NonChangeable &)
      {
        if (test_non_changeable)
          {
            ACE_DEBUG ((LM_DEBUG, "RG_LateBinding_Receiver_impl::set_topic_name_getter - "
                        "Expected NonChangeable caught.\n"));
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "RG_LateBinding_Receiver_impl::set_topic_name_getter - "
                        "ERROR: Caught NonChangeable exception.\n"));
          }
      }
  }
}
