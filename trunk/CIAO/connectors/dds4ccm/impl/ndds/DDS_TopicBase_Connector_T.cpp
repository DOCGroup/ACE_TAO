// -*- C++ -*-
// $Id$

#include "ciao/Logger/Log_Macros.h"
#include "dds4ccm/impl/ndds/TopicListener_T.h"
#include "dds4ccm/impl/ndds/PublisherListener_T.h"
#include "dds4ccm/impl/ndds/SubscriberListener_T.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_TopicBase_Connector_T (void) :
    DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>()
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::~DDS_TopicBase_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete (void)
{
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name");
  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (
  const char * topic_name)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name");
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->topic_name_ = topic_name;
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (
  const ::DDS::StringSeq & key_fields)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields");
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->key_fields_.length (key_fields.length ());
      for (CORBA::ULong i = 0; i < key_fields.length (); ++i)
        {
          this->key_fields_.operator [](i) = CORBA::string_dup (key_fields[i]);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
::DDS::StringSeq *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields");
/*
    For future use, DDS_TYPE doesn't have get_typecode yet
    ::DDS_TypeCode* ptr = ::DDS_TYPE::get_typecode ();
    DDS_ExceptionCode_t ex;
    DDS_UnsignedLong number = ptr->member_count (ex);
    for (DDS_UnsignedLong i = 0; i < number; i++)
    {
     if (ptr->is_member_key (i, ex))
     {
       const char* name = ptr->member_name (i, ex);
       printf ("Name: %s\n", name);
     }
    }
 */
  ::DDS::StringSeq_var retval =
    new ::DDS::StringSeq (this->key_fields_.length ());
  retval->length (this->key_fields_.length ());

  for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
    {
      (*retval)[i] = CORBA::string_dup (this->key_fields_[i]);
    }
  return retval._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configure_subscriber (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "DDS_TopicBase_Connector_T::configure_subscriber - "
                "Configuring subscriber\n"));

  if (CORBA::is_nil (this->subscriber_.in ()))
    {
      this->subscriber_listener_ = new ::CIAO::DDS4CCM::RTI::SubscriberListener_T
        <DDS_TYPE, CCM_TYPE> (
              this->context_->get_connection_error_listener ());

      if (this->library_name_ && this->profile_name_)
        {
          this->subscriber_ = this->domain_participant_->
            create_subscriber_with_profile (
              this->library_name_,
              this->profile_name_,
              this->subscriber_listener_.in (),
              DDS_STATUS_MASK_NONE);
        }
      else
        {
          ::DDS::SubscriberQos sqos;
          this->subscriber_ = this->domain_participant_->
            create_subscriber (
              sqos,
              this->subscriber_listener_.in (),
              DDS_STATUS_MASK_NONE);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configure_publisher (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "DDS_TopicBase_Connector_T::configure_publisher - "
                "Configuring publisher\n"));

  if (CORBA::is_nil (this->publisher_.in ()))
    {
      this->publisher_listener_ = new ::CIAO::DDS4CCM::RTI::PublisherListener_T
        <DDS_TYPE, CCM_TYPE> (
              this->context_->get_connection_error_listener ());

      if (this->library_name_ && this->profile_name_)
        {
          this->publisher_ = this->domain_participant_->
            create_publisher_with_profile (
              this->library_name_,
              this->profile_name_,
              this->publisher_listener_.in (),
              DDS_STATUS_MASK_NONE);
        }
      else
        {
          ::DDS::PublisherQos pqos;
          this->publisher_ = this->domain_participant_->
            create_publisher (
              pqos,
              this->publisher_listener_.in (),
              DDS_STATUS_MASK_NONE);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configure_default_topic (void)
{
  CIAO_DEBUG ((LM_TRACE, CLINFO "DDS_TopicBase_Connector_T::configure_default_topic - "
                "Configuring default topic\n"));

  this->configure_default_domain ();

  if (CORBA::is_nil (this->topic_))
    {
      try
        {
          CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i *part =
            dynamic_cast< CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i * > (
              this->domain_participant_.in ());
          DDS_ReturnCode_t const retcode = DDS_TYPE::type_support::register_type(
                part->get_participant (), DDS_TYPE::type_support::get_type_name ());
          if (retcode == DDS_RETCODE_OK)
            {
              this->topiclistener_ = new ::CIAO::DDS4CCM::TopicListener_T
                <DDS_TYPE, CCM_TYPE> (
                    this->context_->get_connection_error_listener ());

              if (this->library_name_ && this->profile_name_)
                {
                  ::DDS::TopicQos tqos;
                  this->topic_ =
                    this->domain_participant_->create_topic_with_profile (
                      this->topic_name_.in (),
                      DDS_TYPE::type_support::get_type_name (),
                      this->library_name_,
                      this->profile_name_,
                      this->topiclistener_.in (),
                      DDS_INCONSISTENT_TOPIC_STATUS);
                }
              else
                {
                  ::DDS::TopicQos tqos;
                  this->topic_ =
                    this->domain_participant_->create_topic (
                      this->topic_name_.in (),
                      DDS_TYPE::type_support::get_type_name (),
                      tqos,
                      this->topiclistener_.in (),
                      DDS_INCONSISTENT_TOPIC_STATUS);
                }
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


