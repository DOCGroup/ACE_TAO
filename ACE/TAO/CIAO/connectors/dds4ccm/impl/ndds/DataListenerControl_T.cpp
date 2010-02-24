// -*- C++ -*-
// $Id$

template <typename BASE_TYPE>
CCM_DDS_DataListenerControl_T<BASE_TYPE>::CCM_DDS_DataListenerControl_T (void) :
  mode_ (::CCM_DDS::NOT_ENABLED),
  max_delivered_data_ (0)
{
  CIAO_TRACE ("CCM_DDS_DataListenerControl_T::CCM_DDS_DataListenerControl_T");
}

template <typename BASE_TYPE>
CCM_DDS_DataListenerControl_T<BASE_TYPE>::~CCM_DDS_DataListenerControl_T (void)
{
  CIAO_TRACE ("CCM_DDS_DataListenerControl_T::~CCM_DDS_DataListenerControl_T");
}

template <typename BASE_TYPE>
::CCM_DDS::ListenerMode
CCM_DDS_DataListenerControl_T<BASE_TYPE>::mode (void)
{
  CIAO_TRACE ("CCM_DDS_DataListenerControl_T::mode");
  return this->mode_.value ();
}

template <typename BASE_TYPE>
void
CCM_DDS_DataListenerControl_T<BASE_TYPE>::mode (::CCM_DDS::ListenerMode mode)
{
  CIAO_TRACE ("CCM_DDS_DataListenerControl_T::mode");
  this->mode_ = mode;
}

template <typename BASE_TYPE>
::CCM_DDS::DataNumber_t
CCM_DDS_DataListenerControl_T<BASE_TYPE>::max_delivered_data (void)
{
  CIAO_TRACE ("CCM_DDS_DataListenerControl_T::max_delivered_data");
  return this->max_delivered_data_.value ();
}

template <typename BASE_TYPE>
void
CCM_DDS_DataListenerControl_T<BASE_TYPE>::max_delivered_data (
  ::CCM_DDS::DataNumber_t max_delivered_data)
{
  CIAO_TRACE ("CCM_DDS_DataListenerControl_T::max_delivered_data");
  this->max_delivered_data_ = max_delivered_data;
}

