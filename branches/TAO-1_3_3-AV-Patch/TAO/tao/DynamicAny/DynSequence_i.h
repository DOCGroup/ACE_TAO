/* -*- C++ -*- */
// $Id$

// ===================================================================
//
// = LIBRARY
//    TAO_DynamicAny
//
// = FILENAME
//    DynSequence_i.h
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ===================================================================

#ifndef TAO_DYNSEQUENCE_I_H
#define TAO_DYNSEQUENCE_I_H
#include /**/ "ace/pre.h"

#include "DynamicAny.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "DynCommon.h"
#include "ace/Containers.h"

#if defined (_MSC_VER)
# if (_MSC_VER >= 1200)
#  pragma warning(push)
# endif /* _MSC_VER >= 1200 */
# pragma warning (disable:4250)
#endif /* _MSC_VER */

class TAO_DynamicAny_Export TAO_DynSequence_i
  : public virtual DynamicAny::DynSequence,
    public virtual TAO_DynCommon,
    public virtual TAO_Local_RefCounted_Object
{
  // = TITLE
  //    TAO_DynSequence_i
  //
  // = DESCRIPTION
  //    Implementation of Dynamic Any type for Sequences.
  //
public:
  TAO_DynSequence_i (void);
  // Constructor.

  ~TAO_DynSequence_i (void);
  // Destructor.

  void init (CORBA::TypeCode_ptr tc
             ACE_ENV_ARG_DECL);
  // Initialize using just a TypeCode.

  void init (const CORBA::Any& any
             ACE_ENV_ARG_DECL);
  // Initialize using an Any.

  // = LocalObject methods.
  static TAO_DynSequence_i *_narrow (
      CORBA::Object_ptr obj
      ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual void *_tao_QueryInterface (ptrdiff_t type);

  // = Functions specific to DynSequence.

  virtual CORBA::ULong get_length (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_length (
      CORBA::ULong len
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual DynamicAny::AnySeq * get_elements (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_elements (
      const DynamicAny::AnySeq & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual DynamicAny::DynAnySeq * get_elements_as_dyn_any (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_elements_as_dyn_any (
      const DynamicAny::DynAnySeq & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
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
  // Utility, turns the type of elements contained in the sequence.
  CORBA::TypeCode_ptr get_element_type (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

  // Called by both versions of init().
  void init_common (void);

  // = Use copy() or assign() instead of these
  TAO_DynSequence_i (const TAO_DynSequence_i &src);
  TAO_DynSequence_i &operator= (const TAO_DynSequence_i &src);

private:
  ACE_Array_Base<DynamicAny::DynAny_var> da_members_;
  // Each component is also a DynAny.
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNSEQUENCE_I_H */
