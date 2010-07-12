// -*- C++ -*-
//
// $Id$

#include "GCE_Test_Component_exec.h"

namespace CIAO_GCE_Test_GetComponentEventTestComponent_Impl
{
  //============================================================
  // Component_exec_i
  //============================================================
  Component_exec_i::Component_exec_i (void)
    : act_as_sender_ (false)
  {
  }

  Component_exec_i::~Component_exec_i (void)
  {
  }

  ::CORBA::Boolean
  Component_exec_i::act_as_sender (void)
  {
    return this->act_as_sender_;
  }

  void
  Component_exec_i::act_as_sender (::CORBA::Boolean act_as_sender)
  {
    this->act_as_sender_ = act_as_sender;
  }

  ::GCETestConnector::CCM_Listener_ptr
  Component_exec_i::get_info_listen_data_listener (void)
  {
    return ::GCETestConnector::CCM_Listener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_info_get_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_ConnectorStatusListener_ptr
  Component_exec_i::get_status_listener (void)
  {
    return ::CCM_DDS::CCM_ConnectorStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::GCE_Test::CCM_GetComponentEventTestComponent_Context::_narrow (ctx);
    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }


  void
  Component_exec_i::test_writer ()
  {
    //DDS_Update -> Writer
    try
      {
        ::GCETestConnector::Writer_var writer_data =
          this->context_->get_connection_info_write_data ();
        if (! ::CORBA::is_nil (writer_data.in ()))
          {
            CORBA::Object_var cmp = writer_data->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_writer - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("writer_data\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_writer - ")
                                      ACE_TEXT ("Writer on DDS_Write port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_writer - "
                                  ACE_TEXT ("writer_data is NIL.\n"))));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_writer: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_writer - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("writer_data\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_writer - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("writer_data\n")));
      }
  }

  void
  Component_exec_i::test_getter ()
  {
    //Reader on DDS_Get
    try
      {
        ::GCETestConnector::Reader_var getter_data =
          this->context_->get_connection_info_get_data ();
        if (! ::CORBA::is_nil (getter_data.in ()))
          {
            CORBA::Object_var cmp = getter_data->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_getter - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("getter_data\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_getter - ")
                                      ACE_TEXT ("Reader on DDS_Get port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_getter - ")
                                  ACE_TEXT ("getter_data is NIL.\n")));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_getter: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_getter - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("getter_data\n")));
      }
    catch (...)
      {        
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_getter - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("getter_data\n")));
      }

    //Getter on DDS_Get
    try
      {
        ::GCETestConnector::Getter_var getter_fresh_data =
          this->context_->get_connection_info_get_fresh_data ();
        if (! ::CORBA::is_nil (getter_fresh_data.in ()))
          {
            CORBA::Object_var cmp = getter_fresh_data->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_getter - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("getter_fresh_data\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_getter - ")
                                      ACE_TEXT ("Getter on DDS_Get port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_getter - ")
                                  ACE_TEXT ("getter_fresh_data is NIL.\n")));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_getter: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_getter - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("getter_fresh_data\n")));
      }    
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_getter - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("getter_fresh_data\n")));
      }
  }

  void
  Component_exec_i::test_listener ()
  {
    try
      {
         //DDS_Listen
        ::GCETestConnector::Reader_var listen_data =
          this->context_->get_connection_info_listen_data ();
        if (! ::CORBA::is_nil (listen_data.in ()))
          {
            CORBA::Object_var cmp = listen_data->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("listen_data\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_listener - ")
                                      ACE_TEXT ("Reader on DDS_Listen port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                                  ACE_TEXT ("listen_data is NIL.\n")));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_listener: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("listen_data\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("listen_data\n")));
      }

    try
      {
        ::CCM_DDS::DataListenerControl_var  listen_data_control =
          this->context_->get_connection_info_listen_data_control ();
        if (! ::CORBA::is_nil (listen_data_control.in ()))
          {
            CORBA::Object_var cmp = listen_data_control->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("listen_data_control\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_listener - ")
                                      ACE_TEXT ("DataControl on DDS_Listen port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                                  ACE_TEXT ("listen_data_control is NIL.\n")));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_listener: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("listen_data_control\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("listen_data_control\n")));
      }
  }

  void
  Component_exec_i::ccm_activate (void)
  {
    if (this->act_as_sender_)
      {
        this->test_writer ();
      }
    else
      {
        this->test_getter ();
        this->test_listener ();
      }

  }

  void
  Component_exec_i::ccm_passivate (void)
  {
  }

  void
  Component_exec_i::ccm_remove (void)
  {
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_GetComponentEventTest_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

