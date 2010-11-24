// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ace/Reactor.h"

#if (CIAO_DDS4CCM_NDDS==1)
# include "dds4ccm/impl/ndds/TypeSupport.h"
# include "dds4ccm/impl/ndds/DomainParticipant.h"
#endif

template <typename CCM_TYPE, typename DDS_TYPE>
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::DDS_TopicBase_Connector_T (void) :
    BaseConnector ()
  , late_binding_ (false)
{
}

template <typename CCM_TYPE, typename DDS_TYPE>
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::~DDS_TopicBase_Connector_T (void)
{
}

template <typename CCM_TYPE, typename DDS_TYPE>
bool
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binding (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binding");
  return this->late_binding_;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binding (bool late_binding)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binding");
  this->late_binding_ |= late_binding;
}

template <typename CCM_TYPE, typename DDS_TYPE>
bool
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binded (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::late_binded");

  this->late_binding (ACE_OS::strlen (topic_name) == 0);

  if (this->late_binding () &&
      ACE_OS::strlen (topic_name) > 0 &&
      ACE_OS::strlen (this->topic_name_.in ()) == 0)
    {
      DDS_TopicBase_Connector_T::topic_name (topic_name);
      return true;
    }
  else if (this->configuration_complete_)
    {
      throw ::CCM_DDS::NonChangeable ();
    }
  else
    {
      DDS_TopicBase_Connector_T::topic_name (topic_name);
    }
  return false;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::configuration_complete");

  BaseConnector::configuration_complete ();
  const char* typesupport_name = DDS_TYPE::type_support::get_type_name ();

  if (::CORBA::is_nil (this->topic_.in ()))
    {
      this->register_type (typesupport_name);
      this->init_topic (this->topic_.inout () ,
                        this->topic_name_.in (),
                        typesupport_name);
    }
  this->init_subscriber ();
  this->init_publisher ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_activate (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_activate");
  BaseConnector::ccm_activate ();

  this->activate_topic (reactor,
                        this->topic_.in (),
                        this->topiclistener_.inout ());
  this->activate_subscriber (reactor);
  this->activate_publisher (reactor);
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_passivate");
  this->passivate_topic (this->topic_.in (),
                         this->topiclistener_.inout ());
  this->passivate_subscriber ();
  this->passivate_publisher ();
  BaseConnector::ccm_passivate ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::ccm_remove");
  this->remove_topic (this->topic_.inout ());

  const char* typesupport_name = DDS_TYPE::type_support::get_type_name ();
  this->unregister_type (typesupport_name);
  this->remove_subscriber ();
  this->remove_publisher ();
  BaseConnector::ccm_remove ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
char *
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::topic_name (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::topic_name");

  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::topic_name (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::topic_name");

  this->topic_name_ = topic_name;
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::key_fields (
  const ::DDS::StringSeq & key_fields)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::key_fields");
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

template <typename CCM_TYPE, typename DDS_TYPE>
::DDS::StringSeq *
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::key_fields (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::key_fields");
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

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::register_type (
  const char * typesupport_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::register_type");

  ::DDS::ReturnCode_t retcode = ::DDS::RETCODE_OK;
#if (CIAO_DDS4CCM_NDDS==1)
  ::CIAO::NDDS::DDS_DomainParticipant_i *part =
    dynamic_cast< CIAO::NDDS::DDS_DomainParticipant_i * > (
      this->domain_participant_.in ());
  if (!part)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          "DDS_TopicBase_Connector_T::register_type - "
          "Unable to cast the DomainParticipant proxy to its internal "
          "representation.\n"));
      throw ::CORBA::INTERNAL ();
    }

  ::CIAO::NDDS::DDS_TypeFactory_i * factory = new ::CIAO::NDDS::DDS_TypeFactory_T <DDS_TYPE> ();

  ::CIAO::NDDS::DDS_TypeSupport_i::register_type (
      typesupport_name, factory, this->domain_participant_.in ());

  retcode = DDS_TYPE::type_support::register_type(
    part->get_rti_entity (), typesupport_name);
#endif
  if (retcode != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          "DDS_TopicBase_Connector_T::register_type - "
          "Error registering type <%C>\n",
          ::CIAO::DDS4CCM::translate_retcode (retcode)));
      throw ::CCM_DDS::InternalError (retcode, 0);
    }
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::init_topic (
  ::DDS::Topic_ptr & topic,
  const char * topic_name,
  const char * typesupport_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::init_topic");

  ::DDS::Topic_var tp;
  if (this->library_name_ && this->profile_name_)
    {
      tp = this->domain_participant_->create_topic_with_profile (
                                              topic_name,
                                              typesupport_name,
                                              this->library_name_,
                                              this->profile_name_,
                                              ::DDS::TopicListener::_nil (),
                                              0);
    }
  else
    {
      ::DDS::TopicQos tqos;
      tp = this->domain_participant_->create_topic (
                                              topic_name,
                                              typesupport_name,
                                              tqos,
                                              ::DDS::TopicListener::_nil (),
                                              0);
    }
  if (::CORBA::is_nil (tp))
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_DDS_NIL_RETURN, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TopicBase_Connector_T::init_topic - "
                    "Error: Proxy returned a nil topic\n"));
      throw ::CCM_DDS::InternalError (::DDS::RETCODE_ERROR, 0);
    }
  topic = tp._retn ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::init_subscriber (void)
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

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::init_publisher (void)
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

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::activate_topic (
  ACE_Reactor* reactor,
  ::DDS::Topic_ptr topic,
  ::DDS::TopicListener_ptr & listener)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_topic");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
    ::CIAO::DDS4CCM::TopicListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (listener))
        {
          ACE_NEW_THROW_EX (listener,
                            ::CIAO::DDS4CCM::TopicListener (
                              error_listener.in (),
                              reactor),
                            ::CORBA::NO_MEMORY ());

          ::DDS::ReturnCode_t const retcode = topic->set_listener (listener,
                                                                   mask);

          if (retcode != ::DDS::RETCODE_OK)
            {
              DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                            "DDS_TopicBase_Connector_T::activate_topic - "
                            "Error while setting the listener on the topic - <%C>\n",
                            ::CIAO::DDS4CCM::translate_retcode (retcode)));
              throw ::CORBA::INTERNAL ();
            }
        }
    }
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::activate_subscriber (ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_subscriber");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
    ::CIAO::DDS4CCM::SubscriberListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->subscriber_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->subscriber_listener_,
                            ::CIAO::DDS4CCM::SubscriberListener (
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

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::activate_publisher (
  ACE_Reactor* reactor)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::activate_publisher");

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
      ::CIAO::DDS4CCM::PublisherListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (this->publisher_listener_.in ()))
        {
          ACE_NEW_THROW_EX (this->publisher_listener_,
                            ::CIAO::DDS4CCM::PublisherListener (
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

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::passivate_topic (
  ::DDS::Topic_ptr topic,
  ::DDS::TopicListener_ptr & listener)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::passivate_topic");

  ::DDS::ReturnCode_t const retcode =
    topic->set_listener (::DDS::TopicListener::_nil (), 0);
  if (retcode != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                    "DDS_TopicBase_Connector_T::passivate_topic - "
                    "Error while setting the listener on the topic - <%C>\n",
                    ::CIAO::DDS4CCM::translate_retcode (retcode)));
      throw ::CORBA::INTERNAL ();
    }

  listener = ::DDS::TopicListener::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::passivate_subscriber (void)
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

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::passivate_publisher (void)
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

template <typename CCM_TYPE, typename DDS_TYPE>
void DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::remove_topic (
  ::DDS::Topic_ptr & topic)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_default_topic");

  ::DDS::ReturnCode_t retcode =
    this->domain_participant_->delete_topic (topic);
  if (retcode != ::DDS::RETCODE_OK)
    {
      throw ::CCM_DDS::InternalError (retcode, 0);
    }
  topic = ::DDS::Topic::_nil ();

}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::unregister_type (
  const char * typesupport_name)
{
#if (CIAO_DDS4CCM_NDDS==1)
  ::CIAO::NDDS::DDS_TypeFactory_i * factory =
    ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type (
      typesupport_name, this->domain_participant_.in ());
  delete factory;
#endif
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::remove_subscriber (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_subscriber");

  this->domain_participant_->delete_subscriber (this->subscriber_.in ());
  this->subscriber_ = ::DDS::Subscriber::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE>::remove_publisher (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::remove_publisher");

  this->domain_participant_->delete_publisher (this->publisher_.in ());
  this->publisher_ = ::DDS::Publisher::_nil ();
}
