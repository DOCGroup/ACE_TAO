// $Id$

#if !defined (ACE_BASED_POINTER_REPOSITORY_T_CPP)
#define ACE_BASED_POINTER_REPOSITORY_T_CPP

#include "ace/Based_Pointer_Repository.h"
#include "ace/Based_Pointer_Repository_T.h"

template <class CONCRETE>
ACE_Based_Pointer<CONCRETE>::ACE_BasedPointer (void)
  : target_ (0),
    base_offset_ (0)
{
  // Find the base address associated with our <this> pointer.
  void *base_addr =
    ACE_BASED_POINTER_REPOSITORY::instance ()->find (this);

  ACE_ASSERT (base_addr != 0);
  this->base_offset_ = (char *) this - (char *) base_addr;
}

template <class CONCRETE> CONCRETE * 
ACE_Based_Pointer<CONCRETE>::operator->(void)
{
  char *base_addr = (char *) this - this->base_offset_;

  return (CONCRETE *)(base_addr + (long) this->target_);
}

template <class CONCRETE> CONCRETE *
ACE_Based_Pointer<CONCRETE>::operator =(CONCRETE *from)
{
  char *base_addr = (char *) this - this->base_offset_;
  this->target_ = (CONCRETE *)((char *) from - (char *) base_addr);
  return from;
}

template <class CONCRETE> CONCRETE 
ACE_Based_Pointer<CONCRETE>::operator *(void)
{
  char *base_addr = (char *) this - this->base_offset_;
  return *(CONCRETE *)(base_addr + (long) this->target_);
}

template <class CONCRETE> CONCRETE 
ACE_Based_Pointer<CONCRETE>::operator [] (int index)
{
  char *base_addr = (char *) this - this->base_offset_;
  return *((CONCRETE *)(base_addr + (long) this->target_) + index);
}

#endif/* ACE_BASED_POINTER_REPOSITORY_T_CPP */

