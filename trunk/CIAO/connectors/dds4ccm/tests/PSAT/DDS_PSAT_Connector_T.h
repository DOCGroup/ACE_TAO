// $Id$

#ifndef DDS_PSAT_CONNECTOR_T
#define DDS_PSAT_CONNECTOR_T

#include "connectors/dds4ccm/impl/DDS_TopicBase_Connector_T.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
class DDS_PSAT_Event_Connector_T
  : public virtual DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>
{
  virtual
  typename CCM_TYPE::supplier_traits::data_type::_ptr_type
  get_supplier_data (void)
  {
    return CCM_TYPE::supplier_traits::data_type::_nil ();
  }

  virtual
  typename CCM_TYPE::supplier_traits::dds_entity_type::_ptr_type
  get_supplier_dds_entity (void)
  {
    return CCM_TYPE::supplier_traits::dds_entity_type::_nil ();
  }

  virtual
  typename CCM_TYPE::consumer_traits::data_type::_ptr_type
  get_consumer_data (void)
  {
    return CCM_TYPE::consumer_traits::data_type::_nil ();
  }

  virtual
  typename CCM_TYPE::consumer_traits::data_control_type::_ptr_type
  get_consumer_data_control (void)
  {
    return CCM_TYPE::consumer_traits::data_control_type::_nil ();
  }

  virtual
  typename CCM_TYPE::consumer_traits::dds_entity_type::_ptr_type
  get_consumer_dds_entity (void)
  {
    return CCM_TYPE::consumer_traits::dds_entity_type::_nil ();
  }

  virtual
  typename CCM_TYPE::consumer_traits::buffer_control_type::_ptr_type
  get_consumer_buffer_control (void)
  {
    return CCM_TYPE::consumer_traits::buffer_control_type::_nil ();
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
