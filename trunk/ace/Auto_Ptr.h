/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Auto_Ptr.h
//
// = AUTHOR
//    Doug Schmidt, based on code from Jack Reeves (jack@fx.com) and 
//    Dr. Harald M. Mueller (mueller@garwein.hai.siemens.co.at)
// 
// ============================================================================

#if !defined (ACE_AUTO_PTR_H)
#define ACE_AUTO_PTR_H

#include "ace/ACE.h"

template <class X>
class ACE_Auto_Basic_Ptr 
  // = TITLE
  //     Implements the draft C++ standard auto_ptr abstraction.
  //     This class allows one to work on non-object (basic) types
{
public:
  // = Initialization and termination methods
  /* explicit */ ACE_Auto_Basic_Ptr (X *p = 0);
  ACE_Auto_Basic_Ptr (const ACE_Auto_Basic_Ptr<X> &ap);
  ACE_Auto_Basic_Ptr<X> &operator= (const ACE_Auto_Basic_Ptr<X> &rhs);
  ~ACE_Auto_Basic_Ptr (void);

  // = Accessor methods.
  X &operator *() const;
  X *get (void) const;
  X *release (void) const;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  X *p_;
};

#if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && \
            (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#include <memory>
#if defined (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB) && \
            (ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB != 0)
using std::auto_ptr;
#endif /* ACE_USES_STD_NAMESPACE_FOR_STDCPP_LIB */
#else /* ACE_HAS_STANDARD_CPP_LIBRARY */

template <class X>
class auto_ptr : public ACE_Auto_Basic_Ptr <X>
  // = TITLE
  //     Implements the draft C++ standard auto_ptr abstraction.
{
public:
  // = Initialization and termination methods
  /* explicit */ auto_ptr (X *p = 0);

  X *operator-> () const;  
};

#endif /* ACE_HAS_STANDARD_CPP_LIBRARY */

template<class X>
class ACE_Auto_Basic_Array_Ptr 
  // = TITLE
  //     Implements an extension to the draft C++ standard auto_ptr
  //     abstraction.  This class allows one to work on non-object
  //     (basic) types
{
public:
  // = Initialization and termination methods.
  /* explicit */ ACE_Auto_Basic_Array_Ptr (X *p = 0);
  ACE_Auto_Basic_Array_Ptr (const ACE_Auto_Basic_Array_Ptr<X> &ap);
  ACE_Auto_Basic_Array_Ptr<X> &operator= (const ACE_Auto_Basic_Array_Ptr<X> &rhs);
  ~ACE_Auto_Basic_Array_Ptr (void);
  
  // = Accessor methods.
  X &operator* () const;
  X &operator[] (int i) const;
  X *get (void) const;
  X *release (void) const;

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  X *p_;
};

template<class X>
class ACE_Auto_Array_Ptr : public ACE_Auto_Basic_Array_Ptr<X>
  // = TITLE
  //     Implements an extension to the draft C++ standard auto_ptr
  //     abstraction.
{
public:
  // = Initialization and termination methods.
  /* explicit */ ACE_Auto_Array_Ptr (X *p = 0);

  X *operator-> () const;
};

#if defined (__ACE_INLINE__)
#include "ace/Auto_Ptr.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Auto_Ptr.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Auto_Ptr.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_AUTO_PTR_H */
