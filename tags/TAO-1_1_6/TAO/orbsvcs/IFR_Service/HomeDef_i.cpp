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

IR::DefinitionKind
TAO_HomeDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Home;
}

void 
TAO_HomeDef_i::destroy (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_HomeDef_i::describe (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr 
TAO_HomeDef_i::type (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::HomeDef_ptr 
TAO_HomeDef_i::base_home (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ComponentDef_ptr 
TAO_HomeDef_i::managed_component (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::PrimaryKeyDef_ptr 
TAO_HomeDef_i::primary_key (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::FactoryDefSeq *
TAO_HomeDef_i::factories (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::FinderDefSeq *
TAO_HomeDef_i::finders (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::Boolean 
TAO_HomeDef_i::is_basic (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::PrimaryKeyDef_ptr 
TAO_HomeDef_i::create_primary_key (const char * /* id */,
                                   const char * /* name */,
                                   const char * /* version */,
                                   IR::ValueDef_ptr /* primary_key */,
                                   CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::FactoryDef_ptr 
TAO_HomeDef_i::create_factory (const char * /* id */,
                               const char * /* name */,
                               const char * /* version */,
                               const IR::ParDescriptionSeq & /* params */,
                               const IR::ExceptionDefSeq & /* exceptions */,
                               CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::FinderDef_ptr 
TAO_HomeDef_i::create_finder (const char * /* id */,
                              const char * /* name */,
                              const char * /* version */,
                              const IR::ParDescriptionSeq & /* params */,
                              const IR::ExceptionDefSeq & /* exceptions */,
                              CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}
