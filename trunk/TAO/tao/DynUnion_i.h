/* -*- C++ -*- */
// $Id$

// ========================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    DynUnion_i.h
//
// = AUTHOR
//    Jeff Parsons <jp4@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_DYNUNION_I_H
#define TAO_DYNUNION_I_H

#include "tao/DynUnion_i_T.h"

class TAO_Export TAO_DynUnion_i : public POA_CORBA::DynUnion
{
  // = TITLE
  //    TAO_DynUnion_i
  //
  // = DESCRIPTION
  //    Implementation of Dynamic Any type for Unions
  //
public:
  TAO_DynUnion_i (const CORBA_Any& any);
  // constructor taking an Any argument

  TAO_DynUnion_i (CORBA_TypeCode_ptr tc);
  // constructor taking a typecode argument

  ~TAO_DynUnion_i (void);
  // destructor

  // Functions specific to DynUnion

  CORBA::Boolean set_as_default (CORBA::Environment& env);
  // Does union have a default member?

  void set_as_default (CORBA::Boolean set_as_default,
                       CORBA::Environment&);
  // Makes default member (if any) the active menber

  CORBA_DynAny_ptr discriminator (CORBA::Environment&);
  // Returns the discriminator (which is a DynAny)

  CORBA::TCKind discriminator_kind (CORBA::Environment& env);
  // Returns TCKind of discriminator

  CORBA_DynAny_ptr member (CORBA::Environment& env);
  // Returns active member

  // Get and set for member name

  char* member_name (CORBA::Environment& env);

  void member_name (const char* member_name,
                    CORBA::Environment& env);

  CORBA::TCKind member_kind (CORBA::Environment& env);
  // Returns TCKind of active member

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
  CORBA_DynAny_var member_;

  CORBA_DynAny_var discriminator_;

  CORBA::TypeCode_var type_;
  // Holds the typecode of the Union

  CORBA::ULong index_;
  // Index of the active member

  TAO_DynUnion_i (void);
  // Must be instantiated with at least a typecode

  // Use copy() or assign() instead of these
  TAO_DynUnion_i (const TAO_DynUnion_i &src);
  TAO_DynUnion_i &operator= (const TAO_DynUnion_i &src);

  // Some specialized instantiations of template class (defined in 
  // DynUnion_i_T.h)

  class WChar_extractor : public DU_Extractor_base
  {
  public:
    CORBA::Boolean check_match (const CORBA_Any& inside_any,
                                const CORBA_Any& outside_any);
  private:
    CORBA::WChar arg_index_; 
    CORBA::WChar member_index_;
  };

  class Enum_extractor : public DU_Extractor_base
  {
  public:
    CORBA::Boolean check_match (const CORBA_Any& inside_any,
                                const CORBA_Any& outside_any);
  private:
    CORBA::ULong arg_index_; 
    CORBA::ULong member_index_;
  };

  DU_Extractor_base* get_extractor (CORBA::TCKind kind,
                                      CORBA::Environment &env);
  // Returns a pointer to the correct extractor class.

  void set_from_any(const CORBA_Any& any,
                    CORBA::Environment &env);
  // Code common to the constructor from an Any arg and the member
  // function from_any().
};

#endif /* TAO_DYNUNION_I_H */
