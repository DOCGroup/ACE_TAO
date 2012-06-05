// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, bool FIXED>
DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::DDS_Get_T (void)
{
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, bool FIXED>
DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::~DDS_Get_T (void)
{
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, bool FIXED>
void
DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::set_component (::CORBA::Object_ptr component)
{
  SubscriberBase_type::set_component (component);
  this->dds_get_->_set_component (component);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, bool FIXED>
void
DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char * qos_profile,
  DDS_XML_QOS_PARSER_TYPE* qos_xml)
{
  DDS4CCM_TRACE ("DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::configuration_complete");


  SubscriberBase_type::configuration_complete (topic,
                                               subscriber,
                                               qos_profile,
                                               qos_xml);

  ::DDS::DataReader_var dr = this->dds_read_->get_dds_reader ();
  this->dds_get_->set_dds_reader (dr.in (),
                                  &this->condition_manager_);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, bool FIXED>
void
DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::remove");
  SubscriberBase_type::remove (subscriber);
}

template <typename CCM_TYPE, typename TYPED_DDS_READER, typename VALUE_TYPE, typename SEQ_VALUE_TYPE, bool FIXED>
typename CCM_TYPE::fresh_data_type::_ptr_type
DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::get_fresh_data (void)
{
  DDS4CCM_TRACE ("DDS_Get_T<CCM_TYPE, TYPED_DDS_READER, VALUE_TYPE, SEQ_VALUE_TYPE, FIXED>::get_fresh_data");

  return CCM_TYPE::fresh_data_type::_duplicate (this->dds_get_);
}
