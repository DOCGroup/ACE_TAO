// -*- C++ -*-
// $Id$

template <typename BASE_TYPE>
CCM_DDS_StateListenerControl_T<BASE_TYPE>::CCM_DDS_StateListenerControl_T (void)
    : is_filter_interpreted_ (false)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::CCM_DDS_StateListenerControl_T");
}

template <typename BASE_TYPE>
CCM_DDS_StateListenerControl_T<BASE_TYPE>::~CCM_DDS_StateListenerControl_T (void)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::~CCM_DDS_StateListenerControl_T");
}

template <typename BASE_TYPE>
::CORBA::Boolean
CCM_DDS_StateListenerControl_T<BASE_TYPE>::is_filter_interpreted (void)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::is_filter_interpreted");

  return this->is_filter_interpreted_.value ();
}

template <typename BASE_TYPE>
void
CCM_DDS_StateListenerControl_T<BASE_TYPE>::is_filter_interpreted (
  ::CORBA::Boolean is_filter_interpreted)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::is_filter_interpreted");

#if (CIAO_DDS4CCM_NDDS==1)
  // Currently RTI DDS doesn't support this
  throw ::CORBA::NO_IMPLEMENT ();
#else
  this->is_filter_interpreted_ = is_filter_interpreted;
#endif  
}

