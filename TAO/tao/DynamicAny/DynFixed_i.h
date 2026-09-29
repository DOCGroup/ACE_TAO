// -*- C++ -*-

//=============================================================================
/**
 *  @file    DynFixed_i.h
 *
 *  @author Johnny Willemsen <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_DYNFIXED_I_H
#define TAO_DYNFIXED_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"
#include "tao/CDR.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/DynamicAny/DynCommon.h"
#include "tao/LocalObject.h"

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynFixed_i
 *
 * Implementation of Dynamic Any for fixed-point values.
 */
class TAO_DynamicAny_Export TAO_DynFixed_i
  : public virtual DynamicAny::DynFixed,
    public virtual TAO_DynCommon,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_DynFixed_i (CORBA::Boolean allow_truncation = true);

  /// Initialize from an Any or a TypeCode.
  void init (CORBA::Any const& any);
  void init (CORBA::TypeCode_ptr tc);

  // = LocalObject methods.
  static TAO_DynFixed_i *_narrow (CORBA::Object_ptr obj);

  // = DynFixed operations.
  virtual char * get_value ();
  virtual CORBA::Boolean set_value (char const* value);

  // = DynAny operations.
  virtual void from_any (CORBA::Any const& value);
  virtual CORBA::Any * to_any ();
  virtual CORBA::Boolean equal (DynamicAny::DynAny_ptr dyn_any);
  virtual void destroy ();
  virtual DynamicAny::DynAny_ptr current_component ();

private:
  void init_common ();
  bool read_value (TAO_InputCDR& cdr, ACE_CDR::Fixed& value) const;

  ACE_CDR::Fixed value_;

  TAO_DynFixed_i (TAO_DynFixed_i const&) = delete;
  TAO_DynFixed_i& operator= (const TAO_DynFixed_i&) = delete;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNFIXED_I_H */
