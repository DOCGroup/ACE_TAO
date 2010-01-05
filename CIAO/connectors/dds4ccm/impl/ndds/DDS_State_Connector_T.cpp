// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderStateListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Updater_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/PublisherListener_T.h"
#include "dds4ccm/impl/ndds/SubscriberListener_T.h"
#include "dds4ccm/impl/ndds/DataListenerControl_T.h"
#include "dds4ccm/impl/ndds/StateListenerControl_T.h"
#include "dds4ccm/impl/ndds/PortStatusListener_T.h"

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
  return this->observable_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_observable_dds_entity (void)
{
  return this->observable_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_passive_observer_data (void)
{
  return this->passive_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_passive_observer_dds_entity (void)
{
  return this->passive_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_observer_data (void)
{
  return this->pull_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::getter_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_observer_fresh_data (void)
{
  return this->pull_observer_.get_fresh_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_observer_dds_entity (void)
{
  return this->pull_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_data (void)
{
  return this->push_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_data_control (void)
{
  return this->push_observer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_dds_entity (void)
{
  return this->push_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_state_observer_data (void)
{
  return this->push_state_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_StateListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_state_observer_data_control (void)
{
  return this->push_state_observer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_state_observer_dds_entity (void)
{
  return this->push_state_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
  this->push_observer_.configuration_complete (
    this->topic_.in (),
    this->subscriber_.in (),
    this->library_name_,
    this->profile_name_);

  this->push_state_observer_.configuration_complete (
    this->topic_.in (),
    this->subscriber_.in (),
    this->library_name_,
    this->profile_name_);

  this->pull_observer_.configuration_complete (
    this->topic_.in (),
    this->subscriber_.in (),
    this->library_name_,
    this->profile_name_);

  this->passive_observer_.configuration_complete (
    this->topic_.in (),
    this->subscriber_.in (),
    this->library_name_,
    this->profile_name_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate ();
  this->observable_.init (
    this->topic_.in (),
    this->publisher_.in (),
    this->library_name_,
    this->profile_name_);

  this->push_observer_.activate (
    this->context_->get_connection_push_observer_data_listener (),
    this->context_->get_connection_push_observer_status ());

  this->push_state_observer_.activate (
    this->context_->get_connection_push_state_observer_data_listener (),
    this->context_->get_connection_push_state_observer_status ());

  this->pull_observer_.activate (
    this->context_->get_connection_pull_observer_status ());

  this->passive_observer_.activate (
    this->context_->get_connection_passive_observer_status ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate ();
  this->push_observer_.passivate ();
  this->push_state_observer_.passivate ();
  this->pull_observer_.passivate ();
  this->passive_observer_.passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
  this->push_observer_.remove ();
  this->push_state_observer_.remove ();
  this->pull_observer_.remove ();
  this->passive_observer_.remove ();
}

