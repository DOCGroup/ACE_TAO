/**
 * @author Johnny Willemsen  <jwillemsen@remedy.nl>
 *
 * $Id$
 *
 * Wrapper facade for NDDS.
 */
#ifndef DDS_STATE_CONNECTOR_T_H_
#define DDS_STATE_CONNECTOR_T_H_

#include "dds4ccm/impl/ndds/DDS_TopicBase_Connector_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
class DDS_State_Connector_T
  : public virtual DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>
{
public:
  DDS_State_Connector_T (void);
  virtual ~DDS_State_Connector_T (void);

  virtual typename CCM_TYPE::updater_type::_ptr_type
  get_observable_data (void);

  virtual ::DDS::CCM_DataWriter_ptr
  get_observable_dds_entity (void);

  virtual typename CCM_TYPE::reader_type::_ptr_type
  get_passive_observer_data (void);

  virtual ::DDS::CCM_DataReader_ptr
  get_passive_observer_dds_entity (void);

  virtual typename CCM_TYPE::reader_type::_ptr_type
  get_pull_observer_data (void);

  virtual typename CCM_TYPE::getter_type::_ptr_type
  get_pull_observer_fresh_data (void);

  virtual ::DDS::CCM_DataReader_ptr
  get_pull_observer_dds_entity (void);

  virtual typename CCM_TYPE::reader_type::_ptr_type
  get_push_observer_data (void);

  virtual ::CCM_DDS::CCM_DataListenerControl_ptr
  get_push_observer_data_control (void);

  virtual ::DDS::CCM_DataReader_ptr
  get_push_observer_dds_entity (void);

  virtual typename CCM_TYPE::reader_type::_ptr_type
  get_push_state_observer_data (void);

  virtual ::CCM_DDS::CCM_StateListenerControl_ptr
  get_push_state_observer_data_control (void);

  virtual ::DDS::CCM_DataReader_ptr
  get_push_state_observer_dds_entity (void);

  virtual void configuration_complete (void);

  virtual void ccm_activate (void);
  virtual void ccm_passivate (void);
  virtual void ccm_remove (void);

private:
  void configure_port_dds_update (void);
  ::DDS::Publisher_var supplier_publisher_;
  ::DDS::CCM_DataWriter_var observable_data_;
  ::DDS::DataWriterListener_var supplier_listener_;

  // @from DDS_Listen
  void configure_port_dds_listen (void);
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::ListenerMode> listen_datalistener_mode_;
  ACE_Atomic_Op <TAO_SYNCH_MUTEX, ::CCM_DDS::DataNumber_t> listen_datalistener_max_delivered_data_;
  ::DDS::Subscriber_var listen_subscriber_;
  ::DDS::DataReader_var push_consumer_data_;
  ::DDS::DataReaderListener_var __listen_datareaderlistener;

  // @from DDS_Getter
  ::DDS::DataReader_var pull_consumer_fresh_data_;
};

#include "dds4ccm/impl/ndds/DDS_State_Connector_T.cpp"

#endif /* DDS_STATE_CONNECTOR_T_H_ */
