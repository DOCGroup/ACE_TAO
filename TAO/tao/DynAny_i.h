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
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =========================================================================

#ifndef TAO_DYNANY_I_H
#define TAO_DYNANY_I_H

#include "tao/orbconf.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/corbafwd.h"
#include "tao/POA_CORBA.h"

class TAO_Export TAO_DynAny_i : public POA_CORBA::DynAny
{
  // = TITLE
  //    TAO_DynAny_i
  //
  // = DESCRIPTION
  //    Implementation of the basic Dynamic Any datatype.
public:
  TAO_DynAny_i (CORBA_TypeCode_ptr tc);
  // constructor with TypeCode argument

  TAO_DynAny_i (const CORBA_Any& any);
  // constructor with Any argument

  ~TAO_DynAny_i (void);
  // destructor

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
  // In this class, there is only one

  CORBA::Boolean next (CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  // Returns next component, again here there is only one

  CORBA::Boolean seek (CORBA::Long slot,
                       CORBA::Environment &ACE_TRY_ENV =
                         TAO_default_environment ());
  // In this class, returns true only for slot of 0

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

  // The ORB DynAny-factory functions will call these so we don't need
  // to keep an orb pointer in each DynAny object.

  static CORBA_DynAny_ptr create_dyn_any (const CORBA_Any& any,
                                          CORBA::Environment &ACE_TRY_ENV =
                                            TAO_default_environment ());
  static CORBA_DynAny_ptr create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                                CORBA::Environment &ACE_TRY_ENV =
                                                  TAO_default_environment ());
  static CORBA_DynStruct_ptr create_dyn_struct (CORBA_TypeCode_ptr tc,
                                                CORBA::Environment &ACE_TRY_ENV =
                                                  TAO_default_environment ());
  static CORBA_DynSequence_ptr create_dyn_sequence (CORBA_TypeCode_ptr tc,
                                                    CORBA::Environment &ACE_TRY_ENV =
                                                      TAO_default_environment ());
  static CORBA_DynArray_ptr create_dyn_array (CORBA_TypeCode_ptr tc,
                                              CORBA::Environment &ACE_TRY_ENV =
                                                TAO_default_environment ());
  static CORBA_DynUnion_ptr create_dyn_union (CORBA_TypeCode_ptr tc,
                                              CORBA::Environment &ACE_TRY_ENV =
                                                TAO_default_environment ());
  static CORBA_DynEnum_ptr create_dyn_enum (CORBA_TypeCode_ptr tc,
                                            CORBA::Environment &ACE_TRY_ENV =
                                              TAO_default_environment ());

  static CORBA_DynAny_ptr create_dyn_any (CORBA_TypeCode_ptr tc,
                                          CORBA::Environment &ACE_TRY_ENV =
                                            TAO_default_environment ());
  // And an extra generic one (not required in the spec) that calls
  // one of the above passing a typecode.  Comes in handy in
  // implementing some versions of current_component().

  static CORBA::TCKind unalias (CORBA_TypeCode_ptr tc,
                                CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ());
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

#endif /* TAO_HAS_MINIMUM_CORBA */

#endif /* TAO_DYNANY_I_H */
