/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Based_Pointer_Repository_T.h
//
// = AUTHOR
//    Dietrich Quehl <Dietrich.Quehl@med.siemens.de> and
//    Douglas C. Schmidt <schmidt@.cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_BASED_POINTER_REPOSITORY_T_H)
#define ACE_BASED_POINTER_REPOSITORY_T_H

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
  //   This class makes it possible to transparently use "pointers"
  //   in shared memory without having to ensure that the base
  //   addresses of all the pointers are mapped into separate
  //   processes at the same absolute memory base address.
public:
  // = Initialization method.
  ACE_Based_Pointer (void);
  // This constructor initializes the <base_offset_> by computing the
  // difference between <this> and the base address of the region that
  // <this> is contained within.

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
#include "ace/Based_Pointer_Repository_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Based_Pointer_Repository_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_BASED_POINTER_REPOSITORY_T_H */
