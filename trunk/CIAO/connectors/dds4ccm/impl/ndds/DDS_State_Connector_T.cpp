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
  return new CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE, CCM_TYPE>
          (this->observable_data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_observable_dds_entity (void)
{
  return this->observable_data_.in ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_passive_observer_data (void)
{
  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> (
          this->push_observer_data_.in ());
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
  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> (
          this->push_observer_data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_data_control (void)
{
  if (CORBA::is_nil (this->push_observer_data_control_.in ()))
    {
      this->push_observer_data_control_ = new CCM_DDS_DataListenerControl_T
        < ::CCM_DDS::CCM_DataListenerControl> ();
    }

  return this->push_observer_data_control_.in ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_dds_entity (void)
{
  return ::DDS::CCM_DataReader::_nil ();
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
  if (CORBA::is_nil (this->push_observer_state_control_.in ()))
    {
      this->push_observer_state_control_ = new CCM_DDS_StateListenerControl_T
        < ::CCM_DDS::CCM_StateListenerControl> ();
    }

  return this->push_observer_state_control_.in ();
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
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate ();
  this->configure_port_dds_update ();
  this->configure_passive_observer ();
  this->configure_dds_get_pull_observer ();
  this->configure_push_observer ();
  this->configure_push_state_observer ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configure_port_dds_update (void)
{
  try
    {
      if (CORBA::is_nil  (this->observable_data_.in ()))
        {
          this->datawriter_listener_ = new ::CIAO::DDS4CCM::DataWriterListener_T
            <DDS_TYPE, CCM_TYPE> (
                  this->context_,
                  this->context_->get_connection_error_listener ());

          if (this->library_name_ && this->profile_name_)
            {
              ::DDS::DataWriter_var dwv_tmp = this->publisher_->
                create_datawriter_with_profile (
                  this->topic_.in (),
                  this->library_name_,
                  this->profile_name_,
                  this->datawriter_listener_.in (),
                  ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              this->observable_data_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
            }
            else
            {
              ::DDS::DataWriterQos dwqos;
              ::DDS::DataWriter_var dwv_tmp = this->publisher_->
                create_datawriter (
                  this->topic_.in (),
                  dwqos,
                  this->datawriter_listener_.in (),
                  ::CIAO::DDS4CCM::DataWriterListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
              this->observable_data_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
            }
        }
    }
  catch (...)
    {
      CIAO_ERROR ((LM_ERROR, "Caught unknown C++ exception while configuring port info_in_\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configure_passive_observer (void)
{
  try
    {
      if (CORBA::is_nil (this->__listen_datareaderlistener.in ()))
        {
          this->__listen_datareaderlistener = new ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T
            <DDS_TYPE, CCM_TYPE> (
                  this->context_,
                  this->context_->get_connection_error_listener (),
                  this->context_->get_connection_push_state_observer_data_listener (),
                  this->context_->get_connection_pull_observer_status (),
                  this->get_push_state_observer_data_control ());
        }

      if (CORBA::is_nil (this->push_observer_data_.in ()))
        {
          if (this->library_name_ && this->profile_name_)
            {
              this->push_observer_data_ =
                  this->subscriber_->create_datareader_with_profile (
                    this->topic_.in (),
                    this->library_name_,
                    this->profile_name_,
                    this->__listen_datareaderlistener.in (),
                    ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              this->push_observer_data_ =
                  this->subscriber_->create_datareader (
                    this->topic_.in (),
                    drqos,
                    this->__listen_datareaderlistener.in (),
                    ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
        }

      if (CORBA::is_nil (this->pull_observer_fresh_data_.in ()))
        {
          if (this->profile_name_ && this->library_name_)
            {
              this->pull_observer_fresh_data_ =
                  this->subscriber_->create_datareader_with_profile (
                    this->topic_.in (),
                    this->library_name_,
                    this->profile_name_,
                    this->__listen_datareaderlistener.in (),
                    ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              this->pull_observer_fresh_data_ =
                  this->subscriber_->create_datareader (
                    this->topic_.in (),
                    drqos,
                    this->__listen_datareaderlistener.in (),
                    ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }

        }
    }
  catch (...)
    {
      CIAO_ERROR ((LM_EMERGENCY, "Caught unknown c++ exception while creating subscriber entities\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configure_dds_get_pull_observer (void)
{
  CIAO_TRACE ("DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configure_dds_get_pull_observer");

  try
    {
      if (CORBA::is_nil (this->pull_observer_status_.in ()))
        {
          this->pull_observer_status_ = new ::CIAO::DDS4CCM::RTI::PortStatusListener_T
            <DDS_TYPE, CCM_TYPE> (
                  this->context_,
                  this->context_->get_connection_pull_observer_status ());
        }

      if (CORBA::is_nil (this->pull_observer_fresh_data_.in ()))
        {
          if (this->profile_name_ && this->library_name_)
            {
              this->pull_observer_fresh_data_ =
                  this->subscriber_->create_datareader_with_profile (
                    this->topic_.in (),
                    this->library_name_,
                    this->profile_name_,
                    this->pull_observer_status_.in (),
                    ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
          else
            {
              ::DDS::DataReaderQos drqos;
              this->pull_observer_fresh_data_ =
                  this->subscriber_->create_datareader (
                    this->topic_.in (),
                    drqos,
                    this->pull_observer_status_.in (),
                    ::CIAO::DDS4CCM::RTI::PortStatusListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
        }
    }
  catch (...)
    {
      CIAO_ERROR ((LM_EMERGENCY, "Caught unknown c++ exception while creating subscriber entities\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configure_push_observer (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configure_push_state_observer (void)
{
}
