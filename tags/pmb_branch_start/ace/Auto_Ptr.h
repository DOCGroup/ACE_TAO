/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Auto_Ptr.h
 *
 *  $Id$
 *
 *  @author Doug Schmidt <schmidt@uci.edu>
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 *  @author Jack Reeves <jack@fx.com>
 *  @author Dr. Harald M. Mueller <mueller@garwein.hai.siemens.co.at>
 */
//=============================================================================

#ifndef ACE_AUTO_PTR_H
#define ACE_AUTO_PTR_H
#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Trace.h"

#if defined (_MSC_VER)
// Suppress warning e.g. "return type for
// 'ACE_Auto_Array_Pointer<type>::operator ->' is 'type *' (i.e., not a UDT
// or reference to a UDT.  Will produce errors if applied using infix
// notation)"
#  pragma warning(push)
#  pragma warning(disable: 4284)
#endif /* _MSC_VER */

/**
 * @class ACE_Auto_Basic_Ptr
 *
 * @brief Implements the draft C++ standard auto_ptr abstraction.
 * This class allows one to work on non-object (basic) types
 */
template <class X>
class ACE_Auto_Basic_Ptr
{
public:
  // = Initialization and termination methods
  ACE_EXPLICIT ACE_Auto_Basic_Ptr (X *p = 0) : p_ (p) {}

  ACE_Auto_Basic_Ptr (ACE_Auto_Basic_Ptr<X> &ap);
  ACE_Auto_Basic_Ptr<X> &operator= (ACE_Auto_Basic_Ptr<X> &rhs);
  ~ACE_Auto_Basic_Ptr (void);

  // = Accessor methods.
  X &operator *() const;
  X *get (void) const;
  X *release (void);
  void reset (X *p = 0);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  X *p_;
};

#if !defined (ACE_LACKS_AUTO_PTR) && \
     defined (ACE_HAS_STANDARD_CPP_LIBRARY) && \
            (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#include <memory>
#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) && \
            (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB != 0)
using std::auto_ptr;
#endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
#else /* ACE_HAS_STANDARD_CPP_LIBRARY */

/**
 * @class auto_ptr
 *
 * @brief Implements the draft C++ standard auto_ptr abstraction.
 */
template <class X>
class auto_ptr : public ACE_Auto_Basic_Ptr <X>
{
public:
  // = Initialization and termination methods
  ACE_EXPLICIT auto_ptr (X *p = 0) : ACE_Auto_Basic_Ptr<X> (p) {}

  X *operator-> () const;
};

#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

/**
 * @class ACE_Auto_Basic_Array_Ptr
 *
 * @brief Implements an extension to the draft C++ standard auto_ptr
 * abstraction.  This class allows one to work on non-object
 * (basic) types that must be treated as an array, e.g.,
 * deallocated via "delete [] foo".
 */
template<class X>
class ACE_Auto_Basic_Array_Ptr
{
public:
  // = Initialization and termination methods.
  ACE_EXPLICIT ACE_Auto_Basic_Array_Ptr (X *p = 0) : p_ (p) {}

  ACE_Auto_Basic_Array_Ptr (ACE_Auto_Basic_Array_Ptr<X> &ap);
  ACE_Auto_Basic_Array_Ptr<X> &operator= (ACE_Auto_Basic_Array_Ptr<X> &rhs);
  ~ACE_Auto_Basic_Array_Ptr (void);

  // = Accessor methods.
  X &operator* () const;
  X &operator[] (int i) const;
  X *get (void) const;
  X *release (void);
  void reset (X *p = 0);

  /// Dump the state of an object.
  void dump (void) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

protected:
  X *p_;
};

/**
 * @class ACE_Auto_Array_Ptr
 *
 * @brief Implements an extension to the draft C++ standard auto_ptr
 * abstraction.
 */
template<class X>
class ACE_Auto_Array_Ptr : public ACE_Auto_Basic_Array_Ptr<X>
{
public:
  // = Initialization and termination methods.
  ACE_EXPLICIT ACE_Auto_Array_Ptr (X *p = 0)
    : ACE_Auto_Basic_Array_Ptr<X> (p) {}

  X *operator-> () const;
};

// Some platforms have an older version of auto_ptr
// support, which lacks reset, and cannot be disabled
// easily.  Portability to these platforms requires
// use of the following ACE_AUTO_PTR_RESET macro.
# if defined (ACE_AUTO_PTR_LACKS_RESET)
#   define ACE_AUTO_PTR_RESET(X,Y,Z) \
      do { \
        if (Y != X.get ()) \
          { \
            X.release (); \
            X = auto_ptr<Z> (Y); \
          } \
      } while (0)
# else /* ! ACE_AUTO_PTR_LACKS_RESET */
#   define ACE_AUTO_PTR_RESET(X,Y,Z) \
      do { \
         X.reset (Y); \
      } while (0)
# endif /* ACE_AUTO_PTR_LACKS_RESET */

#if defined (__ACE_INLINE__)
#include "ace/Auto_Ptr.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Auto_Ptr.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Auto_Ptr.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if defined (_MSC_VER)
// Restore the warning state to what it was before entry.
#  pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* ACE_AUTO_PTR_H */
