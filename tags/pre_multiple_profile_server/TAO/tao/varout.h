/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    varout.h
//
// = DESCRIPTION
//     Templates for _var and _out types
//
// = AUTHOR
//   Aniruddha Gokhale <gokhale@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_VAROUT_H
#define TAO_VAROUT_H

#include "tao/Object.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class T, class T_var> class TAO_Object_Manager;

template <class T, class T_var>
class TAO_Object_Field_T : public TAO_Object_Field
{
  // = TITLE
  //   Parametric class to implement structure fields corresponding to
  //   object references.
  //
  // = DESCRIPTION
  //   When an object reference appears in a structure the marshaling
  //   and demarhsaling of the structure gets complicated: the
  //   interpreter can only marshal CORBA_Object_ptr when it
  //   dermarshal it creates on of those objects.  The downcasting to
  //   the right type must be executed by classes with compile-time
  //   knowledge of the object type.  The solution addopted in TAO is
  //   to create a special manager class for that field,
public:
  TAO_Object_Field_T (void);
  // default constructor

  TAO_Object_Field_T (T *object);
  // constructor from a T *, assumes ownership of <object> i.e. it
  // *will* release it.

  TAO_Object_Field_T (const T_var &);
  // constructor from a var

  TAO_Object_Field_T (const TAO_Object_Field_T<T,T_var> &);
  // copy constructor

  TAO_Object_Field_T (const TAO_Object_Manager<T,T_var> &);
  // constructor from an Object_Manager (sequence var-like class)

  virtual ~TAO_Object_Field_T (void);
  // destructor

  TAO_Object_Field_T<T,T_var> &operator= (T *object);
  // Assignment from a T*, it assumes ownership of <object>.

  TAO_Object_Field_T<T,T_var> &operator= (const T_var& var);
  // Assignment from a T_var, make a duplicate of the object

  TAO_Object_Field_T<T,T_var> &operator= (const TAO_Object_Field_T<T,T_var> &);
  // Assignment operator, in duplicates the underlying objects.

  TAO_Object_Field_T<T,T_var> &operator= (const TAO_Object_Manager<T,T_var>&);
  // Assignment from a T_var, make a duplicate of the object

  T *operator-> (void) const;
  // smart pointer (const)

  operator T *const &() const;
  // cast operator

  operator T *&();
  // cast operator

  // = operations for parameter passing modes

  T *in (void) const;
  // for in type parameter

  T *&inout (void);
  // for inout type parameter

  T *&out (void);
  // for out parameter

  T *_retn (void);
  // for return type

  T *ptr (void) const;
  // TAO extension for access to the underlying pointer.

  virtual void _downcast (CORBA_Object *base_ptr,
                          CORBA_Environment &ACE_TRY_ENV =
                              TAO_default_environment ());
  // No default to workaround egcs problem with templates and
  // namespaces
  virtual CORBA_Object *_upcast (void);
  virtual void _release (void);
  // Implement the TAO_Object_Field methods.

private:
  T *ptr_;
};

#if defined (__ACE_INLINE__)
#include "tao/varout.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/varout.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("varout.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_VAROUT_H */
