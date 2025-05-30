// -*- C++ -*-

//=============================================================================
/**
 *  @file    Based_Pointer_T.h
 *
 *  @author Dietrich Quehl <Dietrich.Quehl@med.siemens.de>
 *  @author Douglas C. Schmidt <schmidt@.cs.wustl.edu>
 */
//=============================================================================

#ifndef ACE_BASED_POINTER_T_H
#define ACE_BASED_POINTER_T_H

#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"
#include "ace/Basic_Types.h"

#if defined (_MSC_VER)
// Suppress warning e.g. "return type for
// 'ACE_Based_Pointer<long>::operator ->' is 'long *' (i.e., not a UDT
// or reference to a UDT.  Will produce errors if applied using infix
// notation)"
#pragma warning(disable: 4284)
#endif /* _MSC_VER */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Based_Pointer_Basic
 *
 * @brief A proxy that keeps track of the relative offset of a "pointer"
 * from its base address.
 * This class makes it possible to transparently use "pointers" in
 * shared memory as easily as programming with pointers to local
 * memory.  In particular, we don't need to ensure that the base
 * addresses of all the pointers are mapped into separate
 * processes at the same absolute memory base address.
 */
template <class CONCRETE>
class ACE_Based_Pointer_Basic
{
public:
  /**
   * This constructor initializes the <base_offset_> by asking the
   * <ACE_BASED_POINTER_REPOSITORY> Singleton for the base address of
   * the memory region within which it is instantiated.  Two results
   * are possible:
   *
   * 1. An <ACE_*_Memory_Pool> has stored a base address/size pair and the
   *    new based-pointer instance is located between the base address and
   *    the base address + size - 1.  In this case, the repository
   *    returns the base address.
   *
   * 2. No suitable address/size pair was found.  The repository
   *    assumes an address in the regular (not mapped) virtual address
   *    space of the process and returns 0.  In this case, the
   *    based-pointer uses its address as an offset to it's base
   *    address 0.
   */
  ACE_Based_Pointer_Basic ();

  /**
   * Initialize this object using the @a initial pointer.  This
   * constructor initializes the <base_offset_> by asking the
   * <ACE_BASED_POINTER_REPOSITORY> Singleton for the base address of
   * the memory region within which it is instantiated.  Three results
   * are possible:
   *
   * 1. An <ACE_*_Memory_Pool> has stored a base address/size pair and the
   *    new based-pointer instance is located between the base address and
   *    the base address + size - 1.  In this case, the repository
   *    returns the base address.
   *
   * 2. No suitable address/size pair was found.  The repository
   *    assumes an address in the regular (not mapped) virtual address
   *    space of the process and returns 0.  In this case, the
   *    based-pointer uses its address as an offset to its base
   *    address 0.
   *
   * 3. If @a initial is 0 then set the value of <target_> to -1, which
   *    indicates a "NULL" pointer.
   */
  ACE_Based_Pointer_Basic (CONCRETE *initial);

  ACE_Based_Pointer_Basic (const ACE_Based_Pointer_Basic &) = delete;
  ACE_Based_Pointer_Basic (ACE_Based_Pointer_Basic &&) = delete;

  /// Constructor for know base address. @a o is only used to
  /// resolve overload ambiguity.
  ACE_Based_Pointer_Basic (const void *base_addr, int o);

  void operator= (CONCRETE *from);

  void operator= (const ACE_Based_Pointer_Basic &);

  CONCRETE operator * () const;

  bool operator< (const ACE_Based_Pointer_Basic &) const;

  bool operator<= (const ACE_Based_Pointer_Basic &) const;

  bool operator> (const ACE_Based_Pointer_Basic &) const;

  bool operator>= (const ACE_Based_Pointer_Basic &) const;

  bool operator== (const ACE_Based_Pointer_Basic &) const;

  bool operator!= (const ACE_Based_Pointer_Basic &) const;

  CONCRETE operator[] (int index) const;

  void operator+= (int index);

  /// Returns the underlying memory address of the smart pointer.
  operator CONCRETE * () const;

  /// Returns the underlying memory address of the smart pointer.
  CONCRETE *addr () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

  /// Dump the state of the object.
  void dump () const;

protected:
  ptrdiff_t target_;

  /// Keep track of our offset from the base pointer.
  ptrdiff_t base_offset_;

  /// Convert input pointer to CONCRETE using the base_offset_ and target_ adjustments
  static CONCRETE *based (ACE_Based_Pointer_Basic *p);

  /// Convert input pointer to CONCRETE using the base_offset_ and target_ adjustments
  static CONCRETE const *based (ACE_Based_Pointer_Basic const *p);
};

/**
 * @class ACE_Based_Pointer
 *
 * @brief A smart proxy that keeps track of the relative offset of a
 * "pointer" from its base address.
 *
 * This class makes it possible to transparently use "pointers" in
 * shared memory as easily as programming with pointers to local
 * memory by overloading the C++ delegation operator ->().
 */
template <class CONCRETE>
class ACE_Based_Pointer : public ACE_Based_Pointer_Basic<CONCRETE>
{
public:
  /// Constructor.  See constructor for ACE_Based_Pointer_Basic for
  /// details.
  ACE_Based_Pointer ();

  /// Initialize this object using the <initial> pointer.  See
  /// constructor for ACE_Based_Pointer_Basic for details.
  ACE_Based_Pointer (CONCRETE *initial);

  /// Initialize this object with known @a base_addr.  @a dummy is
  /// a dummy value used to resolve overload ambiguity and it
  /// otherwise ignored.
  ACE_Based_Pointer (const void *base_addr, int dummy);

  ACE_Based_Pointer (const ACE_Based_Pointer &) = delete;
  ACE_Based_Pointer (ACE_Based_Pointer &&) = delete;

  void operator= (const ACE_Based_Pointer &);

  /// Pseudo-assignment operator.
  void operator = (CONCRETE *from);

  CONCRETE *operator-> ();
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Based_Pointer_T.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Based_Pointer_T.cpp"

#include /**/ "ace/post.h"

#endif /* ACE_BASED_POINTER_T_H */
