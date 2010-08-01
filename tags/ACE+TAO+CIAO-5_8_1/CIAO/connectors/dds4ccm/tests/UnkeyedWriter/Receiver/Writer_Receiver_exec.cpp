// -*- C++ -*-
//
// $Id$

#include "Writer_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "ace/OS_NS_time.h"

namespace CIAO_Writer_Receiver_Impl
{
  //============================================================
  // Facet Executor Implementation Class: WriterTest_Listener_exec_i
  //============================================================

  WriterTest_Listener_exec_i::WriterTest_Listener_exec_i (CORBA::UShort iterations)
    : iterations_ (iterations)
  {
  }

  WriterTest_Listener_exec_i::~WriterTest_Listener_exec_i (void)
  {
  }

  // Operations from ::CCM_DDS::WriterTest_Listener

  void
  WriterTest_Listener_exec_i::on_one_data (
    const WriterTest & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ACE_DEBUG ((LM_ERROR, ACE_TEXT ("WriterTest_Listener::on_one_data ")
            ACE_TEXT ("received writer info for <%C> at %u\n"),
            an_instance.key.in (),
            an_instance.iteration));
    if (an_instance.iteration > this->iterations_)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: received iteration ")
                        ACE_TEXT ("greater than expected : ")
                        ACE_TEXT ("expected <%u> - received <%u>\n"),
                        an_instance.iteration, this->iterations_));
      }
    if (an_instance.iteration == 0)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: received iteration ")
                        ACE_TEXT ("is zero\n")));
      }
  }

  void
  WriterTest_Listener_exec_i::on_many_data (
    const WriterTestSeq & an_instance,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
    for (CORBA::ULong i = 0; i < an_instance.length (); ++i)
      {
        ACE_DEBUG ((LM_ERROR, ACE_TEXT ("WriterTest_Listener::on_many_data ")
                ACE_TEXT ("received writer info for <%C> at %u\n"),
                an_instance[i].key.in (),
                an_instance[i].iteration));
      }
  }

  //============================================================
  // Component Executor Implementation Class: Receiver_exec_iKeyedTest_Listener_exec_i ();
  //============================================================
  Receiver_exec_i::Receiver_exec_i (void)
    : iterations_ (15)
  {
  }

  Receiver_exec_i::~Receiver_exec_i (void)
  {
  }

  // Port operations.
  ::Writer::WriterTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new WriterTest RAW listener\n")));
    return new WriterTest_Listener_exec_i (this->iterations_);
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
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

  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::Writer::CCM_Receiver_Context::_narrow (ctx);
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
    ::CCM_DDS::DataListenerControl_var lc =
    this->context_->get_connection_info_out_data_control ();

    if (::CORBA::is_nil (lc.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw ::CORBA::INTERNAL ();
      }
    lc->mode (::CCM_DDS::NOT_ENABLED);
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
  create_Writer_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}

