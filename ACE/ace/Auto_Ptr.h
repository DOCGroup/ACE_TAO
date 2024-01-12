/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Auto_Ptr.h
 *
 *  @author Doug Schmidt <d.schmidt@vanderbilt.edu>
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 *  @author Jack Reeves <jack@fx.com>
 *  @author Dr. Harald M. Mueller <mueller@garwein.hai.siemens.co.at>
 */
//=============================================================================

#ifndef ACE_AUTO_PTR_H
#define ACE_AUTO_PTR_H
#include /**/ "ace/pre.h"

#include /**/ "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// C++17 removed std::auto_ptr<>, so also disable the ACE versions when
// using C++17.
#if !defined (ACE_HAS_CPP17)

#if defined (_MSC_VER)
// Suppress warning e.g. "return type for
// 'ACE_Auto_Array_Pointer<type>::operator ->' is 'type *' (i.e., not a UDT
// or reference to a UDT.  Will produce errors if applied using infix
// notation)"
#  pragma warning(push)
#  pragma warning(disable: 4284)
#endif /* _MSC_VER */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class ACE_Auto_Basic_Ptr
 *
 * @brief Implements the draft C++ standard auto_ptr abstraction.
 * This class allows one to work on non-object (basic) types
 */
template <typename X>
class ACE_Auto_Basic_Ptr
{
public:
  typedef X element_type;

  explicit ACE_Auto_Basic_Ptr (X * p = nullptr) : p_ (p) {}

  ACE_Auto_Basic_Ptr (ACE_Auto_Basic_Ptr<X> & ap);
  ACE_Auto_Basic_Ptr<X> &operator= (ACE_Auto_Basic_Ptr<X> & rhs);
  ~ACE_Auto_Basic_Ptr ();

  // = Accessor methods.
  X &operator *() const;
  X *get () const;
  X *release ();
  void reset (X * p = nullptr);

  /// Dump the state of an object.
  void dump () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  X *p_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if !defined (ACE_LACKS_AUTO_PTR)
#  include <memory>
using std::auto_ptr;
#else /* !ACE_LACKS_AUTO_PTR */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class auto_ptr
 *
 * @brief Implements the draft C++ standard auto_ptr abstraction.
 */
template <typename X>
class auto_ptr : public ACE_Auto_Basic_Ptr<X>
{
public:
  typedef X element_type;

  explicit auto_ptr (X * p = 0) : ACE_Auto_Basic_Ptr<X> (p) {}
  auto_ptr (auto_ptr<X> & ap) : ACE_Auto_Basic_Ptr<X> (ap.release ()) {}

  X *operator-> () const;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* !ACE_LACKS_AUTO_PTR */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @brief Implements the draft C++ standard auto_ptr abstraction.
 * This version can be used instead of auto_ptr<T>
 */
template <typename X>
class ACE_Auto_Ptr : public ACE_Auto_Basic_Ptr <X>
{
public:
  typedef X element_type;

  explicit ACE_Auto_Ptr (X * p = 0) : ACE_Auto_Basic_Ptr<X> (p) {}

  X *operator-> () const;
};

/**
 * @class ACE_Auto_Basic_Array_Ptr
 *
 * @brief Implements an extension to the draft C++ standard auto_ptr
 * abstraction.  This class allows one to work on non-object
 * (basic) types that must be treated as an array, e.g.,
 * deallocated via "delete [] foo".
 */
template<typename X>
class ACE_Auto_Basic_Array_Ptr
{
public:
  typedef X element_type;

  explicit ACE_Auto_Basic_Array_Ptr (X * p = 0) : p_ (p) {}

  ACE_Auto_Basic_Array_Ptr (ACE_Auto_Basic_Array_Ptr<X> & ap);
  ACE_Auto_Basic_Array_Ptr<X> &operator= (ACE_Auto_Basic_Array_Ptr<X> & rhs);
  ~ACE_Auto_Basic_Array_Ptr ();

  // = Accessor methods.
  X & operator* () const;
  X & operator[] (int i) const;
  X * get () const;
  X * release ();
  void reset (X * p = 0);

  /// Dump the state of an object.
  void dump () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  X * p_;
};

/**
 * @class ACE_Auto_Array_Ptr
 *
 * @brief Implements an extension to the draft C++ standard auto_ptr
 * abstraction.
 */
template<typename X>
class ACE_Auto_Array_Ptr : public ACE_Auto_Basic_Array_Ptr<X>
{
public:
  typedef X element_type;

  explicit ACE_Auto_Array_Ptr (X *p = 0)
    : ACE_Auto_Basic_Array_Ptr<X> (p) {}

  X *operator-> () const;
};


/**
 * @brief Reset given @c auto_ptr element to new element.
 *
 * Some platforms have an older version of auto_ptr support, which
 * lacks reset, and cannot be disabled easily.  Portability to these
 * platforms requires use of this function template.  This function
 * template also works for the @c ACE_Auto_{Basic_}Array_Ptr class
 * template, as well.
 */
template<typename AUTO_PTR_TYPE, typename PTR_TYPE>
inline void
ACE_auto_ptr_reset (AUTO_PTR_TYPE & ap, PTR_TYPE * p)
{
  ap.reset (p);
}

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Auto_Ptr.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Auto_Ptr.cpp"

#if defined (_MSC_VER)
// Restore the warning state to what it was before entry.
#  pragma warning(pop)
#endif /* _MSC_VER */

#endif /* ACE_HAS_CPP17 */

#include /**/ "ace/post.h"
#endif /* ACE_AUTO_PTR_H */
