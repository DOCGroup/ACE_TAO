// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::DDS_TopicBase_Connector_T (void) :
    BaseConnector ()
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::~DDS_TopicBase_Connector_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::configuration_complete");
  BaseConnector::configuration_complete ();
  this->init_default_topic ();
  this->init_subscriber ();
  this->init_publisher ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_activate (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_activate");
  BaseConnector::ccm_activate ();
  this->activate_default_topic (reactor);
  this->activate_subscriber (reactor);
  this->activate_publisher (reactor);
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_passivate");
  this->passivate_default_topic ();
  this->passivate_subscriber ();
  this->passivate_publisher ();
  BaseConnector::ccm_passivate ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::ccm_remove");
  this->remove_default_topic ();
  this->remove_subscriber ();
  this->remove_publisher ();
  BaseConnector::ccm_remove ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
char *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::topic_name (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::topic_name");

  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::topic_name (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::topic_name");
  if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      this->topic_name_ = topic_name;
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::key_fields (
  const ::DDS::StringSeq & key_fields)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::key_fields");
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

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
::DDS::StringSeq *
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::key_fields (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::key_fields");
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
       ACE_OS::printf ("Name: %s\n", name);
     }
    }
 */
  ::DDS::StringSeq_var retval = 0;
  ACE_NEW_THROW_EX (retval,
                    ::DDS::StringSeq (this->key_fields_.length ()),
                    ::CORBA::NO_MEMORY ());
  retval->length (this->key_fields_.length ());

  for (CORBA::ULong i = 0; i < this->key_fields_.length (); ++i)
    {
      (*retval)[i] = CORBA::string_dup (this->key_fields_[i]);
    }
  return retval._retn ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::init_default_topic (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::init_default_topic");

  if (::CORBA::is_nil (this->topic_.in ()))
    {
      ::CIAO::DDS4CCM::CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> *part =
        dynamic_cast< CIAO::DDS4CCM::CCM_DDS_DomainParticipant_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE> * > (
          this->domain_participant_.in ());
      if (!part)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
              "DDS_TopicBase_Connector_T::init_default_topic - "
              "Unable to cast the DomainParticipant proxy to its internal "
              "representation.\n"));
          throw ::CORBA::INTERNAL ();
        }

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
          if (::CORBA::is_nil (this->topic_.in ()))
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TopicBase_Connector_T::init_default_topic - "
                            "Error: Proxy returned a nil topic\n"));
              throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
            }
        }
      else
        {
          throw ::CCM_DDS::InternalError (retcode, 0);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::init_subscriber (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::init_subscriber");

  if (::CORBA::is_nil (this->subscriber_.in ()))
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
      if (::CORBA::is_nil (this->subscriber_.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TopicBase_Connector_T::init_subscriber - "
                        "Error: Proxy returned a nil subscriber.\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::init_publisher (void)
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
      if (::CORBA::is_nil (this->publisher_.in ()))
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TopicBase_Connector_T::init_publisher - "
                        "Error: Proxy returned a nil publisher.\n"));
          throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::activate_default_topic (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_default_topic");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
    TopicListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->topiclistener_.in ()))
        {
          ACE_NEW_THROW_EX (this->topiclistener_,
                            TopicListener (
                              error_listener.in (),
                              reactor),
                            ::CORBA::NO_MEMORY ());
        }

      ::DDS::ReturnCode_t const retcode = this->topic_->set_listener (
                                  this->topiclistener_.in (), mask);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TopicBase_Connector_T::activate_default_topic - "
                        "Error while setting the listener on the topic - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::activate_subscriber (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_subscriber");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
    SubscriberListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->subscriber_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->subscriber_listener_,
                            SubscriberListener (
                              error_listener.in (),
                              reactor),
                            ::CORBA::NO_MEMORY ());
        }

      ::DDS::ReturnCode_t const retcode =
        this->subscriber_->set_listener (this->subscriber_listener_.in (), mask);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TopicBase_Connector_T::activate_subscriber - "
                        "Error while setting the listener on the subscriber - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::activate_publisher (
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_publisher");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
    PublisherListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->publisher_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->publisher_listener_,
                            PublisherListener (
                              error_listener.in (),
                              reactor),
                            ::CORBA::NO_MEMORY ());
        }

      ::DDS::ReturnCode_t const retcode = this->publisher_->set_listener (
                                this->publisher_listener_.in (), mask);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TopicBase_Connector_T::activate_publisher - "
                        "Error while setting the listener on the publisher - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::passivate_default_topic (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::passivate_default_topic");

  if (!::CORBA::is_nil (this->topiclistener_.in ()))
    {
      ::DDS::ReturnCode_t const retcode =
        this->topic_->set_listener (::DDS::TopicListener::_nil (), 0);
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TopicBase_Connector_T::passivate_default_topic - "
                        "Error while setting the listener on the topic - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }

      this->topiclistener_ = ::DDS::TopicListener::_nil ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::passivate_subscriber (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::passivate_subscriber");

  if (!::CORBA::is_nil (this->subscriber_listener_.in ()))
    {
      ::DDS::ReturnCode_t const retcode = this->subscriber_->set_listener (
                                                  ::DDS::SubscriberListener::_nil (),
                                                  0);
      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TopicBase_Connector_T::passivate_subscriber - "
                        "Error while setting the listener on the subscriber - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }

      this->subscriber_listener_ = ::DDS::SubscriberListener::_nil ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::passivate_publisher (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::passivate_publisher");

  if (!::CORBA::is_nil (this->publisher_listener_.in ()))
    {
      ::DDS::ReturnCode_t const retcode =
        this->publisher_->set_listener (::DDS::PublisherListener::_nil (), 0);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_TopicBase_Connector_T::passivate_publisher - "
                        "Error while setting the listener on the publisher - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }

      this->publisher_listener_ = ::DDS::PublisherListener::_nil ();
    }
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::remove_default_topic (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_default_topic");

  ::DDS::ReturnCode_t retcode =
    this->domain_participant_->delete_topic (this->topic_.in ());
  if (retcode != ::DDS::RETCODE_OK)
    {
      throw ::CCM_DDS::InternalError (retcode, 0);
    }
  this->topic_ = ::DDS::Topic::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::remove_subscriber (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_subscriber");

  this->domain_participant_->delete_subscriber (this->subscriber_.in ());
  this->subscriber_ = ::DDS::Subscriber::_nil ();
}

template <typename DDS_TYPE, typename CCM_TYPE, DDS4CCM_Vendor VENDOR_TYPE>
void
DDS_TopicBase_Connector_T<DDS_TYPE, CCM_TYPE, VENDOR_TYPE>::remove_publisher (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_publisher");

  this->domain_participant_->delete_publisher (this->publisher_.in ());
  this->publisher_ = ::DDS::Publisher::_nil ();
}
