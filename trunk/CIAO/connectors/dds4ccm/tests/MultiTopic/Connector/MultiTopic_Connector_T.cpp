// $Id$

#include "connectors/dds4ccm/impl/DDS_TopicBase_Connector_T.h"

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::DDS_MT_Event_Connector_T (void)
{
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::supplier_sq_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_sq_data (void)
{
  DDS4CCM_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_sq_data");

  typename CCM_TYPE::supplier_sq_traits::data_type::_var_type dw =
    this->sq_supplier_.get_data ();
  return CCM_TYPE::supplier_sq_traits::data_type::_duplicate (dw.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::supplier_sq_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_sq_dds_entity (void)
{

  return CCM_TYPE::supplier_sq_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_sq_traits::fresh_data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_sq_fresh_data (void)
{
  typename CCM_TYPE::pull_consumer_sq_traits::fresh_data_type::_var_type fresh_data =
    this->pull_consumer_sq_.get_fresh_data ();
  return CCM_TYPE::pull_consumer_sq_traits::fresh_data_type::_duplicate (
                                                              fresh_data.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_sq_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_sq_data (void)
{

  return CCM_TYPE::pull_consumer_sq_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_sq_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_sq_dds_entity (void)
{

  return CCM_TYPE::pull_consumer_sq_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::QueryFilter*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::pull_consumer_sq_filter (void)
{
  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::pull_consumer_sq_filter (const ::CCM_DDS::QueryFilter&)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_sq_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_sq_filter_config(void)
{
  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::QueryFilter *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::push_consumer_sq_filter (void)
{
  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::push_consumer_sq_filter (const ::CCM_DDS::QueryFilter &)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_sq_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_sq_data (void)
{

  return CCM_TYPE::push_consumer_sq_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_sq_traits::data_control_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_sq_data_control (void)
{

  return CCM_TYPE::push_consumer_sq_traits::data_control_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_sq_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_sq_dds_entity (void)
{

  return CCM_TYPE::push_consumer_sq_traits::dds_entity_type::_nil ();
}

//TRIANGEL

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::supplier_tr_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_tr_data (void)
{
  DDS4CCM_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_tr_data");

  typename CCM_TYPE::supplier_sq_traits::data_type::_var_type dw =
    this->tr_supplier_.get_data ();
  return CCM_TYPE::supplier_sq_traits::data_type::_duplicate (dw.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::supplier_tr_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_tr_dds_entity (void)
{

  return CCM_TYPE::supplier_tr_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_tr_traits::fresh_data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_tr_fresh_data (void)
{
  typename CCM_TYPE::pull_consumer_tr_traits::fresh_data_type::_var_type fresh_data =
    this->pull_consumer_tr_.get_fresh_data ();

  return CCM_TYPE::pull_consumer_tr_traits::fresh_data_type::_duplicate (
                                                              fresh_data.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_tr_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_tr_data (void)
{

  return CCM_TYPE::pull_consumer_tr_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_tr_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_tr_dds_entity (void)
{

  return CCM_TYPE::pull_consumer_tr_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::QueryFilter*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::pull_consumer_tr_filter (void)
{

  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::pull_consumer_tr_filter (const ::CCM_DDS::QueryFilter&)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_tr_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_tr_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::QueryFilter *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::push_consumer_tr_filter (void)
{

  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::push_consumer_tr_filter (
  const ::CCM_DDS::QueryFilter &)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_tr_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_tr_data (void)
{

  return CCM_TYPE::push_consumer_tr_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_tr_traits::data_control_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_tr_data_control (void)
{

  return CCM_TYPE::push_consumer_tr_traits::data_control_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_tr_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_tr_dds_entity (void)
{

  return CCM_TYPE::push_consumer_tr_traits::dds_entity_type::_nil ();
}

//CIRCLE

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::supplier_cl_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_cl_data (void)
{
  DDS4CCM_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_cl_data");
  typename CCM_TYPE::supplier_sq_traits::data_type::_var_type dw =
    this->cl_supplier_.get_data ();
  return CCM_TYPE::supplier_sq_traits::data_type::_duplicate (dw.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::supplier_cl_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_cl_dds_entity (void)
{

  return CCM_TYPE::supplier_cl_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_cl_traits::fresh_data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_cl_fresh_data (void)
{
  return CCM_TYPE::pull_consumer_cl_traits::fresh_data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_cl_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_cl_data (void)
{

  return CCM_TYPE::pull_consumer_cl_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::pull_consumer_cl_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_cl_dds_entity (void)
{

  return CCM_TYPE::pull_consumer_cl_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::QueryFilter*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::pull_consumer_cl_filter (void)
{

  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::pull_consumer_cl_filter (const ::CCM_DDS::QueryFilter&)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_pull_consumer_cl_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_cl_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::CCM_DDS::QueryFilter *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::push_consumer_cl_filter (void)
{
  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::push_consumer_cl_filter (const ::CCM_DDS::QueryFilter &)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_cl_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_cl_data (void)
{
  return CCM_TYPE::push_consumer_cl_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_cl_traits::data_control_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_cl_data_control (void)
{
  typename CCM_TYPE::push_consumer_cl_traits::data_control_type::_var_type dlc =
    this->push_consumer_cl_.get_data_control ();
  return CCM_TYPE::push_consumer_cl_traits::data_control_type::_duplicate (dlc.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::push_consumer_cl_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_push_consumer_cl_dds_entity (void)
{
  return CCM_TYPE::push_consumer_cl_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
char *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::topic_name_sq (void)
{
  return CORBA::string_dup (this->topic_name_sq_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::topic_name_sq (
  const char * topic_name_sq)
{
  this->topic_name_sq_ = topic_name_sq;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
char *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::topic_name_tr (void)
{
  return CORBA::string_dup (this->topic_name_tr_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::topic_name_tr (
  const char * topic_name_tr)
{
  this->topic_name_tr_ = topic_name_tr;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
char *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::topic_name_cl (void)
{
  return CORBA::string_dup (this->topic_name_cl_.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::topic_name_cl (
  const char * topic_name_cl)
{
  this->topic_name_cl_ = topic_name_cl;
}


template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
::DDS::Topic_ptr
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::create_topic (
  const char * topic_name,
  const char * typesupport_name)
{
  ::DDS::Topic_var tp;
  if (this->library_name_ && this->profile_name_)
    {
      tp  = this->domain_participant_->create_topic_with_profile (
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
      tp = this->domain_participant_->create_topic (topic_name,
                                                    typesupport_name,
                                                    tqos,
                                                    ::DDS::TopicListener::_nil (),
                                                    0);
    }
  return tp._retn ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::create_topics (
  const char * typesupport_name)
{
  this->topic_sq_ = this->create_topic (this->topic_name_sq_.in (),
                                        typesupport_name);
  this->topic_tr_ = this->create_topic (this->topic_name_tr_.in (),
                                        typesupport_name);
  this->topic_cl_ = this->create_topic (this->topic_name_cl_.in (),
                                        typesupport_name);
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::configuration_complete (void)
{
  // Init default domain (Base_Connector)
  this->init_default_domain ();

  // Init type (TopicBase_Connector)
  const char* typesupport_name = DDS_TYPE::type_support::get_type_name ();
  this->init_type (typesupport_name);

  // Create the topics needed (this class)
  this->create_topics (typesupport_name);

  // Init the subscriber and publisher (TopicBase_Connector)
  this->init_subscriber ();
  this->init_publisher ();

  this->sq_supplier_.configuration_complete (
                                      this->topic_sq_.in (),
                                      this->publisher_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->tr_supplier_.configuration_complete (
                                      this->topic_tr_.in (),
                                      this->publisher_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->cl_supplier_.configuration_complete (
                                      this->topic_cl_.in (),
                                      this->publisher_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->pull_consumer_sq_.configuration_complete (
                                      this->topic_sq_.in (),
                                      this->subscriber_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->pull_consumer_tr_.configuration_complete (
                                      this->topic_tr_.in (),
                                      this->subscriber_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->push_consumer_cl_.configuration_complete (
                                      this->topic_cl_.in (),
                                      this->subscriber_.in (),
                                      this->library_name_,
                                      this->profile_name_);
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::activate_topic (
  ::DDS::Topic_ptr topic,
  ::DDS::TopicListener_ptr topiclistener)
{
  ::CCM_DDS::ConnectorStatusListener_var error_listener =
    this->context_->get_connection_error_listener ();

  ::DDS::StatusMask const mask =
    ::CIAO::DDS4CCM::TopicListener::get_mask (error_listener.in ());

  if (mask != 0)
    {
      if (::CORBA::is_nil (topiclistener))
        {
          ACE_NEW_THROW_EX (topiclistener,
                            ::CIAO::DDS4CCM::TopicListener (
                              error_listener.in (),
                              0),
                            ::CORBA::NO_MEMORY ());
        }

      ::DDS::ReturnCode_t const retcode = topic->set_listener (topiclistener,
                                                               mask);

      if (retcode != ::DDS::RETCODE_OK)
        {
          DDS4CCM_ERROR (DDS4CCM_LOG_LEVEL_ERROR, (LM_ERROR, DDS4CCM_INFO
                        "DDS_MT_Event_Connector_T::activate_topic - "
                        "Error while setting the listener on the topic - <%C>\n",
                        ::CIAO::DDS4CCM::translate_retcode (retcode)));
          throw ::CORBA::INTERNAL ();
        }
    }
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::activate_topics (void)
{
  this->activate_topic (this->topic_sq_.in (),
                        this->topiclistener_sq_.in ());
  this->activate_topic (this->topic_tr_.in (),
                        this->topiclistener_tr_.in ());
  this->activate_topic (this->topic_cl_.in (),
                        this->topiclistener_cl_.in ());
}
template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::ccm_activate (void)
{
  TopicBaseConnector::ccm_activate (0);

  this->activate_topics ();

  this->sq_supplier_.activate ();
  this->tr_supplier_.activate ();
  this->cl_supplier_.activate ();

  ::CCM_DDS::PortStatusListener_var pull_consumer_sq_psl =
    this->context_->get_connection_pull_consumer_sq_status ();
  this->pull_consumer_sq_.activate (pull_consumer_sq_psl.in (),
                                    0);

  ::CCM_DDS::PortStatusListener_var pull_consumer_tr_psl =
    this->context_->get_connection_pull_consumer_tr_status ();
  this->pull_consumer_tr_.activate (pull_consumer_tr_psl.in (),
                                    0);

  ::CCM_DDS::PortStatusListener_var push_consumer_cl_psl =
    this->context_->get_connection_push_consumer_cl_status ();

  this->dl_ = this->context_->get_connection_push_consumer_cl_data_listener ();
  this->push_consumer_cl_.activate (
                  this->dl_.in (),
                  push_consumer_cl_psl.in (),
                  0);
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::ccm_passivate (void)
{
  this->sq_supplier_.passivate ();
  this->tr_supplier_.passivate ();
  this->cl_supplier_.passivate ();

  this->pull_consumer_sq_.passivate ();
  this->pull_consumer_tr_.passivate ();

  this->push_consumer_cl_.passivate ();

  TopicBaseConnector::ccm_passivate ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::ccm_remove (void)
{
  this->sq_supplier_.remove (this->publisher_.in ());
  this->tr_supplier_.remove (this->publisher_.in ());
  this->cl_supplier_.remove (this->publisher_.in ());

  this->pull_consumer_sq_.remove (this->subscriber_.in ());
  this->pull_consumer_tr_.remove (this->subscriber_.in ());

  TopicBaseConnector::ccm_remove ();
}
