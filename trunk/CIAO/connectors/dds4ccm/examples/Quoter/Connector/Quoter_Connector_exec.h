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
  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  class Connector_T
    : public virtual CONNECTOR_TYPE::base_type,
      public virtual ::CORBA::LocalObject
  {
  public:
    Connector_T (void);
    virtual ~Connector_T (void);

    virtual char *topic_name (void);

    virtual void topic_name (const char * topic_name);

    virtual ::DDS::StringSeq *key_fields (void);

    virtual void key_fields (const ::DDS::StringSeq & key_fields);

    virtual ::DDS::DomainId_t domain_id (void);

    virtual void domain_id (::DDS::DomainId_t domain_id);

    virtual char *qos_profile (void);

    virtual void qos_profile (const char * qos_profile);

    virtual typename CONNECTOR_TYPE::writer_type::_ptr_type get_info_in_data (void);

    virtual ::DDS::CCM_DataWriter_ptr get_info_in_dds_entity (void);

    virtual typename CONNECTOR_TYPE::reader_type::_ptr_type get_info_out_data (void);

    virtual ::CCM_DDS::CCM_ListenerControl_ptr get_info_out_control (void);

    virtual ::DDS::CCM_DataReader_ptr get_info_out_dds_entity (void);

    virtual void set_session_context (::Components::SessionContext_ptr ctx);

    virtual void configuration_complete (void);

    virtual void ccm_activate (void);
    virtual void ccm_passivate (void);
    virtual void ccm_remove (void);

  private:
    typename CONNECTOR_TYPE::context_type::_var_type context_;

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
    typename CONNECTOR_TYPE::writer_type::_var_type __info_in_writer_;

    // @from DDS_RawListen
    void configure_port_info_out_ (void);
    bool __info_out_configured_;
    typename CONNECTOR_TYPE::rawlistener_type::_var_type __info_out_rawlistener_;
    ACE_Atomic_Op <TAO_SYNCH_MUTEX, bool> __info_out_rawlistener_enabled_;
    CCM_DDS::PortStatusListener_var __info_out_portstatus_;
    ::DDS::Subscriber_var __info_out_subscriber_;
    ::DDS::DataReaderListener_var __info_out_listener_;
    ::DDS::DataReader_var __info_out_datareader_;
    ::DDS::DataReaderListener_var __info_out_datareaderlistener;
  };

  template <typename BASE_TYPE,
            typename WRITER_TYPE,
            typename READER_TYPE,
            typename CONTEXT_TYPE,
            typename RAWLISTENER_TYPE>
  struct Connector_Traits
  {
    typedef BASE_TYPE base_type;
    typedef WRITER_TYPE writer_type;
    typedef READER_TYPE reader_type;
    typedef CONTEXT_TYPE context_type;
    typedef RAWLISTENER_TYPE rawlistener_type;
  };

  // @from use of Stock_Info as a parameter
  typedef CIAO::DDS4CCM::RTI::Type_Traits < ::Quoter::Stock_Info,
    ::Quoter::Stock_Info_Seq,
    ::Quoter::Stock_InfoTypeSupport,
    ::Quoter::Stock_InfoDataWriter,
    ::Quoter::Stock_InfoDataReader > Stock_Info_DDS_Traits;

  typedef Connector_Traits <
    ::CIAO_Quoter_Quoter_Connector_Impl::Quoter_Connector_Exec,
    ::CCM_DDS::CCM_Stock_Info_Writer,
    ::CCM_DDS::CCM_Stock_Info_Reader,
    ::Quoter::CCM_Quoter_Connector_Context,
    ::CCM_DDS::Stock_Info_RawListener> Stock_Info_Connector_Traits;

  class QUOTER_CONNECTOR_EXEC_Export Quoter_Connector_exec_i :
    public Connector_T <Stock_Info_DDS_Traits, Stock_Info_Connector_Traits>
  {
  public:
    Quoter_Connector_exec_i (void);
    virtual ~Quoter_Connector_exec_i (void);
  };

  extern "C" QUOTER_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Quoter_Quoter_Connector_Impl (void);
}

#include /**/ "ace/post.h"

#endif /* ifndef */

