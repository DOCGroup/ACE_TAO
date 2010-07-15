// $Id$

template <typename CCM_TYPE>
CIAO::DDS4CCM::DDS4CCM_LocalObject_T<CCM_TYPE>::DDS4CCM_LocalObject_T (void)
{
}

template <typename CCM_TYPE>
CIAO::DDS4CCM::DDS4CCM_LocalObject_T<CCM_TYPE>::~DDS4CCM_LocalObject_T (void)
{
}

template <typename CCM_TYPE>
::CORBA::Object_ptr
CIAO::DDS4CCM::DDS4CCM_LocalObject_T<CCM_TYPE>::_get_component (void)
{
  return CCM_TYPE::base_type::_duplicate (this->component_.in ());
}

template <typename CCM_TYPE>
void
CIAO::DDS4CCM::DDS4CCM_LocalObject_T<CCM_TYPE>::_set_component (
  typename CCM_TYPE::base_type::_ptr_type component)
{
  this->component_ = CCM_TYPE::base_type::_duplicate (component);
}
