/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Based_Pointer_T.h
//
// = AUTHOR
//    Dietrich Quehl <Dietrich.Quehl@med.siemens.de> and
//    Douglas C. Schmidt <schmidt@.cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_BASED_POINTER_T_H
#define ACE_BASED_POINTER_T_H

#if defined (_MSC_VER)
// Suppress warning e.g. "return type for
// 'ACE_Based_Pointer<long>::operator ->' is 'long *' (i.e., not a UDT
// or reference to a UDT.  Will produce errors if applied using infix
// notation)"
#pragma warning(disable: 4284)  
#endif /* _MSC_VER */

template <class CONCRETE> 
class ACE_Based_Pointer
{
  // = TITLE
  //   A proxy that keeps track of the relative offset of a "pointer"
  //   from its base address. 
  //
  // = DESCRIPTION
  //   This class makes it possible to transparently use "pointers" in
  //   shared memory as easily as programming with pointers to local
  //   memory.  In particular, we don't need to ensure that the base
  //   addresses of all the pointers are mapped into separate
  //   processes at the same absolute memory base address.
public:
  // = Initialization method.
  ACE_Based_Pointer (void);
  // This constructor initializes the <base_offset_> by asking the
  // <ACE_BASED_POINTER_REPOSITORY> Singleton for the base address of
  // the memory region within which it is instantiated.  Two results
  // are possible:
  // 
  // 1. An <ACE_*_Memory_Pool> has stored a base address/size pair and the
  //    new based-pointer instance is located between the base address and
  //    the base address + size - 1.  In this case, the repository
  //    returns the base address.
  //
  // 2. No suitable address/size pair was found.  The repository
  //    assumes an address in the regular (not mapped) virtual address
  //    space of the process and returns 0.  In this case, the
  //    based-pointer uses its address as an offset to it's base
  //    address 0.

  CONCRETE *operator->(void);
  // The C++ "delegation operator".

  CONCRETE *operator =(CONCRETE *from);
  // Pseudo-assignment operator.

  CONCRETE operator *(void);
  // Dereference operator.

  CONCRETE operator [](int index);
  // Subscript operator.

private:
  CONCRETE *target_;

  long base_offset_;
  // Keep track of our offset from the base pointer.
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Based_Pointer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Based_Pointer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_BASED_POINTER_T_H */
