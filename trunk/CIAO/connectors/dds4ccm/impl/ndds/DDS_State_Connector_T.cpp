// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DataReaderStateListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Updater_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/PublisherListener_T.h"
#include "dds4ccm/impl/ndds/SubscriberListener_T.h"
#include "dds4ccm/impl/ndds/DataListenerControl.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_State_Connector_T (void) :
    DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>(),
    listen_datalistener_mode_ ( ::CCM_DDS::NOT_ENABLED),
    listen_datalistener_max_delivered_data_ (0)
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
          this->push_consumer_data_.in ());
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
          this->push_consumer_data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_observer_data_control (void)
{
  return new CCM_DDS_DataListenerControl_i (
          this->listen_datalistener_mode_,
          this->listen_datalistener_max_delivered_data_);
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
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate ();
  this->configure_port_dds_update ();
  this->configure_port_dds_listen ();
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
DDS_State_Connector_T<DDS_TYPE, CCM_TYPE>::configure_port_dds_listen (void)
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
                  this->listen_datalistener_mode_,
                  this->listen_datalistener_max_delivered_data_);
        }

      if (CORBA::is_nil (this->push_consumer_data_.in ()))
        {
          if (this->library_name_ && this->profile_name_)
            {
              this->push_consumer_data_ =
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
              this->push_consumer_data_ =
                  this->subscriber_->create_datareader (
                    this->topic_.in (),
                    drqos,
                    this->__listen_datareaderlistener.in (),
                    ::CIAO::DDS4CCM::RTI::DataReaderStateListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
            }
        }

      if (CORBA::is_nil (this->pull_consumer_fresh_data_.in ()))
        {
          if (this->profile_name_ && this->library_name_)
            {
              this->pull_consumer_fresh_data_ =
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
              this->pull_consumer_fresh_data_ =
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
