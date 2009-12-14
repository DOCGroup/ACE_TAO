// -*- C++ -*-
//
// $Id$


// Test for SAMPLE_LOST status of the subscriber


#include "PSL_SampleLost_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/ndds/Utils.h"


namespace CIAO_PSL_SampleLost_Receiver_Impl
{

//============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
  //============================================================

  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (void)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {

  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
  void ConnectorStatusListener_exec_i::on_inconsistent_topic(
     ::DDS::Topic_ptr /*the_topic*/,
     const DDS::InconsistentTopicStatus & /*status*/){
     // printf("Receiver: ConnectorStatusListener_exec_i::on_incompatible_topic\n");
    }
  void ConnectorStatusListener_exec_i::on_requested_incompatible_qos(
    ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::RequestedIncompatibleQosStatus & /*status*/)  {
     //printf("ConnectorStatusListener_exec_i::on_requested_incompatible_qos, status = %dl\n",status);
    }
  void ConnectorStatusListener_exec_i::on_sample_rejected(
     ::DDS::DataReader_ptr /*the_reader*/,
     const DDS::SampleRejectedStatus & /*status*/)  {
     //printf("ConnectorStatusListener_exec_i::on_sample_rejected\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_deadline_missed(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedDeadlineMissedStatus & /*status*/)  {
     //printf("Receiver: ConnectorStatusListener_exec_i::on_offered_deadline_missed\n");
    }
  void ConnectorStatusListener_exec_i::on_offered_incompatible_qos(
     ::DDS::DataWriter_ptr /*the_writer*/,
     const DDS::OfferedIncompatibleQosStatus & /*status*/)  {
     //printf("Receiver: ConnectorStatusListener_exec_i::on_offered_incompatible_qos\n");
    }
  void ConnectorStatusListener_exec_i::on_unexpected_status(
    ::DDS::Entity_ptr /*the_entity*/,
    ::DDS::StatusKind /*status_kind*/)  {
    //CORBA::ULong kind = status_kind;
    //printf("Receiver :ConnectorStatusListener_exec_i::on_unexpected_status #### status_kind = %d %s \n", kind, ::CIAO::DDS4CCM::translate_statuskind (kind));

    }

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
         this->pulse_callback_.read_all();
      }
    catch (...)
      {
        // @todo
      }
    return 0;
  }
  //============================================================
  // Facet Executor Implementation Class: TestTopic_RawListener_exec_i
  //============================================================

  TestTopic_RawListener_exec_i::TestTopic_RawListener_exec_i (void)
  {
  }

  TestTopic_RawListener_exec_i::~TestTopic_RawListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::TestTopic_RawListener

  void
  TestTopic_RawListener_exec_i::on_one_data (
    const TestTopic & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("TestTopic_RawListener: ")
            ACE_TEXT ("received test_topic_info for <%C> at %u\n"),
            an_instance.key.in (),
            an_instance.x));
  }

  void
  TestTopic_RawListener_exec_i::on_many_data (
    const TestTopic_Seq & /*an_instance */,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
  }
  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
  //============================================================

  PortStatusListener_exec_i::PortStatusListener_exec_i (Atomic_Boolean &sample_port_1, Atomic_Boolean &sample_port_2,int port_nr)
    : sample_port_1_(sample_port_1),
      sample_port_2_(sample_port_2),
      port_nr_(port_nr)

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



  }

  void
  PortStatusListener_exec_i::on_sample_lost (
    ::DDS::DataReader_ptr /* the_reader */,
    const ::DDS::SampleLostStatus & /* status */)
  {
    //printf("Receiver: PortStatusStatusListener_exec_i::SampleLostStatus\n");
    if(this->port_nr_ == 1)
    {
      this->sample_port_1_ = true;
    }
    if(this->port_nr_ == 2)
    {
      this->sample_port_2_ = true;
    }
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iTestTopic_RawListener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
  : rate_ (10),
    sample_port_1_ (false),
    sample_port_2_ (false)
  {
    this->ticker_ = new read_action_Generator (*this);
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Supported operations and attributes.
  void
  Receiver_exec_i::read_all (void)
  {
  //  printf("data\n");
    if (CORBA::is_nil (this->reader_.in ()))
      {
        return;
      }
    TestTopic_Seq_var TestTopic_infos;
    ::CCM_DDS::ReadInfoSeq_var readinfoseq;
    try{
      this->reader_->read_all(TestTopic_infos.out(), readinfoseq.out());
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
      for(CORBA::ULong i = 0; i < TestTopic_infos->length(); ++i)
      {
         CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
              ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
            i,
            TestTopic_infos[i].key.in (),
            TestTopic_infos[i].x));
            //printf("data key %s and x %ld\n",TestTopic_infos[i].key.in (),TestTopic_infos[i].x);
      }
    }
    catch(const CCM_DDS::InternalError& )
    {
      CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("internal error or no data\n")));
    }
  }
  // Component attributes.
  // Port operations.
  ::CCM_DDS::TestTopic::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new TestTopic RAW listener\n")));
    return new TestTopic_RawListener_exec_i (/*this->received_*/);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener get_info_out_status\n")));
    return new PortStatusListener_exec_i ( this->sample_port_1_,this->sample_port_2_, 1);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener get_info_get_status\n")));
    return new PortStatusListener_exec_i (this->sample_port_1_,this->sample_port_2_, 2);
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::PSL_SampleLost::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    this->reader_ = this->context_->get_connection_info_out_data();
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
       lc->mode (::CCM_DDS::NOT_ENABLED);

    // calculate the interval time
    long usec = 1000000 / this->rate_;
    if (this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->schedule_timer (
                                          this->ticker_,
                                          0,
                                          ACE_Time_Value(3, usec),
                                          ACE_Time_Value(3, usec)) == -1)
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
     if(!this->sample_port_1_.value ())
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                         ACE_TEXT ("error 'on_sample_lost' on DDS_Listen port in Receiver\n")
              ));
      }
    else if (!this->sample_port_2_.value ())
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                         ACE_TEXT ("error 'on_sample_lost' on DDS_Get port in Receiver\n")
              ));
      }
    else
      {
        CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("OK : Have received the expected ")
                               ACE_TEXT ("'on_sample_lost' in on DDS_Listen and DDS_GET port Receiver\n")
                    ));
      }
  }


  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_PSL_SampleLost_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

