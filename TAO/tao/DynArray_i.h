/* -*- C++ -*- */
// $Id$

// ===================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynArray_i.h
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ===================================================================

#ifndef TAO_DYNARRAY_I_H
#define TAO_DYNARRAY_I_H

#include "tao/orbconf.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_DynArray_i : public POA_CORBA::DynArray
{
  // = TITLE
  //    TAO_DynArray_i
  //
  // = DESCRIPTION
  //    Implementation of Dynamic Any type for arrays
  //
public:
  TAO_DynArray_i      (const CORBA_Any& any);
  // constructor taking an Any argument

  TAO_DynArray_i (CORBA_TypeCode_ptr tc);
  // constructor taking a typecode argument

  ~TAO_DynArray_i (void);
  // destructor

  // = Functions specific to DynArray

  CORBA_AnySeq_ptr get_elements (CORBA::Environment &ACE_TRY_ENV =
                                   TAO_default_environment ());
  // Assign/dump elements using IDL defined type AnySeq

  void set_elements (const CORBA_AnySeq& value,
                     CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ());

  // = Functions common to all Dynamic Any types

  void assign (CORBA_DynAny_ptr dyn_any,
               CORBA::Environment &ACE_TRY_ENV =
                 TAO_default_environment ());
  // Sets the value of the receiver to the value of the argument.

  CORBA_DynAny_ptr copy (CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ());
  // Creates a copy

  void destroy (CORBA::Environment &ACE_TRY_ENV =
                  TAO_default_environment ());
  // Makes sure the contents are disposed of correctly

  void from_any (const CORBA::Any& any,
                 CORBA::Environment &ACE_TRY_ENV =
                   TAO_default_environment ());
  // Assigns value from an Any argument

  CORBA::Any_ptr to_any (CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ());
  // Outputs contents as an Any

  CORBA::TypeCode_ptr type (CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ());
  // Returns the (constant) type

  CORBA_DynAny_ptr current_component (CORBA::Environment &ACE_TRY_ENV =
                                        TAO_default_environment ());
  // Used in iterating through the contents.

  CORBA::Boolean next (CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  // Returns next component

  CORBA::Boolean seek (CORBA::Long slot,
                       CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  // Jump to component at <slot>

  void rewind (CORBA::Environment &ACE_TRY_ENV =
                 TAO_default_environment ());
  // Makes first component the current one

  // Insert and get functions

  void insert_boolean (CORBA::Boolean value,
                       CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  void insert_octet (CORBA::Octet value,
                     CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ());
  void insert_char (CORBA::Char value,
                    CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  void insert_short (CORBA::Short value,
                     CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ());
  void insert_ushort (CORBA::UShort value,
                      CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
  void insert_long (CORBA::Long value,
                    CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  void insert_ulong (CORBA::ULong value,
                     CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ());
  void insert_float (CORBA::Float value,
                     CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ());
  void insert_double (CORBA::Double value,
                      CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
  void insert_string (const char * value,
                      CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
  void insert_reference (CORBA::Object_ptr value,
                         CORBA::Environment &ACE_TRY_ENV =
                                   TAO_default_environment ());
  void insert_typecode (CORBA::TypeCode_ptr value,
                        CORBA::Environment &ACE_TRY_ENV =
                          TAO_default_environment ());
  void insert_longlong (CORBA::LongLong value,
                        CORBA::Environment &ACE_TRY_ENV =
                          TAO_default_environment ());
  void insert_ulonglong (CORBA::ULongLong value,
                         CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ());
  void insert_wchar (CORBA::WChar value,
                      CORBA::Environment &ACE_TRY_ENV =
                        TAO_default_environment ());
  void insert_any (const CORBA::Any& value,
                   CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  CORBA::Boolean get_boolean (CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ());
  CORBA::Octet get_octet (CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ());
  CORBA::Char get_char (CORBA::Environment &ACE_TRY_ENV =
                          TAO_default_environment ());
  CORBA::Short get_short (CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ());
  CORBA::UShort get_ushort (CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ());
  CORBA::Long get_long (CORBA::Environment &ACE_TRY_ENV =
                          TAO_default_environment ());
  CORBA::ULong get_ulong (CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ());
  CORBA::Float get_float (CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ());
  CORBA::Double get_double (CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ());
  char *get_string (CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());
  CORBA::Object_ptr get_reference (CORBA::Environment &ACE_TRY_ENV =
                                     TAO_default_environment ());
  CORBA::TypeCode_ptr get_typecode (CORBA::Environment &ACE_TRY_ENV =
                                      TAO_default_environment ());
  CORBA::LongLong get_longlong (CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ());
  CORBA::ULongLong get_ulonglong (CORBA::Environment &ACE_TRY_ENV =
                                    TAO_default_environment ());
  CORBA::WChar get_wchar (CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ());
  CORBA::Any_ptr get_any (CORBA::Environment &ACE_TRY_ENV =
                            TAO_default_environment ());

private:
  CORBA::TypeCode_ptr get_element_type (CORBA::Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());
  // Returns the type of elements contained in the array.

  CORBA::ULong get_arg_length (CORBA::TypeCode_ptr tc,
                               CORBA::Environment &ACE_TRY_ENV =
                                 TAO_default_environment ());
  // Gets the length of the array from the typecode

  CORBA::TypeCode_var type_;
  // Holds the typecode of the array

  CORBA::Long current_index_;
  // Slot of the current component

  ACE_Array_Base<CORBA_DynAny_var> da_members_;
  // Each component is also a DynAny

  TAO_DynArray_i (void);
  // Must be instantiated with at least a typecode

  // Use copy() or assign() instead of these
  TAO_DynArray_i (const TAO_DynArray_i &src);
  TAO_DynArray_i &operator= (const TAO_DynArray_i &src);
};

#endif /* TAO_HAS_MINIMUM_CORBA */

#endif /* TAO_DYNARRAY_I_H */
