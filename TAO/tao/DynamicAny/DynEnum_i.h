// -*- C++ -*-

// $Id$

//=============================================================================
/**
 *  @file    DynEnum_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNENUM_I_H
#define TAO_DYNENUM_I_H
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
 * @class TAO_DynEnum_i
 *
 * Implementation of Dynamic Any type for enums
 */
class TAO_DynamicAny_Export TAO_DynEnum_i
  : public virtual DynamicAny::DynEnum,
    public virtual TAO_DynCommon,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_DynEnum_i (CORBA::Boolean allow_truncation=true);

  /// Destructor.
  ~TAO_DynEnum_i (void);

  /// Initialize using just a TypeCode.
  void init (CORBA::TypeCode_ptr tc);

  /// Initialize using an Any.
  void init (const CORBA::Any& any);

  // = LocalObject methods.
  static TAO_DynEnum_i *_narrow (CORBA::Object_ptr obj);

  // = Functions specific to DynEnum.

  virtual char * get_as_string (void);

  virtual void set_as_string (const char * value);

  virtual CORBA::ULong get_as_ulong (void);

  virtual void set_as_ulong (CORBA::ULong value);

  // = DynAny common functions not implemented in class TAO_DynCommon.

  virtual void from_any (const CORBA::Any & value);

  virtual CORBA::Any * to_any (void);

  virtual CORBA::Boolean equal (DynamicAny::DynAny_ptr dyn_any);

  virtual void destroy (void);

  virtual DynamicAny::DynAny_ptr current_component (void);

private:
  // Called by both versions of init().
  void init_common (void);

  // = Use copy() or assign() instead of these.
  TAO_DynEnum_i (const TAO_DynEnum_i &src);
  TAO_DynEnum_i &operator= (const TAO_DynEnum_i &src);

private:
  /// Current numeric value of the enum.
  CORBA::ULong value_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNENUM_I_H */
