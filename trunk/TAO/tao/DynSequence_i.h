/* -*- C++ -*- */
// $Id$

// ===================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynSequence_i.h
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ===================================================================

#ifndef TAO_DYNSEQUENCE_I_H
#define TAO_DYNSEQUENCE_I_H

#include "ace/Containers.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Export TAO_DynSequence_i : public POA_CORBA::DynSequence
{
  // = TITLE
  //    TAO_DynSequence_i
  //
  // = DESCRIPTION
  //    Implementation of Dynamic Any type for Sequences
  //
public:
  TAO_DynSequence_i (const CORBA_Any& any);
  // constructor taking an Any argument

  TAO_DynSequence_i (CORBA_TypeCode_ptr tc);
  // constructor taking a typecode argument

  ~TAO_DynSequence_i (void);
  // destructor

  // Functions specific to DynSequence

  // Get and set length of sequence

  CORBA::ULong length (CORBA::Environment &);

  void length (CORBA::ULong length,
               CORBA::Environment& env);

  // Assign and dump elements using IDL defined type AnySeq

  AnySeq_ptr get_elements (CORBA::Environment& env);

  void set_elements (const AnySeq& value,
                     CORBA::Environment& env);

  // Functions common to all Dynamic Any types

  void assign (CORBA_DynAny_ptr dyn_any,
               CORBA::Environment &env);
  // Sets the value of the receiver to the value of the argument

  CORBA_DynAny_ptr copy (CORBA::Environment &env);
  // Creates a copy

  void destroy (CORBA::Environment &env);
  // Makes sure the contents are disposed of correctly

  void from_any (const CORBA::Any& any,
                 CORBA::Environment &env);
  // Assigns value from an Any argument

  CORBA::Any_ptr to_any (CORBA::Environment &env);
  // Outputs contents as an Any

  CORBA::TypeCode_ptr type (CORBA::Environment &);
  // Returns the (constant) type

  CORBA_DynAny_ptr current_component (CORBA::Environment &env);
  // Used in iterating through the contents

  CORBA::Boolean next (CORBA::Environment &);
  // Returns next component

  CORBA::Boolean seek (CORBA::Long index,
                       CORBA::Environment &);
  // Jump to component at <index>

  void rewind (CORBA::Environment &);
  // Makes first component the current one

  // Insert and get functions

  void insert_boolean (CORBA::Boolean value,
                       CORBA::Environment &env);
  void insert_octet (CORBA::Octet value,
                     CORBA::Environment &env);
  void insert_char (CORBA::Char value,
                    CORBA::Environment &env);
  void insert_short (CORBA::Short value,
                     CORBA::Environment &env);
  void insert_ushort (CORBA::UShort value,
                      CORBA::Environment &env);
  void insert_long (CORBA::Long value,
                    CORBA::Environment &env);
  void insert_ulong (CORBA::ULong value,
                     CORBA::Environment &env);
  void insert_float (CORBA::Float value,
                     CORBA::Environment &env);
  void insert_double (CORBA::Double value,
                      CORBA::Environment &env);
  void insert_string (const char * value,
                      CORBA::Environment &env);
  void insert_reference (CORBA::Object_ptr value,
                         CORBA::Environment &env);
  void insert_typecode (CORBA::TypeCode_ptr value,
                        CORBA::Environment &env);
  void insert_longlong (CORBA::LongLong value,
                        CORBA::Environment &env);
  void insert_ulonglong (CORBA::ULongLong value,
                         CORBA::Environment &env);
  void insert_wchar (CORBA::WChar value,
                      CORBA::Environment &env);
  void insert_any (const CORBA::Any& value,
                   CORBA::Environment &env);
  CORBA::Boolean get_boolean (CORBA::Environment &env);

  CORBA::Octet get_octet (CORBA::Environment &env);

  CORBA::Char get_char (CORBA::Environment &env);

  CORBA::Short get_short (CORBA::Environment &env);

  CORBA::UShort get_ushort (CORBA::Environment &env);

  CORBA::Long get_long (CORBA::Environment &env);

  CORBA::ULong get_ulong (CORBA::Environment &env);

  CORBA::Float get_float (CORBA::Environment &env);

  CORBA::Double get_double (CORBA::Environment &env);

  char * get_string (CORBA::Environment &env);

  CORBA::Object_ptr get_reference (CORBA::Environment &env);

  CORBA::TypeCode_ptr get_typecode (CORBA::Environment &env);

  CORBA::LongLong get_longlong (CORBA::Environment &env);

  CORBA::ULongLong get_ulonglong (CORBA::Environment &env);

  CORBA::WChar get_wchar (CORBA::Environment &env);

  CORBA::Any_ptr get_any (CORBA::Environment &env);

private:
  CORBA::TypeCode_ptr get_element_type (CORBA::Environment& env);
  // Utility, turns the type of elements contained in the sequence.

  CORBA::TypeCode_var type_;
  // Holds the typecode of the Sequence

  CORBA::Long index_;
  // Index of the current component

  ACE_Array_Base<CORBA_DynAny_var> da_members_;
  // Each component is also a DynAny

  TAO_DynSequence_i (void);
  // Must be instantiated with at least a typecode

  // Use copy() or assign() instead of these
  TAO_DynSequence_i (const TAO_DynSequence_i &src);
  TAO_DynSequence_i &operator= (const TAO_DynSequence_i &src);
};

#endif /* TAO_DYNSEQUENCE_I_H */


