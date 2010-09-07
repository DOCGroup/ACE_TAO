// -*- C++ -*-
//
// $Id$

// Test for statelistener: MANY_BY_MANY. Expect the operations on_creation, on_deletion, on_many_updates

#include "SL_ManyByMany_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/TimeUtilities.h"

#define ON_CREATION_EXPECTED 4
#define ON_MANY_EXPECTED 4
#define ON_DELETION_EXPECTED 4
#define ON_READER_EXPECTED 0

namespace CIAO_SL_ManyByMany_Receiver_Impl
{
  //============================================================
  // Facet Executor Implementation Class: ConnectorStatusListener_exec_i
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
  // Facet Executor Implementation Class: StateListener_exec_i
  //============================================================
  StateListener_exec_i::StateListener_exec_i (Atomic_Long &no_operation,
                                              Atomic_Long &on_creation,
                                              Atomic_Long &on_many_update,
                                              Atomic_Long &on_deletion,
                                              Atomic_Bool &create_data,
                                              Atomic_Bool &update_data)
  : no_operation_ (no_operation),
    on_creation_ (on_creation),
    on_many_update_ (on_many_update),
    on_deletion_ (on_deletion),
    create_data_ (create_data),
    update_data_ (update_data)
  {
  }

  StateListener_exec_i::~StateListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::StateListener
  void
  StateListener_exec_i::on_creation (const ::TestTopic & data,
                                     const ::CCM_DDS::ReadInfo & readinfo)
  {
    ++this->on_creation_;
    ACE_Time_Value tv;
    tv <<= readinfo.source_timestamp;
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("on_creation ReadInfo ")
                          ACE_TEXT ("-> UTC date =%#T\n"),
                          &tv));

    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Statelistener:on_creation : ")
                          ACE_TEXT ("Received datum for <%C> at %u\n"),
                          data.key.in (),
                          data.x));
    // One of the data must have the key 'KEY_1' with x == 1
    if (ACE_OS::strcmp (data.key.in(), "KEY_1") == 0 && data.x == 1L)
      {
        this->create_data_ = true;
      }
  }

  void
  StateListener_exec_i::on_one_update (const ::TestTopic & /*datum*/,
                                       const ::CCM_DDS::ReadInfo & /*info*/)
  {
    ++this->no_operation_;
  }

  void
  StateListener_exec_i::on_many_updates (const ::TestTopicSeq & data,
                                         const ::CCM_DDS::ReadInfoSeq & readinfoseq)
  {
    this->on_many_update_ += readinfoseq.length();
    for(CORBA::ULong i = 0; i < readinfoseq.length(); ++i)
      {
        if (readinfoseq[i].instance_status != CCM_DDS::INSTANCE_UPDATED)
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected ")
                                  ACE_TEXT ("info.instance_status ")
                                  ACE_TEXT ("'CCM_DDS::INSTANCE_UPDATED' ")
                                  ACE_TEXT ("with operation 'on_many_updates' ")
                                  ACE_TEXT ("from StateListener in Receiver\n")
                        ));

          }
        ACE_Time_Value tv;
        tv <<= readinfoseq[i].source_timestamp;
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("on_many_updates ReadInfo ")
                              ACE_TEXT ("-> UTC date =%#T\n"),
                              &tv));
      }
    for (CORBA::ULong i = 0; i < data.length(); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("on_many_updates info : ")
                              ACE_TEXT ("Number <%d> : received TestTopic_info ")
                              ACE_TEXT ("for <%C> at %u\n"),
                              i,
                              data[i].key.in (),
                              data[i].x));
        // one of the data must have the key 'KEY_1' with x == 2
        if (ACE_OS::strcmp(data[i].key,"KEY_1") == 0 && data[i].x == 2L)
          {
            this->update_data_ = true;
          }
       }
  }

  void
  StateListener_exec_i::on_deletion (const ::TestTopic & datum,
                                    const ::CCM_DDS::ReadInfo & info)
  {
    if(info.instance_status != CCM_DDS::INSTANCE_DELETED)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did not receive the expected info.instance_status ")
                              ACE_TEXT ("'CCM_DDS::INSTANCE_DELETED'")
                              ACE_TEXT ("  with operation 'on_deletion' from StateListener in Receiver\n")
                    ));

      }
    if (!datum.key.in() == 0 && info.instance_status == CCM_DDS::INSTANCE_DELETED)
      {
        ++this->on_deletion_;
      }
  }
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
  // Component Executor Implementation Class: Receiver_exec_i ();
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
  : rate_ (10),
    no_operation_ (0),
    on_creation_ (0),
    on_many_update_ (0),
    on_deletion_ (0),
    create_data_ (false),
    update_data_ (false),
    reader_data_ (0)
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
    ::SL_ManyByMany::SLManyByManyConnector::Reader_var reader =
      this->context_->get_connection_info_out_data ();

    if ( ::CORBA::is_nil (reader.in ()))
      {
        return;
      }
    try
      {
        TestTopicSeq TestTopic_infos;
        ::CCM_DDS::ReadInfoSeq readinfoseq;
        reader->read_all (TestTopic_infos, readinfoseq);
        this->reader_data_ += TestTopic_infos.length ();
        for (CORBA::ULong i = 0; i < readinfoseq.length (); ++i)
          {
            ACE_Time_Value tv;
            tv <<= readinfoseq[i].source_timestamp;
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL ReadInfo ")
                                  ACE_TEXT ("-> UTC date =%#T\n"),
                                  &tv));
          }
        for (CORBA::ULong i = 0; i < TestTopic_infos.length (); ++i)
          {
            ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("READ_ALL info : ")
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

  // Component attributes.
  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new PortStatuslistener\n")));
    return new PortStatusListener_exec_i ();
  }

  ::SL_ManyByMany::SLManyByManyConnector::CCM_StateListener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return new StateListener_exec_i (this->no_operation_,
                                     this->on_creation_,
                                     this->on_many_update_,
                                     this->on_deletion_,
                                     this->create_data_,
                                     this->update_data_);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::SL_ManyByMany::CCM_Receiver_Context::_narrow (ctx);
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
    ::CCM_DDS::StateListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (::CORBA::is_nil (lc.in ()))
      {
         ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw ::CORBA::INTERNAL ();
      }

    lc->mode (::CCM_DDS::MANY_BY_MANY);
    // calculate the interval time
    long const usec = 1000000 / this->rate_;
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
    if (this->no_operation_.value () > 0)
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: did receive an unexpected ")
                               ACE_TEXT ("operation 'on_one_update' from ")
                               ACE_TEXT ("StateListener in Receiver\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: did not receive an unexpected ")
                               ACE_TEXT ("operation 'on_one_update' from ")
                               ACE_TEXT ("StateListener in Receiver\n")
                    ));
      }

    if (this->on_creation_.value () != ON_CREATION_EXPECTED)
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT ("number of 'on_creation' calls: ")
                               ACE_TEXT ("expected <%d> - received <%d>\n"),
                               ON_CREATION_EXPECTED,
                               this->on_creation_.value ()
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: did receive the expected ")
                               ACE_TEXT ("number of 'on_creation' calls: ")
                               ACE_TEXT ("expected <%d> - received <%d>\n"),
                               ON_CREATION_EXPECTED,
                               this->on_creation_.value ()
                    ));
      }

    if (!this->create_data_ .value ())
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT ("sample in 'on_creation' from ")
                               ACE_TEXT ("StateListener in Receiver\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: did receive the expected ")
                               ACE_TEXT ("sample in 'on_creation' from ")
                               ACE_TEXT ("StateListener in Receiver\n")
                    ));
      }

    if(this->on_many_update_.value  () != ON_MANY_EXPECTED)
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT ("number of 'on_many_update' samples: ")
                               ACE_TEXT ("expected <%d> - received <%d>\n"),
                               ON_MANY_EXPECTED,
                               this->on_many_update_.value ()
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: did receive the expected ")
                               ACE_TEXT ("number of 'on_many_update' samples: ")
                               ACE_TEXT ("expected <%d> - received <%d>\n"),
                               ON_MANY_EXPECTED,
                               this->on_many_update_.value ()
                    ));
      }

    if (!this->update_data_.value ())
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR:didn't receive the expected ")
                               ACE_TEXT (" data with 'on_many_updates' from ")
                               ACE_TEXT ("StateListener in Receiver\n")
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: did receive the expected ")
                               ACE_TEXT ("sample in 'on_many_updates' from ")
                               ACE_TEXT ("StateListener in Receiver\n")
                    ));
      }

    if (this->on_deletion_.value () != ON_DELETION_EXPECTED)
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't receive the expected ")
                               ACE_TEXT ("number of 'on_deletion' calls: ")
                               ACE_TEXT ("expected <%d> - received <%d>\n"),
                               ON_DELETION_EXPECTED,
                               this->on_deletion_.value ()
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: did receive the expected ")
                               ACE_TEXT ("number of 'on_deletion' calls: ")
                               ACE_TEXT ("expected <%d> - received <%d>\n"),
                               ON_DELETION_EXPECTED,
                               this->on_deletion_.value ()
                    ));
      }

    if (this->reader_data_.value () != ON_READER_EXPECTED)
      {
         ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: didn't read the expected ")
                               ACE_TEXT ("number of samples: ")
                               ACE_TEXT ("expected <%d> - received <%d>\n"),
                               ON_READER_EXPECTED,
                               this->reader_data_.value ()
                    ));
      }
    else
      {
         ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: read the expected ")
                               ACE_TEXT ("number of samples: ")
                               ACE_TEXT ("expected <%d> - received <%d>\n"),
                               ON_READER_EXPECTED,
                               this->reader_data_.value ()
                    ));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_SL_ManyByMany_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
