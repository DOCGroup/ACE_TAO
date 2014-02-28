// -*- C++ -*-

//=============================================================================
/**
 *  @file    DynCommon.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNCOMMON_H
#define TAO_DYNCOMMON_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynCommon
 *
 * Contains most of the functionality common to all the
 * Dynamic Any implementation classes.
 */
class TAO_DynamicAny_Export TAO_DynCommon
  : public virtual DynamicAny::DynAny
{
public:
  /// Constructor.
  TAO_DynCommon (CORBA::Boolean allow_truncation);

  /// Destructor.
  virtual ~TAO_DynCommon (void);

  // = Some functions common to all Dynamic Any types.

  virtual CORBA::TypeCode_ptr type (void);

  virtual void assign (DynamicAny::DynAny_ptr dyn_any);

  virtual void insert_boolean (CORBA::Boolean value);

  virtual void insert_octet (CORBA::Octet value);

  virtual void insert_char (CORBA::Char value);
  virtual void insert_short (CORBA::Short value);
  virtual void insert_ushort (CORBA::UShort value);

  virtual void insert_long (CORBA::Long value);

  virtual void insert_ulong (CORBA::ULong value);

  virtual void insert_float (CORBA::Float value);

  virtual void insert_double (CORBA::Double value);

  virtual void insert_string (const char * value);

  virtual void insert_reference (CORBA::Object_ptr value);

  virtual void insert_typecode (CORBA::TypeCode_ptr value);

  virtual void insert_longlong (CORBA::LongLong value);

  virtual void insert_ulonglong (CORBA::ULongLong value);

  virtual void insert_longdouble (CORBA::LongDouble value);

  virtual void insert_wchar (CORBA::WChar value);

  virtual void insert_wstring (const CORBA::WChar * value);

  virtual void insert_any (const CORBA::Any & value);

  virtual void insert_dyn_any (DynamicAny::DynAny_ptr value);

  virtual void insert_val (CORBA::ValueBase * value);

  virtual CORBA::Boolean get_boolean (void);

  virtual CORBA::Octet get_octet (void);

  virtual CORBA::Char get_char (void);

  virtual CORBA::Short get_short (void);

  virtual CORBA::UShort get_ushort (void);

  virtual CORBA::Long get_long (void);

  virtual CORBA::ULong get_ulong (void);

  virtual CORBA::Float get_float (void);

  virtual CORBA::Double get_double (void);

  virtual char * get_string (void);

  virtual CORBA::Object_ptr get_reference (void);

  virtual CORBA::TypeCode_ptr get_typecode (void);

  virtual CORBA::LongLong get_longlong (void);

  virtual CORBA::ULongLong get_ulonglong (void);

  virtual CORBA::LongDouble get_longdouble (void);

  virtual CORBA::WChar get_wchar (void);

  virtual CORBA::WChar * get_wstring (void);

  virtual CORBA::Any * get_any (void);

  virtual DynamicAny::DynAny_ptr get_dyn_any (void);

  virtual CORBA::ValueBase * get_val (void);

  virtual CORBA::Boolean seek (CORBA::Long index);

  virtual void rewind (void);

  virtual CORBA::Boolean next (void);

  virtual DynamicAny::DynAny_ptr copy (void);

  virtual CORBA::ULong component_count (void);

  virtual void insert_abstract (CORBA::AbstractBase_ptr value);

  virtual CORBA::AbstractBase_ptr get_abstract (void);

  virtual void insert_boolean_seq (const CORBA::BooleanSeq & value);

  virtual void insert_octet_seq (const CORBA::OctetSeq & value);

  virtual void insert_char_seq (const CORBA::CharSeq & value);

  virtual void insert_short_seq (const CORBA::ShortSeq & value);

  virtual void insert_ushort_seq (const CORBA::UShortSeq & value);

  virtual void insert_long_seq (const CORBA::LongSeq & value);

  virtual void insert_ulong_seq (const CORBA::ULongSeq & value);

  virtual void insert_float_seq (const CORBA::FloatSeq & value);

  virtual void insert_double_seq (const CORBA::DoubleSeq & value);

  virtual void insert_longlong_seq (const CORBA::LongLongSeq & value);

  virtual void insert_ulonglong_seq (const CORBA::ULongLongSeq & value);

  virtual void insert_longdouble_seq (const CORBA::LongDoubleSeq & value);

  virtual void insert_wchar_seq (const CORBA::WCharSeq & value);

  virtual CORBA::BooleanSeq * get_boolean_seq (void);

  virtual CORBA::OctetSeq * get_octet_seq (void);

  virtual CORBA::CharSeq * get_char_seq (void);

  virtual CORBA::ShortSeq * get_short_seq (void);

  virtual CORBA::UShortSeq * get_ushort_seq (void);

  virtual CORBA::LongSeq * get_long_seq (void);

  virtual CORBA::ULongSeq * get_ulong_seq (void);

  virtual CORBA::FloatSeq * get_float_seq (void);

  virtual CORBA::DoubleSeq * get_double_seq (void);

  virtual CORBA::LongLongSeq * get_longlong_seq (void);

  virtual CORBA::ULongLongSeq * get_ulonglong_seq (void);

  virtual CORBA::LongDoubleSeq * get_longdouble_seq (void);

  virtual CORBA::WCharSeq * get_wchar_seq (void);

  /// This sets one of two flags in this base class. CORBA 2.4.2
  /// requires that destroy() do nothing if it is called on
  /// the result of current_component(), the only non-deep-copying
  /// method in the Dynamic Any API. If the boolean arg below is 0,
  /// the call is from inside current_component(), and the flag
  /// ref_to_component_ is set. If the boolean arg is 1, the call
  /// is from inside destroy(), and the container_is_destroying_
  /// flag is set, overriding the first flag in the component's
  /// destroy() method.
  void set_flag (DynamicAny::DynAny_ptr component, CORBA::Boolean destroying);

  // Utility functions.

  DynamicAny::DynAny_ptr check_component (CORBA::Boolean isValueType = false);

  void check_type (CORBA::TypeCode_ptr tc);

  static bool is_basic_type_seq (CORBA::TypeCode_ptr tc);

  // Accessors

  CORBA::Boolean has_components (void) const;
  CORBA::Boolean destroyed (void) const;
  CORBA::Any &the_any (void);

  // Mutators

  void container_is_destroying (CORBA::Boolean val);
  void ref_to_component (CORBA::Boolean val);

protected:
  /// Were we created by current_component()?
  CORBA::Boolean ref_to_component_;

  /// Flag to let destroy() call by container override the flag above.
  CORBA::Boolean container_is_destroying_;

  /// Do we contain other Dynamic Anys?
  CORBA::Boolean has_components_;

  /// Has destroy() been called on us yet?
  CORBA::Boolean destroyed_;

  /// Slot of the current component (if any).
  CORBA::Long current_position_;

  /// Number of components, as defined by CORBA 2.4.2.
  CORBA::ULong component_count_;

  /// Holder for our type code.
  CORBA::TypeCode_var type_;

  /// Gets a value only for basic types, but used in insert_*
  /// and get_*, defined in this base class.
  CORBA::Any any_;

  /// Are we allowed to truncate any valuetypes in our hyarchy?
  CORBA::Boolean allow_truncation_;

private:
  // Utility functions used by insert_* and get_*.

  CORBA::TypeCode_ptr check_type_and_unalias (CORBA::TypeCode_ptr tc);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_DYNCOMMON_H */
