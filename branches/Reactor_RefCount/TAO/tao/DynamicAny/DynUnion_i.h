/* -*- C++ -*- */
// $Id$

// ========================================================================
//
// = LIBRARY
//    TAO_DynamicAny
//
// = FILENAME
//    DynUnion_i.h
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ========================================================================

#ifndef TAO_DYNUNION_I_H
#define TAO_DYNUNION_I_H
#include "ace/pre.h"

#include "DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DynCommon.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class TAO_DynamicAny_Export TAO_DynUnion_i
  : public virtual DynamicAny::DynUnion,
    public virtual TAO_DynCommon,
    public virtual TAO_Local_RefCounted_Object
{
  // = TITLE
  //    TAO_DynUnion_i
  //
  // = DESCRIPTION
  //    Implementation of Dynamic Any type for Unions.
public:
  TAO_DynUnion_i (void);
  // Constructor.

  ~TAO_DynUnion_i (void);
  // Destructor.

  void init (const CORBA::Any& any ACE_ENV_ARG_DECL);
  // Constructor taking an Any argument.

  void init (CORBA::TypeCode_ptr tc ACE_ENV_ARG_DECL);
  // Constructor taking a typecode argument.

  // = LocalObject methods.
  static TAO_DynUnion_i *_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual void *_tao_QueryInterface (ptr_arith_t type);

  // = Functions specific to DynUnion.

  virtual DynamicAny::DynAny_ptr get_discriminator (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_discriminator (
      DynamicAny::DynAny_ptr d
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ));

  virtual void set_to_default_member (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ));

  virtual void set_to_no_active_member (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ));

  virtual CORBA::Boolean has_no_active_member (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual CORBA::TCKind discriminator_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual DynamicAny::DynAny_ptr member (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual char * member_name (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::TCKind member_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

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
  // Code common to the constructor from an Any arg and the member
  // function from_any().
  void set_from_any (const CORBA::Any &any,
                     CORBA::Boolean from_factory
                     ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAnyFactory::InconsistentTypeCode
      ));

  // Called by both versions of init().
  void init_common (void);

  // Iterative check for label value match.
  CORBA::Boolean label_match (const CORBA::Any &my_any,
                              const CORBA::Any &other_any
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  // Use copy() or assign() instead of these.
  TAO_DynUnion_i (const TAO_DynUnion_i &src);
  TAO_DynUnion_i &operator= (const TAO_DynUnion_i &src);

private:
  // Just two components.
  DynamicAny::DynAny_var member_;
  DynamicAny::DynAny_var discriminator_;

  // The active member's slot in the union type code.
  CORBA::ULong member_slot_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include "ace/post.h"
#endif /* TAO_DYNUNION_I_H */
