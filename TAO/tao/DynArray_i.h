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
//    Jeff Parsons <jp4@cs.wustl.edu>
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

  // Functions specific to DynArray

  // Assign/dump elements using IDL defined type AnySeq

  AnySeq_ptr get_elements (CORBA::Environment &TAO_IN_ENV);

  void set_elements (const AnySeq& value,
                     CORBA::Environment &TAO_IN_ENV);

  // Functions common to all Dynamic Any types

  void assign (CORBA_DynAny_ptr dyn_any,
               CORBA::Environment &TAO_IN_ENV);
  // Sets the value of the receiver to the value of the argument

  CORBA_DynAny_ptr copy (CORBA::Environment &TAO_IN_ENV);
  // Creates a copy

  void destroy (CORBA::Environment &TAO_IN_ENV);
  // Makes sure the contents are disposed of correctly

  void from_any (const CORBA::Any& any,
                 CORBA::Environment &TAO_IN_ENV);
  // Assigns value from an Any argument

  CORBA::Any_ptr to_any (CORBA::Environment &TAO_IN_ENV);
  // Outputs contents as an Any

  CORBA::TypeCode_ptr type (CORBA::Environment &TAO_IN_ENV);
  // Returns the (constant) type

  CORBA_DynAny_ptr current_component (CORBA::Environment &TAO_IN_ENV);
  // Used in iterating through the contents

  CORBA::Boolean next (CORBA::Environment &TAO_IN_ENV);
  // Returns next component

  CORBA::Boolean seek (CORBA::Long index,
                       CORBA::Environment &TAO_IN_ENV);
  // Jump to component at <index>

  void rewind (CORBA::Environment &TAO_IN_ENV);
  // Makes first component the current one

  // Insert and get functions

  void insert_boolean (CORBA::Boolean value,
                       CORBA::Environment &TAO_IN_ENV);
  void insert_octet (CORBA::Octet value,
                     CORBA::Environment &TAO_IN_ENV);
  void insert_char (CORBA::Char value,
                    CORBA::Environment &TAO_IN_ENV);
  void insert_short (CORBA::Short value,
                     CORBA::Environment &TAO_IN_ENV);
  void insert_ushort (CORBA::UShort value,
                      CORBA::Environment &TAO_IN_ENV);
  void insert_long (CORBA::Long value,
                    CORBA::Environment &TAO_IN_ENV);
  void insert_ulong (CORBA::ULong value,
                     CORBA::Environment &TAO_IN_ENV);
  void insert_float (CORBA::Float value,
                     CORBA::Environment &TAO_IN_ENV);
  void insert_double (CORBA::Double value,
                      CORBA::Environment &TAO_IN_ENV);
  void insert_string (const char * value,
                      CORBA::Environment &TAO_IN_ENV);
  void insert_reference (CORBA::Object_ptr value,
                         CORBA::Environment &TAO_IN_ENV);
  void insert_typecode (CORBA::TypeCode_ptr value,
                        CORBA::Environment &TAO_IN_ENV);
  void insert_longlong (CORBA::LongLong value,
                        CORBA::Environment &TAO_IN_ENV);
  void insert_ulonglong (CORBA::ULongLong value,
                         CORBA::Environment &TAO_IN_ENV);
  void insert_wchar (CORBA::WChar value,
                      CORBA::Environment &TAO_IN_ENV);
  void insert_any (const CORBA::Any& value,
                   CORBA::Environment &TAO_IN_ENV);
  CORBA::Boolean get_boolean (CORBA::Environment &TAO_IN_ENV);

  CORBA::Octet get_octet (CORBA::Environment &TAO_IN_ENV);

  CORBA::Char get_char (CORBA::Environment &TAO_IN_ENV);

  CORBA::Short get_short (CORBA::Environment &TAO_IN_ENV);

  CORBA::UShort get_ushort (CORBA::Environment &TAO_IN_ENV);

  CORBA::Long get_long (CORBA::Environment &TAO_IN_ENV);

  CORBA::ULong get_ulong (CORBA::Environment &TAO_IN_ENV);

  CORBA::Float get_float (CORBA::Environment &TAO_IN_ENV);

  CORBA::Double get_double (CORBA::Environment &TAO_IN_ENV);

  char * get_string (CORBA::Environment &TAO_IN_ENV);

  CORBA::Object_ptr get_reference (CORBA::Environment &TAO_IN_ENV);

  CORBA::TypeCode_ptr get_typecode (CORBA::Environment &TAO_IN_ENV);

  CORBA::LongLong get_longlong (CORBA::Environment &TAO_IN_ENV);

  CORBA::ULongLong get_ulonglong (CORBA::Environment &TAO_IN_ENV);

  CORBA::WChar get_wchar (CORBA::Environment &TAO_IN_ENV);

  CORBA::Any_ptr get_any (CORBA::Environment &TAO_IN_ENV);

private:
  CORBA::TypeCode_ptr get_element_type (CORBA::Environment &TAO_IN_ENV);
  // Returns the type of elements contained in the array.

  CORBA::ULong get_arg_length (CORBA::TypeCode_ptr tc,
                               CORBA::Environment &TAO_IN_ENV);
  // Gets the length of the array from the typecode

  CORBA::TypeCode_var type_;
  // Holds the typecode of the array

  CORBA::Long index_;
  // Index of the current component

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
