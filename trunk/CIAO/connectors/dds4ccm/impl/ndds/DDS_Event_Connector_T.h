/**
 * @author Marcel Smit <msmit@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_EVENT_CONNECTOR_T_H_
#define DDS_EVENT_CONNECTOR_T_H_

#include "dds4ccm/impl/ndds/DDS_TopicBase_Connector_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_Event_Connector_T
  : public virtual DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>
{
public:
  DDS_Event_Connector_T (void);
  virtual ~DDS_Event_Connector_T (void);

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

  virtual void configuration_complete (void);

  virtual void ccm_activate (void);
  virtual void ccm_passivate (void);
  virtual void ccm_remove (void);

private:
  /**
   * DDS_Write supplier
   */
  //@{
  void configure_port_dds_write_supplier (void);
  ::DDS::CCM_DataWriter_var supplier_writer_;
  ::DDS::DataWriterListener_var datawriter_listener_;
  //@}

  /**
   * DDS_Listen push_consumer
   */
  //@{
  void configure_port_dds_listen_push_consumer (void);
  ::DDS::DataReader_var push_consumer_data_;
  ::CCM_DDS::CCM_DataListenerControl_var push_consumer_data_control_;
  ::DDS::DataReaderListener_var push_consumer_data_listener_;
  //@}

  /**
   * DDS_Get pull_consumer
   */
  //@{
  void configure_port_dds_get_pull_consumer (void);
  ::DDS::DataReader_var pull_consumer_data_;
  ::DDS::DataReader_var pull_consumer_fresh_data_;
  ::DDS::DataReaderListener_var pull_consumer_status_;
  //@}
};

#include "dds4ccm/impl/ndds/DDS_Event_Connector_T.cpp"

#endif /* DDS_EVENT_CONNECTOR_T_H_ */
