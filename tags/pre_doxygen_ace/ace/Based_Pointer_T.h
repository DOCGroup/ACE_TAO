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
#include "ace/pre.h"

#include "ace/OS.h"              // Need ACE_Export

#if defined (_MSC_VER)
// Suppress warning e.g. "return type for
// 'ACE_Based_Pointer<long>::operator ->' is 'long *' (i.e., not a UDT
// or reference to a UDT.  Will produce errors if applied using infix
// notation)"
#pragma warning(disable: 4284)
#endif /* _MSC_VER */

template <class CONCRETE>
class ACE_Based_Pointer_Basic
{
  // = TITLE
  //   A proxy that keeps track of the relative offset of a "pointer"
  //   from its base address.
  //
  //   This class makes it possible to transparently use "pointers" in
  //   shared memory as easily as programming with pointers to local
  //   memory.  In particular, we don't need to ensure that the base
  //   addresses of all the pointers are mapped into separate
  //   processes at the same absolute memory base address.
public:
  ACE_Based_Pointer_Basic (void);
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

  ACE_Based_Pointer_Basic (CONCRETE *initial);
  // Initialize this object using the <initial> pointer.  This
  // constructor initializes the <base_offset_> by asking the
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

  ACE_Based_Pointer_Basic (const ACE_Based_Pointer_Basic<CONCRETE> &);
  // Copy constructor.

  ACE_Based_Pointer_Basic (const void *base_addr, int o);
  // Constructor for know base address. <o> is only used to
  // resolve overload ambiguity.

  void operator = (CONCRETE *from);
  // Pseudo-assignment operator.

  void operator = (const ACE_Based_Pointer_Basic<CONCRETE> &);
  // Pseudo-assignment operator.

  CONCRETE operator * (void) const;
  // Dereference operator.

  int operator < (const ACE_Based_Pointer_Basic<CONCRETE> &) const;
  // Less than operator.

  int operator <= (const ACE_Based_Pointer_Basic<CONCRETE> &) const;
  // Less than or equal operator.

  int operator > (const ACE_Based_Pointer_Basic<CONCRETE> &) const;
  // Greater than operator.

  int operator >= (const ACE_Based_Pointer_Basic<CONCRETE> &) const;
  // Greater than or equal operator.

  int operator == (const ACE_Based_Pointer_Basic<CONCRETE> &) const;
  // Equality operator.

  int operator != (const ACE_Based_Pointer_Basic<CONCRETE> &) const;
  // Inequality operator.

  CONCRETE operator [](int index) const;
  // Subscript operator.

  void operator+= (int index);
  // Increment operator.

  operator CONCRETE *() const;
  // Returns the underlying memory address of the smart pointer.

  CONCRETE *addr (void) const;
  // Returns the underlying memory address of the smart pointer.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

  void dump (void) const;
  // Dump the state of the object.

protected:
  long target_;

  long base_offset_;
  // Keep track of our offset from the base pointer.
};

template <class CONCRETE>
class ACE_Based_Pointer : public ACE_Based_Pointer_Basic<CONCRETE>
{
  // = TITLE
  //   A smart proxy that keeps track of the relative offset of a
  //   "pointer" from its base address.
  //
  // = DESCRIPTION
  //   This class makes it possible to transparently use "pointers" in
  //   shared memory as easily as programming with pointers to local
  //   memory by overloading the C++ delegation operator ->().
public:
  // = Initialization method.
  ACE_Based_Pointer (void);
  // Constructor.  See constructor for <ACE_Based_Pointer_Basic> for
  // details.

  ACE_Based_Pointer (CONCRETE *initial);
  // Initialize this object using the <initial> pointer.

  ACE_Based_Pointer (const void *base_addr, int o);
  // Initialize this object with known <base_addr>.  <o> is
  // only used to resolve overload ambiguity.

  ACE_Based_Pointer (const ACE_Based_Pointer<CONCRETE> &);
  // Copy constructor (not implemented yet).

  void operator = (const ACE_Based_Pointer<CONCRETE> &);
  // Assignment operator.

  void operator = (CONCRETE *from);
  // Pseudo-assignment operator.

  CONCRETE *operator-> (void);
  // The C++ "delegation operator".
};

#if defined (__ACE_INLINE__)
#include "ace/Based_Pointer_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Based_Pointer_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Based_Pointer_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* ACE_BASED_POINTER_T_H */
