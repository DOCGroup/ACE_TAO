// $Id$

#include "connectors/dds4ccm/impl/DDS_TopicBase_Connector_T.h"

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::DDS_MT_Event_Connector_T (void)
{
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::supplier_sq_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_sq_data (void)
{
  DDS4CCM_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_sq_data");

  typename CCM_TYPE::supplier_sq_traits::data_type::_var_type dw =
    this->sq_supplier_.get_data ();
  return CCM_TYPE::supplier_sq_traits::data_type::_duplicate (dw.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::supplier_sq_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_sq_dds_entity (void)
{

  return CCM_TYPE::supplier_sq_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_sq_traits::fresh_data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_sq_fresh_data (void)
{
  typename CCM_TYPE::pull_consumer_sq_traits::fresh_data_type::_var_type fresh_data =
    this->pull_consumer_sq_.get_fresh_data ();
  return CCM_TYPE::pull_consumer_sq_traits::fresh_data_type::_duplicate (
                                                              fresh_data.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_sq_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_sq_data (void)
{

  return CCM_TYPE::pull_consumer_sq_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_sq_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_sq_dds_entity (void)
{

  return CCM_TYPE::pull_consumer_sq_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::QueryFilter*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::pull_consumer_sq_filter (void)
{
  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::pull_consumer_sq_filter (const ::CCM_DDS::QueryFilter&)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting_ptr
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_sq_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_sq_filter_config(void)
{
  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::QueryFilter *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::push_consumer_sq_filter (void)
{
  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::push_consumer_sq_filter (const ::CCM_DDS::QueryFilter &)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_sq_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_sq_data (void)
{

  return CCM_TYPE::push_consumer_sq_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_sq_traits::data_control_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_sq_data_control (void)
{

  return CCM_TYPE::push_consumer_sq_traits::data_control_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_sq_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_sq_dds_entity (void)
{

  return CCM_TYPE::push_consumer_sq_traits::dds_entity_type::_nil ();
}

//TRIANGEL

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::supplier_tr_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_tr_data (void)
{
  DDS4CCM_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_tr_data");

  typename CCM_TYPE::supplier_sq_traits::data_type::_var_type dw =
    this->tr_supplier_.get_data ();
  return CCM_TYPE::supplier_sq_traits::data_type::_duplicate (dw.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::supplier_tr_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_tr_dds_entity (void)
{

  return CCM_TYPE::supplier_tr_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_tr_traits::fresh_data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_tr_fresh_data (void)
{
  typename CCM_TYPE::pull_consumer_tr_traits::fresh_data_type::_var_type fresh_data =
    this->pull_consumer_tr_.get_fresh_data ();

  return CCM_TYPE::pull_consumer_tr_traits::fresh_data_type::_duplicate (
                                                              fresh_data.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_tr_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_tr_data (void)
{

  return CCM_TYPE::pull_consumer_tr_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_tr_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_tr_dds_entity (void)
{

  return CCM_TYPE::pull_consumer_tr_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::QueryFilter*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::pull_consumer_tr_filter (void)
{

  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::pull_consumer_tr_filter (const ::CCM_DDS::QueryFilter&)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_tr_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_tr_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::QueryFilter *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::push_consumer_tr_filter (void)
{

  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::push_consumer_tr_filter (
  const ::CCM_DDS::QueryFilter &)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_tr_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_tr_data (void)
{

  return CCM_TYPE::push_consumer_tr_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_tr_traits::data_control_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_tr_data_control (void)
{

  return CCM_TYPE::push_consumer_tr_traits::data_control_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_tr_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_tr_dds_entity (void)
{

  return CCM_TYPE::push_consumer_tr_traits::dds_entity_type::_nil ();
}

//CIRCLE

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::supplier_cl_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_cl_data (void)
{
  DDS4CCM_TRACE ("DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_cl_data");
  typename CCM_TYPE::supplier_sq_traits::data_type::_var_type dw =
    this->cl_supplier_.get_data ();
  return CCM_TYPE::supplier_sq_traits::data_type::_duplicate (dw.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::supplier_cl_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_supplier_cl_dds_entity (void)
{

  return CCM_TYPE::supplier_cl_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_cl_traits::fresh_data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_cl_fresh_data (void)
{
  return CCM_TYPE::pull_consumer_cl_traits::fresh_data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_cl_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_cl_data (void)
{

  return CCM_TYPE::pull_consumer_cl_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::pull_consumer_cl_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_cl_dds_entity (void)
{

  return CCM_TYPE::pull_consumer_cl_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::QueryFilter*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::pull_consumer_cl_filter (void)
{

  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::pull_consumer_cl_filter (const ::CCM_DDS::QueryFilter&)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_pull_consumer_cl_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::CCM_ContentFilterSetting*
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_cl_filter_config(void)
{

  return ::CCM_DDS::CCM_ContentFilterSetting::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
::CCM_DDS::QueryFilter *
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::push_consumer_cl_filter (void)
{
  return 0;
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::push_consumer_cl_filter (const ::CCM_DDS::QueryFilter &)
{

}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_cl_traits::data_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_cl_data (void)
{
  return CCM_TYPE::push_consumer_cl_traits::data_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_cl_traits::data_control_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_cl_data_control (void)
{
  typename CCM_TYPE::push_consumer_cl_traits::data_control_type::_var_type dlc =
    this->push_consumer_cl_.get_data_control ();
  return CCM_TYPE::push_consumer_cl_traits::data_control_type::_duplicate (dlc.in ());
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
typename CCM_TYPE::push_consumer_cl_traits::dds_entity_type::_ptr_type
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::get_push_consumer_cl_dds_entity (void)
{
  return CCM_TYPE::push_consumer_cl_traits::dds_entity_type::_nil ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::configuration_complete (void)
{
  TopicBaseConnector::configuration_complete ();

  this->sq_supplier_.configuration_complete (
                                      this->topic_.in (),
                                      this->publisher_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->tr_supplier_.configuration_complete (
                                      this->topic_.in (),
                                      this->publisher_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->cl_supplier_.configuration_complete (
                                      this->topic_.in (),
                                      this->publisher_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->pull_consumer_sq_.configuration_complete (
                                      this->topic_.in (),
                                      this->subscriber_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->pull_consumer_tr_.configuration_complete (
                                      this->topic_.in (),
                                      this->subscriber_.in (),
                                      this->library_name_,
                                      this->profile_name_);
  this->push_consumer_cl_.configuration_complete (
                                      this->topic_.in (),
                                      this->subscriber_.in (),
                                      this->library_name_,
                                      this->profile_name_);
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::ccm_activate (void)
{
  TopicBaseConnector::ccm_activate (0);

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

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::ccm_passivate (void)
{
  this->sq_supplier_.passivate ();
  this->tr_supplier_.passivate ();
  this->cl_supplier_.passivate ();

  this->pull_consumer_sq_.passivate ();
  this->pull_consumer_tr_.passivate ();

  this->push_consumer_cl_.passivate ();

  TopicBaseConnector::ccm_passivate ();
}

template <typename CCM_TYPE, typename DDS_TYPE, bool FIXED>
void
DDS_MT_Event_Connector_T<CCM_TYPE, DDS_TYPE, FIXED>::ccm_remove (void)
{
  this->sq_supplier_.remove (this->publisher_.in ());
  this->tr_supplier_.remove (this->publisher_.in ());
  this->cl_supplier_.remove (this->publisher_.in ());

  this->pull_consumer_sq_.remove (this->subscriber_.in ());
  this->pull_consumer_tr_.remove (this->subscriber_.in ());

  TopicBaseConnector::ccm_remove ();
}
