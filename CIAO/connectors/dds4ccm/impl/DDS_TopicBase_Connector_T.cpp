// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"
#include "ace/Reactor.h"

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::DDS_TopicBase_Connector_T (void) :
    BaseConnector ()
  , init_subscriber_ (false)
  , init_publisher_(false)
  , late_binding_ (false)
{
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::~DDS_TopicBase_Connector_T (void)
{
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
bool
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::late_binding (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::late_binding");
  return this->late_binding_;
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::late_binding (bool late_binding)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::late_binding");
  this->late_binding_ |= late_binding;
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
bool
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::stop_dds (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::stop_dds");

  if (!::CORBA::is_nil (this->topic_name_.in ()))
    { //topic name already set
      // do not stop DDS when topic names are equal
      if (ACE_OS::strlen (this->topic_name_.in ()) == 0)
        return false;
      return ACE_OS::strcmp (this->topic_name_.in (), topic_name) != 0;
    }
  else
    { //topic is not set
      return false;
    }
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
bool
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::late_binded (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::late_binded");

  this->late_binding (ACE_OS::strlen (topic_name) == 0);

  if (ACE_OS::strlen (topic_name) > 0 &&
      !CORBA::is_nil (this->topic_name_) &&
      ACE_OS::strlen (this->topic_name_.in ()) == 0)
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    ACE_TEXT ("DDS_TopicBase_Connector_T::late_binded - ")
                    ACE_TEXT ("Topic <%C> is late binded.\n"), topic_name));
      DDS_TopicBase_Connector_T::topic_name (topic_name);
      return true;
    }
  else if (!this->configuration_complete_)
    {
      DDS_TopicBase_Connector_T::topic_name (topic_name);
    }
  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("DDS_TopicBase_Connector_T::late_binded - ")
                ACE_TEXT ("Topic <%C> is not late binded.\n"), topic_name));
  return false;
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::configuration_complete (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::configuration_complete");

  BaseConnector::configuration_complete ();
  ::CORBA::String_var typesupport_name;
#if (CIAO_DDS4CCM_NDDS==1)
  typesupport_name = ::CORBA::string_dup (DDS_TYPE::type_support::get_type_name ());
#elif (CIAO_DDS4CCM_OPENDDS==1)
  typename DDS_TYPE::type_support type;
  typesupport_name = type.get_type_name ();
#endif

  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();
  this->init_subscriber_ |= ! ::CORBA::is_nil (error_listener.in ());
  this->init_publisher_  |= ! ::CORBA::is_nil (error_listener.in ());

  if (::CORBA::is_nil (this->topic_.in ()))
    {
      this->register_type (this->domain_participant_.in (),
                           typesupport_name);

      this->init_topic (this->domain_participant_.in (),
                        this->topic_.inout () ,
                        this->topic_name_.in (),
                        typesupport_name.in ());
    }

  if (this->init_subscriber_)
    {
      this->init_subscriber (this->domain_participant_.in (),
                            this->subscriber_.inout ());
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_TopicBase_Connector_T::configuration_complete - "
                    "No need to create a subscriber.\n"));
    }

  if (this->init_publisher_)
    {
      this->init_publisher (this->domain_participant_.in (),
                            this->publisher_.inout ());
    }
  else
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    "DDS_TopicBase_Connector_T::configuration_complete - "
                    "No need to create a publisher.\n"));
    }
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::ccm_activate (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::ccm_activate");
  BaseConnector::ccm_activate ();

  ACE_Reactor *reactor = this->reactor ();
  this->activate_topic (reactor,
                        this->topic_.in (),
                        this->topiclistener_.inout ());
  if (this->init_subscriber_)
    {
      this->activate_subscriber (reactor,
                                this->subscriber_.in (),
                                this->subscriber_listener_.inout ());
    }
  if (this->init_publisher_)
    {
      this->activate_publisher (reactor,
                                this->publisher_.in (),
                                this->publisher_listener_.inout ());
    }
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::ccm_passivate");

  ::DDS::TopicListener_var topiclistener =
    this->topiclistener_._retn ();
  if (! CORBA::is_nil (topiclistener.in ()))
    {
      this->passivate_topic (this->topic_.in (),
                             topiclistener.in ());
    }
  ::DDS::SubscriberListener_var subscriber_listener =
    this->subscriber_listener_._retn ();
  if (! CORBA::is_nil (subscriber_listener.in ()))
    {
      this->passivate_subscriber (this->subscriber_.in (),
                                  subscriber_listener.in ());
    }
  ::DDS::PublisherListener_var publisher_listener =
    this->publisher_listener_._retn ();
  if (!::CORBA::is_nil (publisher_listener.in ()))
    {
      this->passivate_publisher (this->publisher_.in (),
                                 publisher_listener.in ());
    }

  BaseConnector::ccm_passivate ();
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::ccm_remove (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::ccm_remove");
  ::DDS::Topic_var topic = this->topic_._retn ();
  if (! CORBA::is_nil (topic.in ()))
    {
      this->remove_topic (this->domain_participant_.in (),
                          topic.in ());
    }

  ::CORBA::String_var typesupport_name;
#if (CIAO_DDS4CCM_NDDS==1)
  typesupport_name = ::CORBA::string_dup (DDS_TYPE::type_support::get_type_name ());
#elif (CIAO_DDS4CCM_OPENDDS==1)
  typename DDS_TYPE::type_support type;
  typesupport_name = type.get_type_name ();
#endif

  this->unregister_type (this->domain_participant_.in (),
                         typesupport_name.in ());

  ::DDS::Subscriber_var subscriber = this->subscriber_._retn ();
  if (! CORBA::is_nil (subscriber.in ()))
    {
      this->remove_subscriber (this->domain_participant_.in (),
                               subscriber.in ());
    }
  ::DDS::Publisher_var publisher = this->publisher_._retn ();
  if (!::CORBA::is_nil (publisher.in ()))
    {
      this->remove_publisher (this->domain_participant_.in (),
                              publisher.in ());
    }
  BaseConnector::ccm_remove ();
}


/**
  * Type registration
 **/
template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::register_type (
  ::DDS::DomainParticipant_ptr participant,
  const char * typesupport_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::register_type");

  DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                ACE_TEXT ("DDS_TopicBase_Connector_T::register_type - ")
                ACE_TEXT ("Going to register type <%C>.\n"),
                typesupport_name));

  ::DDS::ReturnCode_t retcode = ::DDS::RETCODE_ERROR;
