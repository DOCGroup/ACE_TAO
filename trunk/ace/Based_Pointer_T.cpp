// $Id$

#ifndef ACE_BASED_POINTER_T_CPP
#define ACE_BASED_POINTER_T_CPP

#define ACE_BUILD_DLL
#include "ace/Based_Pointer_T.h"
#include "ace/Based_Pointer_Repository.h"

#   define ACE_TRACEX(X) ACE_Trace ____ (ASYS_TEXT (X), __LINE__, ASYS_TEXT (__FILE__))

#if !defined (__ACE_INLINE__)
#include "ace/Based_Pointer_T.i"
#endif /* __ACE_INLINE__ */

template <class CONCRETE> ACE_INLINE
ACE_Based_Pointer<CONCRETE>::ACE_Based_Pointer (void)
{
  ACE_TRACE ("ACE_Based_Pointer<CONCRETE>::ACE_Based_Pointer");
}

template <class CONCRETE> void
ACE_Based_Pointer_Basic<CONCRETE>::dump (void) const
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::dump");

  ACE_DEBUG ((LM_DEBUG, ACE_BEGIN_DUMP, this));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("\ntarget_ = %d\n"), this->target_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("base_offset_ = %d\n"), this->base_offset_));
  ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("computed pointer = %x\n"), (CONCRETE *)(ACE_COMPUTE_BASED_POINTER (this))));
  ACE_DEBUG ((LM_DEBUG, ACE_END_DUMP));
}

template <class CONCRETE>
ACE_Based_Pointer<CONCRETE>::ACE_Based_Pointer (CONCRETE *initial)
  : ACE_Based_Pointer_Basic<CONCRETE> (initial)
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::ACE_Based_Pointer_Basic");
}

template <class CONCRETE>
ACE_Based_Pointer_Basic<CONCRETE>::ACE_Based_Pointer_Basic (void)
  : target_ (0),
    base_offset_ (0)
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::ACE_Based_Pointer_Basic");
  void *base_addr = 0;

  // Find the base address associated with our <this> pointer.  Note
  // that it's ok for <find> to return 0, which simply indicates that
  // the address is not in memory-mapped virtual address space.
  ACE_BASED_POINTER_REPOSITORY::instance ()->find (this, 
                                                   base_addr);
  this->base_offset_ = (char *) this - (char *) base_addr;
}

template <class CONCRETE>
ACE_Based_Pointer_Basic<CONCRETE>::ACE_Based_Pointer_Basic (CONCRETE *rhs)
  : target_ (0),
    base_offset_ (0)
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::ACE_Based_Pointer_Basic");

  if (rhs == 0)
    // Store a value of <target_> that indicate "NULL" pointer.
    this->target_ = -1;
  else
    {
      void *base_addr = 0;

      // Find the base address associated with the <addr> pointer.
      // Note that it's ok for <find> to return 0, which simply
      // indicates that the address is not in memory-mapped virtual
      // address space.
      ACE_BASED_POINTER_REPOSITORY::instance ()->find (this,
                                                       base_addr);
      this->base_offset_ = (char *) this - (char *) base_addr;
      this->target_ = ((char *) rhs - (char *) base_addr);
    }
}

template <class CONCRETE> ACE_INLINE
ACE_Based_Pointer_Basic<CONCRETE>::ACE_Based_Pointer_Basic (const ACE_Based_Pointer_Basic<CONCRETE> &rhs)
{
  ACE_TRACE ("ACE_Based_Pointer_Basic<CONCRETE>::ACE_Based_Pointer_Basic");

  ACE_ASSERT (!"not implemented");
  void *base_addr = 0;

  // Find the base address associated with the <addr> pointer.  Note
  // that it's ok for <find> to return 0, which simply indicates that
  // the address is not in memory-mapped virtual address space.
  ACE_BASED_POINTER_REPOSITORY::instance ()->find (this,
                                                   base_addr);
  this->base_offset_ = (char *) this - (char *) base_addr;
  this->target_ = ((char *) &rhs - (char *) base_addr);
}

template <class CONCRETE> ACE_INLINE
ACE_Based_Pointer<CONCRETE>::ACE_Based_Pointer (const ACE_Based_Pointer<CONCRETE> &rhs)
  : ACE_Based_Pointer_Basic<CONCRETE> (rhs)
{
  ACE_TRACE ("ACE_Based_Pointer<CONCRETE>::ACE_Based_Pointer");
  ACE_ASSERT (!"not implemented");
}

#endif /* ACE_BASED_POINTER_T_CPP */
