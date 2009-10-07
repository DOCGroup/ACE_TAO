// -*- C++ -*-
// $Id$

#include "Quoter_Connector_exec.h"
#include "ciao/CIAO_common.h"

#include "dds4ccm/impl/ndds/NDDS_Traits.h"
#include "dds4ccm/impl/ndds/DomainParticipantFactory.h"
#include "dds4ccm/impl/ndds/DomainParticipant.h"
#include "dds4ccm/impl/ndds/DataReaderListener_T.h"

// Should be removed after we refactor stuff back into ndds impl.
#include "dds4ccm/impl/ndds/DataReader.h"
#include "dds4ccm/impl/ndds/ListenerControl.h"

namespace CIAO_Quoter_Quoter_Connector_Impl
{
  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::Connector_T (void)
    : default_domain_configured_ (false),
      domain_id_ (0),
      default_topic_configured_ (false),
      topic_name_ ("Quoter_Topic"),
      __info_in_configured_ (false),
      __info_out_configured_ (false),
      __info_out_rawlistener_enabled_ (false)
  {
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::~Connector_T (void)
  {
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  char *
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::topic_name (void)
  {
    // @from DDS_TopicBase
    return CORBA::string_dup (this->topic_name_.in ());
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::topic_name (
    const char * topic_name)
  {
    // @from DDS_TopicBase
    this->topic_name_ = topic_name;
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  ::DDS::StringSeq *
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::key_fields (void)
  {
    // @from DDS_TopicBase
    ::DDS::StringSeq *retval =
      new ::DDS::StringSeq (this->key_fields_.length ());

    for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
      (*retval)[i] = CORBA::string_dup (this->key_fields_[i]);

    return retval;
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::key_fields (
    const ::DDS::StringSeq & key_fields)
  {
    // @from DDS_TopicBase
    this->key_fields_.length (key_fields.length ());

    for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
      this->key_fields_[i] = CORBA::string_dup (key_fields[i]);
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  ::DDS::DomainId_t
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::domain_id (void)
  {
    // @from DDS_Base
    return this->domain_id_;
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::domain_id (
    ::DDS::DomainId_t domain_id)
  {
    // @from DDS_Base
    this->domain_id_ = domain_id;
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  char *
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::qos_profile (void)
  {
    // @from DDS_Base
    return CORBA::string_dup (this->qos_profile_.in ());
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::qos_profile (
    const char * qos_profile)
  {
    // @from DDS_Base
    this->qos_profile_ = qos_profile;
  }

  // Port operations.

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::configure_default_domain_ (void)
  {
    CIAO_DEBUG ((LM_TRACE, CLINFO "Quoter_Connector_exec_i::configure_default_domain_ - "
                 "Configuring default domain\n"));

    if (this->default_domain_configured_) return;

    try
      {
        NDDSConfigLogger::get_instance()->set_verbosity_by_category(NDDS_CONFIG_LOG_CATEGORY_API,
                                                                   NDDS_CONFIG_LOG_VERBOSITY_STATUS_ALL );

        // Generic code
        this->domain_factory_ = new ::CIAO::DDS4CCM::RTI::RTI_DomainParticipantFactory_i ();

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

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::configure_default_topic_ (void)
  {
    CIAO_DEBUG ((LM_TRACE, CLINFO "Quoter_Connector_exec_i::configure_default_topic_ - "
                 "Configuring default topic\n"));
    if (this->default_topic_configured_) return;

    this->configure_default_domain_ ();

    try
      {
        if (CORBA::is_nil (this->topic_))
          {
            CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i *part = dynamic_cast< CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i * > (this->domain_.in ());
            DDS_ReturnCode_t retcode = NDDS_TYPE::type_support::register_type(
              part->get_participant (), NDDS_TYPE::type_support::get_type_name ());
            if (retcode == DDS_RETCODE_OK)
              {
                ::DDS::TopicQos tqos;
                this->topic_ =
                  this->domain_->create_topic (this->topic_name_.in (),
                                               NDDS_TYPE::type_support::get_type_name (),
                                               tqos,
                                               0,
                                               0);
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

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::configure_port_info_in_ (void)
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
            ::DDS::DataWriterQos dwqos;
            ::DDS::DataWriter_var dwv_tmp = this->__info_in_publisher_->create_datawriter (this->topic_.in (),
                                                                                           dwqos,
                                                                                           0,
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

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::configure_port_info_out_ (void)
  {
    if (this->__info_out_configured_)
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
            this->__info_out_portstatus_ = this->context_->get_connection_info_out_status ();

            this->__info_out_datareaderlistener = new ::CIAO::DDS4CCM::RTI::DataReaderListener_T<NDDS_TYPE, CONNECTOR_TYPE::rawlistener_type, ::CCM_DDS::PortStatusListener> (
              this->context_->get_connection_info_out_listener (),
              this->context_->get_connection_info_out_status (),
              this->__info_out_rawlistener_enabled_);
            ::DDS::DataReaderQos drqos;
            this->__info_out_datareader_ =
              this->__info_out_subscriber_->create_datareader (this->topic_.in (),
                                                               drqos,
                                                               this->__info_out_datareaderlistener.in (),
                                                               DDS_DATA_AVAILABLE_STATUS);
          }

      }
    catch (...)
      {
        CIAO_ERROR ((LM_EMERGENCY, "Caught unknown c++ exception while creating subscriber entities\n"));
        throw CORBA::INTERNAL ();
      }
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  typename CONNECTOR_TYPE::writer_type::_ptr_type
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::get_info_in_data (void)
  {
    std::cerr << "get_info_in_data" << std::endl;

    this->configure_port_info_in_ ();

    return new CIAO::DDS4CCM::RTI::Writer_T<NDDS_TYPE,
      CONNECTOR_TYPE::writer_type> (this->__info_in_datawriter_.in ());
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  ::DDS::CCM_DataWriter_ptr
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::get_info_in_dds_entity (void)
  {
    this->configure_port_info_in_ ();

    return this->__info_in_datawriter_.in ();
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  typename CONNECTOR_TYPE::reader_type::_ptr_type
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::get_info_out_data (void)
  {
    std::cerr << "get_info_out_data" << std::endl;

    //this->configure_port_info_in_ ();

    return new CIAO::DDS4CCM::RTI::Reader_T<NDDS_TYPE,
      CONNECTOR_TYPE::reader_type> (this->__info_out_datareader_.in ());
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  ::CCM_DDS::CCM_ListenerControl_ptr
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::get_info_out_control (void)
  {
    return new CCM_DDS_ListenerControl_i (this->__info_out_rawlistener_enabled_);
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  ::DDS::CCM_DataReader_ptr
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::get_info_out_dds_entity (void)
  {
    return ::DDS::CCM_DataReader::_nil ();
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::set_session_context (
    ::Components::SessionContext_ptr ctx)
  {
    CONNECTOR_TYPE::context_type::_var_type lctx =
      CONNECTOR_TYPE::context_type::_narrow (ctx);

    if ( ::CORBA::is_nil (lctx.in ()))
      {
        throw ::CORBA::INTERNAL ();
      }

    this->context_ = lctx;
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::configuration_complete (void)
  {
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::ccm_activate (void)
  {
    if (!CORBA::is_nil (this->context_->get_connection_info_out_listener ()) ||
        !CORBA::is_nil (this->context_->get_connection_info_out_status ()))
      {
        this->configure_port_info_out_ ();
      }
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::ccm_passivate (void)
  {
  }

  template <typename NDDS_TYPE, typename CONNECTOR_TYPE>
  void
  Connector_T<NDDS_TYPE, CONNECTOR_TYPE>::ccm_remove (void)
  {
  }

  Quoter_Connector_exec_i::Quoter_Connector_exec_i (void)
  {
  }
  
  Quoter_Connector_exec_i::~Quoter_Connector_exec_i (void) 
  {
  }

  extern "C" QUOTER_CONNECTOR_EXEC_Export ::Components::EnterpriseComponent_ptr
  create_Quoter_Quoter_Connector_Impl (void)
  {
    ::Components::EnterpriseComponent_ptr retval =
      ::Components::EnterpriseComponent::_nil ();

    ACE_NEW_NORETURN (
      retval,
      Quoter_Connector_exec_i);

    return retval;
  }
}

