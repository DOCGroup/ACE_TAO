/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/orbsvcs/IFRService
//
// = FILENAME
//    EnumDef_i.h
//
// = DESCRIPTION
//    EnumDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_ENUMDEF_I_H
#define TAO_ENUMDEF_I_H

#include "TypedefDef_i.h"
#include "ifr_service_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_IFRService_Export TAO_EnumDef_i : public virtual TAO_TypedefDef_i
{
public:
  // = TITLE
  //    TAO_EnumDef_i
  //
  // = DESCRIPTION
  //    Represents an OMG IDL enumeration definition.
  //
public:
  TAO_EnumDef_i (TAO_Repository_i *repoy);
  // Constructor

  virtual ~TAO_EnumDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual CORBA::TypeCode_ptr type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::EnumMemberSeq *members (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::EnumMemberSeq *members_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void members (
      const CORBA::EnumMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  void members_i (
      const CORBA::EnumMemberSeq &members
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_ENUMDEF_I_H */
