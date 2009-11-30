// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Writer_T.h"
#include "dds4ccm/impl/ndds/Updater_T.h"
#include "dds4ccm/impl/ndds/Getter_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/DataListenerControl.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_State_Connector_T (void) :
    DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>()
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_State_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::updater_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_observable_data (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_observable_dds_entity (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_passive_observer_data (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_passive_observer_dds_entity (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_observer_data (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::getter_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_observer_fresh_data (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_observer_dds_entity (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_data (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_data_control (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_dds_entity (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_state_observer_data (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_StateListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_state_observer_data_control (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_state_observer_dds_entity (void)
{
  return 0;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
//  this->configure_port_dds_listen ();
//  this->configure_port_dds_write ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
}
