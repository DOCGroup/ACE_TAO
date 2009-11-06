// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/DataReaderListener_T.h"
#include "dds4ccm/impl/ndds/DataWriterListener_T.h"
#include "dds4ccm/impl/ndds/Writer_T.h"
#include "dds4ccm/impl/ndds/Updater_T.h"
#include "dds4ccm/impl/ndds/Getter_T.h"
#include "dds4ccm/impl/ndds/Reader_T.h"
#include "dds4ccm/impl/ndds/ListenerControl.h"

#include "ciao/Logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
Connector_T<DDS_TYPE, CCM_TYPE>::Connector_T (const char * topic_name)
  : default_domain_configured_ (false),
    domain_id_ (0),
    default_topic_configured_ (false),
    topic_name_ (topic_name),
    __info_in_configured_ (false),
    __info_out_configured_ (false),
    __info_out_rawlistener_enabled_ (false)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
Connector_T<DDS_TYPE, CCM_TYPE>::~Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (void)
{
  // @from DDS_TopicBase
  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (
  const char * topic_name)
{
  // @from DDS_TopicBase
  this->topic_name_ = topic_name;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StringSeq *
Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (void)
{
  // @from DDS_TopicBase
  ::DDS::StringSeq *retval =
    new ::DDS::StringSeq (this->key_fields_.length ());

  for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
    (*retval)[i] = CORBA::string_dup (this->key_fields_[i]);

  return retval;
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::DomainId_t
Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (void)
{
  // @from DDS_Base
  return this->domain_id_;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::domain_id (
  ::DDS::DomainId_t domain_id)
{
  // @from DDS_Base
  this->domain_id_ = domain_id;
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (void)
{
  // @from DDS_Base
  return CORBA::string_dup (this->qos_profile_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::qos_profile (
  const char * qos_profile)
{
  // @from DDS_Base
  this->qos_profile_ = qos_profile;
}

// Port operations.

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::configure_default_domain_ (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "Connector_T::configure_default_domain_ - "
                "Configuring default domain\n"));

  if (this->default_domain_configured_)
    return;

  try
    {
      //NDDSConfigLogger::get_instance()->set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API,
      //                                                           NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL );

      // Generic code
      this->domain_factory_ = new ::CIAO::DDS4CCM::RTI::RTI_DomainParticipantFactory_i ();

      ::DDS::DomainParticipantQos qos;
      this->domain_ =
        this->domain_factory_->create_participant (this->domain_id_,
                                                    qos,
                                                    0,
                                                    0);
      default_domain_configured_ = true;
    }
  catch (...)
    {
      CIAO_ERROR ((LM_ERROR, "Caught unknown C++ exception whilst configuring default domain\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::configure_default_topic_ (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "Connector_T::configure_default_topic_ - "
                "Configuring default topic\n"));

  if (this->default_topic_configured_)
    return;

  this->configure_default_domain_ ();

  try
    {
      if (CORBA::is_nil (this->topic_))
        {
          CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i *part = dynamic_cast< CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i * > (this->domain_.in ());
          DDS_ReturnCode_t retcode = DDS_TYPE::type_support::register_type(
                part->get_participant (), DDS_TYPE::type_support::get_type_name ());
          if (retcode == DDS_RETCODE_OK)
            {
              ::DDS::TopicQos tqos;
              this->topic_ =
                this->domain_->create_topic (this->topic_name_.in (),
                                             DDS_TYPE::type_support::get_type_name (),
                                             tqos,
                                             0,
                                             0);
              this->default_topic_configured_ = true;
            }
          else
            {
              throw CORBA::INTERNAL ();
            }
        }
    }
  catch (...)
    {
      CIAO_ERROR ((LM_ERROR, "Caught unknown error while configuring default topic\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::configure_port_info_in_ (void)
{
  if (this->__info_in_configured_)
    return;

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
          this->__info_out_datawriterlistener = new ::CIAO::DDS4CCM::DataWriterListener_T
            <DDS_TYPE, CCM_TYPE> (
                  this->context_,
                  this->__info_out_rawlistener_enabled_);

          ::DDS::DataWriterQos dwqos;
          //test mh
     //     dwqos.history.depth=1;
      //    dwqos.resource_limits.max_instances   = 5; // >= Initial Instances
      //    dwqos.resource_limits.max_samples_per_instance = 4; //>= Depth,  <=Max Samples
      //    dwqos.deadline.period.nanosec = 0;
     //     dwqos.deadline.period.sec = 1;
          ::DDS::DataWriter_var dwv_tmp = this->__info_in_publisher_->create_datawriter (this->topic_.in (),
                                                                                          dwqos,
                                                                                          this->__info_out_datawriterlistener.in (),
                                                                                          0);
          this->__info_in_datawriter_ = ::DDS::CCM_DataWriter::_narrow (dwv_tmp);
          __info_in_configured_ = true;
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
Connector_T<DDS_TYPE, CCM_TYPE>::configure_port_info_out_ (bool create_getter)
{
  if (this->__info_out_configured_ && this->__info_get_configured_ )
    return;

  this->configure_default_topic_ ();

  try
    {
      if (CORBA::is_nil (this->__info_out_subscriber_.in ()))
        {
          ::DDS::SubscriberQos sqos;
          this->__info_out_subscriber_ = this->domain_->create_subscriber (sqos,
                                                                            0,
                                                                            0);
        }

      if (CORBA::is_nil (this->__info_out_datareader_.in ()))
        {
          this->__info_out_datareaderlistener = new ::CIAO::DDS4CCM::RTI::DataReaderListener_T
            <DDS_TYPE, CCM_TYPE> (
                  this->context_,
                  this->__info_out_rawlistener_enabled_);
          ::DDS::DataReaderQos drqos;
          if (create_getter)
            {
              this->__info_get_datareader_ =
                  this->__info_out_subscriber_->create_datareader (this->topic_.in (),
                                                               drqos,
                                                               this->__info_out_datareaderlistener.in (),
                                                               DDS_DATA_AVAILABLE_STATUS);
              this->__info_get_configured_ = true;
            }
          else
            {
               this->__info_out_datareader_ =
                  this->__info_out_subscriber_->create_datareader (this->topic_.in (),
                                                               drqos,
                                                               this->__info_out_datareaderlistener.in (),
                                                               DDS_DATA_AVAILABLE_STATUS);
              this->__info_out_configured_ = true;
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
typename CCM_TYPE::writer_type::_ptr_type
Connector_T<DDS_TYPE, CCM_TYPE>::get_info_in_data (void)
{
  CIAO_TRACE ("get_info_in_data");

  this->configure_port_info_in_ ();

  return new CIAO::DDS4CCM::RTI::Writer_T<DDS_TYPE,
    typename CCM_TYPE::writer_type> (this->__info_in_datawriter_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataWriter_ptr
Connector_T<DDS_TYPE, CCM_TYPE>::get_info_in_dds_entity (void)
{
  CIAO_TRACE ("get_info_in_dds_entity");

  this->configure_port_info_in_ ();

  return this->__info_in_datawriter_.in ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::updater_type::_ptr_type
Connector_T<DDS_TYPE, CCM_TYPE>::get_info_update_data (void)
{
  CIAO_TRACE ("get_info_update_data");

  this->configure_port_info_in_ ();
  return new CIAO::DDS4CCM::RTI::Updater_T<DDS_TYPE,
  typename CCM_TYPE::updater_type> (this->__info_in_datawriter_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::getter_type::_ptr_type
Connector_T<DDS_TYPE, CCM_TYPE>::get_info_get_out_data (void)
{
  CIAO_TRACE ("get_info_get_out_data");

  this->configure_port_info_out_ (true);

  return new CIAO::DDS4CCM::RTI::Getter_T<DDS_TYPE,
  typename CCM_TYPE::getter_type> (this->__info_get_datareader_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
typename CCM_TYPE::reader_type::_ptr_type
Connector_T<DDS_TYPE, CCM_TYPE>::get_info_out_data (void)
{
  CIAO_TRACE ("get_info_out_data");

  this->configure_port_info_out_ (false);

  return new CIAO::DDS4CCM::RTI::Reader_T<DDS_TYPE,
  typename CCM_TYPE::reader_type> (this->__info_out_datareader_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
::CCM_DDS::CCM_ListenerControl_ptr
Connector_T<DDS_TYPE, CCM_TYPE>::get_info_out_control (void)
{
  CIAO_TRACE ("get_info_out_control");
  return new CCM_DDS_ListenerControl_i (this->__info_out_rawlistener_enabled_);
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::CCM_DataReader_ptr
Connector_T<DDS_TYPE, CCM_TYPE>::get_info_out_dds_entity (void)
{
  CIAO_TRACE ("get_info_out_dds_entity");
  return ::DDS::CCM_DataReader::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::set_session_context (
  ::Components::SessionContext_ptr ctx)
{
  CIAO_TRACE ("set_session_context");
  typename CCM_TYPE::context_type::_var_type lctx =
    CCM_TYPE::context_type::_narrow (ctx);

  if ( ::CORBA::is_nil (lctx.in ()))
    {
      throw ::CORBA::INTERNAL ();
    }

  this->context_ = lctx;
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  if (!CORBA::is_nil (this->context_->get_connection_info_out_listener ()) ||
     (!CORBA::is_nil (this->context_->get_connection_info_out_status () )))
    {
      this->configure_port_info_out_ (false);
      this->configure_port_info_out_ (true);
      this->configure_port_info_in_ ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
}
