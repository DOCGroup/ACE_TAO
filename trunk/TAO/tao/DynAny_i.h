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

#if !defined (TAO_DYNANY_I_H)
#define	TAO_DYNANY_I_H

#include "tao/corba.h"

class TAO_Export TAO_DynAny_i : public POA_CORBA::DynAny
{
public:
                      TAO_DynAny_i      (CORBA_TypeCode_ptr tc);

                      TAO_DynAny_i      (const CORBA_Any& any);

                      ~TAO_DynAny_i     (void);

  // Common fuctions

  void                assign            (CORBA_DynAny_ptr dyn_any,
                                         CORBA::Environment &env);

  CORBA_DynAny_ptr    copy              (CORBA::Environment &env);

  void                destroy           (CORBA::Environment &env);

  void                from_any          (const CORBA::Any& any,
                                         CORBA::Environment &env);

  CORBA::Any_ptr      to_any            (CORBA::Environment &env);

  CORBA::TypeCode_ptr type              (CORBA::Environment &);

  CORBA_DynAny_ptr    current_component (CORBA::Environment &env);

  CORBA::Boolean      next              (CORBA::Environment &);

  CORBA::Boolean      seek              (CORBA::Long index, 
                                         CORBA::Environment &);

  void                rewind            (CORBA::Environment &);

  // Insert and get functions

  void                insert_boolean    (CORBA::Boolean value,
                                         CORBA::Environment &env);

  void                insert_octet      (CORBA::Octet value,
                                         CORBA::Environment &env);

  void                insert_char       (CORBA::Char value,
                                         CORBA::Environment &env);

  void                insert_short      (CORBA::Short value,
                                         CORBA::Environment &env);

  void                insert_ushort     (CORBA::UShort value,
                                         CORBA::Environment &env);

  void                insert_long       (CORBA::Long value,
                                         CORBA::Environment &env);

  void                insert_ulong      (CORBA::ULong value,
                                         CORBA::Environment &env);
  void                insert_float      (CORBA::Float value,
                                         CORBA::Environment &env);

  void                insert_double     (CORBA::Double value,
                                         CORBA::Environment &env);

  void                insert_string     (const char * value,
                                         CORBA::Environment &env);

  void                insert_reference  (CORBA::Object_ptr value,
                                         CORBA::Environment &env);

  void                insert_typecode   (CORBA::TypeCode_ptr value,
                                         CORBA::Environment &env);

  void                insert_longlong   (CORBA::LongLong value,
                                         CORBA::Environment &env);

  void                insert_ulonglong  (CORBA::ULongLong value,
                                         CORBA::Environment &env);

  void                insert_wchar      (CORBA::WChar value,
                                         CORBA::Environment &env);

  void                insert_any        (const CORBA::Any& value,
                                         CORBA::Environment &env);

  CORBA::Boolean      get_boolean       (CORBA::Environment &env);

  CORBA::Octet        get_octet         (CORBA::Environment &env);

  CORBA::Char         get_char          (CORBA::Environment &env);

  CORBA::Short        get_short         (CORBA::Environment &env);

  CORBA::UShort       get_ushort        (CORBA::Environment &env);

  CORBA::Long         get_long          (CORBA::Environment &env);

  CORBA::ULong        get_ulong         (CORBA::Environment &env);

  CORBA::Float        get_float         (CORBA::Environment &env);

  CORBA::Double       get_double        (CORBA::Environment &env);

  char *              get_string        (CORBA::Environment &env);

  CORBA::Object_ptr   get_reference     (CORBA::Environment &env);

  CORBA::TypeCode_ptr get_typecode      (CORBA::Environment &env);

  CORBA::LongLong     get_longlong      (CORBA::Environment &env);

  CORBA::ULongLong    get_ulonglong     (CORBA::Environment &env);

  CORBA::WChar        get_wchar         (CORBA::Environment &env);

  CORBA::Any_ptr      get_any           (CORBA::Environment &env);

/////////////////////////////////////////////////////////////////////////////
// The ORB DynAny-factory functions will call these so we don't need to
// keep an orb pointer in each DynAny object.

  static CORBA_DynAny_ptr       create_dyn_any       (const CORBA_Any& any,
                                                      CORBA::Environment& env);

  static CORBA_DynAny_ptr       create_basic_dyn_any (CORBA_TypeCode_ptr tc,
                                                      CORBA::Environment& env);

  static CORBA_DynStruct_ptr    create_dyn_struct    (CORBA_TypeCode_ptr tc,
                                                      CORBA::Environment& env);

  static CORBA_DynSequence_ptr  create_dyn_sequence  (CORBA_TypeCode_ptr tc,
                                                      CORBA::Environment& env);

  static CORBA_DynArray_ptr     create_dyn_array     (CORBA_TypeCode_ptr tc,
                                                      CORBA::Environment& env);

  static CORBA_DynUnion_ptr     create_dyn_union     (CORBA_TypeCode_ptr tc,
                                                      CORBA::Environment& env);

  static CORBA_DynEnum_ptr      create_dyn_enum      (CORBA_TypeCode_ptr tc,
                                                      CORBA::Environment& env);

  // An extra generic one that calls one of the above passing a typecode.
  // Comes in handy in implementing some versions of current_component().
  static CORBA_DynAny_ptr       create_dyn_any       (CORBA_TypeCode_ptr tc,
                                                      CORBA::Environment& env);

////////////////////////////////////////////////////////////////////////////

  // In case we have tk_alias.
  static CORBA::TCKind          unalias              (CORBA_TypeCode_ptr tc,
                                                      CORBA::Environment& env);

private:
  CORBA_Any                     value_;

  // Must be instantiated with at least a typecode
                                TAO_DynAny_i         (void);

  // Use copy() or assign()
                                TAO_DynAny_i         (const TAO_DynAny_i &src);
  TAO_DynAny_i &                operator=            (const TAO_DynAny_i &src);
};

#endif /* TAO_DYNANY_I_H */


