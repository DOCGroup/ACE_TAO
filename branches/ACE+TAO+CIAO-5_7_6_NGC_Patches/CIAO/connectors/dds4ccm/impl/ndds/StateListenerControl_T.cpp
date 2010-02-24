// -*- C++ -*-
// $Id$

template <typename BASE_TYPE>
CCM_DDS_StateListenerControl_T<BASE_TYPE>::CCM_DDS_StateListenerControl_T (void)
    : is_filter_interpreted_ (false)
{
  CIAO_TRACE ("CCM_DDS_StateListenerControl_T::CCM_DDS_StateListenerControl_T");
}

template <typename BASE_TYPE>
CCM_DDS_StateListenerControl_T<BASE_TYPE>::~CCM_DDS_StateListenerControl_T (void)
{
  CIAO_TRACE ("CCM_DDS_StateListenerControl_T::~CCM_DDS_StateListenerControl_T");
}

template <typename BASE_TYPE>
::CORBA::Boolean
CCM_DDS_StateListenerControl_T<BASE_TYPE>::is_filter_interpreted (void)
{
  CIAO_TRACE ("CCM_DDS_StateListenerControl_T::is_filter_interpreted");

  return this->is_filter_interpreted_.value ();
}

template <typename BASE_TYPE>
void
CCM_DDS_StateListenerControl_T<BASE_TYPE>::is_filter_interpreted (
  ::CORBA::Boolean is_filter_interpreted)
{
  CIAO_TRACE ("CCM_DDS_StateListenerControl_T::is_filter_interpreted");

  this->is_filter_interpreted_ = is_filter_interpreted;
}

