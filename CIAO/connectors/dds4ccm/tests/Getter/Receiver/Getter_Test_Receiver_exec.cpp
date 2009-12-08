// -*- C++ -*-
//
// $Id$

#include "Getter_Test_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Getter_Test_Receiver_Impl
{
  ConnectorStatusListener_exec_i::ConnectorStatusListener_exec_i (Receiver_exec_i &callback)
    : callback_ (callback),
      has_run_ (false)
  {
  }

  ConnectorStatusListener_exec_i::~ConnectorStatusListener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::ConnectorStatusListener
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
    ::DDS::StatusKind )
  {
  }

  Invoker_exec_i::Invoker_exec_i (Receiver_exec_i & callback)
    : callback_ (callback)
  {
  }

  Invoker_exec_i::~Invoker_exec_i (void)
  {
  }

  void
  Invoker_exec_i::set_getter_properties (CORBA::UShort nr_keys,
                                         CORBA::UShort nr_iterations)
  {
    this->callback_.keys (nr_keys);
    this->callback_.iterations (nr_iterations);
  }

  void 
  Invoker_exec_i::start_getting_data (const char * key,
                                     CORBA::Long iteration)
  {
    this->callback_.start_getting_data (key, iteration);
  }

  void
  Invoker_exec_i::get_no_data ()
  {
    this->callback_.get_no_data ();
  }


  GetterHandler::GetterHandler (Receiver_exec_i &callback,
                                const char * key,
                                CORBA::Long iteration)
    : callback_ (callback),
      key_ (key),
      iteration_ (iteration)
  {
  }

  GetterHandler::~GetterHandler ()
  {
  }

  int
  GetterHandler::handle_exception (ACE_HANDLE)
  {
    this->callback_.get_data (this->key_, this->iteration_);
    return 0;
  }

  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (10),
      keys_ (5)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  void
  Receiver_exec_i::start_getting_data (const char * key,
                                          CORBA::Long iteration)
  {
    GetterHandler* rh = new  GetterHandler (*this, CORBA::string_dup (key), iteration);
    this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ()->notify (rh);
  }

  void 
  Receiver_exec_i::get_data (const char * key, CORBA::Long iteration)
  {
    DDS::Duration_t to;
    to.sec = 10;
    to.nanosec = 0;
    this->getter_->time_out (to);
    CIAO_DEBUG ((LM_DEBUG, CLINFO ACE_TEXT ("Receiver_exec_i::get_data: ")
                              ACE_TEXT ("Start getting data from DDS: ")
                              ACE_TEXT ("key <%C> - iteration <%d> ")
                              ACE_TEXT (" with timeout: ")
                              ACE_TEXT ("sec <%d> - nanosec <%u>\n"),
                              key, iteration,
                              this->getter_->time_out ().sec,
                              this->getter_->time_out ().nanosec));
    GetterTest *gettertest_info = new GetterTest;
    ::CCM_DDS::ReadInfo readinfo;
    bool result = this->getter_->get_one (gettertest_info, readinfo);
    if (result)
      {
        if (ACE_OS::strcmp (gettertest_info->key, key) != 0)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("ERROR: GET DATA: ")
                                  ACE_TEXT ("Expected key does ")
                                  ACE_TEXT ("not match received key: ")
                                  ACE_TEXT ("expected <%C> - received <%C>\n"),
                                  key,
                                  gettertest_info->key.in ()));
          }
        if (gettertest_info->iteration != iteration)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("ERROR: GET DATA: ")
                                  ACE_TEXT ("Expected iteration does ")
                                  ACE_TEXT ("not match received iteration: ")
                                  ACE_TEXT ("expected <%d> - received <%d>\n"),
                                  iteration,
                                  gettertest_info->iteration));
          }
        CIAO_DEBUG ((LM_DEBUG, CLINFO ACE_TEXT ("Receiver_exec_i::get_data: ")
                              ACE_TEXT ("Returned data : key <%C> - iteration <%d>\n"),
                              gettertest_info->key.in (),
                              gettertest_info->iteration));
      }
    else
      {
        CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("ERROR: GET DATA: ")
                              ACE_TEXT ("Time out while waiting for ")
                              ACE_TEXT ("key <%C> - iteration <%d>\n"),
                              key,
                              iteration));
      }
  }

  void
  Receiver_exec_i::get_no_data ()
  {
    try
      {
        DDS::Duration_t to;
        to.sec = 1;
        to.nanosec = 0;
        this->getter_->time_out (to);
        CIAO_DEBUG ((LM_DEBUG, CLINFO ACE_TEXT ("Receiver_exec_i::get_no_data: ")
                                  ACE_TEXT ("Start getting data from DDS: timeout: ")
                                  ACE_TEXT ("sec <%d> - nanosec <%u>\n"),
                                  this->getter_->time_out ().sec,
                                  this->getter_->time_out ().nanosec));
        GetterTest *gettertest_info = new GetterTest;
        ::CCM_DDS::ReadInfo readinfo;
        bool result = this->getter_->get_one (gettertest_info, readinfo);
        if (result)
          {
            CIAO_ERROR ((LM_ERROR, CLINFO ACE_TEXT ("ERROR: GET NO DATA: ")
                                 ACE_TEXT ("Returning true when get no data.\n")));
          }
        else
          {
            CIAO_DEBUG ((LM_DEBUG, CLINFO ACE_TEXT ("Receiver_exec_i::get_no_data: ")
                              ACE_TEXT ("Expected to return no data.\n")));
          }
      }
    catch(CCM_DDS::InternalError& ex)
      {
        CIAO_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: GET NO DATA: ")
                               ACE_TEXT ("Caught excepted InternalError ")
                               ACE_TEXT ("exception\n")));
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: GET NO DATA:");
        CIAO_ERROR ((LM_ERROR,
          ACE_TEXT ("ERROR: Receiver_exec_i::get_no_data : Exception caught\n")));
      }
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
  }

  // Port operations.
  ::CCM_DDS::GetterTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return 0;
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_get_status (void)
  {
    return 0;
  }

  ::CCM_GetInvoker_ptr
  Receiver_exec_i::get_getter_invoke ()
  {
    return new Invoker_exec_i (*this);
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Receiver_exec_i::get_info_out_connector_status (void)
  {
    return new ConnectorStatusListener_exec_i (*this);
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Getter_Test::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Receiver_exec_i::configuration_complete (void)
  {
    this->getter_ = this->context_->get_connection_info_get_fresh_data ();
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
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Getter_Test_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

