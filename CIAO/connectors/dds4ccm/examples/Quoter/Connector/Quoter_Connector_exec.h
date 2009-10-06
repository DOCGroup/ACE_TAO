// -*- C++ -*-
// $Id$

#ifndef CIAO_QUOTER_CONNECTOR_EXEC_H_
#define CIAO_QUOTER_CONNECTOR_EXEC_H_

#include /**/ "ace/pre.h"

#include "Quoter_ConnectorEC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "Quoter_Connector_exec_export.h"
#include "tao/LocalObject.h"

#include "dds4ccm/impl/ndds/NDDS_Traits.h"
#include "dds4ccm/impl/ndds/DataWriter.h"
#include "dds4ccm/impl/ndds/Writer.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "Base/Quoter_BaseSupport.h"

namespace CIAO_Quoter_Quoter_Connector_Impl
{
  class QUOTER_CONNECTOR_EXEC_Export Quoter_Connector_exec_i
    : public virtual Quoter_Connector_Exec,
      public virtual ::CORBA::LocalObject
  {
  public:
    Quoter_Connector_exec_i (void);
    virtual ~Quoter_Connector_exec_i (void);

    virtual char *topic_name (void);

    virtual void topic_name (const char * topic_name);

    virtual ::DDS::StringSeq *key_fields (void);

    virtual void key_fields (const ::DDS::StringSeq & key_fields);

    virtual ::DDS::DomainId_t domain_id (void);

    virtual void domain_id (::DDS::DomainId_t domain_id);

    virtual char *qos_profile (void);

    virtual void qos_profile (const char * qos_profile);

    virtual ::CCM_DDS::CCM_Stock_Info_Writer_ptr get_info_in_data (void);

    virtual ::DDS::CCM_DataWriter_ptr get_info_in_dds_entity (void);

    virtual ::CCM_DDS::CCM_Stock_Info_Reader_ptr get_info_out_data (void);

    virtual ::CCM_DDS::CCM_ListenerControl_ptr get_info_out_control (void);

    virtual ::DDS::CCM_DataReader_ptr get_info_out_dds_entity (void);

    virtual void et_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    ::Quoter::CCM_Quoter_Connector_Context_var context_;

    // @from use of Stock_Info as a parameter
    typedef CIAO::DDS4CCM::RTI::Type_Traits < ::Quoter::Stock_Info,
      ::Quoter::Stock_Info_Seq,
      ::Quoter::Stock_InfoTypeSupport,
      ::Quoter::Stock_InfoDataWriter,
      ::Quoter::Stock_InfoDataReader > Stock_Info_Traits;

    // Default stuff
    void configure_default_domain_ (void);
    bool default_domain_configured_;
    ::DDS::DomainParticipantFactory_var domain_factory_;
    ::DDS::DomainParticipant_var domain_;

    // @from DDS_Base
    CCM_DDS::ConnectorStatusListener_var error_listener_;
    DDS::DomainId_t domain_id_;
    CORBA::String_var qos_profile_;

    // @from DDS_TopicBase
    void configure_default_topic_ (void);
    bool default_topic_configured_;
    ::DDS::Topic_var topic_;
    CORBA::String_var topic_name_;
    DDS::StringSeq key_fields_;

    // @from DDS_Write for Stock_Info
    void configure_port_info_in_ (void);
    bool __info_in_configured_;
    ::DDS::Publisher_var __info_in_publisher_;
    ::DDS::CCM_DataWriter_var __info_in_datawriter_;
    ::CCM_DDS::CCM_Stock_Info_Writer_var __info_in_writer_;

    // @from DDS_RawListen
    void configure_port_info_out_ (void);
    bool __info_out_configured_;
    CCM_DDS::Stock_Info_RawListener_var __info_out_rawlistener_;
    ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> __info_out_rawlistener_enabled_;
    CCM_DDS::PortStatusListener_var __info_out_portstatus_;
    ::DDS::Subscriber_var __info_out_subscriber_;
    ::DDS::DataReaderListener_var __info_out_listener_;
    ::DDS::DataReader_var __info_out_datareader_;
    ::DDS::DataReaderListener_var __info_out_datareaderlistener;
  };

  extern "C" QUOTER_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Quoter_Quoter_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

