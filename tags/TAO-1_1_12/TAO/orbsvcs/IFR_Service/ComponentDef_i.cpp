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

IR_DefinitionKind
TAO_ComponentDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return dk_Component;
}

void 
TAO_ComponentDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_TRY_ENV);
}

void 
TAO_ComponentDef_i::destroy_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR_Contained::Description *
TAO_ComponentDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_TRY_ENV);
}

IR_Contained::Description *
TAO_ComponentDef_i::describe_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr 
TAO_ComponentDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_ComponentDef_i::type_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->supported_interfaces_i (ACE_TRY_ENV);
}

IR_InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ComponentDef_i::supported_interfaces (
    const IR_InterfaceDefSeq &supported_interfaces,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->supported_interfaces_i (supported_interfaces,
                                ACE_TRY_ENV);
}

void 
TAO_ComponentDef_i::supported_interfaces_i (
    const IR_InterfaceDefSeq & /* supported_interfaces */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR_ComponentDef_ptr 
TAO_ComponentDef_i::base_component (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR_ComponentDef::_nil ());

  return this->base_component_i (ACE_TRY_ENV);
}

IR_ComponentDef_ptr 
TAO_ComponentDef_i::base_component_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_ProvidesDefSeq *
TAO_ComponentDef_i::provides_interfaces (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->provides_interfaces_i (ACE_TRY_ENV);
}

IR_ProvidesDefSeq *
TAO_ComponentDef_i::provides_interfaces_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_UsesDefSeq *
TAO_ComponentDef_i::uses_interfaces (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->uses_interfaces_i (ACE_TRY_ENV);
}

IR_UsesDefSeq *
TAO_ComponentDef_i::uses_interfaces_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_EmitsDefSeq *
TAO_ComponentDef_i::emits_events (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->emits_events_i (ACE_TRY_ENV);
}

IR_EmitsDefSeq *
TAO_ComponentDef_i::emits_events_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_PublishesDefSeq *
TAO_ComponentDef_i::publishes_events (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->publishes_events_i (ACE_TRY_ENV);
}

IR_PublishesDefSeq *
TAO_ComponentDef_i::publishes_events_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_ConsumesDefSeq *
TAO_ComponentDef_i::consumes_events (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->consumes_events_i (ACE_TRY_ENV);
}

IR_ConsumesDefSeq *
TAO_ComponentDef_i::consumes_events_i (CORBA::Environment & /* ACE_TRY_ENV */)
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

IR_ProvidesDef_ptr 
TAO_ComponentDef_i::create_provides (
    const char *id,
    const char *name,
    const char *version,
    IR_InterfaceDef_ptr interface_type,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR_ProvidesDef::_nil ());

  return this->create_provides_i (id,
                                  name,
                                  version,
                                  interface_type,
                                  ACE_TRY_ENV);
}

IR_ProvidesDef_ptr 
TAO_ComponentDef_i::create_provides_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    IR_InterfaceDef_ptr  /* interface_type */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_UsesDef_ptr 
TAO_ComponentDef_i::create_uses (const char *id,
                                 const char *name,
                                 const char *version,
                                 IR_InterfaceDef_ptr interface_type,
                                 CORBA::Boolean is_multiple,
                                 CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR_UsesDef::_nil ());

  return this->create_uses_i (id,
                              name,
                              version,
                              interface_type,
                              is_multiple,
                              ACE_TRY_ENV);
}

IR_UsesDef_ptr 
TAO_ComponentDef_i::create_uses_i (const char * /* id */,
                                   const char * /* name */,
                                   const char * /* version */,
                                   IR_InterfaceDef_ptr  /* interface_type */,
                                   CORBA::Boolean /* is_multiple */,
                                   CORBA::Environment & /* ACE_TRY_ENV */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_EmitsDef_ptr 
TAO_ComponentDef_i::create_emits (const char *id,
                                  const char *name,
                                  const char *version,
                                  IR_ValueDef_ptr value,
                                  CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR_EmitsDef::_nil ());

  return this->create_emits_i (id,
                               name,
                               version,
                               value,
                                ACE_TRY_ENV);
}

IR_EmitsDef_ptr 
TAO_ComponentDef_i::create_emits_i (const char * /* id */,
                                    const char * /* name */,
                                    const char * /* version */,
                                    IR_ValueDef_ptr  /* value */,
                                    CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_PublishesDef_ptr 
TAO_ComponentDef_i::create_publishes (const char *id,
                                      const char *name,
                                      const char *version,
                                      IR_ValueDef_ptr value,
                                      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR_PublishesDef::_nil ());

  return this->create_publishes_i (id,
                                   name,
                                   version,
                                   value,
                                   ACE_TRY_ENV);
}

IR_PublishesDef_ptr 
TAO_ComponentDef_i::create_publishes_i (const char * /* id */,
                                        const char * /* name */,
                                        const char * /* version */,
                                        IR_ValueDef_ptr  /* value */,
                                        CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR_ConsumesDef_ptr 
TAO_ComponentDef_i::create_consumes (const char *id,
                                     const char *name,
                                     const char *version,
                                     IR_ValueDef_ptr value,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR_ConsumesDef::_nil ());

  return this->create_consumes_i (id,
                                  name,
                                  version,
                                  value,
                                  ACE_TRY_ENV);
}

IR_ConsumesDef_ptr 
TAO_ComponentDef_i::create_consumes_i (const char * /* id */,
                                       const char * /* name */,
                                       const char * /* version */,
                                       IR_ValueDef_ptr  /* value */,
                                       CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

