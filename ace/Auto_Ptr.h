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
class auto_basic_ptr 
  // = TITLE
  //     Implements the draft C++ standard auto_ptr abstraction.
  //     This class allows one to work on non-object (basic) types
{
public:
  // = Initialization and termination methods
  auto_basic_ptr (X *p = 0);
  auto_basic_ptr (auto_basic_ptr<X> &ap);
  ~auto_basic_ptr (void);
  void operator= (auto_basic_ptr<X> &rhs);

  // = Accessor methods.
  X &operator *() const;
  X *get (void) const;
  X *release (void);
  X *reset (X *p);

  static void remove (X* &x);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  X *p_;
};

template <class X>
class auto_ptr : public auto_basic_ptr <X>
  // = TITLE
  //     Implements the draft C++ standard auto_ptr abstraction.
{
public:
  // = Initialization and termination methods
  auto_ptr (X *p = 0);

  X *operator-> () const;  
};


template<class X>
class auto_basic_array_ptr 
  // = TITLE
  //     Implements an extension to the draft C++ standard auto_ptr
  //     abstraction.  This class allows one to work on non-object
  //     (basic) types
{
public:
  // = Initialization and termination methods.
  auto_basic_array_ptr (X *p = 0);
  auto_basic_array_ptr (auto_basic_array_ptr<X> &ap);
  ~auto_basic_array_ptr (void);
  void operator= (auto_basic_array_ptr<X> &rhs);

  // = Accessor methods.
  X &operator* ();
  X &operator[] (int i);
  X operator[] (int i) const;
  X *get (void) const;
  X *release (void);
  X *reset (X *p);

  static void remove (X *&x);

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  X *p_;
};

template<class X>
class auto_array_ptr : public auto_basic_array_ptr 
  // = TITLE
  //     Implements an extension to the draft C++ standard auto_ptr
  //     abstraction.
{
public:
  // = Initialization and termination methods.
  auto_array_ptr (X *p = 0);

  X *operator-> ();
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
