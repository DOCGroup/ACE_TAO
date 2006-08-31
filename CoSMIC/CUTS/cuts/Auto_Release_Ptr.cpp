// $Id$

#if !defined (__CUTS_INLINE__)
#include "cuts/Auto_Release_Ptr.inl"
#endif

//
// Auto_Release_Ptr
//
template <typename TYPE>
CUTS_Auto_Release_Ptr <TYPE>::CUTS_Auto_Release_Ptr (TYPE * ptr)
: ptr_ (ptr)
{

}

//
// ~Auto_Release_Ptr
//
template <typename TYPE>
CUTS_Auto_Release_Ptr <TYPE>::~CUTS_Auto_Release_Ptr (void)
{
  if (this->ptr_)
  {
    this->ptr_->release ();
    this->ptr_ = 0;
  }
}

//
// reset
//
template <typename TYPE>
void CUTS_Auto_Release_Ptr <TYPE>::reset (TYPE * ptr)
{
  if (this->ptr_ != ptr)
  {
    if (this->ptr_ != 0)
    {
      this->ptr_->release ();
    }

    this->ptr_ = ptr;
  }
}

