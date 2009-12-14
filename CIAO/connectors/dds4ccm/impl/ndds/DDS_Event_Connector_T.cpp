// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Writer_T.h"
#include "dds4ccm/impl/ndds/Getter_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/DataListenerControl_T.h"
#include "dds4ccm/impl/ndds/PortStatusListener_T.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_Event_Connector_T (void) :
    DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>()
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_Event_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::writer_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_data (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_data");

  return this->supplier_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_dds_entity (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_dds_entity");

  return this->supplier_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::getter_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_fresh_data (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_fresh_data");

  return this->pull_consumer_.get_fresh_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data");

  return this->push_consumer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_data (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_data");

  return this->pull_consumer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data_control (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data_control");

  return this->push_consumer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_dds_entity (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_dds_entity");

  return this->pull_consumer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_dds_entity (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_dds_entity");

  return this->push_consumer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete");

  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate");
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate ();
  this->push_consumer_.init (
    this->context_->get_connection_push_consumer_data_listener (),
    this->context_->get_connection_push_consumer_status (),
    this->topic_.in (),
    this->subscriber_.in (),
    this->library_name_,
    this->profile_name_);

  this->supplier_.init (
    this->topic_.in (),
    this->publisher_.in (),
    this->library_name_,
    this->profile_name_);

  this->pull_consumer_.init (
    this->context_->get_connection_pull_consumer_status (),
    this->topic_.in (),
    this->subscriber_.in (),
    this->library_name_,
    this->profile_name_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate");

  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove");

  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
}
