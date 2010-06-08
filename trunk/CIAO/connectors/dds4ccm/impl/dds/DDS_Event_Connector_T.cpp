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
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_pull_consumer_filter_config");

  this->pull_consumer_obtained_ = true;
  return this->pull_consumer_.get_filter_config ();
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
::CCM_DDS::QueryFilter*
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::pull_consumer_filter (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::pull_consumer_filter");

  return this->pull_consumer_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::pull_consumer_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::pull_consumer_filter");

  this->pull_consumer_.filter (filter);
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
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_filter_config (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::get_push_consumer_filter_config");

  this->push_consumer_obtained_ = true;
  return this->push_consumer_.get_filter_config ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
::CCM_DDS::QueryFilter*
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_consumer_filter (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_consumer_filter");

  return this->push_consumer_.filter ();
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_consumer_filter (
  const ::CCM_DDS::QueryFilter & filter)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::push_consumer_filter");

  this->push_consumer_.filter (filter);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");

  TopicBaseConnector::configuration_complete ();

  this->push_consumer_obtained_ |=
    ! ::CORBA::is_nil (this->context_->get_connection_push_consumer_data_listener ());

  ::CCM_DDS::PortStatusListener_var push_consumer_psl =
    this->context_->get_connection_push_consumer_status ();
  this->push_consumer_obtained_ |= ! ::CORBA::is_nil (push_consumer_psl.in ());

  ::CCM_DDS::PortStatusListener_var pull_consumer_psl =
    this->context_->get_connection_pull_consumer_status ();
  this->pull_consumer_obtained_ |=
    ! ::CORBA::is_nil (pull_consumer_psl.in ());

  if (this->push_consumer_obtained_)
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                    "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete - "
                    "Creating push consumer port.\n"));
      this->push_consumer_.configuration_complete (
        this->topic_.in (),
        this->subscriber_.in (),
        this->library_name_,
        this->profile_name_);
    }
  else
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                    "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete - "
                    "No need to create push consumer port.\n"));
    }

  if (this->supplier_obtained_)
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                    "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete - "
                    "Creating supplier port.\n"));
      this->supplier_.configuration_complete(
        this->topic_.in (),
        this->publisher_.in (),
        this->library_name_,
        this->profile_name_);
    }
  else
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                    "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete - "
                    "No need to create supplier port.\n"));
    }

  if (this->pull_consumer_obtained_)
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                    "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete - "
                    "Creating pull consumer port.\n"));
      this->pull_consumer_.configuration_complete (
        this->topic_.in (),
        this->subscriber_.in (),
        this->library_name_,
        this->profile_name_);
    }
  else
    {
      DDS4CCM_DEBUG (6, (LM_DEBUG, CLINFO
                    "DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete - "
                    "No need to create pull consumer port.\n"));
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
