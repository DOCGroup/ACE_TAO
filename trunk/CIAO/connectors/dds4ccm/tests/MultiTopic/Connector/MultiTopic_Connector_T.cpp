// $Id$

#include "connectors/dds4ccm/impl/DDS_TopicBase_Connector_T.h"

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::DDS_MT_Event_Connector_T (void)
  : reactor_ (0)
{
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
typename CCM_TYPE::supplier_sq_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_sq_data (void)
{
  ACE_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_sq_data");

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
  ACE_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_tr_data");

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
  ACE_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::get_supplier_cl_data");
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
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::create_topics (
  const char * typesupport_name)
{
   this->init_topic (this->domain_participant_.in (),
                     this->topic_sq_.inout (),
                     this->topic_name_sq_.in (),
                     typesupport_name);
   if (::CORBA::is_nil (this->topic_sq_.in ()))
     {
       ACE_ERROR ((LM_ERROR, "DDS_MT_Event_Connector_T::create_topics - "
                  "Error creating topic for Square\n"));
       throw ::CORBA::INTERNAL ();
     }
   this->init_topic (this->domain_participant_.in (),
                     this->topic_tr_.inout (),
                     this->topic_name_tr_.in (),
                     typesupport_name);
   if (::CORBA::is_nil (this->topic_tr_.in ()))
     {
       ACE_ERROR ((LM_ERROR, "DDS_MT_Event_Connector_T::create_topics - "
                  "Error creating topic for Triangle\n"));
       throw ::CORBA::INTERNAL ();
     }
   this->init_topic (this->domain_participant_.in (),
                     this->topic_cl_.inout (),
                     this->topic_name_cl_.in (),
                     typesupport_name);
   if (::CORBA::is_nil (this->topic_cl_.in ()))
     {
       ACE_ERROR ((LM_ERROR, "DDS_MT_Event_Connector_T::create_topics - "
                  "Error creating topic for Circle\n"));
       throw ::CORBA::INTERNAL ();
     }
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::configuration_complete (void)
{
  // Init default domain (Base_Connector)
  this->init_domain (this->domain_participant_.inout ());

  // Init type (TopicBase_Connector)
  ::CORBA::String_var typesupport_name;
#if (CIAO_DDS4CCM_NDDS==1)
  typesupport_name = ::CORBA::string_dup (DDS_TYPE::type_support::get_type_name ());
#elif (CIAO_DDS4CCM_OPENDDS==1)
  typename DDS_TYPE::type_support type;
  typesupport_name = type.get_type_name ();
#endif
  this->register_type (this->domain_participant_.in (),
                       typesupport_name.in ());

  // Create the topics needed (this class)
  this->create_topics (typesupport_name);

  // Init the subscriber and publisher (TopicBase_Connector)
  this->init_subscriber (this->domain_participant_.in (),
                         this->subscriber_.inout ());
  this->init_publisher (this->domain_participant_.in (),
                        this->publisher_.inout ());

  this->sq_supplier_.configuration_complete (
                                      this->topic_sq_.in (),
                                      this->publisher_.in (),
#if (CIAO_DDS4CCM_NDDS==1)
                                      this->qos_profile_);
#elif (CIAO_DDS4CCM_OPENDDS==1)
                                      this->qos_profile_,
                                      this->qos_xml_);
#endif
  this->tr_supplier_.configuration_complete (
                                      this->topic_tr_.in (),
                                      this->publisher_.in (),
#if (CIAO_DDS4CCM_NDDS==1)
                                      this->qos_profile_);
#elif (CIAO_DDS4CCM_OPENDDS==1)
                                      this->qos_profile_,
                                      this->qos_xml_);
#endif
  this->cl_supplier_.configuration_complete (
                                      this->topic_cl_.in (),
                                      this->publisher_.in (),
#if (CIAO_DDS4CCM_NDDS==1)
                                      this->qos_profile_);
#elif (CIAO_DDS4CCM_OPENDDS==1)
                                      this->qos_profile_,
                                      this->qos_xml_);
#endif
  this->pull_consumer_sq_.configuration_complete (
                                      this->topic_sq_.in (),
                                      this->subscriber_.in (),
#if (CIAO_DDS4CCM_NDDS==1)
                                      this->qos_profile_);
#elif (CIAO_DDS4CCM_OPENDDS==1)
                                      this->qos_profile_,
                                      this->qos_xml_);
#endif
  this->pull_consumer_tr_.configuration_complete (
                                      this->topic_tr_.in (),
                                      this->subscriber_.in (),
#if (CIAO_DDS4CCM_NDDS==1)
                                      this->qos_profile_);
