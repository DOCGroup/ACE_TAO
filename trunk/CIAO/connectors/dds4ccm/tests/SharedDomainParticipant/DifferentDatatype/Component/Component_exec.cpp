// -*- C++ -*-
// $Id$

#include "Component_exec.h"
#include "ace/Log_Msg.h"

#include "Base/BaseSupport.h"
#include "Connector1/Connector1_conn.h"
#include "Connector2/Connector2_conn.h"
#include "Connector3/Connector3_conn.h"

#include "dds4ccm/impl/DataWriter_T.h"

namespace CIAO_SharedDP_SharedDPComponent_Impl
{
  //============================================================
  // Component Executor Implementation Class: Component_exec_i
  //============================================================

  Component_exec_i::Component_exec_i (void)
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
            throw CORBA::INTERNAL ();
          }
        typedef ::CIAO::DDS4CCM::CCM_DDS_DataWriter_T<
            CIAO_SharedDP_Connector1_SharedDPTest1Connector_DDS_Event_Impl::SharedDPTest1_DDS_Traits,
            CIAO_SharedDP_Connector1_SharedDPTest1Connector_DDS_Event_Impl::DDS_DDS_Event_Traits> DataWriter_type;

        DataWriter_type * typed_ccm_dw = reinterpret_cast <DataWriter_type *> (dw1.in ());
        if (typed_ccm_dw)
          {
            DDSDataWriter * dds_dw1 = typed_ccm_dw->get_impl ();
            DDSPublisher * dds_p1 = dds_dw1->get_publisher ();
            this->dds_dp1_ = dds_p1->get_participant ();
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Error casting DataWriter 1 to typed DataWriter 1\n"));
            throw CORBA::INTERNAL ();
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
            throw CORBA::INTERNAL ();
          }
        typedef ::CIAO::DDS4CCM::CCM_DDS_DataWriter_T<
            CIAO_SharedDP_Connector2_SharedDPTest2Connector_DDS_Event_Impl::SharedDPTest2_DDS_Traits,
            CIAO_SharedDP_Connector2_SharedDPTest2Connector_DDS_Event_Impl::DDS_DDS_Event_Traits> DataWriter_type;

        DataWriter_type * typed_ccm_dw = reinterpret_cast <DataWriter_type *> (dw2.in ());
        if (typed_ccm_dw)
          {
            DDSDataWriter * dds_dw2 = typed_ccm_dw->get_impl ();
            DDSPublisher * dds_p2 = dds_dw2->get_publisher ();
            this->dds_dp2_ = dds_p2->get_participant ();
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Error casting DataWriter 2 to typed DataWriter 2\n"));
            throw CORBA::INTERNAL ();
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
            throw CORBA::INTERNAL ();
          }
        typedef ::CIAO::DDS4CCM::CCM_DDS_DataWriter_T<
            CIAO_SharedDP_Connector3_SharedDPTest3Connector_DDS_Event_Impl::SharedDPTest3_DDS_Traits,
            CIAO_SharedDP_Connector3_SharedDPTest3Connector_DDS_Event_Impl::DDS_DDS_Event_Traits> DataWriter_type;

        DataWriter_type * typed_ccm_dw = reinterpret_cast <DataWriter_type *> (dw3.in ());
        if (typed_ccm_dw)
          {
            DDSDataWriter * dds_dw3 = typed_ccm_dw->get_impl ();
            DDSPublisher * dds_p3 = dds_dw3->get_publisher ();
            this->dds_dp3_ = dds_p3->get_participant ();
          }
        else
          {
            ACE_ERROR ((LM_ERROR, "ERROR : Component_exec_i::ccm_activate - "
                        "Error casting DataWriter 3 to typed DataWriter 3\n"));
            throw CORBA::INTERNAL ();
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
    if (!this->dds_dp1_)
      ACE_ERROR ((LM_ERROR, "ERROR: DomainParticipant for Connector 1 seems to be NIL\n"));
    if (!this->dds_dp2_)
      ACE_ERROR ((LM_ERROR, "ERROR: DomainParticipant for Connector 2 seems to be NIL\n"));
    if (!this->dds_dp3_)
      ACE_ERROR ((LM_ERROR, "ERROR: DomainParticipant for Connector 3 seems to be NIL\n"));
  }

  void
  Component_exec_i::ccm_remove (void)
  {
    if (this->dds_dp1_ != this->dds_dp2_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 2 don't seem to "
                              "share the same DomainParticipant\n"));
      }
    else
      {
        ACE_DEBUG ((LM_DEBUG, "Connector 1 and 2 seems to  "
                              "share the same DomainParticipant\n"));
      }
    if (this->dds_dp1_ == this->dds_dp3_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Connector 1 and 3 seem to "
                              "share the same DomainParticipant\n"));
      }
    if (this->dds_dp2_ == this->dds_dp3_)
      {
        ACE_ERROR ((LM_ERROR, "ERROR: Connector 2 and 3 seem to "
                              "share the same DomainParticipant\n"));
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

