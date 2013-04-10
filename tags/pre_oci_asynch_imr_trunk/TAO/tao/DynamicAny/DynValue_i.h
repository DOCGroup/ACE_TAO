// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynValue_i.h
 *
 *  $Id$
 *
 *  @author simon dot massey at prismtech dot com
 */
//=============================================================================


#ifndef TAO_DYNVALUE_I_H
#define TAO_DYNVALUE_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynValueCommon_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynValue_i
 *
 * @brief Implementation of the DynValue class.
 */
class TAO_DynamicAny_Export TAO_DynValue_i
  : public virtual DynamicAny::DynValue,
    public virtual TAO_DynValueCommon_i
{
public:
  /// Constructor.
  TAO_DynValue_i (CORBA::Boolean allow_truncation=true);

  /// Destructor.
  ~TAO_DynValue_i (void);

  /// Initialize using an Any.
  void init (const CORBA::Any& any);

  /// Initialize using a TypeCode and an input stream
  void init (CORBA::TypeCode_ptr tc, TAO_InputCDR &in);

  /// Initialize using just a TypeCode.
  void init (CORBA::TypeCode_ptr tc);

  // = LocalObject methods.
  static TAO_DynValue_i *_narrow (CORBA::Object_ptr obj);

  // = Functions specific to DynValue.
  virtual DynamicAny::FieldName current_member_name (void);

  virtual CORBA::TCKind current_member_kind (void);

  virtual DynamicAny::NameValuePairSeq * get_members (void);

  virtual void set_members (const DynamicAny::NameValuePairSeq & value);

  virtual DynamicAny::NameDynAnyPairSeq * get_members_as_dyn_any (void);

  virtual void set_members_as_dyn_any (
      const DynamicAny::NameDynAnyPairSeq & value);

  // = DynAny common functions not implemented in class TAO_DynCommon.
  virtual void from_any (const CORBA::Any & value);

  virtual CORBA::Any * to_any (void);

  virtual CORBA::Boolean equal (DynamicAny::DynAny_ptr dyn_any);

  virtual void destroy (void);

  virtual DynamicAny::DynAny_ptr current_component (void);

  virtual void insert_val (CORBA::ValueBase * value);

  virtual CORBA::ValueBase * get_val (void);

  // = DynValueCommon needed to be provided here
  virtual void set_to_value (void);

private:
  /// List of base types.
  typedef ACE_Array_Base<CORBA::TypeCode_var> BaseTypesList_t;

  /// Decompose the given TypeCode into its hiarchical list of
  /// basetypes. The first element of the list is our actual type,
  /// each basetype follows in order backwards down the hiarchy.
  /// All types stored in the list are de-aliased. Optionally
  /// return the total_member_count of the fully derived type.
  static void get_base_types (
    CORBA::TypeCode_ptr,
    BaseTypesList_t &,
    CORBA::ULong *total_member_count = 0);

  /// Return the unaliased valuetype typecode that corresponds to
  /// index (0..total_members-1) from the given hiarchical list of
  /// the derived type and it basetypes.
  static CORBA::TypeCode_ptr get_correct_base_type (
    const BaseTypesList_t &base_types,
    CORBA::ULong &index);

  /// Return the member_type at index (0..total_members-1) from
  /// the given hiarchical list of the derived type and it basetypes.
  static CORBA::TypeCode_ptr get_member_type (
    const BaseTypesList_t &,
    CORBA::ULong index);

  /// Return the member_name at index (0..total_members-1) from
  /// the given hiarchical list of the derived type and it basetypes.
  static const char * get_member_name (
    const BaseTypesList_t &,
    CORBA::ULong index);

  /// Check if the typecode is acceptable.
  void check_typecode (CORBA::TypeCode_ptr tc);

  /// Common code from the init() functions, initializes the
  /// private bits from the given TypeCode
  void init_helper (CORBA::TypeCode_ptr tc);

  /// Code common to the init(Any) and the member
  /// function from_any().
  void from_any_helper (const CORBA::Any &any);

  /// Write the value to the output stream.
  void to_outputCDR (TAO_OutputCDR &);

  /// Read the value from the input stream
  void from_inputCDR (TAO_InputCDR &);

  /// These are not implimented!
  /// Use copy() or assign() instead of these.
  TAO_DynValue_i (const TAO_DynValue_i &src);
  TAO_DynValue_i &operator= (const TAO_DynValue_i &src);

  /// Each component of DynValue and DynValueBox is also a DynAny.
  ACE_Array_Base<DynamicAny::DynAny_var> da_members_;

  /// First element of this is our type, each basetype follows
  /// in order backwards down the hiarchy. All types stored are
  /// de-aliased.
  BaseTypesList_t da_base_types_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNVALUE_I_H */
