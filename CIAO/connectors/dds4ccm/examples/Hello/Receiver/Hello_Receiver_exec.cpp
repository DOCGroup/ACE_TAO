// -*- C++ -*-
//
// $Id$

#include "Hello_Receiver_exec.h"
#include "ciao/Logger/Log_Macros.h"
#include "tao/ORB_Core.h"

namespace CIAO_Hello_Receiver_Impl
{
  //============================================================
  // Facet Executor Implementation Class: DDSHello_RawListener_exec_i
  //============================================================
  
  DDSHello_RawListener_exec_i::DDSHello_RawListener_exec_i (void)
  {
  }
  
  DDSHello_RawListener_exec_i::~DDSHello_RawListener_exec_i (void)
  {
  }
  
  // Operations from ::CCM_DDS::DDSHello_RawListener
  
  void
  DDSHello_RawListener_exec_i::on_data (
    const DDSHello & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("DDSHello_RawListener: ")
            ACE_TEXT ("received hello for <%s> - iterator <%d>\n"),
            an_instance.hello.in (),
            an_instance.iterator));
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
  // Component Executor Implementation Class: Receiver_exec_iDDSHello_RawListener_exec_i ();
  //============================================================
  
  Receiver_exec_i::Receiver_exec_i (void)
  {
  }
  
  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }
  
  // Port operations.
  ::CCM_DDS::CCM_DDSHello_RawListener_ptr
  Receiver_exec_i::get_info_out_listener (void)
  {
    CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("new DDSHello RAW listener\n")));
    return new DDSHello_RawListener_exec_i ();
  }
  
  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
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
      ::Hello::CCM_Receiver_Context::_narrow (ctx);
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
    ::CCM_DDS::ListenerControl_var lc = 
    this->context_->get_connection_info_out_control ();

    if (CORBA::is_nil (lc.in ()))
      {
        CIAO_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->enabled (true);
  }
  
  void
  Receiver_exec_i::ccm_passivate (void)
  {
  }
  
  void
  Receiver_exec_i::ccm_remove (void)
  {
  }
  
  extern "C" HELLO_RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Hello_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

