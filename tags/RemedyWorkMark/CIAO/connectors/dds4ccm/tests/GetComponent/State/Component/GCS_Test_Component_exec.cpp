// -*- C++ -*-
//
// $Id$

#include "GCS_Test_Component_exec.h"

namespace CIAO_GCS_Test_GetComponentStateTestComponent_Impl
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

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_info_read_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_info_get_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::GCSTestConnector::CCM_Listener_ptr
  Component_exec_i::get_info_listen_data_listener (void)
  {
    return ::GCSTestConnector::CCM_Listener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_info_listen_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  ::GCSTestConnector::CCM_StateListener_ptr
  Component_exec_i::get_info_statelisten_data_listener (void)
  {
    return ::GCSTestConnector::CCM_StateListener::_nil ();
  }

  ::CCM_DDS::CCM_PortStatusListener_ptr
  Component_exec_i::get_info_statelisten_status (void)
  {
    return ::CCM_DDS::CCM_PortStatusListener::_nil ();
  }

  // Operations from Components::SessionComponent.
  void
  Component_exec_i::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::GCS_Test::CCM_GetComponentStateTestComponent_Context::_narrow (ctx);
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
  Component_exec_i::test_updater ()
  {
    //DDS_Update -> Writer
    try
      {
        ::GCSTestConnector::Updater_var updater_data =
          this->context_->get_connection_info_update_data ();
        if (! ::CORBA::is_nil (updater_data.in ()))
          {
            CORBA::Object_var cmp = updater_data->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_updater - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("updater_data\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_updater - ")
                                      ACE_TEXT ("Writer on DDS_Update port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_updater - "
                                  ACE_TEXT ("updater_data is NIL.\n"))));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_updater: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_reader - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("updater_data\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_updater - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("updater_data\n")));
      }
  }

  void
  Component_exec_i::test_reader ()
  {
    //DDS_Read -> Reader
    try
      {
        ::GCSTestConnector::Reader_var reader_data =
          this->context_->get_connection_info_read_data ();
        if (! ::CORBA::is_nil (reader_data.in ()))
          {
            CORBA::Object_var cmp = reader_data->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_reader - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("data_reader\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_reader - ")
                                      ACE_TEXT ("Reader on DDS_Read port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_reader - "
                                  ACE_TEXT ("reader_data is NIL.\n"))));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_reader: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_reader - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("data_reader\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_reader - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("data_reader\n")));
      }
  }

  void
  Component_exec_i::test_getter ()
  {
    //Reader on DDS_Get
    try
      {
        ::GCSTestConnector::Reader_var getter_data =
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
        ::GCSTestConnector::Getter_var getter_fresh_data =
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
        ::GCSTestConnector::Reader_var listen_data =
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
  Component_exec_i::test_statelistener ()
  {
    //DDS_StateListen
    try
      {
        ::GCSTestConnector::Reader_var statelisten_data =
          this->context_->get_connection_info_statelisten_data ();
        if (! ::CORBA::is_nil (statelisten_data.in ()))
          {
            CORBA::Object_var cmp = statelisten_data->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_statelistener - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("statelisten_data\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_statelistener - ")
                                      ACE_TEXT ("Reader on DDS_StateListen port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_statelistener - ")
                                  ACE_TEXT ("statelisten_data is NIL.\n")));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_statelistener: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("statelisten_data\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_statelistener - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("statelisten_data\n")));
      }

    try
      {
        ::CCM_DDS::StateListenerControl_var statelisten_data_control =
          this->context_->get_connection_info_statelisten_data_control ();
        if (! ::CORBA::is_nil (statelisten_data_control.in ()))
          {
            CORBA::Object_var cmp = statelisten_data_control->_get_component ();
            if (::CORBA::is_nil (cmp.in ()))
              {
                ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_statelistener - ")
                                      ACE_TEXT ("Unable to get component interface of ")
                                      ACE_TEXT ("statelisten_data_control\n")));
              }
            else
              {
                ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("OK: Component_exec_i::test_statelistener - ")
                                      ACE_TEXT ("DataControl on DDS_StateListen port returned a ")
                                      ACE_TEXT ("pointer on _get_component.\n")));
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_statelistener - ")
                                  ACE_TEXT ("statelisten_data_control is NIL.\n")));
          }
      }
    catch (const ::CORBA::Exception& ex)
      {
        ex._tao_print_exception ("ERROR: Component_exec_i::test_statelistener: ");
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_listener - ")
                              ACE_TEXT ("CORBA exception caught while testing ")
                              ACE_TEXT ("statelisten_data_control\n")));
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("ERROR: Component_exec_i::test_statelistener - ")
                              ACE_TEXT ("Unknown exception caught while testing ")
                              ACE_TEXT ("statelisten_data_control\n")));
      }
  }

  void
  Component_exec_i::ccm_activate (void)
  {
    if (this->act_as_sender_)
      {
        this->test_updater ();
      }
    else
      {
        this->test_reader ();
        this->test_getter ();
        this->test_listener ();
        this->test_statelistener ();
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
  create_GetComponentStateTest_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

