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
#include "ace/pre.h"

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

  void init (CORBA_TypeCode_ptr tc,
             CORBA::Environment &ACE_TRY_ENV);
  // Initialize using just a TypeCode.

  void init (const CORBA_Any& any,
             CORBA::Environment &ACE_TRY_ENV);
  // Initialize using an Any.

  // = LocalObject methods.
  static TAO_DynSequence_i *_narrow (
      CORBA::Object_ptr obj,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    );
  virtual void *_tao_QueryInterface (ptr_arith_t type);

  // = Functions specific to DynSequence.

  virtual CORBA::ULong get_length (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_length (
      CORBA::ULong len,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual DynamicAny::AnySeq * get_elements (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_elements (
      const DynamicAny::AnySeq & value,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual DynamicAny::DynAnySeq * get_elements_as_dyn_any (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void set_elements_as_dyn_any (
      const DynamicAny::DynAnySeq & value,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));


  // = DynAny common functions not implemented in class TAO_DynCommon.

  virtual void from_any (
      const CORBA::Any & value,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));

  virtual CORBA::Any * to_any (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual CORBA::Boolean equal (
      DynamicAny::DynAny_ptr dyn_any,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual void destroy (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));

  virtual DynamicAny::DynAny_ptr current_component (
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ()
    )
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch
      ));

private:
  // Utility, turns the type of elements contained in the sequence.
  CORBA::TypeCode_ptr get_element_type (CORBA::Environment &ACE_TRY_ENV =
                                          TAO_default_environment ());

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

#include "ace/post.h"
#endif /* TAO_DYNSEQUENCE_I_H */
