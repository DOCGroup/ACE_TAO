// $Id$

template <typename CCM_TYPE>
CIAO::DDS4CCM::LocalObject_T<CCM_TYPE>::LocalObject_T (void)
{
}

template <typename CCM_TYPE>
CIAO::DDS4CCM::LocalObject_T<CCM_TYPE>::~LocalObject_T (void)
{
}

template <typename CCM_TYPE>
::CORBA::Object_ptr
CIAO::DDS4CCM::LocalObject_T<CCM_TYPE>::_get_component (void)
{
  return CCM_TYPE::base_type::_duplicate (this->component_.in ());
}

template <typename CCM_TYPE>
void
CIAO::DDS4CCM::LocalObject_T<CCM_TYPE>::_set_component (
  typename CCM_TYPE::base_type::_ptr_type component)
{
  this->component_ = CCM_TYPE::base_type::_duplicate (component);
}

template <typename LOCAL_OBJECT>
CIAO::DDS4CCM::LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::LocalObject_Auto_Ptr_T (void) : l (new LOCAL_OBJECT)
{
}

template <typename LOCAL_OBJECT>
CIAO::DDS4CCM::LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::~LocalObject_Auto_Ptr_T (void)
{
  ::CORBA::release (l);
}

template <typename LOCAL_OBJECT>
LOCAL_OBJECT*
CIAO::DDS4CCM::LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::operator-> (void) const
{
  return this->l;
}

template <typename LOCAL_OBJECT>
CIAO::DDS4CCM::LocalObject_Auto_Ptr_T<LOCAL_OBJECT>::operator LOCAL_OBJECT *& (void)
{
  return this->l;
}

