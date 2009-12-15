// -*- C++ -*-
//
// $Id$

#include "LMBM_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"

namespace CIAO_LMBM_Test_Receiver_Impl
{
  //============================================================
  // ListenManyByManyTest_Listener_exec_i
  //============================================================
  ListenManyByManyTest_Listener_exec_i::ListenManyByManyTest_Listener_exec_i (
                                              Atomic_ULong &received_one_by_one,
                                              Atomic_ULong &received_many_by_many,
                                              Atomic_Bool &many_received)
    : received_one_by_one_ (received_one_by_one),
      received_many_by_many_ (received_many_by_many),
      many_received_ (many_received)

  {
  }

  ListenManyByManyTest_Listener_exec_i::~ListenManyByManyTest_Listener_exec_i (void)
  {
  }

  void
  ListenManyByManyTest_Listener_exec_i::on_one_data (
                                  const ListenManyByManyTest & /*an_instance*/,
                                  const ::CCM_DDS::ReadInfo & /*info*/)
  {
    //just count; this value is checked on ccm_remove.
    ++this->received_one_by_one_;
  }

  void
  ListenManyByManyTest_Listener_exec_i::on_many_data (
                                  const ListenManyByManyTest_Seq & an_instance,
                                  const ::CCM_DDS::ReadInfoSeq & info)
  {
    if (an_instance.length () == 0)
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: ListenManyByManyTest_Listener_exec_i::on_many_data:"
                               "instance sequence length is nil\n"));
        return;
      }
    for (CORBA::ULong i = 0 ; i < info.length(); ++i)
      {
        CIAO_DEBUG ((LM_DEBUG, "ListenManyByManyTest_Listener_exec_i::on_many_data:"
                               "key <%C> - iteration <%d>\n",
                               an_instance[i].key.in (),
                               an_instance[i].iteration));
        if (!info[i].instance_handle.isValid)
          {
            CIAO_ERROR ((LM_ERROR, "ERROR: ListenManyByManyTest_Listener_exec_i::on_many_data:"
                                "instance handle %d seems to be invalid"
                                "key <%C> - iteration <%d>\n",
                                 i,
                                 an_instance[i].key.in (),
                                 an_instance[i].iteration));
          }
        if (info[i].source_timestamp.sec == 0 &&
            info[i].source_timestamp.nanosec == 0)
          {
            CIAO_ERROR ((LM_ERROR, "ERROR: ListenManyByManyTest_Listener_exec_i::on_one_data: "
                                "source timestamp seems to be invalid (nil) "
                                "key <%C> - iteration <%d>\n",
                                an_instance[i].key.in (),
                                an_instance[i].iteration));
          }
      }
    this->received_many_by_many_ += an_instance.length ();
    if (!this->many_received_.value () && an_instance.length () > 1)
      {
        this->many_received_ = true;
      }
  }

  //============================================================
  // ConnectorStatusListener_exec_i
  //============================================================
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (
            Receiver_exec_i &callback,
            const CORBA::ULong &expected)
    : callback_ (callback),
      expected_ (expected),
      received_ (0)
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
    if (status_kind == ::DDS::DATA_ON_READERS_STATUS)
      ++this->received_;
      if (this->received_ >= this->expected_)
      {
        this->callback_.start ();
      }
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : received_one_by_one_ (0),
      received_many_by_many_ (0),
      started_ (false),
      many_received_ (false),
      expected_ (50),
      iterations_ (10),
      keys_ (5)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::start ()
  {
    if (!this->started_.value ())
      {
        this->started_ = true;
        ::CCM_DDS::DataListenerControl_var dlc =
            this->context_->get_connection_info_listen_data_control ();
        dlc->mode (::CCM_DDS::MANY_BY_MANY);
      }
  }

  ::CCM_DDS::ListenManyByManyTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_listen_data_listener (void)
  {
    return new ListenManyByManyTest_Listener_exec_i (
                this->received_one_by_one_,
                this->received_many_by_many_,
                this->many_received_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_listen_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (
                    *this,
                    this->expected_);
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
    this->expected_ = this->keys_ * this->iterations_;
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
    this->expected_ = this->keys_ * this->iterations_;
  }

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::LMBM_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
  }

  void
  Receiver_exec_i::ccm_activate (void)
  {
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    this->expected_ = this->keys_ * this->iterations_;
    if (this->received_many_by_many_.value () == 0)
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: MANY_BY_MANY: "
                               "Didn't receive samples on many_by_many "
                               "callback while mode is MANY_BY_MANY "
                               "expected <%u> - received <%u>\n",
                               this->expected_,
                               this->received_many_by_many_.value ()));
      }
    else if (this->expected_ != this->received_many_by_many_.value ())
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: MANY_BY_MANY: "
                               "Didn't receive the expected "
                               "number of samples on many_by_many "
                               "callback while mode is MANY_BY_MANY "
                               "expected <%u> - received <%u>\n",
                               this->expected_,
                               this->received_many_by_many_.value ()));
      }
    if (this->received_one_by_one_.value () > 0)
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: MANY_BY_MANY: "
                               "Received samples on one_by_one "
                               "callback while mode is MANY_BY_MANY "
                               "expected <0> - received <%u>\n",
                               this->received_one_by_one_.value ()));
      }
    if (!this->started_.value ())
      {
        CIAO_ERROR ((LM_ERROR, "ERROR: MANY_BY_MANY: "
                               "Didn't received DATA_ON_READERS_STATUS on "
                               "ConnectorStatusListener\n"));
      }
    if (this->received_one_by_one_.value () == 0   &&
        this->received_many_by_many_.value () > 0 &&
        this->started_.value ())
      {
        if (this->many_received_.value ())
          {
            CIAO_DEBUG ((LM_DEBUG, "MANY_BY_MANY: "
                                   "Received only data on "
                                   "many_by_many callback. "
                                   "Test passed!\n"));
          }
        else
          {
            CIAO_DEBUG ((LM_ERROR, "ERROR: MANY_BY_MANY: "
                                   "Received only data on "
                                   "many_by_many callback but "
                                   "all samples were received "
                                   "one by one.\n"));
          }
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_LMBM_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
