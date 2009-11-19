// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/DataReaderListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/TopicListener_T.h"
#include "dds4ccm/impl/ndds/Writer_T.h"
#include "dds4ccm/impl/ndds/Updater_T.h"
#include "dds4ccm/impl/ndds/Getter_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/DataListenerControl.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_Event_Connector_T (const char * topic_name)
  : domain_id_ (0),
    topic_name_ (topic_name),
    __listen_datalistener_mode_ ( ::CCM_DDS::NOT_ENABLED),
    __listen_datalistener_max_delivered_data_ (0)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_Event_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (void)
{
  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (
  const char * topic_name)
{
  this->topic_name_ = topic_name;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (const ::DDS::StringSeq & key_fields)
{
  ACE_UNUSED_ARG (key_fields);
  // @todo
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StringSeq *
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (void)
{
  // @from DDS_TopicBase
  ::DDS::StringSeq *retval =
    new ::DDS::StringSeq (this->key_fields_.length ());

  for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
    {
      (*retval)[i] = CORBA::string_dup (this->key_fields_[i]);
    }
  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::DomainId_t
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (void)
{
  return this->domain_id_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (
  ::DDS::DomainId_t domain_id)
{
  this->domain_id_ = domain_id;
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (void)
{
  return CORBA::string_dup (this->qos_profile_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (
  const char * qos_profile)
{
  this->qos_profile_ = qos_profile;
}

// Port operations.

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configure_default_domain_ (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "DDS_Event_Connector_T::configure_default_domain_ - "
                "Configuring default domain\n"));

  if (CORBA::is_nil (this->domain_factory_.in ()))
    {
      try
      {
        //NDDSConfigLogger::get_instance()->set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API,
        //                                                           NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL );

        // Generic code
        this->domain_factory_ =
          new ::CIAO::DDS4CCM::RTI::RTI_DomainParticipantFactory_i ();

        ::DDS::DomainParticipantQos qos;
        this->domain_ =
          this->domain_factory_->create_participant (this->domain_id_,
                                                     qos,
                                                     0,
                                                     0);
      }
    catch (...)
      {
        CIAO_ERROR ((LM_ERROR, "Caught unknown C++ exception whilst configuring default domain\n"));
        throw CORBA::INTERNAL ();
      }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configure_default_topic_ (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "DDS_Event_Connector_T::configure_default_topic_ - "
                "Configuring default topic\n"));

  this->configure_default_domain_ ();

  if (CORBA::is_nil (this->topic_))
    {
      try
        {
          CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i *part =
            dynamic_cast< CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i * > (this->domain_.in ());
          DDS_ReturnCode_t const retcode = DDS_TYPE::type_support::register_type(
                part->get_participant (), DDS_TYPE::type_support::get_type_name ());
          if (retcode == DDS_RETCODE_OK)
            {
              this->__listen_topiclistener = new ::CIAO::DDS4CCM::TopicListener_T
                <DDS_TYPE, CCM_TYPE> (
                    this->context_);

              ::DDS::TopicQos tqos;
              this->topic_ =
                this->domain_->create_topic (this->topic_name_.in (),
                                             DDS_TYPE::type_support::get_type_name (),
                                             tqos,
                                             this->__listen_topiclistener.in (),
                                             DDS_INCONSISTENT_TOPIC_STATUS);
            }
          else
            {
              throw CCM_DDS::InternalError (retcode, 0);
            }
        }
      catch (...)
        {
          CIAO_ERROR ((LM_ERROR, "Caught unknown error while configuring default topic\n"));
          throw CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configure_port_dds_write (void)
{
  this->configure_default_topic_ ();

  try
    {
      if (CORBA::is_nil (this->__info_in_publisher_.in ()))
        {
          ::DDS::PublisherQos pqos;
          this->__info_in_publisher_ = this->domain_->create_publisher (pqos,
                                                                        0,
                                                                        0);
        }

      if (CORBA::is_nil  (this->__info_in_datawriter_.in ()))
        {
          this->__listen_datawriterlistener = new ::CIAO::DDS4CCM::DataWriterListener_T
            <DDS_TYPE, CCM_TYPE> (
                  this->context_,
                  this->__listen_datalistener_mode_,
                  this->__listen_datalistener_max_delivered_data_);

          ::DDS::DataWriterQos dwqos;
          ::DDS::DataWriter_var dwv_tmp = this->__info_in_publisher_->create_datawriter (this->topic_.in (),
                                                                                          dwqos,
                                                                                          this->__listen_datawriterlistener.in (),
                                                                                          0);
          this->__info_in_datawriter_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
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
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configure_port_listen (void)
{
  this->configure_default_topic_ ();

  try
    {
      if (CORBA::is_nil (this->__listen_subscriber_.in ()))
        {
          ::DDS::SubscriberQos sqos;
          this->__listen_subscriber_ = this->domain_->create_subscriber (sqos,
                                                                         0,
                                                                          0);
        }

      if (CORBA::is_nil (this->__listen_datareaderlistener.in ()))
        {
        this->__listen_datareaderlistener = new ::CIAO::DDS4CCM::RTI::DataReaderListener_T
          <DDS_TYPE, CCM_TYPE> (
                this->context_,
                this->__listen_datalistener_mode_,
                this->__listen_datalistener_max_delivered_data_);
        }
      if (CORBA::is_nil (this->__listen_datareader_.in ()))
        {
          ::DDS::DataReaderQos drqos;
          this->__listen_datareader_ =
              this->__listen_subscriber_->create_datareader (this->topic_.in (),
                                                           drqos,
                                                           this->__listen_datareaderlistener.in (),
                                                           DDS_DATA_AVAILABLE_STATUS);
        }

      if (CORBA::is_nil (this->__info_get_datareader_.in ()))
        {
          ::DDS::DataReaderQos drqos;
          this->__info_get_datareader_ =
              this->__listen_subscriber_->create_datareader (this->topic_.in (),
                                                           drqos,
                                                           this->__listen_datareaderlistener.in (),
                                                           DDS_DATA_AVAILABLE_STATUS);
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
  CIAO_TRACE ("get_info_in_data");

  return new CIAO::DDS4CCM::RTI::Writer_T<DDS_TYPE, CCM_TYPE>
          (this->__info_in_datawriter_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_supplier_dds_entity (void)
{
  CIAO_TRACE ("get_info_in_dds_entity");

  return this->__info_in_datawriter_.in ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::getter_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_fresh_data (void)
{
  CIAO_TRACE ("get_info_get_out_data");

  return new CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE, CCM_TYPE> (
          this->__info_get_datareader_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data (void)
{
  CIAO_TRACE ("get_push_consumer_data");

  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> (
          this->__listen_datareader_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_data (void)
{
  CIAO_TRACE ("get_pull_consumer_data");

  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE, CCM_TYPE> (
          this->__listen_datareader_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_DataListenerControl_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_data_control (void)
{
  CIAO_TRACE ("get_push_consumer_data_control");
  return new CCM_DDS_DataListenerControl_i (
          this->__listen_datalistener_mode_,
          this->__listen_datalistener_max_delivered_data_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_pull_consumer_dds_entity (void)
{
  CIAO_TRACE ("get_listen_dds_entity");
  return ::DDS::CCM_DataReader::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::get_push_consumer_dds_entity (void)
{
  CIAO_TRACE ("get_listen_dds_entity");
  return ::DDS::CCM_DataReader::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::set_session_context (
  ::Components::SessionContext_ptr ctx)
{
  CIAO_TRACE ("set_session_context");
  typename CCM_TYPE::context_type::_var_type lctx =
    CCM_TYPE::context_type::_narrow (ctx);

  if (::CORBA::is_nil (lctx.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }

  this->context_ = lctx;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_Event_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  this->configure_port_listen ();
  this->configure_port_dds_write ();
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
}