#if (CIAO_DDS4CCM_NDDS==1)
  ::CIAO::NDDS::DDS_DomainParticipant_i *part =
    dynamic_cast< CIAO::NDDS::DDS_DomainParticipant_i * > (participant);
  if (!part)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          "DDS_TopicBase_Connector_T::register_type - "
          "Unable to cast the DomainParticipant proxy to its internal "
          "representation.\n"));
      throw ::CORBA::INTERNAL ();
    }

  dds_type_factory *factory = 0;
  ACE_NEW_THROW_EX (factory,
                    dds_type_factory (),
                    ::CORBA::NO_MEMORY ());

  if (! ::CIAO::NDDS::DDS_TypeSupport_i::register_type (typesupport_name, factory, participant))
    {
      DDS4CCM_DEBUG (DDS4CCM_LOG_LEVEL_ACTION, (LM_DEBUG, DDS4CCM_INFO
                    ACE_TEXT ("DDS_TopicBase_Connector_T::register_type - ")
                    ACE_TEXT ("Type <%C> is already registered.\n"),
                    typesupport_name));
      delete factory;
    }

  retcode = DDS_TYPE::type_support::register_type(
        part->get_rti_entity (), typesupport_name);
#else
  typename DDS_TYPE::type_support::_var_type ts = new typename DDS_TYPE::type_support;
  retcode = ts->register_type (participant, typesupport_name);
#endif
  if (retcode != ::DDS::RETCODE_OK)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          "DDS_TopicBase_Connector_T::register_type - "
          "Error registering type <%C> in DDS\n",
          ::CIAO::DDS4CCM::translate_retcode (retcode)));
      throw ::CCM_DDS::InternalError (retcode, 0);
    }
}

/**
  * Unregister type
 **/
template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::unregister_type (
  ::DDS::DomainParticipant_ptr participant,
  const char * typesupport_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T::unregister_type");

#if (CIAO_DDS4CCM_NDDS==1)
  ::CIAO::NDDS::DDS_TypeFactory_i * factory =
    ::CIAO::NDDS::DDS_TypeSupport_i::unregister_type (typesupport_name, participant);

  delete factory;
#else
  ACE_UNUSED_ARG (participant);
  ACE_UNUSED_ARG (typesupport_name);
#endif
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
char *
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::topic_name (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::topic_name");

  return CORBA::string_dup (this->topic_name_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::topic_name (
  const char * topic_name)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::topic_name");

  this->topic_name_ = topic_name;
}

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
void
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::key_fields (
  const ::DDS::StringSeq & key_fields)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::key_fields");
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

template <typename CCM_TYPE, typename DDS_TYPE, typename SEQ_TYPE>
::DDS::StringSeq *
DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::key_fields (void)
{
  DDS4CCM_TRACE ("DDS_TopicBase_Connector_T<CCM_TYPE, DDS_TYPE, SEQ_TYPE>::key_fields");
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

