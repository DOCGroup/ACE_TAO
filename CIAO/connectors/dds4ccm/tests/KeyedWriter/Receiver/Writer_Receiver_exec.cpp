// -*- C++ -*-
//
// $Id$

#include "Writer_Receiver_exec.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_time.h"

namespace CIAO_Writer_Receiver_Impl
{
  //============================================================
  // WriterTest_Listener_exec_i
  //============================================================
  WriterTest_Listener_exec_i::WriterTest_Listener_exec_i ()
  {
  }

  WriterTest_Listener_exec_i::~WriterTest_Listener_exec_i (void)
  {
  }

  void
  WriterTest_Listener_exec_i::on_one_data (
    const WriterTest & an_instance ,
    const ::CCM_DDS::ReadInfo & /* info */)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("WriterTest_Listener: ")
            ACE_TEXT ("received writer info for <%C> at %u\n"),
            an_instance.key.in (),
            an_instance.iteration));
  }

  void
  WriterTest_Listener_exec_i::on_many_data (
    const WriterTest_Seq & an_instance,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
    for (unsigned int i = 0; i < an_instance.length (); ++i)
      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("WriterTest_Listener: ")
                ACE_TEXT ("received writer info for <%C> at %u\n"),
                an_instance[i].key.in (),
                an_instance[i].iteration));
      }
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

  ::CCM_DDS::WriterTest::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new WriterTest RAW listener\n")));
    return new WriterTest_Listener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return 0;
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

    if (CORBA::is_nil (lc.in ()))
      {
        ACE_ERROR ((LM_INFO, ACE_TEXT ("Error:  Listener control receptacle is null!\n")));
        throw CORBA::INTERNAL ();
      }
    lc->mode (::CCM_DDS::ONE_BY_ONE);
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
