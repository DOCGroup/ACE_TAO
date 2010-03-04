// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/dds/DataReaderStateListener_T.h"
#include "dds4ccm/impl/dds/DataWriterListener_T.h"
#include "dds4ccm/impl/dds/Updater_T.h"
#include "dds4ccm/impl/dds/Reader_T.h"
#include "dds4ccm/impl/dds/PublisherListener_T.h"
#include "dds4ccm/impl/dds/SubscriberListener_T.h"
#include "dds4ccm/impl/dds/DataListenerControl_T.h"
#include "dds4ccm/impl/dds/StateListenerControl_T.h"
#include "dds4ccm/impl/dds/PortStatusListener_T.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_State_Connector_T (void) :
    DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE> ()
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_State_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::updater_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_observable_data (void)
{
  return this->observable_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataWriter_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_observable_dds_entity (void)
{
  return this->observable_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_passive_observer_data (void)
{
  return this->passive_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_passive_observer_dds_entity (void)
{
  return this->passive_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_data (void)
{
  return this->pull_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::getter_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_fresh_data (void)
{
  return this->pull_observer_.get_fresh_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_observer_dds_entity (void)
{
  return this->pull_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_data (void)
{
  return this->push_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_data_control (void)
{
  return this->push_observer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_observer_dds_entity (void)
{
  return this->push_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_data (void)
{
  return this->push_state_observer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_StateListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_data_control (void)
{
  return this->push_state_observer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_state_observer_dds_entity (void)
{
  return this->push_state_observer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
  this->observable_.configuration_complete (
    this->topic_.in (),
    this->publisher_.in (),
    this->library_name_,
    this->profile_name_);

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

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_activate (void)
{
  ACE_Reactor* reactor = 0;
#if defined (CIAO_DDS4CCM_CONTEXT_SWITCH) && (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
  reactor = this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ();
#endif
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (reactor);

  this->observable_.activate ();

  this->push_observer_.activate (
    this->context_->get_connection_push_observer_data_listener (),
    this->context_->get_connection_push_observer_status (),
    reactor);

  this->push_state_observer_.activate (
    this->context_->get_connection_push_state_observer_data_listener (),
    this->context_->get_connection_push_state_observer_status (),
    reactor);

  this->pull_observer_.activate (
    this->context_->get_connection_pull_observer_status (),
    reactor);

  this->passive_observer_.activate (
    this->context_->get_connection_passive_observer_status (),
    reactor);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_passivate (void)
{
  this->observable_.passivate ();
  this->push_observer_.passivate ();
  this->push_state_observer_.passivate ();
  this->pull_observer_.passivate ();
  this->passive_observer_.passivate ();
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_remove (void)
{
  this->observable_.remove (this->publisher_.in ());
  this->push_observer_.remove (this->subscriber_.in ());
  this->push_state_observer_.remove (this->subscriber_.in ());
  this->pull_observer_.remove (this->subscriber_.in ());
  this->passive_observer_.remove (this->subscriber_.in ());
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
}

