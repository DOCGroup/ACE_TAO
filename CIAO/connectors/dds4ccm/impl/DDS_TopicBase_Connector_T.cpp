// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::DDS_TopicBase_Connector_T (void) :
    BaseConnector ()
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
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::configuration_complete");
  BaseConnector::configuration_complete ();
  this->init_default_topic ();
  this->init_subscriber ();
  this->init_publisher ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_activate");
  BaseConnector::ccm_activate ();
  this->activate_default_topic (reactor);
  this->activate_subscriber (reactor);
  this->activate_publisher (reactor);
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_passivate");
  this->passivate_default_topic ();
  this->passivate_subscriber ();
  this->passivate_publisher ();
  BaseConnector::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::ccm_remove");
  this->remove_default_topic ();
  this->remove_subscriber ();
  this->remove_publisher ();
  BaseConnector::ccm_remove ();
}

template <typename DDS_TYPE, typename CCM_TYPE>
char *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name");
  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::topic_name");
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
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields");
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
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::key_fields");
/*
    For future use, DDS_TYPE doesn't have get_typecode yet
    ::DDS_TypeCode* ptr = ::DDS_TYPE::type_support::get_typecode ();
    DDS_ExceptionCode_t ex;
    DDS_UnsignedLong const number = ptr->member_count (ex);
    for (DDS_UnsignedLong i = 0; i < number; i++)
    {
     if (ptr->is_member_key (i, ex))
     {
       const char* name = ptr->member_name (i, ex);
       printf ("Name: %s\n", name);
     }
    }
 */
  ::DDS::StringSeq_var retval = 0;
  ACE_NEW_THROW_EX (retval,
                    ::DDS::StringSeq (this->key_fields_.length ()),
                    CORBA::NO_MEMORY ());
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
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::init_default_topic");

  if (::CORBA::is_nil (this->topic_.in ()))
    {
      try
        {
          ::CIAO::DDS4CCM::CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE> *part =
            dynamic_cast< CIAO::DDS4CCM::CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE> * > (
              this->domain_participant_.in ());
          DDS_ReturnCode_t const retcode = DDS_TYPE::type_support::register_type(
            part->get_impl (), DDS_TYPE::type_support::get_type_name ());

          if (retcode == DDS_RETCODE_OK)
            {
              if (this->library_name_ && this->profile_name_)
                {
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
          DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::init_default_topic: "
                                    "Caught unexpected exception.\n"));
          throw CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::init_subscriber (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::init_subscriber");

  if (::CORBA::is_nil (this->subscriber_.in ()))
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
          DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::init_subscriber: "
                                    "Caught unexpected exception.\n"));
          throw CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::init_publisher (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::init_publisher");

  if (::CORBA::is_nil (this->publisher_.in ()))
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
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::activate_default_topic (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_default_topic");
  try
    {
      if (::CORBA::is_nil (this->topiclistener_.in ()))
        {
          ACE_NEW_THROW_EX (this->topiclistener_,
                            TopicListener (
                              this->context_->get_connection_error_listener (),
                              reactor),
                            CORBA::NO_MEMORY ());
        }
      this->topic_->set_listener (
        this->topiclistener_.in (),
        TopicListener::get_mask (
          this->context_->get_connection_error_listener ()));
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::activate_default_topic: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::activate_subscriber (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_subscriber");

  try
    {
      if (::CORBA::is_nil (this->subscriber_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->subscriber_listener_,
                            SubscriberListener (
                              this->context_->get_connection_error_listener (),
                              reactor),
                            CORBA::NO_MEMORY ());
        }
      this->subscriber_->set_listener (
        this->subscriber_listener_.in (),
        SubscriberListener::get_mask (
          this->context_->get_connection_error_listener ()));
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::activate_subscriber: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::activate_publisher (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_publisher");

  try
    {
      if (::CORBA::is_nil (this->publisher_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->publisher_listener_,
                            PublisherListener (
                              this->context_->get_connection_error_listener (),
                              reactor),
                            CORBA::NO_MEMORY ());
        }
      this->publisher_->set_listener (
        this->publisher_listener_.in (),
        PublisherListener::get_mask (
          this->context_->get_connection_error_listener ()));
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::activate_publisher: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::passivate_default_topic (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::passivate_default_topic");

  try
    {
      this->topic_->set_listener (
        ::DDS::TopicListener::_nil (),
        0);

      this->topiclistener_ = ::DDS::TopicListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::passivate_default_topic: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::passivate_subscriber (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::passivate_subscriber");

  try
    {
      this->subscriber_->set_listener (
        ::DDS::SubscriberListener::_nil (),
        0);
      this->subscriber_listener_ = ::DDS::SubscriberListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::passivate_subscriber: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::passivate_publisher (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::passivate_publisher");

  try
    {
      this->publisher_->set_listener (
        ::DDS::PublisherListener::_nil (),
        0);
      this->publisher_listener_ = ::DDS::PublisherListener::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::passivate_default_topic: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::remove_default_topic (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_default_topic");

  try
    {
      DDS::ReturnCode_t retval =
        this->domain_participant_->delete_topic (this->topic_.in ());
      if (retval != DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (1, (LM_ERROR, CLINFO
            "DDS_TopicBase_Connector_T::remove - "
            "Unable to delete Topic: <%C>\n",
            ::CIAO::DDS4CCM::translate_retcode (retval)));
          throw CORBA::INTERNAL ();
        }
      this->topic_ = ::DDS::Topic::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::remove_default_topic: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::remove_subscriber (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_subscriber");

  try
    {
      this->domain_participant_->delete_subscriber (this->subscriber_.in ());
      this->subscriber_ = ::DDS::Subscriber::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::remove_subscriber: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE>::remove_publisher (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_publisher");

  try
    {
      this->domain_participant_->delete_publisher (this->publisher_.in ());
      this->publisher_ = ::DDS::Publisher::_nil ();
    }
  catch (...)
    {
      DDS4CCM_ERROR (1, (LM_ERROR, "DDS_TopicBase_Connector_T::remove_publisher: "
                                "Caught unexpected exception.\n"));
      throw CORBA::INTERNAL ();
    }
}
