/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef CONNECTOR_T_H_
#define CONNECTOR_T_H_

#include "dds4ccm/idl/dds_rtf2_dcpsC.h"
#include "ace/Copy_Disabled.h"
#include "dds4ccm/impl/ndds/DDS_Base_Connector_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_Event_Connector_T
  : public virtual DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>
{
public:
  DDS_Event_Connector_T (const char * topic_name);
  virtual ~DDS_Event_Connector_T (void);

  virtual char *topic_name (void);

  virtual void topic_name (const char * topic_name);

  virtual ::DDS::StringSeq *key_fields (void);

  virtual void key_fields (const ::DDS::StringSeq & key_fields);

  /**
   * @name DDS_Write
   * DDS_Write operations
   */
  //@{
  virtual typename CCM_TYPE::writer_type::_ptr_type get_supplier_data (void);

  virtual ::DDS::CCM_DataWriter_ptr get_supplier_dds_entity (void);
  //@}

  /**
   * @name DDS_Get
   * DDS_Get operations
   */
  //@{
  virtual typename CCM_TYPE::reader_type::_ptr_type get_pull_consumer_data (void);

  virtual typename CCM_TYPE::getter_type::_ptr_type get_pull_consumer_fresh_data (void);

  virtual ::DDS::CCM_DataReader_ptr get_pull_consumer_dds_entity (void);
  //@}

  /**
   * @name DDS_Listen
   * DDS_Listen operations
   */
  //@{
  virtual typename CCM_TYPE::reader_type::_ptr_type get_push_consumer_data (void);

  virtual ::CCM_DDS::CCM_DataListenerControl_ptr get_push_consumer_data_control (void);

  virtual ::DDS::CCM_DataReader_ptr get_push_consumer_dds_entity (void);
  //@}

  virtual void set_session_context (::Components::SessionContext_ptr ctx);

  virtual void configuration_complete (void);

  virtual void ccm_activate (void);
  virtual void ccm_passivate (void);
  virtual void ccm_remove (void);

private:
  typename CCM_TYPE::context_type::_var_type context_;

  // Default stuff
  void configure_default_domain_ (void);
  ::DDS::DomainParticipantFactory_var domain_factory_;
  ::DDS::DomainParticipant_var domain_;

  // @from DDS_TopicBase
  void configure_default_topic_ (void);
  ::DDS::Topic_var topic_;
  CORBA::String_var topic_name_;
  DDS::StringSeq key_fields_;

  // @from DDS_Write for
  void configure_port_dds_write (void);
  ::DDS::Publisher_var __info_in_publisher_;
  ::DDS::CCM_DataWriter_var __info_in_datawriter_;
  typename CCM_TYPE::writer_type::_var_type __info_in_writer_;

  // @from DDS_Listen
  void configure_port_listen (void);
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> __listen_datalistener_mode_;
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> __listen_datalistener_max_delivered_data_;
  ::DDS::Subscriber_var __listen_subscriber_;
  ::DDS::DataReader_var __listen_datareader_;
  ::DDS::DataReaderListener_var __listen_datareaderlistener;
  ::DDS::DataWriterListener_var __listen_datawriterlistener;
  ::DDS::TopicListener_var __listen_topiclistener;

  // @from DDS_Getter
  ::DDS::DataReader_var __info_get_datareader_;
};

#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.cpp"

#endif /* CONNECTOR_T_H_ */
