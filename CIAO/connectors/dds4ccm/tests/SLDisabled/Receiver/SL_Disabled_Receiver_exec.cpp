// -*- C++ -*-
//
// $Id$

// Test for statelistener: NOT_ENABLED

#include "SL_Disabled_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/dds/Utils.h"
#include "dds4ccm/impl/dds/TimeUtilities.h"

namespace CIAO_SL_Disabled_Receiver_Impl
{
  //============================================================
  // read_action_Generator
  //============================================================
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
  // StateListener_exec_i
  //============================================================
  StateListener_exec_i::StateListener_exec_i (Atomic_Boolean &no_operation)
    :no_operation_(no_operation)
  {
  }

  StateListener_exec_i::~StateListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::StateListener
  void
  StateListener_exec_i::on_creation (const ::TestTopic & /*datum*/,
                                     const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->no_operation_ = false;
  }

  void
  StateListener_exec_i::on_one_update (const ::TestTopic & /*datum*/,
                                       const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->no_operation_ = false;
  }

  void
  StateListener_exec_i::on_many_updates (const ::TestTopic_Seq & /*data*/,
                                         const ::CCM_DDS::ReadInfoSeq & /*infos*/)
  {
    this->no_operation_ = false;
  }

  void
  StateListener_exec_i::on_deletion (const ::TestTopic & /*datum*/,
                                     const ::CCM_DDS::ReadInfo & /*info*/)
  {
    this->no_operation_ = false;
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
  : rate_ (10),
    no_operation_ (true),
    updater_data_ (false)
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
    try
      {
        this->reader_->read_all(TestTopic_infos.out(), readinfoseq.out());
        for (CORBA::ULong i = 0; i < readinfoseq->length(); ++i)
          {
            this->updater_data_ = true;
            ACE_Time_Value tv;
            tv <<= readinfoseq[i].source_timestamp;
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                                  ACE_TEXT ("-> UTC date =%#T\n"),
                                  &tv));
          }
        for (CORBA::ULong i = 0; i < TestTopic_infos->length(); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL keyed test info : ")
                       ACE_TEXT ("Number <%d> : received TestTopic_info for <%C> at %u\n"),
                        i,
                        TestTopic_infos[i].key.in (),
                        TestTopic_infos[i].x));
          }
      }
    catch (const CCM_DDS::InternalError& )
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("internal error or no data\n")));
      }
  }

  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::TestTopic::CCM_StateListener_ptr
    Receiver_exec_i::get_info_out_data_listener (void)
  {
     return new StateListener_exec_i(this->no_operation_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::SL_Disabled::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    this->reader_ = this->context_->get_connection_info_out_data ();
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
    if (!this->no_operation_.value ()|| !this->updater_data_.value())
      {

         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Received an unexpected ")
                               ACE_TEXT (" operation. StateListener or Updater doesn't work in Receiver")
                    ));
      }

    else
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK : Haven't received an  unexpected ")
                              ACE_TEXT (" oparation from StateListener in Receiver")
                   ));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_Disabled_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
