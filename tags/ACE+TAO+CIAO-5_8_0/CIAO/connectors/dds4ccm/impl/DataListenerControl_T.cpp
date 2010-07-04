// -*- C++ -*-
// $Id$

template <typename BASE_TYPE, typename CCM_TYPE>
CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>::CCM_DDS_DataListenerControl_T (void) :
  mode_ (::CCM_DDS::NOT_ENABLED),
  max_delivered_data_ (0),
  component_ (CCM_TYPE::base_type::_nil ())
{
  DDS4CCM_TRACE ("CCM_DDS_DataListenerControl_T::CCM_DDS_DataListenerControl_T");
}

template <typename BASE_TYPE, typename CCM_TYPE>
CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>::~CCM_DDS_DataListenerControl_T (void)
{
  DDS4CCM_TRACE ("CCM_DDS_DataListenerControl_T::~CCM_DDS_DataListenerControl_T");
}

template <typename BASE_TYPE, typename CCM_TYPE>
::CCM_DDS::ListenerMode
CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>::mode (void)
{
  DDS4CCM_TRACE ("CCM_DDS_DataListenerControl_T::mode");
  return this->mode_.value ();
}

template <typename BASE_TYPE, typename CCM_TYPE>
void
CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>::mode (::CCM_DDS::ListenerMode mode)
{
  DDS4CCM_TRACE ("CCM_DDS_DataListenerControl_T::mode");
  this->mode_ = mode;
}

template <typename BASE_TYPE, typename CCM_TYPE>
::CCM_DDS::DataNumber_t
CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>::max_delivered_data (void)
{
  DDS4CCM_TRACE ("CCM_DDS_DataListenerControl_T::max_delivered_data");
  return this->max_delivered_data_.value ();
}

template <typename BASE_TYPE, typename CCM_TYPE>
void
CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>::max_delivered_data (
  ::CCM_DDS::DataNumber_t max_delivered_data)
{
  DDS4CCM_TRACE ("CCM_DDS_DataListenerControl_T::max_delivered_data");
  this->max_delivered_data_ = max_delivered_data;
}

template <typename BASE_TYPE, typename CCM_TYPE>
::CORBA::Object_ptr
CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>::_get_component (void)
{
  DDS4CCM_TRACE ("CCM_DDS_DataListenerControl_T::_get_component");

  return CCM_TYPE::base_type::_duplicate (this->component_.in ());
}

template <typename BASE_TYPE, typename CCM_TYPE>
void
CCM_DDS_DataListenerControl_T<BASE_TYPE, CCM_TYPE>::_set_component (
  typename CCM_TYPE::base_type::_ptr_type component)
{
  DDS4CCM_TRACE ("CCM_DDS_DataListenerControl_T::_set_component");

  this->component_ = CCM_TYPE::base_type::_duplicate (component);
}

