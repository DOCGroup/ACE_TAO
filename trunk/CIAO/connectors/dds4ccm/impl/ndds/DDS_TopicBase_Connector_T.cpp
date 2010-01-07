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
  CIAO_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete");
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete ();
  this->init_default_topic ();
  this->init_subscriber ();
  this->init_publisher ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate");
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate ();
  this->activate_default_topic ();
  this->activate_subscriber ();
  this->activate_publisher ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate");
  this->passivate_default_topic ();
  this->passivate_subscriber ();
  this->passivate_publisher ();
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove");
  this->remove_default_topic ();
  this->remove_subscriber ();
  this->remove_publisher ();
  DDS_Base_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove ();
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
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::init_default_topic (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::init_default_topic");

  if (CORBA::is_nil (this->topic_))
    {
      try
        {
          CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i *part =
            dynamic_cast< CIAO::DDS4CCM::RTI::RTI_DomainParticipant_i * > (
              this->domain_participant_.in ());
          DDS_ReturnCode_t const retcode = DDS_TYPE::type_support::register_type(
            part->get_impl (), DDS_TYPE::type_support::get_type_name ());

          if (retcode == DDS_RETCODE_OK)
            {
              if (this->library_name_ && this->profile_name_)
                {
                  ::DDS::TopicQos tqos;
                  this->topic_ =
                    this->domain_participant_->create_topic_with_profile (
                      this->topic_name_.in (),
                      DDS_TYPE::type_support::get_type_name (),
                      this->library_name_,
                      this->profile_name_,
                      ::DDS::TopicListener::_nil (),
                      0);
                }
              else
                {
                  ::DDS::TopicQos tqos;
                  this->topic_ =
                    this->domain_participant_->create_topic (
                      this->topic_name_.in (),
                      DDS_TYPE::type_support::get_type_name (),
                      tqos,
                      ::DDS::TopicListener::_nil (),
                      0);
                }
            }
          else
            {
              throw CCM_DDS::InternalError (retcode, 0);
            }
        }
      catch (...)
        {
          CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::init_default_topic: "
                                    "Caught unknown C++ exception.\n"));
          throw CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::init_subscriber (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::init_subscriber");

  if (CORBA::is_nil (this->subscriber_.in ()))
    {
      try
        {
          if (this->library_name_ && this->profile_name_)
            {
              this->subscriber_ = this->domain_participant_->
                create_subscriber_with_profile (
                  this->library_name_,
                  this->profile_name_,
                  ::DDS::SubscriberListener::_nil (),
                  0);
            }
          else
            {
              ::DDS::SubscriberQos sqos;
              this->subscriber_ = this->domain_participant_->
                create_subscriber (
                  sqos,
                  ::DDS::SubscriberListener::_nil (),
                  0);
            }
        }
      catch (...)
        {
          CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::init_subscriber: "
                                    "Caught unknown C++ exception.\n"));
          throw CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::init_publisher (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::init_publisher");

  if (CORBA::is_nil (this->publisher_.in ()))
    {
      if (this->library_name_ && this->profile_name_)
        {
          this->publisher_ = this->domain_participant_->
            create_publisher_with_profile (
              this->library_name_,
              this->profile_name_,
              ::DDS::PublisherListener::_nil (),
              0);
        }
      else
        {
          ::DDS::PublisherQos pqos;
          this->publisher_ = this->domain_participant_->
            create_publisher (
              pqos,
              ::DDS::PublisherListener::_nil (),
              0);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::activate_default_topic (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::activate_default_topic");
  try
    {
      if (CORBA::is_nil (this->topiclistener_.in ()))
        {
          this->topiclistener_ = new ::CIAO::DDS4CCM::TopicListener_T
            <DDS_TYPE, CCM_TYPE> (
                this->context_->get_connection_error_listener ());
        }
          this->topic_->set_listener (
            this->topiclistener_.in (),
            ::CIAO::DDS4CCM::TopicListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::activate_default_topic: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::activate_subscriber (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::activate_subscriber");

  try
    {
      if (CORBA::is_nil (this->subscriber_listener_.in ()))
        {
          this->subscriber_listener_ = new ::CIAO::DDS4CCM::SubscriberListener_T
            <DDS_TYPE, CCM_TYPE> (
              this->context_,
              this->context_->get_connection_error_listener ());
        }
      this->subscriber_->set_listener (
        this->subscriber_listener_.in (),
        ::CIAO::DDS4CCM::SubscriberListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::activate_subscriber: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::activate_publisher (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::activate_publisher");

  try
    {
      if (CORBA::is_nil (this->publisher_listener_.in ()))
        {
          this->publisher_listener_ = new ::CIAO::DDS4CCM::PublisherListener_T
            <DDS_TYPE, CCM_TYPE> (
              this->context_,
              this->context_->get_connection_error_listener ());
        }
      this->publisher_->set_listener (
        this->publisher_listener_.in (),
        ::CIAO::DDS4CCM::PublisherListener_T<DDS_TYPE, CCM_TYPE>::get_mask ());
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::activate_publisher: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::passivate_default_topic (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::passivate_default_topic");

  try
    {
      this->topic_->set_listener (
        ::DDS::TopicListener::_nil (),
        0);

      this->topiclistener_ = ::DDS::TopicListener::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::passivate_default_topic: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::passivate_subscriber (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::passivate_subscriber");

  try
    {
      this->subscriber_->set_listener (
        ::DDS::SubscriberListener::_nil (),
        0);
      this->subscriber_listener_ = ::DDS::SubscriberListener::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::passivate_subscriber: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::passivate_publisher (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::passivate_publisher");

  try
    {
      this->publisher_->set_listener (
        ::DDS::PublisherListener::_nil (),
        0);
      this->publisher_listener_ = ::DDS::PublisherListener::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::passivate_default_topic: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::remove_default_topic (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::remove_default_topic");

  try
    {
      this->domain_participant_->delete_topic (this->topic_.in ());
      this->topic_ = ::DDS::Topic::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::remove_default_topic: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::remove_subscriber (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::remove_subscriber");

  try
    {
      this->domain_participant_->delete_subscriber (this->subscriber_.in ());
      this->subscriber_ = ::DDS::Subscriber::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::remove_subscriber: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::remove_publisher (void)
{
  CIAO_TRACE ("DDS_TopicBase_Connector_T::remove_publisher");

  try
    {
      this->domain_participant_->delete_publisher (this->publisher_.in ());
      this->publisher_ = ::DDS::Publisher::_nil ();
    }
  catch (...)
    {
      CIAO_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::remove_publisher: "
                                "Caught unknown C++ exception.\n"));
      throw CORBA::INTERNAL ();
    }
}
