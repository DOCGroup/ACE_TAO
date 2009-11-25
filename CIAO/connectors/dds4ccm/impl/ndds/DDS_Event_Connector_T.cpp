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
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_Event_Connector_T (void) :
    DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>(),
    listen_datalistener_mode_ ( ::CCM_DDS::NOT_ENABLED),
    listen_datalistener_max_delivered_data_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_Event_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configure_port_dds_write (void)
{
  this->configure_default_topic ();

  try
    {
      if (CORBA::is_nil (this->supplier_publisher_.in ()))
        {
          ::DDS::PublisherQos pqos;
          this->supplier_publisher_ = this->domain_participant_->create_publisher (pqos,
                                                                        0,
                                                                        0);
        }

      if (CORBA::is_nil  (this->supplier_writer_.in ()))
        {
          this->supplier_listener_ = new ::CIAO::DDS4CCM::DataWriterListener_T
            <DDS_TYPE, CCM_TYPE> (
                  this->context_,
                  this->listen_datalistener_mode_,
                  this->listen_datalistener_max_delivered_data_);

          ::DDS::DataWriterQos dwqos;
          ::DDS::DataWriter_var dwv_tmp = this->supplier_publisher_->create_datawriter (this->topic_.in (),
                                                                                        dwqos,
                                                                                        this->supplier_listener_.in (),
                                                                                        DDS_OFFERED_DEADLINE_MISSED_STATUS | DDS_OFFERED_INCOMPATIBLE_QOS_STATUS | DDS_LIVELINESS_LOST_STATUS | DDS_PUBLICATION_MATCHED_STATUS);
          this->supplier_writer_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
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
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configure_port_dds_listen (void)
{
  this->configure_default_topic ();

  try
    {
      if (CORBA::is_nil (this->listen_subscriber_.in ()))
        {
          ::DDS::SubscriberQos sqos;
          this->listen_subscriber_ = this->domain_participant_->create_subscriber (sqos,
                                                                       0,
                                                                       0);
        }

      if (CORBA::is_nil (this->__listen_datareaderlistener.in ()))
        {
          this->__listen_datareaderlistener = new ::CIAO::DDS4CCM::RTI::DataReaderListener_T
            <DDS_TYPE, CCM_TYPE> (
                  this->context_,
                  this->listen_datalistener_mode_,
                  this->listen_datalistener_max_delivered_data_);
        }

      if (CORBA::is_nil (this->push_consumer_data_.in ()))
        {
          ::DDS::DataReaderQos drqos;
          this->push_consumer_data_ =
              this->listen_subscriber_->create_datareader (this->topic_.in (),
                                                           drqos,
                                                           this->__listen_datareaderlistener.in (),
                                                           DDS_DATA_AVAILABLE_STATUS | DDS_REQUESTED_DEADLINE_MISSED_STATUS | DDS_SAMPLE_LOST_STATUS | DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS);
        }

      if (CORBA::is_nil (this->pull_consumer_fresh_data_.in ()))
        {
          ::DDS::DataReaderQos drqos;
          this->pull_consumer_fresh_data_ =
              this->listen_subscriber_->create_datareader (this->topic_.in (),
                                                           drqos,
                                                           this->__listen_datareaderlistener.in (),
                                                           DDS_DATA_AVAILABLE_STATUS | DDS_REQUESTED_DEADLINE_MISSED_STATUS | DDS_SAMPLE_LOST_STATUS | DDS_REQUESTED_INCOMPATIBLE_QOS_STATUS);

        }
    }
  catch (...)
    {
      CIAO_ERROR ((LM_EMERGENCY, "Caught unknown c++ exception while creating subscriber entities\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::writer_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_data (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_data");

  return new CIAO::DDS4CCM::RTI::Writer_T<DDS_TYPE, CCM_TYPE>
          (this->supplier_writer_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_dds_entity (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_dds_entity");

  return this->supplier_writer_.in ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::getter_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_fresh_data (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_fresh_data");

  return new CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE> (
          this->pull_consumer_fresh_data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data");

  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> (
          this->push_consumer_data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_data (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_data");

  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> (
          this->push_consumer_data_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data_control (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data_control");
  return new CCM_DDS_DataListenerControl_i (
          this->listen_datalistener_mode_,
          this->listen_datalistener_max_delivered_data_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_dds_entity (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_dds_entity");
  return ::DDS::CCM_DataReader::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_dds_entity (void)
{
  CIAO_TRACE ("DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_dds_entity");
  return ::DDS::CCM_DataReader::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
  this->configure_port_dds_listen ();
  this->configure_port_dds_write ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
}
