/* -*- C++ -*- */
// $Id$
// =========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynAny_i.h
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// =========================================================================

#ifndef TAO_DYNANY_I_H
#define TAO_DYNANY_I_H

#include "tao/corba.h"

class TAO_Export TAO_DynAny_i : public POA_CORBA::DynAny
{
  // = TITLE
  //    TAO_DynAny_i
  //
  // = DESCRIPTION
  //    Implementation of the basic Dynamic Any datatype.
  //
public:
  TAO_DynAny_i (CORBA_TypeCode_ptr tc);
  // constructor with TypeCode argument

  TAO_DynAny_i (const CORBA_Any& any);
  // constructor with Any argument

  ~TAO_DynAny_i (void);
  // destructor

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
  // In this class, there is only one

  CORBA::Boolean next (CORBA::Environment &);
  // Returns next component, again here there is only one

  CORBA::Boolean seek (CORBA::Long index,
                       CORBA::Environment &);
  // In this class, returns true only for index of 0

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

// The ORB DynAny-factory functions will call these so we don't need to
// keep an orb pointer in each DynAny object.

  static CORBA_DynAny_ptr create_dyn_any (const CORBA_Any& any,
                                           CORBA::Environment& env);

  static CORBA_DynAny_ptr create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                                CORBA::Environment& env);

  static CORBA_DynStruct_ptr create_dyn_struct (CORBA_TypeCode_ptr tc,
                                                CORBA::Environment& env);

  static CORBA_DynSequence_ptr create_dyn_sequence (CORBA_TypeCode_ptr tc,
                                                    CORBA::Environment& env);

  static CORBA_DynArray_ptr create_dyn_array (CORBA_TypeCode_ptr tc,
                                              CORBA::Environment& env);

  static CORBA_DynUnion_ptr create_dyn_union (CORBA_TypeCode_ptr tc,
                                              CORBA::Environment& env);

  static CORBA_DynEnum_ptr create_dyn_enum (CORBA_TypeCode_ptr tc,
                                            CORBA::Environment& env);

  // And an extra generic one (not required in the spec)
  // that calls one of the above passing a typecode.
  // Comes in handy in implementing some versions of current_component().
  static CORBA_DynAny_ptr create_dyn_any (CORBA_TypeCode_ptr tc,
                                          CORBA::Environment& env);


  static CORBA::TCKind unalias (CORBA_TypeCode_ptr tc,
                                CORBA::Environment& env);
  // Utility, in case we have tk_alias.

private:
  CORBA_Any value_;
  // contents

  TAO_DynAny_i (void);
  // Must be instantiated with at least a typecode

  // Use copy() or assign() instead of these
  TAO_DynAny_i (const TAO_DynAny_i &src);
  TAO_DynAny_i &operator= (const TAO_DynAny_i &src);
};

#endif /* TAO_DYNANY_I_H */


