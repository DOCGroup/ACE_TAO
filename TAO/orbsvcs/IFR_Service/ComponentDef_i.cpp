/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ComponentDef_i.h"

ACE_RCSID(IFR_Service, ComponentDef_i, "$Id$")

TAO_ComponentDef_i::TAO_ComponentDef_i (
    TAO_Repository_i *repo,
    ACE_Configuration_Section_Key section_key
  )
  : TAO_IRObject_i (repo, section_key),
    TAO_Container_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key),
    TAO_InterfaceDef_i (repo, section_key)
{
}

TAO_ComponentDef_i::~TAO_ComponentDef_i (void)
{
}

IR::DefinitionKind
TAO_ComponentDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Component;
}

void 
TAO_ComponentDef_i::destroy (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_ComponentDef_i::describe (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr 
TAO_ComponentDef_i::type (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ComponentDef_i::supported_interfaces (
    const IR::InterfaceDefSeq &supported_interfaces,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::ComponentDef_ptr 
TAO_ComponentDef_i::base_component (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ProvidesDefSeq *
TAO_ComponentDef_i::provides_interfaces (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::UsesDefSeq *
TAO_ComponentDef_i::uses_interfaces (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::EmitsDefSeq *
TAO_ComponentDef_i::emits_events (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::PublishesDefSeq *
TAO_ComponentDef_i::publishes_events (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ConsumesDefSeq *
TAO_ComponentDef_i::consumes_events (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::Boolean 
TAO_ComponentDef_i::is_basic (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ProvidesDef_ptr 
TAO_ComponentDef_i::create_provides (const char *id,
                                     const char *name,
                                     const char *version,
                                     IR::InterfaceDef_ptr interface_type,
                                     CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::UsesDef_ptr 
TAO_ComponentDef_i::create_uses (const char *id,
                                 const char *name,
                                 const char *version,
                                 IR::InterfaceDef_ptr interface_type,
                                 CORBA::Boolean is_multiple,
                                 CORBA::Environment & /* ACE_TRY_ENV */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::EmitsDef_ptr 
TAO_ComponentDef_i::create_emits (const char *id,
                                  const char *name,
                                  const char *version,
                                  IR::ValueDef_ptr value,
                                  CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::PublishesDef_ptr 
TAO_ComponentDef_i::create_publishes (const char *id,
                                      const char *name,
                                      const char *version,
                                      IR::ValueDef_ptr value,
                                      CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ConsumesDef_ptr 
TAO_ComponentDef_i::create_consumes (const char *id,
                                     const char *name,
                                     const char *version,
                                     IR::ValueDef_ptr value,
                                     CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

