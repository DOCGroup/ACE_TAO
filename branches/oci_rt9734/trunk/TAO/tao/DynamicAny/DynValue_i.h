// -*- C++ -*-
//
// $Id$

//=============================================================================
/**
 *  @file    DynValue_i.h
 *
 *  $Id$
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_DYNVALUE_I_H
#define TAO_DYNVALUE_I_H
#include /**/ "ace/pre.h"

#include "tao/DynamicAny/DynValueCommon_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (_MSC_VER)
# pragma warning(push)
# pragma warning (disable:4250)
#endif /* _MSC_VER */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_DynValue_i
 *
 * @brief Implementation of the DynValue class.
 */
class TAO_DynamicAny_Export TAO_DynValue_i
  : public virtual DynamicAny::DynValue,
    public virtual TAO_DynValueCommon_i
{
public:
  TAO_DynValue_i (void);

  ~TAO_DynValue_i (void);

  virtual char * current_member_name (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));
  
  virtual CORBA::TCKind current_member_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));
  
  virtual DynamicAny::NameValuePairSeq * get_members (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
  
  virtual void set_members (
      const DynamicAny::NameValuePairSeq & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));
  
  virtual DynamicAny::NameDynAnyPairSeq * get_members_as_dyn_any (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException
      ));
  
  virtual void set_members_as_dyn_any (
      const DynamicAny::NameDynAnyPairSeq & value
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((
        CORBA::SystemException,
        DynamicAny::DynAny::TypeMismatch,
        DynamicAny::DynAny::InvalidValue
      ));
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined(_MSC_VER)
# pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_DYNVALUE_I_H */
