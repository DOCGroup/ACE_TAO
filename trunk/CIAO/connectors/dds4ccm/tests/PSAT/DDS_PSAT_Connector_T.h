// $Id$

#ifndef DDS_PSAT_CONNECTOR_T
#define DDS_PSAT_CONNECTOR_T

#include "connectors/dds4ccm/impl/DDS_TopicBase_Connector_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_PSAT_Event_Connector_T
  : public virtual DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>
{
  virtual
  ::CSLDeadlineConnector::CCM_PSAT_Writer_ptr
  get_supplier_data (void)
  {
    return ::CSLDeadlineConnector::CCM_PSAT_Writer::_nil ();
  }
    
  virtual
  ::DDS::CCM_DataWriter_ptr
  get_supplier_dds_entity (void)
  {
    return ::DDS::CCM_DataWriter::_nil ();
  }
    
  virtual
  ::CSLDeadlineConnector::DDS_Typed::CCM_Reader_ptr
  get_consumer_data (void)
  {
    return ::CSLDeadlineConnector::DDS_Typed::CCM_Reader::_nil ();
  }
    
  virtual
  ::CCM_DDS::CCM_DataListenerControl_ptr
  get_consumer_data_control (void)
  {
    return ::CCM_DDS::CCM_DataListenerControl::_nil ();
  }
    
  virtual
  ::DDS::CCM_DataReader_ptr
  get_consumer_dds_entity (void)
  {
    return ::DDS::CCM_DataReader::_nil ();
  }
    
  virtual
  ::CSLDeadlineConnector::CCM_PSAT_BufferControl_ptr
  get_consumer_buffer_control (void)
  {
    return ::CSLDeadlineConnector::CCM_PSAT_BufferControl::_nil ();
  }
    
  virtual
  ::CCM_DDS_PSAT::SharedMemoryConfig *
  shared_memory_config (void)
  {
    return 0;
  }
    
  virtual
  void
  shared_memory_config (
    const ::CCM_DDS_PSAT::SharedMemoryConfig & /*shared_memory_config */)
  {
  }
    
  virtual
  ::CCM_DDS_PSAT::BufferPoolConfig
  attach_buffer_config (void)
  {
    return ::CCM_DDS_PSAT::BufferPoolConfig ();
  }
    
  virtual
  void
  attach_buffer_config (
    const ::CCM_DDS_PSAT::BufferPoolConfig & /* attach_buffer_config */)
  {
  }
    
  virtual
  ::CCM_DDS_PSAT::TransportConfig *
  transports_config (void)
  {
    return 0;
  }
    
  virtual
  void
  transports_config (
    const ::CCM_DDS_PSAT::TransportConfig & /* transports_config */)
  {
  }
};

#endif
