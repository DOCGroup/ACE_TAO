// -*- C++ -*-
// $Id$

template <typename BASE_TYPE, typename CCM_TYPE>
CCM_DDS_StateListenerControl_T<BASE_TYPE, CCM_TYPE>::CCM_DDS_StateListenerControl_T (void)
    : is_filter_interpreted_ (false),
      component_ (CCM_TYPE::base_type::_nil ())
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::CCM_DDS_StateListenerControl_T");
}

template <typename BASE_TYPE, typename CCM_TYPE>
CCM_DDS_StateListenerControl_T<BASE_TYPE, CCM_TYPE>::~CCM_DDS_StateListenerControl_T (void)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::~CCM_DDS_StateListenerControl_T");
}

template <typename BASE_TYPE, typename CCM_TYPE>
::CORBA::Boolean
CCM_DDS_StateListenerControl_T<BASE_TYPE, CCM_TYPE>::is_filter_interpreted (void)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::is_filter_interpreted");

  return this->is_filter_interpreted_.value ();
}

template <typename BASE_TYPE, typename CCM_TYPE>
void
CCM_DDS_StateListenerControl_T<BASE_TYPE, CCM_TYPE>::is_filter_interpreted (
  ::CORBA::Boolean /*is_filter_interpreted*/)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::is_filter_interpreted");

#if (CIAO_DDS4CCM_NDDS==1)
  // Currently RTI DDS doesn't support this
  throw ::CORBA::NO_IMPLEMENT ();
#else
  this->is_filter_interpreted_ = is_filter_interpreted;
#endif  
}

template <typename BASE_TYPE, typename CCM_TYPE>
::CORBA::Object_ptr
CCM_DDS_StateListenerControl_T<BASE_TYPE, CCM_TYPE>::_get_component (void)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::_get_component");

  return CCM_TYPE::base_type::_duplicate (this->component_.in ());
}

template <typename BASE_TYPE, typename CCM_TYPE>
void
CCM_DDS_StateListenerControl_T<BASE_TYPE, CCM_TYPE>::_set_component (
  typename CCM_TYPE::base_type::_ptr_type component)
{
  DDS4CCM_TRACE ("CCM_DDS_StateListenerControl_T::_set_component");

  this->component_ = CCM_TYPE::base_type::_duplicate (component);
}

