/* -*- C++ -*- */
// $Id$

// ========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynStruct_i.h
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_DYNSTRUCT_I_H
#define TAO_DYNSTRUCT_I_H

#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_DynStruct_i : public POA_CORBA::DynStruct
{
  // = TITLE
  //    TAO_DynStruct_i
  //
  // = DESCRIPTION
  //    Implementation of Dynamic Any type for Structs
  //
public:
  TAO_DynStruct_i (const CORBA_Any& any);
  // constructor taking an Any argument

  TAO_DynStruct_i (CORBA_TypeCode_ptr tc);
  // constructor taking a typecode argument

  ~TAO_DynStruct_i (void);
  // destructor

  // Functions specific to DynStruct

  FieldName current_member_name (CORBA::Environment &TAO_IN_ENV);
  // FieldName is IDL typedef for string

  CORBA::TCKind current_member_kind (CORBA::Environment &TAO_IN_ENV);

  // Assign and dump elements using IDL defined type NameValuePairSeq

  NameValuePairSeq* get_members (CORBA::Environment &TAO_IN_ENV);

  void set_members (const NameValuePairSeq& value,
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
  CORBA::TypeCode_var type_;
  // Holds the typecode of the Struct

  CORBA::Long index_;
  // Index of the current component

  ACE_Array_Base<CORBA_DynAny_var> da_members_;
  // Each component is also a DynAny

  TAO_DynStruct_i (void);
  // Must be instantiated with at least a typecode

  // Use copy() or assign() instead of these
  TAO_DynStruct_i (const TAO_DynStruct_i &src);
  TAO_DynStruct_i &operator= (const TAO_DynStruct_i &src);
};

#endif /* TAO_DYNSTRUCT_I_H */
