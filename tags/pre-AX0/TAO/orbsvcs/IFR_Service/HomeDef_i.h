/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/IFR_Service
//
// = FILENAME
//    HomeDef_i.h
//
// = DESCRIPTION
//    HomeDef servant class.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_HOMEDEF_I_H
#define TAO_HOMEDEF_I_H

#include "InterfaceDef_i.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "IFR_ComponentsS.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_HomeDef_i : public virtual TAO_InterfaceDef_i
{
  // = TITLE
  //    HomTAO_HomeDef_ieDef_i
  //
  // = DESCRIPTION
  //    Represents a home definition, containing factory and finder.
  //
public:
  TAO_HomeDef_i (TAO_Repository_i *repo,
                 ACE_Configuration_Section_Key section_key);
  // Constructor

  virtual ~TAO_HomeDef_i (void);
  // Destructor

  virtual CORBA::DefinitionKind def_kind (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return our definition kind.

  virtual void destroy (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // Remove the repository entry.

  virtual void destroy_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA_Contained::Description *describe (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA_Contained::Description *describe_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From Contained_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual CORBA::TypeCode_ptr type_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
  // From IDLType_i's pure virtual function.

  virtual IR::HomeDef_ptr base_home (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::HomeDef_ptr base_home_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::ComponentDef_ptr managed_component (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::ComponentDef_ptr managed_component_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::PrimaryKeyDef_ptr primary_key (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::PrimaryKeyDef_ptr primary_key_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::FactoryDefSeq *factories (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::FactoryDefSeq *factories_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::FinderDefSeq *finders (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::FinderDefSeq *finders_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual CORBA::Boolean is_basic (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::Boolean is_basic_i (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::PrimaryKeyDef_ptr create_primary_key (
      const char *id,
      const char *name,
      const char *version,
      CORBA_ValueDef_ptr primary_key
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::PrimaryKeyDef_ptr create_primary_key_i (
      const char *id,
      const char *name,
      const char *version,
      CORBA_ValueDef_ptr primary_key
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::FactoryDef_ptr create_factory (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_ParDescriptionSeq &params,
      const CORBA_ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::FactoryDef_ptr create_factory_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_ParDescriptionSeq &params,
      const CORBA_ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual IR::FinderDef_ptr create_finder (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_ParDescriptionSeq &params,
      const CORBA_ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));

  IR::FinderDef_ptr create_finder_i (
      const char *id,
      const char *name,
      const char *version,
      const CORBA_ParDescriptionSeq &params,
      const CORBA_ExceptionDefSeq &exceptions
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)

    ACE_THROW_SPEC ((CORBA::SystemException));
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_HOMEDEF_I_H */



