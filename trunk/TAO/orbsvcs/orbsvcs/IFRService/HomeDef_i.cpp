/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "HomeDef_i.h"

ACE_RCSID(IFR_Service, HomeDef_i, "$Id$")

TAO_HomeDef_i::TAO_HomeDef_i (TAO_Repository_i *repo,
                              ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key),
    TAO_Container_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key),
    TAO_InterfaceDef_i (repo, section_key)
{
}

TAO_HomeDef_i::~TAO_HomeDef_i (void)
{
}

CORBA::DefinitionKind
TAO_HomeDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Home;
}

void
TAO_HomeDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_HomeDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Contained::Description *
TAO_HomeDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_HomeDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr
TAO_HomeDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_HomeDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::HomeDef_ptr
TAO_HomeDef_i::base_home (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR::HomeDef::_nil ());

  return this->base_home_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

IR::HomeDef_ptr
TAO_HomeDef_i::base_home_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ComponentDef_ptr
TAO_HomeDef_i::managed_component (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR::ComponentDef::_nil ());

  return this->managed_component_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

IR::ComponentDef_ptr
TAO_HomeDef_i::managed_component_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::PrimaryKeyDef_ptr
TAO_HomeDef_i::primary_key (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR::PrimaryKeyDef::_nil ());

  return this->primary_key_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

IR::PrimaryKeyDef_ptr
TAO_HomeDef_i::primary_key_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::FactoryDefSeq *
TAO_HomeDef_i::factories (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->factories_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

IR::FactoryDefSeq *
TAO_HomeDef_i::factories_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::FinderDefSeq *
TAO_HomeDef_i::finders (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->finders_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

IR::FinderDefSeq *
TAO_HomeDef_i::finders_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::Boolean
TAO_HomeDef_i::is_basic (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_basic_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_HomeDef_i::is_basic_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::PrimaryKeyDef_ptr
TAO_HomeDef_i::create_primary_key (const char *id,
                                   const char *name,
                                   const char *version,
                                   CORBA::ValueDef_ptr primary_key
                                   ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR::PrimaryKeyDef::_nil ());

  return this->create_primary_key_i (id,
                                     name,
                                     version,
                                     primary_key
                                     ACE_ENV_ARG_PARAMETER);
}

IR::PrimaryKeyDef_ptr
TAO_HomeDef_i::create_primary_key_i (const char * /* id */,
                                     const char * /* name */,
                                     const char * /* version */,
                                     CORBA::ValueDef_ptr /* primary_key */
                                     ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::FactoryDef_ptr
TAO_HomeDef_i::create_factory (const char *id,
                               const char *name,
                               const char *version,
                               const CORBA::ParDescriptionSeq &params,
                               const CORBA::ExceptionDefSeq &exceptions
                               ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR::FactoryDef::_nil ());

  return this->create_factory_i (id,
                                 name,
                                 version,
                                 params,
                                 exceptions
                                 ACE_ENV_ARG_PARAMETER);
}

IR::FactoryDef_ptr
TAO_HomeDef_i::create_factory_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    const CORBA::ParDescriptionSeq & /* params */,
    const CORBA::ExceptionDefSeq & /* exceptions */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::FinderDef_ptr
TAO_HomeDef_i::create_finder (const char *id,
                              const char *name,
                              const char *version,
                              const CORBA::ParDescriptionSeq &params,
                              const CORBA::ExceptionDefSeq &exceptions
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR::FinderDef::_nil ());

  return this->create_finder_i (id,
                                name,
                                version,
                                params,
                                exceptions
                                ACE_ENV_ARG_PARAMETER);
}

IR::FinderDef_ptr
TAO_HomeDef_i::create_finder_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    const CORBA::ParDescriptionSeq & /* params */,
    const CORBA::ExceptionDefSeq & /* exceptions */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}
