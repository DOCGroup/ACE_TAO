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
#include "dds4ccm/impl/dds4ccm_conf.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Event_Connector_T (void) :
    TopicBaseConnector (),
    supplier_obtained_ (false),
    push_consumer_obtained_ (false),
    pull_consumer_obtained_ (false)

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

  this->supplier_obtained_ = true;
  return this->supplier_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataWriter_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_supplier_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_supplier_dds_entity");

  this->supplier_obtained_ = true;
  return this->supplier_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::getter_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_fresh_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_fresh_data");

  this->pull_consumer_obtained_ = true;
  return this->pull_consumer_.get_fresh_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_data");

  this->push_consumer_obtained_ = true;
  return this->push_consumer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_data (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_data");

  this->pull_consumer_obtained_ = true;
  return this->pull_consumer_.get_data ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_data_control (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_data_control");

  this->push_consumer_obtained_ = true;
  return this->push_consumer_.get_data_control ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_dds_entity");

  this->pull_consumer_obtained_ = true;
  return this->pull_consumer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_dds_entity (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_dds_entity");

  this->push_consumer_obtained_ = true;
  return this->push_consumer_.get_dds_entity ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  TopicBaseConnector::configuration_complete ();

  this->push_consumer_obtained_ |=
    ! ::CORBA::is_nil (this->context_->get_connection_push_consumer_data_listener ());
  this->push_consumer_obtained_ |=
    ! ::CORBA::is_nil (this->context_->get_connection_push_consumer_status ());
  this->pull_consumer_obtained_ |=
    ! ::CORBA::is_nil (this->context_->get_connection_pull_consumer_status ());

  if (this->push_consumer_obtained_)
    {
      this->push_consumer_.configuration_complete (
        this->topic_.in (),
        this->subscriber_.in (),
        this->library_name_,
        this->profile_name_);
    }

  if (this->supplier_obtained_)
    {
      this->supplier_.configuration_complete(
        this->topic_.in (),
        this->publisher_.in (),
        this->library_name_,
        this->profile_name_);
    }

  if (this->pull_consumer_obtained_)
    {
      this->pull_consumer_.configuration_complete (
        this->topic_.in (),
        this->subscriber_.in (),
        this->library_name_,
        this->profile_name_);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_activate");
  ACE_Reactor* reactor = 0;
#if (CIAO_DDS4CCM_CONTEXT_SWITCH == 1)
  reactor = this->context_->get_CCM_object()->_get_orb ()->orb_core ()->reactor ();
#endif
  TopicBaseConnector::ccm_activate (reactor);

  if (this->push_consumer_obtained_)
    {
      this->push_consumer_.activate (
        this->context_->get_connection_push_consumer_data_listener (),
        this->context_->get_connection_push_consumer_status (),
        reactor);
    }

  if (this->supplier_obtained_)
    {
      this->supplier_.activate ();
    }

  if (this->pull_consumer_obtained_)
    {
      this->pull_consumer_.activate (
        this->context_->get_connection_pull_consumer_status (),
        reactor);
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_passivate");

  if (this->push_consumer_obtained_)
    {
      this->push_consumer_.passivate ();
    }

  if (this->supplier_obtained_)
    {
      this->supplier_.passivate ();
    }

  if (this->pull_consumer_obtained_)
    {
      this->pull_consumer_.passivate ();
    }
  TopicBaseConnector::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::ccm_remove");

  if (this->push_consumer_obtained_)
    {
      this->push_consumer_.remove (this->subscriber_.in ());
    }

  if (this->supplier_obtained_)
    {
      this->supplier_.remove (this->publisher_.in ());
    }

  if (this->pull_consumer_obtained_)
    {
      this->pull_consumer_.remove (this->subscriber_.in ());
    }
  TopicBaseConnector::ccm_remove ();
}
