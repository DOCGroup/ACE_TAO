// -*- C++ -*-
//
// $Id$

#include "Keyed_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Keyed_Test_Receiver_Impl
{
  read_action_Generator::read_action_Generator (Receiver_exec_i &callback)
    : pulse_callback_ (callback)
  {
  }

  read_action_Generator::~read_action_Generator ()
  {
  }

  int
  read_action_Generator::handle_timeout (const ACE_Time_Value &, const void *)
  {
    try
      {
        if (pulse_callback_.read_data ())
          {
            this->pulse_callback_.read_one();
            this->pulse_callback_.read_all();
          }
        if (pulse_callback_.get_data ())
          {
            this->pulse_callback_.get_one ();
            this->pulse_callback_.get_all ();
          }
      }
    catch (...)
      {
        // @todo
      }
    return 0;
  }

  //============================================================
  // Facet Executor Implementation Class: KeyedTest_Listener_exec_i
  //============================================================

  KeyedTest_Listener_exec_i::KeyedTest_Listener_exec_i (Atomic_ULong &received)
      : received_ (received)
  {
  }

  KeyedTest_Listener_exec_i::~KeyedTest_Listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::KeyedTest_Listener
  void
  KeyedTest_Listener_exec_i::on_many_data (
    const KeyedTest_Seq & /* an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }

  void
  KeyedTest_Listener_exec_i::on_one_data (
    const KeyedTest & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ++this->received_;
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("KeyedTest_Listener: ")
            ACE_TEXT ("received keyed_test_info for <%C> at %u\n"),
            an_instance.key.in (),
            an_instance.iteration));
  }

  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
  //============================================================

  PortStatusListener_exec_i::PortStatusListener_exec_i (void)
  {
  }

  PortStatusListener_exec_i::~PortStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::PortStatusListener

  void
  PortStatusListener_exec_i::on_requested_deadline_missed (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::RequestedDeadlineMissedStatus & /* status */)
  {
    /* Your code here. */
  }

  void
  PortStatusListener_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {

  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iKeyedTest_Listener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
    : rate_ (0),
      iterations_ (10),
      keys_ (5),
      get_data_ (true),
      read_data_ (true),
      raw_listen_ (false),
      expected_ (50),
      received_ (0)

  {
    this->ticker_ = new read_action_Generator (*this);
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.
  void
  Receiver_exec_i::read_one (void)
  {
    if (CORBA::is_nil (this->reader_.in ()))
      {
        return;
      }
    try
      {
        for (CORBA::UShort i = 1; i < this->keys_ + 1; ++i)
          {
            KeyedTest keyedtest_info;
            char key[100];
            ACE_OS::sprintf (key, "KEY_%d", i);
            keyedtest_info.key = CORBA::string_dup (key);
            ::CCM_DDS::ReadInfo readinfo;
            this->reader_->read_one_last (keyedtest_info, readinfo, ::DDS::HANDLE_NIL);
            ++this->received_;
            time_t tim = readinfo.source_timestamp.sec;
            tm* time = ACE_OS::localtime(&tim);
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ONE Read_Info ")
                      ACE_TEXT (" -> date = %02d:%02d:%02d.%d\n"),
                                time ? time->tm_hour : 0,
                                time ? time->tm_min : 0,
                                time ? time->tm_sec : 0,
                                readinfo.source_timestamp.nanosec));
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ ONE keyed test info : ")
                ACE_TEXT ("received keyedtest_info for <%C> at %u\n"),
                keyedtest_info.key.in (),
                keyedtest_info.iteration));
          }
      }
    catch(CCM_DDS::NonExistent& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("KeyedTest_Read_One: ")
                  ACE_TEXT ("no keyedtest_info received\n")));
      }
  }

  void
  Receiver_exec_i::read_all (void)
  {
    if (CORBA::is_nil (this->reader_.in ()))
      {
        return;
      }
      
    KeyedTest_Seq_var keyedtest_infos;
    ::CCM_DDS::ReadInfoSeq_var readinfoseq;
    this->reader_->read_all(keyedtest_infos.out(), readinfoseq.out());
    for(CORBA::ULong i = 0; i < readinfoseq->length(); ++i)
      {
        time_t tim = readinfoseq[i].source_timestamp.sec;
        tm* time = ACE_OS::localtime(&tim);
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
            ACE_TEXT ("-> UTC date = %02d:%02d:%02d.%d\n"),
                            time ? time->tm_hour : 0,
                            time ? time->tm_min : 0,
                            time ? time->tm_sec : 0,
                            readinfoseq[i].source_timestamp.nanosec));
      }
    for(CORBA::ULong i = 0; i < keyedtest_infos->length(); ++i)
      {
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
              ACE_TEXT ("Number <%d> : received keyedtest_info for <%C> at %u\n"),
            i,
            keyedtest_infos[i].key.in (),
            keyedtest_infos[i].iteration));
      }
  }

  void
  Receiver_exec_i::get_one (void)
  {
    if (CORBA::is_nil (this->getter_.in ()))
      {
        return;
      }
      
    KeyedTest_var keyedtest_info;
    //keyedtest_info.key = "KEY1";
    ::CCM_DDS::ReadInfo_var readinfo;

    try
      {
        if (this->getter_->get_one (keyedtest_info.out (), readinfo.out ()))
          {
            time_t tim = readinfo->source_timestamp.sec;
            tm* time = ACE_OS::localtime(&tim);
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE ReadInfo -> ")
                                   ACE_TEXT ("date = %02d:%02d:%02d.%d\n"),
                                time ? time->tm_hour : 0,
                                time ? time->tm_min : 0,
                                time ? time->tm_sec : 0,
                                readinfo->source_timestamp.nanosec));
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE KeyedTest : ")
                                   ACE_TEXT ("received keyedtest_info for <%C> at %u\n"),
                keyedtest_info->key.in (),
                keyedtest_info->iteration));
          }
        else
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("GET_ONE No data available for <%C>\n"),
                    keyedtest_info->key.in ()));
          }
      }
    catch(CCM_DDS::NonExistent& )
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("KeyedTest_Read_One: no keyedtest_info received\n")));
      }
  }

  void
  Receiver_exec_i::get_all (void)
  {
    if (CORBA::is_nil (this->getter_.in ()))
      {
        return;
      }
  }

  // Component attributes.
  ::CORBA::ULong
  Receiver_exec_i::rate (void)
  {
    return this->rate_;
  }

  void
  Receiver_exec_i::rate (
    ::CORBA::ULong rate)
  {
    this->rate_ = rate;
  }

  ::CORBA::UShort
  Receiver_exec_i::iterations (void)
  {
    return this->iterations_;
  }

  void
  Receiver_exec_i::iterations (::CORBA::UShort iterations)
  {
    this->iterations_ = iterations;
    this->expected_ = this->iterations_ * this->keys_;
  }

  ::CORBA::UShort
  Receiver_exec_i::keys (void)
  {
    return this->keys_;
  }

  void
  Receiver_exec_i::keys (::CORBA::UShort keys)
  {
    this->keys_ = keys;
    this->expected_ = this->iterations_ * this->keys_;
  }

  ::CORBA::Boolean
  Receiver_exec_i::get_data (void)
  {
    return this->get_data_;
  }

  void
  Receiver_exec_i::get_data (
    ::CORBA::Boolean get_data)
  {
    this->get_data_ = get_data;
  }

  ::CORBA::Boolean
  Receiver_exec_i::read_data (void)
  {
    return this->read_data_;
  }

  void
  Receiver_exec_i::read_data (
    ::CORBA::Boolean read_data)
  {
    this->read_data_ = read_data;
  }

  ::CORBA::Boolean
  Receiver_exec_i::raw_listen (void)
  {
    return this->raw_listen_;
  }

  void
  Receiver_exec_i::raw_listen (
    ::CORBA::Boolean raw_listen)
  {
    this->raw_listen_ = raw_listen;
  }

  // Port operations.
  ::CCM_DDS::KeyedTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new KeyedTest RAW listener\n")));
    return new KeyedTest_Listener_exec_i (this->received_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Keyed_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    if (this->read_data ())
      {
       this->reader_ = this->context_->get_connection_info_out_data();
      }
    if (this->get_data ())
      {
        this->getter_ = this->context_->get_connection_info_get_fresh_data();
      }
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (CORBA::is_nil (lc.in ()))
      {
        CIAO_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->mode (this->raw_listen_ ? ::CCM_DDS::ONE_BY_ONE : ::CCM_DDS::NOT_ENABLED);

    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(0, usec),
                                          ACE_Time_Value(0, usec)) == -1)
      {
        CIAO_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
      }
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->cancel_timer (this->ticker_);
    delete this->ticker_;
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    if (this->received_ != this->expected_)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected")
                               ACE_TEXT (" number of samples! Received <%d> ")
                               ACE_TEXT ("Expected <%d>\n"),
          this->received_.value (), this->expected_));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Keyed_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

