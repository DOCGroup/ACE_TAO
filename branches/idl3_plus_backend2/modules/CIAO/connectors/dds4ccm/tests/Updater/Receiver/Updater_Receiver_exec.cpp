// -*- C++ -*-
// $Id$

// Test for UPDATER 

#include "Updater_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/ndds/Utils.h"

namespace CIAO_Updater_Receiver_Impl
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
      //   this->pulse_callback_.read_all();
      }
    catch (...)
      {
        // @todo
      }
    return 0;
  }
  //============================================================
  // Facet Executor Implementation Class: StateListener_exec_i
  //============================================================
  StateListener_exec_i::StateListener_exec_i (void)
  {
  }

  StateListener_exec_i::~StateListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::StateListener
  void
      StateListener_exec_i::on_creation (const ::TestTopic & /*datum*/,
        const ::CCM_DDS::ReadInfo & /*info*/)
  {}
  
   void
      StateListener_exec_i::on_one_update (
        const ::TestTopic & /*datum*/,
        const ::CCM_DDS::ReadInfo & /*info*/)
   {}
      
   void
      StateListener_exec_i::on_many_updates (
        const ::TestTopic_Seq & /*data*/,
        const ::CCM_DDS::ReadInfoSeq & /*infos*/)
   {}
   void
      StateListener_exec_i::on_deletion (
        const ::TestTopic & /*datum*/,
        const ::CCM_DDS::ReadInfo & /*info*/)
   {}
  //============================================================
  // Facet Executor Implementation Class: PortStatusListener_exec_i
  //============================================================
  PortStatusListener_exec_i::PortStatusListener_exec_i ()
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
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iTestTopic_RawListener_exec_i ();
  //============================================================

  Receiver_exec_i::Receiver_exec_i (void)
  : rate_ (10)
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
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
              ACE_TEXT ("-> UTC date = %02d:%02d:%02d.%d\n"),
                              time ? time->tm_hour : 0,
                              time ? time->tm_min : 0,
                              time ? time->tm_sec : 0,
                              readinfoseq[i].source_timestamp.nanosec));
        }
      for(CORBA::ULong i = 0; i < TestTopic_infos->length(); ++i)
        {
           ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
                ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
              i,
              TestTopic_infos[i].key.in (),
              TestTopic_infos[i].x));
        }
    }
    catch( const CCM_DDS::InternalError& )
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("internal error or no data\n")));
    }
  }
  // Component attributes.
  // Port operations.
  
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

  ::CCM_DDS::TestTopic::CCM_StateListener_ptr
    Receiver_exec_i::get_info_out_data_listener (void)
  {
    return new StateListener_exec_i();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Updater::CCM_Receiver_Context::_narrow (ctx);
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
    ::CCM_DDS::StateListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

     if (CORBA::is_nil (lc.in ()))
      {
         ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
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
        ACE_ERROR ((LM_ERROR, "Unable to schedule Timer\n"));
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
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Updater_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
