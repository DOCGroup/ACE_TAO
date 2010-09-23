// -*- C++ -*-
// $Id$

// Test for combination of AMI4CCM and DDS4CCM
#include "AmiDds_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/TimeUtilities.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_AmiDds_Receiver_Impl
{
  //============================================================
  // MyFoo_exec_i     AMI4CCM
  //============================================================
  MyFoo_exec_i::MyFoo_exec_i (void)
  {
  }

  MyFoo_exec_i::~MyFoo_exec_i (void)
  {
  }

  void
  MyFoo_exec_i::hello (::CORBA::Long_out answer)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver (AMI4CCM)) :\thello! \n"));
    ACE_OS::sleep (ACE_OS::rand () % 2);
    answer = ACE_OS::rand () % 100;
  }

  //============================================================
  // ListenOneByOneTest_Listener_exec_i   DDS4CCM
  //============================================================
  ListenOneByOneTest_Listener_exec_i::ListenOneByOneTest_Listener_exec_i (void)
  {
  }

  ListenOneByOneTest_Listener_exec_i::~ListenOneByOneTest_Listener_exec_i (void)
  {
  }

  void
  ListenOneByOneTest_Listener_exec_i::on_one_data (
                                  const TestTopic & an_instance,
                                  const ::CCM_DDS::ReadInfo & info)
  {
    ACE_DEBUG ((LM_DEBUG, "Receiver (DDS4CCM)) :\tget data with on_one_data: "
                          "key <%C> - iteration <%d>\n",
                          an_instance.key.in (),
                          an_instance.x));
    if (!info.instance_handle.isValid)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ListenOneByOneTest_Listener_exec_i::on_one_data: "
                            "instance handle seems to be invalid "
                            "key <%C> - iteration <%d>\n",
                            an_instance.key.in (),
                            an_instance.x));
      }
  }

  void
  ListenOneByOneTest_Listener_exec_i::on_many_data (
                                  const TestTopicSeq & ,
                                  const ::CCM_DDS::ReadInfoSeq & )
  {
  }

  //============================================================
  // Receiver_exec_i
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::AmiDds::AmiDdsConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    return new ListenOneByOneTest_Listener_exec_i ();
  }

  AmiDds::CCM_MyFoo_ptr
  Receiver_exec_i::get_do_my_foo (void)
  {
    return new MyFoo_exec_i ();
  }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::AmiDds::CCM_Receiver_Context::_narrow (ctx);
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
    ::CCM_DDS::DataListenerControl_var dlc =
        this->context_->get_connection_info_out_data_control ();
    dlc->mode (::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {

  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
  }

  extern "C" ::Components::EnterpriseComponent_ptr
  create_AmiDds_AMI_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();


    ACE_NEW_RETURN (
      retval,
      Receiver_exec_i,
      ::Components::EnterpriseComponent::_nil ());


    return retval;
  }
}
