// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/dds/DataReaderListener_T.h"
#include "dds4ccm/impl/dds/DataWriterListener_T.h"
#include "dds4ccm/impl/dds/Writer_T.h"
#include "dds4ccm/impl/dds/Getter_T.h"
#include "dds4ccm/impl/dds/Reader_T.h"
#include "dds4ccm/impl/dds/DataListenerControl_T.h"
#include "dds4ccm/impl/dds/PortStatusListener_T.h"

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Event_Connector_T (void) :
    DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>()
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_Event_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::writer_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_supplier_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_supplier_data");

  return this->supplier_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataWriter_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_supplier_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_supplier_dds_entity");

  return this->supplier_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::getter_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_fresh_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_fresh_data");

  return this->pull_consumer_.get_fresh_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_data");

  return this->push_consumer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_data");

  return this->pull_consumer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_data_control (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_data_control");

  return this->push_consumer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_dds_entity");

  return this->pull_consumer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_dds_entity");

  return this->push_consumer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();

  this->push_consumer_.configuration_complete (
    this->topic_.in (),
    this->subscriber_.in (),
    this->library_name_,
    this->profile_name_);

  this->supplier_.configuration_complete(
    this->topic_.in (),
    this->publisher_.in (),
    this->library_name_,
    this->profile_name_);

  this->pull_consumer_.configuration_complete (
    this->topic_.in (),
    this->subscriber_.in (),
    this->library_name_,
    this->profile_name_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_activate");
  ACE_Reactor* reactor = 0;
#if defined (CIAO_DDS4CCM_CONTEXT_SWITCH) && (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
  reactor = this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ();
#endif
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (reactor);

  this->push_consumer_.activate (
    this->context_->get_connection_push_consumer_data_listener (),
    this->context_->get_connection_push_consumer_status (),
    reactor);

  this->supplier_.activate ();

  this->pull_consumer_.activate (
    this->context_->get_connection_pull_consumer_status (),
    reactor);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_passivate");

  this->push_consumer_.passivate ();
  this->supplier_.passivate ();
  this->pull_consumer_.passivate ();
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_remove");

  this->push_consumer_.remove (this->subscriber_.in ());
  this->supplier_.remove (this->publisher_.in ());
  this->pull_consumer_.remove (this->subscriber_.in ());
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
}
