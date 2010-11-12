// -*- C++ -*-
// $Id$

#include "dds4ccm/impl/logger/Log_Macros.h"

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::DDS_Get_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::~DDS_Get_T (void)
{
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::set_component (::CORBA::Object_ptr component)
{
  DDS_Subscriber_Base_T<DDS_TYPE, CCM_TYPE, FIXED>::set_component (component);
  this->dds_get_->_set_component (component);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete (
  ::DDS::Topic_ptr topic,
  ::DDS::Subscriber_ptr subscriber,
  const char* library_name,
  const char* profile_name)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::configuration_complete");
  DDSSubscriberBase_type::configuration_complete (topic,
                                                  subscriber,
                                                  library_name,
                                                  profile_name);
  this->dds_get_->set_dds_entity (this->dr_,
                                  &this->condition_manager_);
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
void
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::remove (
  ::DDS::Subscriber_ptr subscriber)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::remove");
  DDSSubscriberBase_type::remove (subscriber);
  this->dds_get_->_set_component (::CORBA::Object::_nil ());
}

template <typename DDS_TYPE, typename CCM_TYPE, bool FIXED>
typename CCM_TYPE::getter_type::_ptr_type
DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::get_fresh_data (void)
{
  DDS4CCM_TRACE ("DDS_Get_T<DDS_TYPE, CCM_TYPE, FIXED>::get_fresh_data");

  return CCM_TYPE::getter_type::_duplicate (this->dds_get_);
}
