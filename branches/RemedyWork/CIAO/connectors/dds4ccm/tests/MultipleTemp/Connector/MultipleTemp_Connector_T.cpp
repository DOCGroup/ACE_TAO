// $Id$

#include "connectors/dds4ccm/impl/DDS_Base_Connector_T.h"
#include "ace/Reactor.h"

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::FooConnector_Connector_T(void)
  : push_consumer_t_ (),
    push_consumer_y_ (),
    reactor_ (0)
{
}


/**
  * Type registration both topics
 **/
template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
           typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
 DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::register_type (
  ::DDS::DomainParticipant_ptr participant,
  const char * /*typesupport_name*/)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::register_type");

  ::DDS::ReturnCode_t retcode_t = ::DDS::RETCODE_OK;
  ::DDS::ReturnCode_t retcode_y = ::DDS::RETCODE_OK;
#if (CIAO_DDS4CCM_NDDS==1)
  ::CIAO::NDDS::DDS_DomainParticipant_i *part =
    dynamic_cast< CIAO::NDDS::DDS_DomainParticipant_i * > (participant);
  if (!part)
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          "DDS_MultipleTemp_FooConnector_T::register_type - "
          "Unable to cast the DomainParticipant proxy to its internal "
          "representation.\n"));
      throw ::CORBA::INTERNAL ();
    }

  typedef ::CIAO::NDDS::DDS_TypeFactory_T <DDS_TYPE_T, SEQ_TYPE_T> dds_type_factory_t;
  typedef ::CIAO::NDDS::DDS_TypeFactory_T <DDS_TYPE_Y, SEQ_TYPE_Y> dds_type_factory_y;

  ::CIAO::NDDS::DDS_TypeFactory_i * factory_t = 0;
  ::CIAO::NDDS::DDS_TypeFactory_i * factory_y = 0;

  const char* typesupport_name_t = DDS_TYPE_T::type_support::get_type_name ();
  const char* typesupport_name_y = DDS_TYPE_Y::type_support::get_type_name ();


  ACE_NEW_THROW_EX (factory_t,
                    dds_type_factory_t (),
                    ::CORBA::NO_MEMORY ());
  ::CIAO::NDDS::DDS_TypeSupport_i::register_type (typesupport_name_t,
                                                  factory_t, participant);

  ACE_NEW_THROW_EX (factory_y,
                      dds_type_factory_y (),
                      ::CORBA::NO_MEMORY ());

  ::CIAO::NDDS::DDS_TypeSupport_i::register_type (typesupport_name_y,
                                                  factory_y, participant);

  retcode_t = DDS_TYPE_T::type_support::register_type(
    part->get_rti_entity (), typesupport_name_t);

  retcode_y = DDS_TYPE_Y::type_support::register_type(
      part->get_rti_entity (), typesupport_name_y);

