// -*- C++ -*-
//
// $Id$

#include "Writer_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "ace/OS_NS_time.h"
#include "Base/Writer_BaseSupport.h"
#include "Connector/Writer_Connector_conn.h"

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
    const WriterTestSeq & an_instance,
    const ::CCM_DDS::ReadInfoSeq & /* info */)
  {
    for (CORBA::ULong i = 0; i < an_instance.length (); ++i)
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

  ::WriterTestConnector::CCM_Listener_ptr
  Receiver_exec_i::get_info_out_data_listener (void)
  {
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("new WriterTest RAW listener\n")));
    return new WriterTest_Listener_exec_i ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Receiver_exec_i::get_info_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
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
    ::DDS::DataReader_var dds_dr =
      this->context_->get_connection_info_out_dds_entity ();

    if (::CORBA::is_nil (dds_dr.in ()))
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::configuration_complete - "
                    "Datareader connection is NIL.\n"));
        throw ::CORBA::INTERNAL ();
      }

    typedef ::CIAO::DDS4CCM::CCM_DDS_DataReader_Base DataReader_type;

    DataReader_type * typed_ccm_dr = dynamic_cast <DataReader_type*> (dds_dr.in ());
    if (typed_ccm_dr)
      {
        this->reader_ = WriterTestDataReader::narrow (typed_ccm_dr->get_impl ());
        if (!this->reader_)
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::configuration_complete - "
                        "Error casting the typed CCM DataReader to a typed "
                        "DDS DataReader.\n"));
            throw ::CORBA::INTERNAL ();
          }
      }
    else
      {
        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::configuration_complete - "
                    "Error casting DataReader to typed DataReader\n"));
        throw ::CORBA::INTERNAL ();
      }
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
