// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynStruct_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNSTRUCT_I_H
#define TAO_DYNSTRUCT_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/DynamicAny/DynCommon.h"
#include "tao/LocalObject.h"
#include "ace/Containers.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynStruct_i
 *
 * Implementation of Dynamic Any type for Structs
 */
class TAO_DynamicAny_Export TAO_DynStruct_i
  : public virtual DynamicAny::DynStruct,
    public virtual TAO_DynCommon,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_DynStruct_i (CORBA::Boolean allow_truncation=true);

  /// Destructor.
  ~TAO_DynStruct_i (void);

  /// Initialize using just a TypeCode.
  void init (CORBA::TypeCode_ptr tc);

  /// Initialize using an Any.
  void init (const CORBA::Any& any);

  // = LocalObject methods.
  static TAO_DynStruct_i *_narrow (CORBA::Object_ptr obj);

  // = Functions specific to DynStruct.

  virtual DynamicAny::FieldName current_member_name (void);

  virtual CORBA::TCKind current_member_kind (void);

  virtual DynamicAny::NameValuePairSeq *get_members (void);

  virtual void set_members (const DynamicAny::NameValuePairSeq& value);

  virtual DynamicAny::NameDynAnyPairSeq * get_members_as_dyn_any (void);

  virtual void set_members_as_dyn_any (
      const DynamicAny::NameDynAnyPairSeq & value);

  // = DynAny common functions not implemented in class TAO_DynCommon.

  virtual void from_any (const CORBA::Any & value);

  virtual CORBA::Any * to_any (void);

  virtual CORBA::Boolean equal (DynamicAny::DynAny_ptr dyn_any);

  virtual void destroy (void);

  virtual DynamicAny::DynAny_ptr current_component (void);

private:
  /// Check if the typecode is acceptable.
  void check_typecode (CORBA::TypeCode_ptr tc);

  /// Code common to the constructor from an Any arg and the member
  /// function from_any().
  void set_from_any (const CORBA::Any &any);

  /// Called by both versions of init().
  void init_common (void);

  // = Use copy() or assign() instead of these.
  TAO_DynStruct_i (const TAO_DynStruct_i &src);
  TAO_DynStruct_i &operator= (const TAO_DynStruct_i &src);

private:
  /// Each component is also a DynAny.
  ACE_Array_Base<DynamicAny::DynAny_var> da_members_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNSTRUCT_I_H */
