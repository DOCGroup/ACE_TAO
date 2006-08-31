// $Id$

//
// detach
//
template <typename TYPE>
CUTS_INLINE
TYPE * CUTS_Auto_Release_Ptr <TYPE>::detach (void)
{
  TYPE * ptr = this->ptr_;
  this->ptr_ = 0;
  return ptr;
}

//
// detach
//
template <typename TYPE>
CUTS_INLINE
TYPE * CUTS_Auto_Release_Ptr <TYPE>::get (void)
{
  return this->ptr_;
}

//
// operator ->
//
template <typename TYPE>
CUTS_INLINE
TYPE * CUTS_Auto_Release_Ptr <TYPE>::operator -> (void)
{
  return this->ptr_;
}

//
// operator *
//
template <typename TYPE>
CUTS_INLINE
TYPE & CUTS_Auto_Release_Ptr <TYPE>::operator * (void)
{
  return *this->ptr_;
}