#elif (CIAO_DDS4CCM_OPENDDS==1)
                                      this->qos_profile_,
                                      this->qos_xml_);
#endif
  this->push_consumer_cl_.configuration_complete (
                                      this->topic_cl_.in (),
                                      this->subscriber_.in (),
#if (CIAO_DDS4CCM_NDDS==1)
                                      this->qos_profile_);
#elif (CIAO_DDS4CCM_OPENDDS==1)
                                      this->qos_profile_,
                                      this->qos_xml_);
#endif
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::activate_topics (void)
{
  this->activate_topic (this->reactor_,
                        this->topic_sq_.in (),
                        this->topiclistener_sq_.inout ());
  this->activate_topic (this->reactor_,
                        this->topic_tr_.in (),
                        this->topiclistener_tr_.inout ());
  this->activate_topic (this->reactor_,
                        this->topic_cl_.in (),
                        this->topiclistener_cl_.inout ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::ccm_activate (void)
{
  this->activate_topics ();
  this->activate_subscriber (this->reactor_,
                             this->subscriber_.in (),
                             this->subscriber_listener_.inout ());
  this->activate_publisher (this->reactor_,
                            this->publisher_.in (),
                            this->publisher_listener_.inout ());

  this->sq_supplier_.activate ();
  this->tr_supplier_.activate ();
  this->cl_supplier_.activate ();

  ::CCM_DDS::PortStatusListener_var pull_consumer_sq_psl =
    this->context_->get_connection_pull_consumer_sq_status ();
  this->pull_consumer_sq_.activate (pull_consumer_sq_psl.in (),
                                    this->reactor_);

  ::CCM_DDS::PortStatusListener_var pull_consumer_tr_psl =
    this->context_->get_connection_pull_consumer_tr_status ();
  this->pull_consumer_tr_.activate (pull_consumer_tr_psl.in (),
                                    this->reactor_);

  ::CCM_DDS::PortStatusListener_var push_consumer_cl_psl =
    this->context_->get_connection_push_consumer_cl_status ();

  this->dl_ = this->context_->get_connection_push_consumer_cl_data_listener ();
  this->push_consumer_cl_.activate (this->dl_.in (),
                                    push_consumer_cl_psl.in (),
                                    this->reactor_);
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::passivate_topics (void)
{
  ::DDS::TopicListener_var topic_listener = this->topiclistener_sq_._retn ();
  if (!::CORBA::is_nil (topic_listener.in ()))
    {
      this->passivate_topic (this->topic_sq_.in (),
                             topic_listener.in ());
    }
  topic_listener = this->topiclistener_tr_._retn ();
  if (!::CORBA::is_nil (topic_listener.in ()))
    {
      this->passivate_topic (this->topic_tr_.in (),
                             topic_listener.in ());
    }
  topic_listener = this->topiclistener_cl_._retn ();
  if (!::CORBA::is_nil (topic_listener.in ()))
    {
      this->passivate_topic (this->topic_cl_.in (),
                             topic_listener.in ());
    }
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

  this->passivate_topics ();

  ::DDS::SubscriberListener_var subscriber_listener =
    this->subscriber_listener_._retn ();
  if (!::CORBA::is_nil (subscriber_listener.in ()))
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
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED, typename SEQ_TYPE>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED, SEQ_TYPE>::remove_topics (void)
{
  this->remove_topic (this->domain_participant_.in (),
                      this->topic_sq_.inout ());
  this->remove_topic (this->domain_participant_.in (),
                      this->topic_tr_.inout ());
  this->remove_topic (this->domain_participant_.in (),
                      this->topic_cl_.inout ());
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
  this->push_consumer_cl_.remove (this->subscriber_.in ());

  this->remove_topics ();

  /**
    *
    * In order to close down in a thread safe and memory leak free manner, one
    * should first declare a temporary _var variable and assign this one with the
    * class member, using _retn (). This'll free all references to the class member
    * so one's sure that the only one left is the temporary _var variable. This
    * one should be used in calls to the 'remove_xxx' methods.
  **/
  ::DDS::Publisher_var publisher = this->publisher_._retn ();
  if (!::CORBA::is_nil (publisher.in ()))
    {
      this->remove_publisher (this->domain_participant_.in (),
                              publisher.in ());
    }
  ::DDS::Subscriber_var subscriber = this->subscriber_._retn ();
  if (!::CORBA::is_nil (subscriber.in ()))
    {
      this->remove_subscriber (this->domain_participant_.in (),
                               subscriber.in ());
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

  ::DDS::DomainParticipant_var dp = this->domain_participant_._retn ();
  if (!::CORBA::is_nil (dp.in ()))
    {
      this->remove_domain (dp.in ());
    }
}
