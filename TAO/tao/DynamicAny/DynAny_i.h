// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynAny_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNANY_I_H
#define TAO_DYNANY_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynamicAny.h"

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
 * @class TAO_DynAny_i
 *
 * @brief Implementation of the basic Dynamic Any datatype.
 */
class TAO_DynamicAny_Export TAO_DynAny_i
  : public virtual DynamicAny::DynAny,
    public virtual TAO_DynCommon,
    public virtual ::CORBA::LocalObject
{
public:
  /// Constructor.
  TAO_DynAny_i (void);

  /// Destructor.
  ~TAO_DynAny_i (void);

  /// Initialize using just a TypeCode
  void init (CORBA::TypeCode_ptr tc);

  /// Initialize using an Any.
  void init (const CORBA::Any& any);

  // = LocalObject methods.
  static TAO_DynAny_i *_narrow (CORBA::Object_ptr obj);

  // = DynAny common functions not implemented in class TAO_DynCommon.

  virtual void from_any (const CORBA::Any & value);

  virtual CORBA::Any * to_any (void);

  virtual CORBA::Boolean equal (DynamicAny::DynAny_ptr dyn_any);

  virtual void destroy (void);

  virtual DynamicAny::DynAny_ptr current_component (void);

private:
  /// Check if the typecode is acceptable.
  void check_typecode (CORBA::TypeCode_ptr tc);

  /// Used when we are created from a typecode.
  void set_to_default_value (CORBA::TypeCode_ptr tc);

  /// Called by both versions of init().
  void init_common (void);

  // Use copy() or assign() instead of these.
  TAO_DynAny_i (const TAO_DynAny_i &src);
  TAO_DynAny_i &operator= (const TAO_DynAny_i &src);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNANY_I_H */
