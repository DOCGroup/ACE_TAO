/* -*- C++ -*- */
// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO_DynamicAny
//
// = FILENAME
//    DynAny_i.h
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =========================================================================

#ifndef TAO_DYNANY_I_H
#define TAO_DYNANY_I_H
#include "ace/pre.h"

#include "DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DynCommon.h"
#include "tao/Any.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class TAO_DynamicAny_Export TAO_DynAny_i
  : public virtual DynamicAny::DynAny,
    public virtual TAO_DynCommon,
    public virtual TAO_Local_RefCounted_Object
{
  // = TITLE
  //    TAO_DynAny_i
  //
  // = DESCRIPTION
  //    Implementation of the basic Dynamic Any datatype.
public:
  TAO_DynAny_i (void);
  // Constructor.

  ~TAO_DynAny_i (void);
  // Destructor.

  void init (CORBA_TypeCode_ptr tc
             ACE_ENV_ARG_DECL);
  // Initialize using just a TypeCode

  void init (const CORBA_Any& any
             ACE_ENV_ARG_DECL);
  // Initialize using an Any.

  // = LocalObject methods.
  static TAO_DynAny_i *_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual void *_tao_QueryInterface (ptr_arith_t type);

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
  // Check if the typecode is acceptable.
  void check_typecode (CORBA::TypeCode_ptr tc
                       ACE_ENV_ARG_DECL);

  // Used when we are created from a typecode.
  void set_to_default_value (CORBA::TypeCode_ptr tc
                             ACE_ENV_ARG_DECL);

  // Called by both versions of init().
  void init_common (void);

  // Use copy() or assign() instead of these.
  TAO_DynAny_i (const TAO_DynAny_i &src);
  TAO_DynAny_i &operator= (const TAO_DynAny_i &src);
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_DYNANY_I_H */
