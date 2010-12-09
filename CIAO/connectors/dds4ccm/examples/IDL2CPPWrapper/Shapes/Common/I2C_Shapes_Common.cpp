// $Id$

#include "I2C_Shapes_Common.h"

#include "ace/Env_Value_T.h"

#include "connectors/dds4ccm/impl/ndds/DomainParticipant.h"
#include "connectors/dds4ccm/impl/logger/Log_Macros.h"

#define TOPIC_NAME "Square"


I2C_Shapes_Common::I2C_Shapes_Common (void)
  : factory_ (0)
{
  ACE_Env_Value<int> trace (ACE_TEXT("DDS4CCM_TRACE_ENABLE"), 0);
  if (trace)
    {
      DDS4CCM_ENABLE_TRACE ();
    }
  else
    {
      DDS4CCM_DISABLE_TRACE ();
    }

}

I2C_Shapes_Common::~I2C_Shapes_Common (void)
{
  if (this->factory_)
    {
      delete this->factory_;
    }
}

int
I2C_Shapes_Common::get_domain_id (void)
{
  int domain_id = 0;
  ACE_Env_Value<int> id (ACE_TEXT("DDS4CCM_DEFAULT_DOMAIN_ID"), domain_id);
  return id;
}

void
I2C_Shapes_Common::initialize (void)
{
  ::DDS::DomainParticipantQos dp_qos;
  this->participant_ = this->participant_factory_.create_participant (
                                  this->get_domain_id (),
                                  dp_qos,
                                  ::DDS::DomainParticipantListener::_nil (),
                                  0);

  ::CIAO::NDDS::DDS_DomainParticipant_i *part =
    dynamic_cast< CIAO::NDDS::DDS_DomainParticipant_i * > (this->participant_.in ());
  if (!part)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          "I2C_Shapes_Common::initialize - Unable to cast the "
          "DomainParticipant proxy to its internal representation.\n"));
      throw ::CORBA::INTERNAL ();
    }

  ACE_NEW_THROW_EX (this->factory_,
                    dds_type_factory (),
                    ::CORBA::NO_MEMORY ());

  const char* typesupport_name = ShapeType_DDS_Traits::type_support::get_type_name ();

  ::CIAO::NDDS::DDS_TypeSupport_i::register_type (typesupport_name,
                                                  this->factory_,
                                                  this->participant_);

  ::DDS::ReturnCode_t retcode = ShapeType_DDS_Traits::type_support::register_type (
        part->get_rti_entity (),
        typesupport_name);
  if (retcode != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          "I2C_Shapes_Common::initialize - Unable to register type\n"));
      throw ::CORBA::INTERNAL ();
    }
  ::DDS::TopicQos tqos;
  this->topic_ = this->participant_->create_topic (TOPIC_NAME,
                                  typesupport_name,
                                  tqos,
                                  ::DDS::TopicListener::_nil (),
                                  0);
  if (::CORBA::is_nil (this->topic_.in ()))
    {
      ACE_ERROR ((LM_ERROR, "I2C_Shapes_Common::initialize - "
                "Topic seems to be nil\n"));
      throw ::CORBA::INTERNAL ();
    }
}

::DDS::Topic_ptr
I2C_Shapes_Common::get_topic (void)
{
  return ::DDS::Topic::_duplicate (this->topic_.in ());
}

::DDS::Publisher_ptr
I2C_Shapes_Common::create_publisher (void)
{
  if (::CORBA::is_nil (participant_.in ()))
    {
      initialize();
    }
  ::DDS::Publisher_var publisher;
      ::DDS::PublisherQos pqos;
  publisher = participant_->create_publisher (pqos,
                                  ::DDS::PublisherListener::_nil (),
                                  0);
  return publisher._retn ();
}

::DDS::Subscriber_ptr
I2C_Shapes_Common::create_subscriber (void)
{
  if (::CORBA::is_nil (participant_.in ()))
    {
      initialize();
    }
  ::DDS::Subscriber_var subscriber;
      ::DDS::SubscriberQos sqos;
  subscriber = participant_->create_subscriber (sqos,
                                  ::DDS::SubscriberListener::_nil (),
                                  0);
  return subscriber._retn ();
}
