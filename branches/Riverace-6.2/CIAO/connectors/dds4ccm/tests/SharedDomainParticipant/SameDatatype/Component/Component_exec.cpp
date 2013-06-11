// -*- C++ -*-
// $Id$

// Test whether Connector 1,2 and 3 are sharing the same DomainParticipant.
// Connector 4 should have a different DomainParticipant.
// Also test whether Connector 1 and 2 share the same topic.

#include "Component_exec.h"
#include "ace/Log_Msg.h"

#include "Connector/Connector_conn.h"

const char * tp_name_conn_1_ = "SharedDP";
const char * tp_name_conn_2_ = "SharedDP";
const char * tp_name_conn_3_ = "SharedDP1";
const char * tp_name_conn_4_ = "StandaloneDP";

namespace CIAO_SharedDP_SharedDPComponent_Impl
{
  //============================================================
  // Component Executor Implementation Class: Component_exec_i
  //============================================================

  Component_exec_i::Component_exec_i (void)
    : perform_test_ (false)
    , dp1_hnd_ (DDS::HANDLE_NIL)
    , dp2_hnd_ (DDS::HANDLE_NIL)
    , dp3_hnd_ (DDS::HANDLE_NIL)
    , dp4_hnd_ (DDS::HANDLE_NIL)
    , dp1_ptr_ (DDS::DomainParticipant::_nil ())
    , dp2_ptr_ (DDS::DomainParticipant::_nil ())
    , dp3_ptr_ (DDS::DomainParticipant::_nil ())
    , dp4_ptr_ (DDS::DomainParticipant::_nil ())
    , tp1_hnd_ (DDS::HANDLE_NIL)
    , tp2_hnd_ (DDS::HANDLE_NIL)
    , tp3_hnd_ (DDS::HANDLE_NIL)
    , tp4_hnd_ (DDS::HANDLE_NIL)
    , tp1_ptr_ (DDS::Topic::_nil ())
    , tp2_ptr_ (DDS::Topic::_nil ())
    , tp3_ptr_ (DDS::Topic::_nil ())
    , tp4_ptr_ (DDS::Topic::_nil ())
  {
  }

  Component_exec_i::~Component_exec_i (void)
  {
  }

