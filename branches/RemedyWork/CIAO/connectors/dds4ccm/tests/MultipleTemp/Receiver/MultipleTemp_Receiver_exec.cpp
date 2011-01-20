// -*- C++ -*-
// $Id$

// Test for MULTIPLE_TEMPLATES

#include "MultipleTemp_Receiver_exec.h"
#include "ace/Log_Msg.h"

#include "ace/OS_NS_time.h"
#include "dds4ccm/impl/Utils.h"
#include "dds4ccm/impl/TimeUtilities.h"

#include "dds4ccm/impl/dds4ccm_conf.h"

namespace CIAO_MultipleTemp_Receiver_Impl
{
  CORBA::UShort received_one= 0;
  CORBA::UShort received_two= 0;

  //============================================================
  // ListenOne_Listener_exec_i Listener to topic One
  //============================================================
  ListenOne_Listener_exec_i::ListenOne_Listener_exec_i (
      ::MultipleTemp::CCM_Receiver_Context_ptr ctx)
  : ciao_context_ (
      ::MultipleTemp::CCM_Receiver_Context::_duplicate (ctx))

  {
  }

  ListenOne_Listener_exec_i::~ListenOne_Listener_exec_i (void)
  {
  }

  void
  ListenOne_Listener_exec_i::on_one_data (
                                  const TestTopicOne & an_instance,
                                  const ::CCM_DDS::ReadInfo & info)
  {
    ACE_DEBUG ((LM_DEBUG, "ListenOne_Listener_exec_i::on_one_data: "
                            "key <%C> - x <%d>\n",
                            an_instance.keyOne.in (),
                            an_instance.x));
    if (!info.instance_handle.isValid)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ListenOne_Listener_exec_i::on_one_data: "
                            "instance handle seems to be invalid "
                            "key <%C> - x <%d>\n",
                            an_instance.keyOne.in (),
                            an_instance.x));
      }
    if (info.source_timestamp.sec == 0 &&
        info.source_timestamp.nanosec == 0)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: ListenOne_Listener_exec_i::on_one_data: "
                            "source timestamp seems to be invalid (nil) "
                            "key <%C> - x <%d>\n",
                            an_instance.keyOne.in (),
                            an_instance.x));
      }
    ++received_one;
  }

  void
  ListenOne_Listener_exec_i::on_many_data (
                                  const TestTopicOneSeq & ,
                                  const ::CCM_DDS::ReadInfoSeq & )
  {
  }
  //============================================================
  // ListenTwo_Listener_exec_i Listener to topic Two
  //============================================================
  ListenTwo_Listener_exec_i::ListenTwo_Listener_exec_i (
      ::MultipleTemp::CCM_Receiver_Context_ptr ctx)
  : ciao_context_ (
      ::MultipleTemp::CCM_Receiver_Context::_duplicate (ctx))
  {
  }

  ListenTwo_Listener_exec_i::~ListenTwo_Listener_exec_i (void)
  {
  }

  void
  ListenTwo_Listener_exec_i::on_one_data (
                                  const TestTopicTwo & an_instance,
                                  const ::CCM_DDS::ReadInfo & info)
    {
      ACE_DEBUG ((LM_DEBUG, "ListenTwo_Listener_exec_i::on_one_data: "
                              "key <%C> - y <%d>\n",
                              an_instance.keyTwo.in (),
                              an_instance.y));
      if (!info.instance_handle.isValid)
        {
          ACE_ERROR ((LM_ERROR, "ERROR: ListenTwo_Listener_exec_i::on_one_data: "
                              "instance handle seems to be invalid "
                              "key <%C> - y <%d>\n",
                              an_instance.keyTwo.in (),
                              an_instance.y));
        }
      if (info.source_timestamp.sec == 0 &&
          info.source_timestamp.nanosec == 0)
        {
          ACE_ERROR ((LM_ERROR, "ERROR: ListenTwo_Listener_exec_i::on_one_data: "
                              "source timestamp seems to be invalid (nil) "
                              "key <%C> - iteration <%d>\n",
                              an_instance.keyTwo.in (),
                              an_instance.y));
        }
    ++received_two;
  }

  void
  ListenTwo_Listener_exec_i::on_many_data (
                                  const TestTopicTwoSeq & ,
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
  Receiver_exec_i::get_info_one_out_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
   Receiver_exec_i::get_info_two_out_status (void)
   {
     return ::CCM_DDS::CCM_PortStatusListener::_nil ();
   }

  ::MultipleTemp::MultipleTempConnector::T_Typed::CCM_Listener_ptr
  Receiver_exec_i::get_info_one_out_data_listener (void)
  {
    if ( ::CORBA::is_nil (this->ciao_info_out_one_data_listener_.in ()))
       {
         ListenOne_Listener_exec_i *tmp = 0;
         ACE_NEW_RETURN (
           tmp,
           ListenOne_Listener_exec_i (
             this->ciao_context_.in ()),
             ::MultipleTemp::MultipleTempConnector::T_Typed::CCM_Listener::_nil ());
           this->ciao_info_out_one_data_listener_ = tmp;
       }

     return
         ::MultipleTemp::MultipleTempConnector::T_Typed::CCM_Listener::_duplicate (
         this->ciao_info_out_one_data_listener_.in ());
   }

  ::MultipleTemp::MultipleTempConnector::Y_Typed::CCM_Listener_ptr
   Receiver_exec_i::get_info_two_out_data_listener (void)
   {
    if ( ::CORBA::is_nil (this->ciao_info_out_two_data_listener_.in ()))
        {
          ListenTwo_Listener_exec_i *tmp = 0;
          ACE_NEW_RETURN (
            tmp,
            ListenTwo_Listener_exec_i (
              this->ciao_context_.in ()),
              ::MultipleTemp::MultipleTempConnector::Y_Typed::CCM_Listener::_nil ());
            this->ciao_info_out_two_data_listener_ = tmp;
        }

      return
          ::MultipleTemp::MultipleTempConnector::Y_Typed::CCM_Listener::_duplicate (
          this->ciao_info_out_two_data_listener_.in ());
   }

  // Operations from Components::SessionComponent.
  void
  Receiver_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->ciao_context_ =
      ::MultipleTemp::CCM_Receiver_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->ciao_context_.in ()))
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
    ::CCM_DDS::DataListenerControl_var dlc_one =
        this->ciao_context_->get_connection_info_one_out_data_control ();
    ::CCM_DDS::DataListenerControl_var dlc_two =
        this->ciao_context_->get_connection_info_two_out_data_control ();
    dlc_one->mode (::CCM_DDS::ONE_BY_ONE);
    dlc_two->mode (::CCM_DDS::ONE_BY_ONE);
  }

  void
  Receiver_exec_i::ccm_passivate (void)
  {

  }

  void
  Receiver_exec_i::ccm_remove (void)
  {
    if (received_one != 8)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver: "
                               "Received wrong number of topic one: "
                               "received <%C> - expected <8>\n",
                               received_one));
       }
    if (received_two != 8)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Receiver: "
                               "Received wrong number of topic two: "
                               "received <%C> - expected <8>\n",
                               received_two));
       }
    if ((received_two == 8) && (received_one == 8))
      {
        ACE_DEBUG ((LM_DEBUG, "OK: Receiver: "
                               "Received received expected topics.\n"));
      }
  }

  extern "C" RECEIVER_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_MultipleTemp_Receiver_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Receiver_exec_i);

    return retval;
  }
}
