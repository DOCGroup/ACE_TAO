// $Id$

#ifndef ACE_BASED_POINTER_T_CPP
#define ACE_BASED_POINTER_T_CPP

#define ACE_BUILD_DLL
#include "ace/Based_Pointer_T.h"
#include "ace/Based_Pointer_Repository.h"

#if !defined (__ACE_INLINE__)
#include "ace/Based_Pointer_T.i"
#endif /* __ACE_INLINE__ */

template <class CONCRETE> ACE_Based_Pointer<CONCRETE> 
operator+ (const ACE_Based_Pointer<CONCRETE> &lhs, long increment)
{
  // Perform pointer arithmetic.
  CONCRETE *ptr = ((CONCRETE *) ACE_COMPUTE_BASED_POINTER (&lhs)) + increment;
  ACE_Based_Pointer<CONCRETE> tmp (ptr);
  return tmp;
}

template <class CONCRETE>
ACE_Based_Pointer<CONCRETE>::ACE_Based_Pointer (void)
  : target_ (0),
    base_offset_ (0)
{
  void *base_addr = 0;

  // Find the base address associated with our <this> pointer.  Note
  // that it's ok for <find> to return 0, which simply indicates that
  // the address is not in memory-mapped virtual address space.
  ACE_BASED_POINTER_REPOSITORY::instance ()->find (this, 
                                                   base_addr);
  this->base_offset_ = (char *) this - (char *) base_addr;
}

template <class CONCRETE>
ACE_Based_Pointer<CONCRETE>::ACE_Based_Pointer (CONCRETE *addr)
  : target_ (0),
    base_offset_ (0)
{
  void *base_addr = 0;

  // Find the base address associated with the <addr> pointer.  Note
  // that it's ok for <find> to return 0, which simply indicates that
  // the address is not in memory-mapped virtual address space.
  ACE_BASED_POINTER_REPOSITORY::instance ()->find (addr,
                                                   base_addr);
  this->base_offset_ = (char *) addr - (char *) base_addr;
}

#endif/* ACE_BASED_POINTER_T_CPP */