  void
  Component_exec_i::set_session_context (::Components::SessionContext_ptr ctx)
  {
    this->context_ =
      ::SharedDP::CCM_SharedDPComponent_Context::_narrow (ctx);

    if ( ::CORBA::is_nil (this->context_.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }
  }

  void
  Component_exec_i::configuration_complete (void)
  {
  }

  CORBA::Boolean
  Component_exec_i::perform_test (void)
  {
    return this->perform_test_;
  }

  void
  Component_exec_i::perform_test (CORBA::Boolean perform_test)
  {
    this->perform_test_ = perform_test;
  }

  void
  Component_exec_i::ccm_activate (void)
  {
    try
      {
        ::DDS::DataWriter_var dw1 =
          this->context_->get_connection_write1_dds_entity ();

        if (::CORBA::is_nil (dw1.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Datawriter 1 connection is NIL.\n"));
            throw ::CORBA::INTERNAL ();
          }
        ::CIAO::DDS4CCM::CCM_DataWriter * ccm_dw1 =
          dynamic_cast < ::CIAO::DDS4CCM::CCM_DataWriter * > (dw1.in ());

        if (ccm_dw1)
          {
            ::DDS::DataWriter_var tmp = ccm_dw1->get_dds_entity ();
            if (! ::CORBA::is_nil (tmp.in ()))
              {
                ::DDS::Publisher_var publisher = tmp->get_publisher ();

                if (! ::CORBA::is_nil (publisher.in ()))
                  {
                    ::DDS::DomainParticipant_var dp = publisher->get_participant ();

                    if (! ::CORBA::is_nil (publisher.in ()))
                      {

                        this->dp1_hnd_ = dp->get_instance_handle ();
                        this->dp1_ptr_ = dp.ptr();

                        ::DDS::TopicDescription_var tpd = dp->lookup_topicdescription (tp_name_conn_1_);

                        if (! ::CORBA::is_nil (tpd.in ()))
                          {
                            ::DDS::Topic_var topic = ::DDS::Topic::_narrow (tpd.in ());
                            if (! ::CORBA::is_nil (topic.in ()))
                              {
                                this->tp1_hnd_ = topic->get_instance_handle ();
                                this->tp1_ptr_ = topic.ptr ();
                              }
                            else
                              {
                                ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                            "Error narrowing TopicDescription 1 to Topic 1.\n"));
                                throw ::CORBA::INTERNAL ();
                              }
                          }
                        else
                          {
                            ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                        "Error getting TopicDescription 1.\n"));
                            throw ::CORBA::INTERNAL ();
                          }
                      }
                    else
                      {
                        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                    "Error getting DomainParticipant 1.\n"));
                        throw ::CORBA::INTERNAL ();
                      }
                  }
                else
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                "Error getting Publisher 1.\n"));
                    throw ::CORBA::INTERNAL ();
                  }
              }
            else
              {
                ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                            "Error getting DDS DataWriter 1.\n"));
                throw ::CORBA::INTERNAL ();
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Error casting DataWriter 1 to CCM DataWriter 1\n"));
            throw ::CORBA::INTERNAL ();
          }
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Unable to create dds_entity for writer1\n"));
      }

    try
      {
        ::DDS::DataWriter_var dw2 =
          this->context_->get_connection_write2_dds_entity ();

        if (::CORBA::is_nil (dw2.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Datawriter 2 connection is NIL.\n"));
            throw ::CORBA::INTERNAL ();
          }
        ::CIAO::DDS4CCM::CCM_DataWriter * ccm_dw2 =
          dynamic_cast < ::CIAO::DDS4CCM::CCM_DataWriter * > (dw2.in ());

        if (ccm_dw2)
          {
            ::DDS::DataWriter_var tmp = ccm_dw2->get_dds_entity ();
            if (! ::CORBA::is_nil (tmp.in ()))
              {
                ::DDS::Publisher_var publisher = tmp->get_publisher ();

                if (! ::CORBA::is_nil (publisher.in ()))
                  {
                    ::DDS::DomainParticipant_var dp = publisher->get_participant ();

                    if (! ::CORBA::is_nil (publisher.in ()))
                      {

                        this->dp2_hnd_ = dp->get_instance_handle ();
                        this->dp2_ptr_ = dp.ptr();

                        ::DDS::TopicDescription_var tpd = dp->lookup_topicdescription (tp_name_conn_2_);

                        if (! ::CORBA::is_nil (tpd.in ()))
                          {
                            ::DDS::Topic_var topic = ::DDS::Topic::_narrow (tpd.in ());
                            if (! ::CORBA::is_nil (topic.in ()))
                              {
                                this->tp2_hnd_ = topic->get_instance_handle ();
                                this->tp2_ptr_ = topic.ptr ();
                              }
                            else
                              {
                                ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                            "Error narrowing TopicDescription 2 to Topic 2.\n"));
                                throw ::CORBA::INTERNAL ();
                              }
                          }
                        else
                          {
                            ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                        "Error getting TopicDescription 2.\n"));
                            throw ::CORBA::INTERNAL ();
                          }
                      }
                    else
                      {
                        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                    "Error getting DomainParticipant 2.\n"));
                        throw ::CORBA::INTERNAL ();
                      }
                  }
                else
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                "Error getting Publisher 2.\n"));
                    throw ::CORBA::INTERNAL ();
                  }
              }
            else
              {
                ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                            "Error getting DDS DataWriter 2.\n"));
                throw ::CORBA::INTERNAL ();
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Error casting DataWriter 2 to CCM DataWriter 2\n"));
            throw ::CORBA::INTERNAL ();
          }
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Unable to create dds_entity for writer2\n"));
      }

    try
      {
        ::DDS::DataWriter_var dw3 =
          this->context_->get_connection_write3_dds_entity ();

        if (::CORBA::is_nil (dw3.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Datawriter 3 connection is NIL.\n"));
            throw ::CORBA::INTERNAL ();
          }

        ::CIAO::DDS4CCM::CCM_DataWriter * ccm_dw3 =
          dynamic_cast < ::CIAO::DDS4CCM::CCM_DataWriter * > (dw3.in ());

        if (ccm_dw3)
          {
            ::DDS::DataWriter_var tmp = ccm_dw3->get_dds_entity ();
            if (! ::CORBA::is_nil (tmp.in ()))
              {
                ::DDS::Publisher_var publisher = tmp->get_publisher ();

                if (! ::CORBA::is_nil (publisher.in ()))
                  {
                    ::DDS::DomainParticipant_var dp = publisher->get_participant ();

                    if (! ::CORBA::is_nil (publisher.in ()))
                      {

                        this->dp3_hnd_ = dp->get_instance_handle ();
                        this->dp3_ptr_ = dp.ptr();

                        ::DDS::TopicDescription_var tpd = dp->lookup_topicdescription (tp_name_conn_3_);

                        if (! ::CORBA::is_nil (tpd.in ()))
                          {
                            ::DDS::Topic_var topic = ::DDS::Topic::_narrow (tpd.in ());
                            if (! ::CORBA::is_nil (topic.in ()))
                              {
                                this->tp3_hnd_ = topic->get_instance_handle ();
                                this->tp3_ptr_ = topic.ptr ();
                              }
                            else
                              {
                                ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                            "Error narrowing TopicDescription 3 to Topic 3.\n"));
                                throw ::CORBA::INTERNAL ();
                              }
                          }
                        else
                          {
                            ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                        "Error getting TopicDescription 3.\n"));
                            throw ::CORBA::INTERNAL ();
                          }
                      }
                    else
                      {
                        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                    "Error getting DomainParticipant 3.\n"));
                        throw ::CORBA::INTERNAL ();
                      }
                  }
                else
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                "Error getting Publisher 3.\n"));
                    throw ::CORBA::INTERNAL ();
                  }
              }
            else
              {
                ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                            "Error getting DDS DataWriter 3.\n"));
                throw ::CORBA::INTERNAL ();
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Error casting DataWriter 3 to CCM DataWriter 3\n"));
            throw ::CORBA::INTERNAL ();
          }
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Unable to create dds_entity for writer3\n"));
      }
    try
      {
        ::DDS::DataWriter_var dw4 =
          this->context_->get_connection_write4_dds_entity ();

        if (::CORBA::is_nil (dw4.in ()))
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Datawriter 4 connection is NIL.\n"));
            throw ::CORBA::INTERNAL ();
          }

        ::CIAO::DDS4CCM::CCM_DataWriter * ccm_dw4 =
          dynamic_cast < ::CIAO::DDS4CCM::CCM_DataWriter * > (dw4.in ());

        if (ccm_dw4)
          {
            ::DDS::DataWriter_var tmp = ccm_dw4->get_dds_entity ();
            if (! ::CORBA::is_nil (tmp.in ()))
              {
                ::DDS::Publisher_var publisher = tmp->get_publisher ();

                if (! ::CORBA::is_nil (publisher.in ()))
                  {
                    ::DDS::DomainParticipant_var dp = publisher->get_participant ();

                    if (! ::CORBA::is_nil (publisher.in ()))
                      {

                        this->dp4_hnd_ = dp->get_instance_handle ();
                        this->dp4_ptr_ = dp.ptr();

                        if (this->perform_test_)
                          {

                            ::DDS::TopicDescription_var tpd = dp->lookup_topicdescription (tp_name_conn_4_);

                            if (! ::CORBA::is_nil (tpd.in ()))
                              {
                                ::DDS::Topic_var topic = ::DDS::Topic::_narrow (tpd.in ());
                                if (! ::CORBA::is_nil (topic.in ()))
                                  {
                                    this->tp4_hnd_ = topic->get_instance_handle ();
                                    this->tp4_ptr_ = topic.ptr ();
                                  }
                                else
                                  {
                                    ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                                "Error narrowing TopicDescription 4 to Topic 4.\n"));
                                    throw ::CORBA::INTERNAL ();
                                  }
                              }
                            else
                              {
                                ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                            "Error getting TopicDescription 4.\n"));
                                throw ::CORBA::INTERNAL ();
                              }
                          }
                      }
                    else
                      {
                        ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                    "Error getting DomainParticipant 4.\n"));
                        throw ::CORBA::INTERNAL ();
                      }
                  }
                else
                  {
                    ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                                "Error getting Publisher 4.\n"));
                    throw ::CORBA::INTERNAL ();
                  }
              }
            else
              {
                ACE_ERROR ((LM_ERROR, "ERROR : Receiver_exec_i::ccm_activate - "
                            "Error getting DDS DataWriter 4.\n"));
                throw ::CORBA::INTERNAL ();
              }
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Error casting DataWriter 4 to CCM DataWriter 4\n"));
            throw ::CORBA::INTERNAL ();
          }
      }
    catch (...)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Unable to create dds_entity for writer4\n"));
      }
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
    if (this->perform_test_)
      {
        if (this->dp1_hnd_ == DDS::HANDLE_NIL)
          ACE_ERROR ((LM_ERROR, "ERROR: Handle for DomainParticipant for Connector 1 seems to be NIL\n"));
        if (this->dp2_hnd_ == DDS::HANDLE_NIL)
          ACE_ERROR ((LM_ERROR, "ERROR: Handle for DomainParticipant for Connector 2 seems to be NIL\n"));
        if (this->dp3_hnd_ == DDS::HANDLE_NIL)
          ACE_ERROR ((LM_ERROR, "ERROR: Handle for DomainParticipant for Connector 3 seems to be NIL\n"));
        if (this->dp4_hnd_ == DDS::HANDLE_NIL)
          ACE_ERROR ((LM_ERROR, "ERROR: Handle for DomainParticipant for Connector 4 seems to be NIL\n"));

        if (this->tp1_hnd_ == DDS::HANDLE_NIL)
          ACE_ERROR ((LM_ERROR, "ERROR: Handle for Topic for Connector 1 seems to be NIL\n"));
        if (this->tp2_hnd_ == DDS::HANDLE_NIL)
          ACE_ERROR ((LM_ERROR, "ERROR: Handle for Topic for Connector 2 seems to be NIL\n"));
        if (this->tp3_hnd_ == DDS::HANDLE_NIL)
          ACE_ERROR ((LM_ERROR, "ERROR: Handle for Topic for Connector 3 seems to be NIL\n"));
        if (this->tp4_hnd_ == DDS::HANDLE_NIL)
          ACE_ERROR ((LM_ERROR, "ERROR: Handle for Topic for Connector 4 seems to be NIL\n"));

        //check shared DomainParticipants
        if ((this->dp1_hnd_ != this->dp2_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
            && (this->dp1_ptr_ == this->dp2_ptr_)
#endif
            )
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 2 don't seem to "
                                  "share the same DomainParticipant\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "OK: Connector 1 and 2 seems to "
                                  "share the same DomainParticipant\n"));
          }
        if ((this->dp1_hnd_ == this->dp4_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
            && (this->dp1_ptr_ == this->dp4_ptr_)
#endif
            )
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 4 seem to "
                                  "share the same DomainParticipant\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "OK: Connector 1 and 4 seems to "
                                  "share the same DomainParticipant\n"));
          }
        if ((this->dp2_hnd_ == this->dp4_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
            && (this->dp2_ptr_ == this->dp4_ptr_)
#endif
            )
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Connector 2 and 4 seem to "
                                  "share the same DomainParticipant\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "OK: Connector 2 and 4 seems to "
                                  "share the same DomainParticipant\n"));
          }
        if ((this->dp3_hnd_ == this->dp4_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
            && (this->dp3_ptr_ == this->dp4_ptr_)
#endif
            )
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Connector 3 and 4 seem to "
                                  "share the same DomainParticipant\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "OK: Connector 3 and 4 seems to "
                                  "share the same DomainParticipant\n"));
          }

        //check shared Topics
        if ((this->tp1_hnd_ != this->tp2_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
            && (this->tp1_ptr_ == this->tp2_ptr_)
#endif
            )
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 2 don't seem to "
                                  "share the same Topic\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "OK: Connector 1 and 2 seems to "
                                  "share the same Topic\n"));
          }
        if ((this->tp1_hnd_ == this->tp3_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
            && (this->tp1_ptr_ == this->tp3_ptr_)
#endif
            )
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 3 seem to "
                                  "share the same Topic\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "OK: Connector 1 and 3 seems to "
                                  "share the same Topic\n"));
          }
        if ((this->tp1_hnd_ == this->tp4_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
            && (this->tp1_ptr_ == this->tp4_ptr_)
#endif
            )
          {
            ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 4 seem to "
                                  "share the same Topic\n"));
          }
        else
          {
            ACE_DEBUG ((LM_DEBUG, "OK: Connector 1 and 4 seems to "
                                  "share the same Topic\n"));
          }
      }
  }

  void
  Component_exec_i::ccm_remove (void)
  {
    if (!this->perform_test_)
      {
        ACE_DEBUG ((LM_DEBUG, "Don't test this component. It's used for compilation errors.\n"));
      }
  }

  extern "C" COMPONENT_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Component_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Component_exec_i);

    return retval;
  }
}

