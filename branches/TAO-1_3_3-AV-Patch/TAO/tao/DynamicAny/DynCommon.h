/* -*- C++ -*- */
// $Id$

// =========================================================================
//
// = LIBRARY
//    TAO_DynamicAny
//
// = FILENAME
//    DynCommon_i.h
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// =========================================================================

#ifndef TAO_DYNCOMMON_I_H
#define TAO_DYNCOMMON_I_H
#include /**/ "ace/pre.h"

#include "DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class TAO_DynamicAny_Export TAO_DynCommon
  : public virtual DynamicAny::DynAny
{
  // = TITLE
  //    TAO_DynCommon
  //
  // = DESCRIPTION
  //    Contains most of the functionality common to all the
  //    Dynamic Any implementation classes.
public:
  TAO_DynCommon (void);
  // Constructor.

  virtual ~TAO_DynCommon (void);
  // Destructor.

  // = Some functions common to all Dynamic Any types.

  virtual CORBA::TypeCode_ptr type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void assign (
      DynamicAny::DynAny_ptr dyn_any
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ));

  virtual void insert_boolean (
      CORBA::Boolean value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_octet (
      CORBA::Octet value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_char (
      CORBA::Char value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_short (
      CORBA::Short value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_ushort (
      CORBA::UShort value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_long (
      CORBA::Long value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_ulong (
      CORBA::ULong value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_float (
      CORBA::Float value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_double (
      CORBA::Double value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_string (
      const char * value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_reference (
      CORBA::Object_ptr value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_typecode (
      CORBA::TypeCode_ptr value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_longlong (
      CORBA::LongLong value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_ulonglong (
      CORBA::ULongLong value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_longdouble (
      CORBA::LongDouble value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_wchar (
      CORBA::WChar value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_wstring (
      const CORBA::WChar * value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_any (
      const CORBA::Any & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_dyn_any (
      DynamicAny::DynAny_ptr value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual void insert_val (
      CORBA::ValueBase * value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Boolean get_boolean (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Octet get_octet (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Char get_char (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Short get_short (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::UShort get_ushort (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Long get_long (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::ULong get_ulong (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Float get_float (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Double get_double (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual char * get_string (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Object_ptr get_reference (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::TypeCode_ptr get_typecode (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::LongLong get_longlong (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::ULongLong get_ulonglong (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::LongDouble get_longdouble (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::WChar get_wchar (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::WChar * get_wstring (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Any * get_any (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual DynamicAny::DynAny_ptr get_dyn_any (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::ValueBase * get_val (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Boolean seek (
      CORBA::Long index
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void rewind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual CORBA::Boolean next (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual DynamicAny::DynAny_ptr copy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual CORBA::ULong component_count (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

protected:
  // This sets one of two flags in this base class. CORBA 2.4.2
  // requires that destroy() do nothing if it is called on
  // the result of current_component(), the only non-deep-copying
  // method in the Dynamic Any API. If the boolean arg below is 0,
  // the call is from inside current_component(), and the flag
  // ref_to_component_ is set. If the boolean arg is 1, the call
  // is from inside destroy(), and the container_is_destroying_
  // flag is set, overriding the first flag in the component's
  // destroy() method.
  void set_flag (
      DynamicAny::DynAny_ptr component,
      CORBA::Boolean destroying
      ACE_ENV_ARG_DECL
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

protected:
  CORBA::Boolean ref_to_component_;
  // Were we created by current_component()?

  CORBA::Boolean container_is_destroying_;
  // Flag to let destroy() call by container override the flag above.

  CORBA::Boolean has_components_;
  // Do we contain other Dynamic Anys?

  CORBA::Boolean destroyed_;
  // Has destroy() been called on us yet?

  CORBA::Long current_position_;
  // Slot of the current component (if any).

  CORBA::ULong component_count_;
  // Number of components, as defined by CORBA 2.4.2.

  CORBA::TypeCode_var type_;
  // Holder for our type code.

  CORBA::Any any_;
  // Gets a value only for basic types, but used in insert_*
  // and get_*, defined in this base class.

private:
  // Utility functions used by insert_* and get_*.

  DynamicAny::DynAny_ptr check_component (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  void check_type (CORBA::TypeCode_ptr tc
                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ));

  CORBA::TypeCode_ptr check_type_and_unalias (CORBA::TypeCode_ptr tc
                                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNCOMMON_I_H */
