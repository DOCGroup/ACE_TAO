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

IR_DefinitionKind
TAO_HomeDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return dk_Home;
}

void 
TAO_HomeDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_TRY_ENV);
}

void 
TAO_HomeDef_i::destroy_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR_Contained::Description *
TAO_HomeDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_TRY_ENV);
}

IR_Contained::Description *
TAO_HomeDef_i::describe_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr 
TAO_HomeDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_HomeDef_i::type_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_HomeDef_ptr 
TAO_HomeDef_i::base_home (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR_HomeDef::_nil ());

  return this->base_home_i (ACE_TRY_ENV);
}

IR_HomeDef_ptr 
TAO_HomeDef_i::base_home_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_ComponentDef_ptr 
TAO_HomeDef_i::managed_component (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR_ComponentDef::_nil ());

  return this->managed_component_i (ACE_TRY_ENV);
}

IR_ComponentDef_ptr 
TAO_HomeDef_i::managed_component_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_PrimaryKeyDef_ptr 
TAO_HomeDef_i::primary_key (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR_PrimaryKeyDef::_nil ());

  return this->primary_key_i (ACE_TRY_ENV);
}

IR_PrimaryKeyDef_ptr 
TAO_HomeDef_i::primary_key_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_FactoryDefSeq *
TAO_HomeDef_i::factories (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->factories_i (ACE_TRY_ENV);
}

IR_FactoryDefSeq *
TAO_HomeDef_i::factories_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_FinderDefSeq *
TAO_HomeDef_i::finders (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->finders_i (ACE_TRY_ENV);
}

IR_FinderDefSeq *
TAO_HomeDef_i::finders_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::Boolean 
TAO_HomeDef_i::is_basic (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_basic_i (ACE_TRY_ENV);
}

CORBA::Boolean 
TAO_HomeDef_i::is_basic_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_PrimaryKeyDef_ptr 
TAO_HomeDef_i::create_primary_key (const char *id,
                                   const char *name,
                                   const char *version,
                                   IR_ValueDef_ptr primary_key,
                                   CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR_PrimaryKeyDef::_nil ());

  return this->create_primary_key_i (id,
                                     name,
                                     version,
                                     primary_key,
                                     ACE_TRY_ENV);
}

IR_PrimaryKeyDef_ptr 
TAO_HomeDef_i::create_primary_key_i (const char * /* id */,
                                     const char * /* name */,
                                     const char * /* version */,
                                     IR_ValueDef_ptr /* primary_key */,
                                     CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_FactoryDef_ptr 
TAO_HomeDef_i::create_factory (const char *id,
                               const char *name,
                               const char *version,
                               const IR_ParDescriptionSeq &params,
                               const IR_ExceptionDefSeq &exceptions,
                               CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR_FactoryDef::_nil ());

  return this->create_factory_i (id,
                                 name,
                                 version,
                                 params,
                                 exceptions,
                                 ACE_TRY_ENV);
}

IR_FactoryDef_ptr 
TAO_HomeDef_i::create_factory_i (const char * /* id */,
                                 const char * /* name */,
                                 const char * /* version */,
                                 const IR_ParDescriptionSeq & /* params */,
                                 const IR_ExceptionDefSeq & /* exceptions */,
                                 CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_FinderDef_ptr 
TAO_HomeDef_i::create_finder (const char *id,
                              const char *name,
                              const char *version,
                              const IR_ParDescriptionSeq &params,
                              const IR_ExceptionDefSeq &exceptions,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR_FinderDef::_nil ());

  return this->create_finder_i (id,
                                name,
                                version,
                                params,
                                exceptions,
                                ACE_TRY_ENV);
}

IR_FinderDef_ptr 
TAO_HomeDef_i::create_finder_i (const char * /* id */,
                                const char * /* name */,
                                const char * /* version */,
                                const IR_ParDescriptionSeq & /* params */,
                                const IR_ExceptionDefSeq & /* exceptions */,
                                CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}
