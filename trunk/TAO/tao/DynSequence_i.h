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

class TAO_Export TAO_DynSequence_i : public POA_CORBA::DynSequence
{
public:
                        TAO_DynSequence_i   (const CORBA_Any& any);

                        TAO_DynSequence_i   (CORBA_TypeCode_ptr tc);

                        ~TAO_DynSequence_i  (void);

  // Functions specific to DynSequence

  CORBA::ULong          length              (CORBA::Environment &);

  void                  length              (CORBA::ULong length,
                                             CORBA::Environment& env);

  AnySeq_ptr            get_elements        (CORBA::Environment& env);

  void                  set_elements        (const AnySeq& value,
                                             CORBA::Environment& env);

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

  CORBA::Boolean        next                (CORBA::Environment &);

  CORBA::Boolean        seek                (CORBA::Long index,
                                             CORBA::Environment &);

  void                  rewind              (CORBA::Environment &);

  // The insert and get functions

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
  // Returns the type of elements contained in the sequence.
  CORBA::TypeCode_ptr   get_element_type    (CORBA::Environment& env);

  CORBA::TypeCode_var   type_;

  CORBA::Long           index_;

  ACE_Array<CORBA_DynAny_var> da_members_;

  // Must be instantiated with at least a typecode
                        TAO_DynSequence_i   (void);

  // Use copy() or assign()
                        TAO_DynSequence_i   (const TAO_DynSequence_i &src);
  TAO_DynSequence_i &   operator=           (const TAO_DynSequence_i &src);
};

#endif /* TAO_DYNSEQUENCE_I_H */


