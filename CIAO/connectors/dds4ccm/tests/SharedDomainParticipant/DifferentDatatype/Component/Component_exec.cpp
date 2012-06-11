// -*- C++ -*-
// $Id$

#include "Component_exec.h"
#include "ace/Log_Msg.h"

#include "Connector1/Connector1_conn.h"
#include "Connector2/Connector2_conn.h"
#include "Connector3/Connector3_conn.h"

namespace CIAO_SharedDP_SharedDPComponent_Impl
{
  //============================================================
  // Component Executor Implementation Class: Component_exec_i
  //============================================================

  Component_exec_i::Component_exec_i (void)
    : dp1_hnd_ (DDS::HANDLE_NIL)
    , dp2_hnd_ (DDS::HANDLE_NIL)
    , dp3_hnd_ (DDS::HANDLE_NIL)
    , dp1_ptr_ (DDS::DomainParticipant::_nil ())
    , dp2_ptr_ (DDS::DomainParticipant::_nil ())
    , dp3_ptr_ (DDS::DomainParticipant::_nil ())
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
                        this->dp1_ptr_ = dp.ptr ();
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
                        this->dp2_ptr_ = dp.ptr ();
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
                        this->dp3_ptr_ = dp.ptr ();
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
  }

  void
  Component_exec_i::ccm_passivate (void)
  {
    if (this->dp1_hnd_ == DDS::HANDLE_NIL)
      ACE_ERROR ((LM_ERROR, "ERROR: Handle of DomainParticipant for Connector 1 seems to be NIL\n"));
    if (this->dp2_hnd_ == DDS::HANDLE_NIL)
      ACE_ERROR ((LM_ERROR, "ERROR: Handle of DomainParticipant for Connector 2 seems to be NIL\n"));
    if (this->dp3_hnd_ == DDS::HANDLE_NIL)
      ACE_ERROR ((LM_ERROR, "ERROR: Handle of DomainParticipant for Connector 3 seems to be NIL\n"));

    if ((this->dp1_hnd_ != this->dp2_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
        && (this->dp1_ptr_ != this->dp2_ptr_)
#endif
        )
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 2 don't seem to "
                              "share the same DomainParticipant\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Connector 1 and 2 seems to "
                              "share the same DomainParticipant\n"));
      }
    if ((this->dp1_hnd_ == this->dp3_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
        && (this->dp1_ptr_ == this->dp3_ptr_)
#endif
       )
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 3 seem to "
                              "share the same DomainParticipant\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Connector 1 and 3 seems to "
                              "share the same DomainParticipant\n"));
      }

    if ((this->dp2_hnd_ == this->dp3_hnd_)
#if (CIAO_DDS4CCM_OPENDDS==1)
        && (this->dp2_ptr_ == this->dp3_ptr_)
#endif
       )
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Connector 2 and 3 seem to "
                              "share the same DomainParticipant\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Connector 2 and 3 seems to "
                              "share the same DomainParticipant\n"));
      }
  }

  void
  Component_exec_i::ccm_remove (void)
  {
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

