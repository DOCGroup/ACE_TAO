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

#if !defined TAO_DYNUNION_I_H
#define TAO_DYNUNION_I_H

#include "tao/DynUnion_i_T.h"

class TAO_Export TAO_DynUnion_i : public POA_CORBA::DynUnion
{
public:
                        TAO_DynUnion_i      (const CORBA_Any& any);

                        TAO_DynUnion_i      (CORBA_TypeCode_ptr tc);

                        ~TAO_DynUnion_i     (void);

  // Functions specific to DynUnion

  CORBA::Boolean        set_as_default      (CORBA::Environment& env);

  void                  set_as_default      (CORBA::Boolean set_as_default,
                                             CORBA::Environment&);

  CORBA_DynAny_ptr      discriminator       (CORBA::Environment&);

  CORBA::TCKind         discriminator_kind  (CORBA::Environment& env);

  CORBA_DynAny_ptr      member              (CORBA::Environment& env);

  char*                 member_name         (CORBA::Environment& env);

  void                  member_name         (const char* member_name,
                                             CORBA::Environment& env);

  CORBA::TCKind         member_kind         (CORBA::Environment& env);

  // Common functions

  void                  assign              (CORBA_DynAny_ptr dyn_any,
                                             CORBA::Environment &env);

  CORBA_DynAny_ptr      copy                (CORBA::Environment &env);

  void                  destroy             (CORBA::Environment &env);

  void                  from_any            (const CORBA::Any& any,
                                             CORBA::Environment &env);

  CORBA::Any_ptr        to_any              (CORBA::Environment &env);

  CORBA::TypeCode_ptr   type                (CORBA::Environment &);

  CORBA_DynAny_ptr      current_component   (CORBA::Environment &env);

  CORBA::Boolean        next                (CORBA::Environment &env);

  CORBA::Boolean        seek                (CORBA::Long index,
                                             CORBA::Environment &env);

  void                  rewind              (CORBA::Environment &env);

  // The insert and get functions.

  void                  insert_boolean      (CORBA::Boolean value,
                                             CORBA::Environment &env);

  void                  insert_octet        (CORBA::Octet value,
                                             CORBA::Environment &env);

  void                  insert_char         (CORBA::Char value,
                                             CORBA::Environment &env);

  void                  insert_short        (CORBA::Short value,
                                             CORBA::Environment &env);

  void                  insert_ushort       (CORBA::UShort value,
                                             CORBA::Environment &env);

  void                  insert_long         (CORBA::Long value,
                                             CORBA::Environment &env);

  void                  insert_ulong        (CORBA::ULong value,
                                             CORBA::Environment &env);

  void                  insert_float        (CORBA::Float value,
                                             CORBA::Environment &env);

  void                  insert_double       (CORBA::Double value,
                                             CORBA::Environment &env);

  void                  insert_string       (const char * value,
                                            CORBA::Environment &env);

  void                  insert_reference    (CORBA::Object_ptr value,
                                             CORBA::Environment &env);

  void                  insert_typecode     (CORBA::TypeCode_ptr value,
                                             CORBA::Environment &env);

  void                  insert_longlong     (CORBA::LongLong value,
                                             CORBA::Environment &env);

  void                  insert_ulonglong    (CORBA::ULongLong value,
                                            CORBA::Environment &env);

  void                  insert_wchar        (CORBA::WChar value,
                                            CORBA::Environment &env);

  void                  insert_any          (const CORBA::Any& value,
                                             CORBA::Environment &env);

  CORBA::Boolean        get_boolean         (CORBA::Environment &env);

  CORBA::Octet          get_octet           (CORBA::Environment &env);

  CORBA::Char           get_char            (CORBA::Environment &env);

  CORBA::Short          get_short           (CORBA::Environment &env);

  CORBA::UShort         get_ushort          (CORBA::Environment &env);

  CORBA::Long           get_long            (CORBA::Environment &env);

  CORBA::ULong          get_ulong           (CORBA::Environment &env);

  CORBA::Float          get_float           (CORBA::Environment &env);

  CORBA::Double         get_double          (CORBA::Environment &env);

  char *                get_string          (CORBA::Environment &env);

  CORBA::Object_ptr     get_reference       (CORBA::Environment &env);

  CORBA::TypeCode_ptr   get_typecode        (CORBA::Environment &env);

  CORBA::LongLong       get_longlong        (CORBA::Environment &env);

  CORBA::ULongLong      get_ulonglong       (CORBA::Environment &env);

  CORBA::WChar          get_wchar           (CORBA::Environment &env);

  CORBA::Any_ptr        get_any             (CORBA::Environment &env);

private:
  CORBA::TypeCode_var   type_;

  CORBA_DynAny_var      discriminator_;

  CORBA::ULong          index_;

  CORBA_DynAny_var      member_;


  // Must be instantiated with at least a typecode
                        TAO_DynUnion_i      (void);

  // Use copy() or assign()
                        TAO_DynUnion_i      (const TAO_DynUnion_i &src);
  TAO_DynUnion_i &      operator=           (const TAO_DynUnion_i &src);
  
  // Some specialized instantiations of template class defined in this 
  // file at global scope.

  class WChar_extractor : public ::DU_Extractor_base
  {
  public:
    CORBA::Boolean      check_match     (const CORBA_Any& inside_any,
                                         const CORBA_Any& outside_any);
  private:
    CORBA::WChar        arg_index_, 
                        member_index_;
  };

  class Enum_extractor : public ::DU_Extractor_base
  {
  public:
    CORBA::Boolean      check_match     (const CORBA_Any& inside_any,
                                         const CORBA_Any& outside_any);
  private:
    CORBA::ULong        arg_index_, 
                        member_index_;
  };

  // Returns a pointer to the correctr extractor class.
  ::DU_Extractor_base*       get_extractor   (CORBA::TCKind kind,
                                              CORBA::Environment &env);

  // Code common to the constructor from an Any arg and the member
  // function from_any().
  void                  set_from_any    (const CORBA_Any& any,
                                         CORBA::Environment &env);
};

#endif /* TAO_DYNUNION_I_H */
