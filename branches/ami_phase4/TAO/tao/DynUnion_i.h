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
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_DYNUNION_I_H
#define TAO_DYNUNION_I_H

#include "tao/orbconf.h"

#if !defined (TAO_HAS_MINIMUM_CORBA)

#include "tao/DynUnion_i_T.h"

class TAO_Export TAO_DynUnion_i : public POA_CORBA::DynUnion
{
  // = TITLE
  //    TAO_DynUnion_i
  //
  // = DESCRIPTION
  //    Implementation of Dynamic Any type for Unions.
public:
  TAO_DynUnion_i (const CORBA_Any& any);
  // constructor taking an Any argument

  TAO_DynUnion_i (CORBA_TypeCode_ptr tc);
  // constructor taking a typecode argument

  ~TAO_DynUnion_i (void);
  // destructor

  // = Functions specific to DynUnion

  CORBA::Boolean set_as_default (CORBA::Environment &ACE_TRY_ENV =
                                   TAO_default_environment ());
  // Does union have a default member?

  void set_as_default (CORBA::Boolean set_as_default,
                       CORBA::Environment& ACE_TRY_ENV =
                         TAO_default_environment ());
  // Makes default member (if any) the active menber

  CORBA_DynAny_ptr discriminator (CORBA::Environment& ACE_TRY_ENV =
                                    TAO_default_environment ());
  // Returns the discriminator (which is a DynAny)

  CORBA::TCKind discriminator_kind (CORBA::Environment &ACE_TRY_ENV =
                                      TAO_default_environment ());
  // Returns TCKind of discriminator

  CORBA_DynAny_ptr member (CORBA::Environment &ACE_TRY_ENV =
                             TAO_default_environment ());
  // Returns active member

  // = Get and set for member name

  char *member_name (CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ());
  void member_name (const char* member_name,
                    CORBA::Environment &ACE_TRY_ENV =
                      TAO_default_environment ());

  CORBA::TCKind member_kind (CORBA::Environment &ACE_TRY_ENV =
                               TAO_default_environment ());
  // Returns TCKind of active member

  // Functions common to all Dynamic Any types

  void assign (CORBA_DynAny_ptr dyn_any,
               CORBA::Environment &ACE_TRY_ENV =
                 TAO_default_environment ());
  // Sets the value of the receiver to the value of the argument

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
  // Used in iterating through the contents

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
  CORBA_DynAny_var member_;

  CORBA_DynAny_var discriminator_;

  CORBA::TypeCode_var type_;
  // Holds the typecode of the Union

  CORBA::ULong current_index_;
  // Index of the active member

  TAO_DynUnion_i (void);
  // Must be instantiated with at least a typecode

  // Use copy() or assign() instead of these
  TAO_DynUnion_i (const TAO_DynUnion_i &src);
  TAO_DynUnion_i &operator= (const TAO_DynUnion_i &src);

  // Some specialized instantiations of template class (defined in
  // DynUnion_i_T.h)

  class Boolean_extractor : public DU_Extractor_base
  {
  public:
    CORBA::Boolean check_match (const CORBA_Any &inside_any,
                                const CORBA_Any &outsice_any);
  private:
    CORBA::Boolean arg_index_;
    CORBA::Boolean member_index_;
  };

  class Char_extractor : public DU_Extractor_base
  {
  public: 
    CORBA::Boolean check_match (const CORBA_Any &inside_any,
                                const CORBA_Any &outside_any);
  private:
    CORBA::Char arg_index_;
    CORBA::Char member_index_;
  };

  class WChar_extractor : public DU_Extractor_base
  {
  public:
    CORBA::Boolean check_match (const CORBA_Any &inside_any,
                                const CORBA_Any &outside_any);
  private:
    CORBA::WChar arg_index_;
    CORBA::WChar member_index_;
  };

  class Enum_extractor : public DU_Extractor_base
  {
  public:
    CORBA::Boolean check_match (const CORBA_Any &inside_any,
                                const CORBA_Any &outside_any);
  private:
    CORBA::ULong arg_index_;
    CORBA::ULong member_index_;
  };

  DU_Extractor_base* get_extractor (CORBA::TCKind kind,
                                    CORBA::Environment &ACE_TRY_ENV =
                                      TAO_default_environment ());
  // Returns a pointer to the correct extractor class.

  void set_from_any (const CORBA_Any& any,
                     CORBA::Environment &ACE_TRY_ENV =
                       TAO_default_environment ());
  // Code common to the constructor from an Any arg and the member
  // function from_any().
};

#endif /* TAO_HAS_MINIMUM_CORBA */

#endif /* TAO_DYNUNION_I_H */