#endif
  if ((retcode_t != ::DDS::RETCODE_OK)  && (retcode_y != ::DDS::RETCODE_OK))
    {
      DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
          "DDS_MultipleTemp_FooConnector_T::register_type - "
          "Error registering type T <%C> and type Y <%C>\n",
          ::CIAO::DDS4CCM::translate_retcode (retcode_t),::CIAO::DDS4CCM::translate_retcode (retcode_y)));
      throw ::CCM_DDS::InternalError (retcode_t, 0);
    }
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
char *
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::topic_name_t (void)
{
  return CORBA::string_dup (this->topic_name_t_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
 DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::topic_name_t (
  const char * topic_name_t)
{
  this->topic_name_t_ = topic_name_t;
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
::CCM_DDS::CCM_ContentFilterSetting*
 FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
 DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_push_consumer_t_filter_config(void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::get_push_consumer_t_filter_config");
  this->push_consumer_t_.set_component (this);
  return this->push_consumer_t_.get_filter_config ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
::CCM_DDS::QueryFilter *
 FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
 DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::push_consumer_t_filter (void)
{
  return this->push_consumer_t_.filter ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::push_consumer_t_filter (const ::CCM_DDS::QueryFilter &filter)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::push_consumer_t_filter");
  this->push_consumer_t_.filter (filter);
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::push_consumer_t_traits::data_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_push_consumer_t_data (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::get_push_consumer_t_data");
  this->push_consumer_t_.set_component (this);
  return this->push_consumer_t_.get_data ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::push_consumer_t_traits::data_control_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_push_consumer_t_data_control (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::get_push_consumer_t_data_control");
  this->push_consumer_t_.set_component (this);
  return this->push_consumer_t_.get_data_control ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::push_consumer_t_traits::dds_entity_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_push_consumer_t_dds_entity (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::get_push_consumer_t_dds_entity");
  this->push_consumer_t_.set_component (this);
  return this->push_consumer_t_.get_dds_entity ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::observable_t_traits::data_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_observable_t_data (void)
{
//  return CCM_TYPE::observable_t_traits::data_type::_nil();
  this->observable_t_.set_component (this);
  return this->observable_t_.get_data ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::observable_t_traits::dds_entity_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_observable_t_dds_entity (void)
{
  this->observable_t_.set_component (this);
  return this->observable_t_.get_dds_entity ();
}

// Y
template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
char *
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::topic_name_y (void)
{
  return CORBA::string_dup (this->topic_name_y_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
 DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::topic_name_y (
  const char * topic_name_y)
{
  this->topic_name_y_ = topic_name_y;
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
::CCM_DDS::CCM_ContentFilterSetting*
 FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
 DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_push_consumer_y_filter_config(void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::get_push_consumer_y_filter_config");
  this->push_consumer_y_.set_component (this);
  return this->push_consumer_y_.get_filter_config ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
::CCM_DDS::QueryFilter *
 FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
 DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::push_consumer_y_filter (void)
{
  return this->push_consumer_y_.filter ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::push_consumer_y_filter (const ::CCM_DDS::QueryFilter &filter)
{
  this->push_consumer_y_.filter (filter);
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::push_consumer_y_traits::data_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_push_consumer_y_data (void)
{
  this->push_consumer_y_.set_component (this);
  return this->push_consumer_y_.get_data ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::push_consumer_y_traits::data_control_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_push_consumer_y_data_control (void)
{
  this->push_consumer_y_.set_component (this);
  return this->push_consumer_y_.get_data_control ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::push_consumer_y_traits::dds_entity_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_push_consumer_y_dds_entity (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::get_push_consumer_y_dds_entity");
  this->push_consumer_y_.set_component (this);
  return this->push_consumer_y_.get_dds_entity ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::observable_y_traits::data_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_observable_y_data (void)
{
  this->observable_y_.set_component (this);
  return this->observable_y_.get_data ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
typename CCM_TYPE::observable_y_traits::dds_entity_type::_ptr_type
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::get_observable_y_dds_entity (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::get_observable_y_dds_entity");
  this->observable_y_.set_component (this);
  return this->observable_y_.get_dds_entity ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::configuration_complete (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::configuration complete");
  // Init default domain in Base_Connector
  BaseConnector::configuration_complete ();

  if (::CORBA::is_nil (this->domain_participant_.in ()))
    {
      ACE_ERROR ((LM_ERROR, "FooConnector_Connector_T::configuration complete init_domain - "
                   "Error domain_participant\n"));
       throw ::CORBA::INTERNAL ();
    }

  // Second argument can be removed if register_type is changed in (Topic)Base_Connector_T
  this->register_type (this->domain_participant_.in (),
                       "dummy_name");

  const char* typesupport_name_t = DDS_TYPE_T::type_support::get_type_name ();
  const char* typesupport_name_y = DDS_TYPE_Y::type_support::get_type_name ();

  // Create the topics needed
  this->init_topic (this->domain_participant_.in (),
                       this->topic_t_.inout () ,
                       this->topic_name_t_.in (),
                       typesupport_name_t);

  if (::CORBA::is_nil (this->topic_t_.in ()))
     {
       ACE_ERROR ((LM_ERROR, "FooConnector_Connector_T*** configuration complete init_topic - "
                  "Error creating topic for t\n"));
       throw ::CORBA::INTERNAL ();
     }

  this->init_topic (this->domain_participant_.in (),
                         this->topic_y_.inout () ,
                         this->topic_name_y_.in (),
                         typesupport_name_y);

  if (::CORBA::is_nil (this->topic_y_.in ()))
      {
        ACE_ERROR ((LM_ERROR, "FooConnector_Connector_T*** configuration complete init_topic - "
                   "Error creating topic for y\n"));
        throw ::CORBA::INTERNAL ();
      }

  // Init the subscriber and publisher
  this->init_subscriber (this->domain_participant_.in (),
                         this->subscriber_t_.inout ());
  this->init_publisher (this->domain_participant_.in (),
                        this->publisher_t_.inout ());
  this->init_subscriber (this->domain_participant_.in (),
                         this->subscriber_y_.inout ());
  this->init_publisher (this->domain_participant_.in (),
                      this->publisher_y_.inout ());

  this->observable_t_.configuration_complete (
          this->topic_t_.in (),
          this->publisher_t_.in (),
          this->library_name_,
          this->profile_name_);
  this->observable_y_.configuration_complete (
           this->topic_y_.in (),
           this->publisher_y_.in (),
           this->library_name_,
           this->profile_name_);

  this->dl_t_ = this->context_->get_connection_push_consumer_t_data_listener ();
  this->dl_y_ = this->context_->get_connection_push_consumer_y_data_listener ();

  this->push_consumer_t_psl =
     this->context_->get_connection_push_consumer_t_status ();

  this->push_consumer_y_psl =
     this->context_->get_connection_push_consumer_y_status ();


  this->push_consumer_t_.configuration_complete (
                                      this->topic_t_.in (),
                                      this->subscriber_t_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->push_consumer_y_.configuration_complete (
                                       this->topic_y_.in (),
                                       this->subscriber_y_.in (),
                                       this->library_name_,
                                       this->profile_name_);

}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::ccm_activate (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::ccm_activate");
  BaseConnector::ccm_activate ();
  ACE_Reactor* reactor = 0;

  this->activate_subscriber (reactor,
                             this->subscriber_t_.in (),
                             this->subscriber_t_listener_.inout ());
  this->activate_publisher (reactor,
                            this->publisher_t_.in (),
                            this->publisher_t_listener_.inout ());

  this->activate_topic ( reactor,
                         this->topic_t_.in (),
                         this->topiclistener_t_.inout ());
  this->activate_topic (reactor,
                        this->topic_y_.in (),
                        this->topiclistener_y_.inout ());

  this->push_consumer_t_.activate (this->dl_t_.in (),
                                    push_consumer_t_psl.in (),
                                    this->reactor_);

  this->push_consumer_y_.activate (this->dl_y_.in (),
                                          push_consumer_y_psl.in (),
                                          this->reactor_);
  this->observable_t_.activate ();
  this->observable_y_.activate ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::ccm_passivate (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::ccm_passivate");

  if (ACE_OS::strlen (this->topic_name_t_) != 0)
    {
      this->observable_t_.passivate ();
      this->push_consumer_t_.passivate ();
    }
  if (ACE_OS::strlen (this->topic_name_y_) != 0)
    {
      this->observable_y_.passivate ();
      this->push_consumer_y_.passivate ();
    }

  ::DDS::TopicListener_var topiclistener_t =
     this->topiclistener_t_._retn ();
   if (! CORBA::is_nil (topiclistener_t.in ()))
     {
       this->passivate_topic (this->topic_t_.in (),
                              topiclistener_t.in ());
     }

   ::DDS::TopicListener_var topiclistener_y =
       this->topiclistener_y_._retn ();
   if (! CORBA::is_nil (topiclistener_y.in ()))
     {
       this->passivate_topic (this->topic_y_.in (),
                              topiclistener_y.in ());
     }

  ::DDS::SubscriberListener_var subscriber_t_listener =
    this->subscriber_t_listener_._retn ();
  if (!::CORBA::is_nil (subscriber_t_listener.in ()))
    {
      this->passivate_subscriber (this->subscriber_t_.in (),
                                    subscriber_t_listener.in ());
    }

  ::DDS::PublisherListener_var publisher_t_listener =
    this->publisher_t_listener_._retn ();
  if (!::CORBA::is_nil (publisher_t_listener.in ()))
    {
      this->passivate_publisher (this->publisher_t_.in (),
                                   publisher_t_listener.in ());
    }

  ::DDS::SubscriberListener_var subscriber_y_listener =
      this->subscriber_y_listener_._retn ();
  if (!::CORBA::is_nil (subscriber_y_listener.in ()))
    {
      this->passivate_subscriber (this->subscriber_y_.in (),
                                    subscriber_y_listener.in ());
    }

  ::DDS::PublisherListener_var publisher_y_listener =
      this->publisher_y_listener_._retn ();
  if (!::CORBA::is_nil (publisher_y_listener.in ()))
    {
      this->passivate_publisher (this->publisher_y_.in (),
                                   publisher_y_listener.in ());
    }

  BaseConnector::ccm_passivate ();
}

template <typename CCM_TYPE, typename DDS_TYPE_T, bool FIXED_T, typename SEQ_TYPE_T, bool FIXED_SEQ_TYPE_T,
typename DDS_TYPE_Y, bool FIXED_Y, typename SEQ_TYPE_Y, bool FIXED_SEQ_TYPE_Y>
void
FooConnector_Connector_T<CCM_TYPE, DDS_TYPE_T, FIXED_T, SEQ_TYPE_T, FIXED_SEQ_TYPE_T,
DDS_TYPE_Y, FIXED_Y, SEQ_TYPE_Y, FIXED_SEQ_TYPE_Y>::ccm_remove (void)
{
  DDS4CCM_TRACE ("FooConnector_Connector_T::ccm_remove");

  if (ACE_OS::strlen (this->topic_name_t_) != 0)
    {
      this->observable_t_.remove (this->publisher_t_.in ());
      this->push_consumer_t_.remove (this->subscriber_t_.in ());

    }

  if (ACE_OS::strlen (this->topic_name_y_) != 0)
    {
      this->observable_y_.remove (this->publisher_y_.in ());
      this->push_consumer_y_.remove (this->subscriber_y_.in ());
    }

  ::DDS::Topic_var topic_t = this->topic_t_._retn ();
  if (! CORBA::is_nil (topic_t.in ()))
    {
      this->remove_topic (this->domain_participant_.in (),
                          topic_t.in ());
    }

  ::DDS::Topic_var topic_y = this->topic_y_._retn ();
  if (! CORBA::is_nil (topic_y.in ()))
    {
      this->remove_topic (this->domain_participant_.in (),
                          topic_y.in ());
    }

  const char* typesupport_name_t = DDS_TYPE_T::type_support::get_type_name ();
  this->unregister_type (this->domain_participant_.in (),
                        typesupport_name_t);
  const char* typesupport_name_y = DDS_TYPE_Y::type_support::get_type_name ();
  this->unregister_type (this->domain_participant_.in (),
                        typesupport_name_y);

  ::DDS::Publisher_var publisher_t = this->publisher_t_._retn ();
  if (!::CORBA::is_nil (publisher_t.in ()))
    {
      this->remove_publisher (this->domain_participant_.in (),
                              publisher_t.in ());
    }

  ::DDS::Subscriber_var subscriber_t = this->subscriber_t_._retn ();
  if (!::CORBA::is_nil (subscriber_t.in ()))
    {
       this->remove_subscriber (this->domain_participant_.in (),
                                   subscriber_t.in ());
    }

  ::DDS::Publisher_var publisher_y = this->publisher_y_._retn ();
  if (!::CORBA::is_nil (publisher_y.in ()))
    {
      this->remove_publisher (this->domain_participant_.in (),
                                 publisher_y.in ());
    }
  ::DDS::Subscriber_var subscriber_y = this->subscriber_y_._retn ();
  if (!::CORBA::is_nil (subscriber_y.in ()))
    {
      this->remove_subscriber (this->domain_participant_.in (),
                                  subscriber_y.in ());
    }

  BaseConnector::ccm_remove ();
 }
