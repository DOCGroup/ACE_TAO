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
    public virtual TAO_Local_RefCounted_Object
{
public:
  /// Constructor.
  TAO_DynEnum_i (void);

  /// Destructor.
  ~TAO_DynEnum_i (void);

  /// Initialize using just a TypeCode.
  void init (CORBA::TypeCode_ptr tc
             ACE_ENV_ARG_DECL);

  /// Initialize using an Any.
  void init (const CORBA::Any& any
             ACE_ENV_ARG_DECL);

  // = LocalObject methods.
  static TAO_DynEnum_i *_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  // = Functions specific to DynEnum.

  virtual char * get_as_string (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_as_string (
      const char * value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::ULong get_as_ulong (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_as_ulong (
      CORBA::ULong value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::InvalidValue
      ));

  // = DynAny common functions not implemented in class TAO_DynCommon.

  virtual void from_any (
      const CORBA::Any & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Any * to_any (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual CORBA::Boolean equal (
      DynamicAny::DynAny_ptr dyn_any
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual DynamicAny::DynAny_ptr current_component (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ));

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
