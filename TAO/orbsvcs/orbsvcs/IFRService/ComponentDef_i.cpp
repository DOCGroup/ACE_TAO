/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ComponentDef_i.h"

ACE_RCSID (IFRService, 
           ComponentDef_i, 
           "$Id$")

TAO_ComponentDef_i::TAO_ComponentDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceDef_i (repo),
    TAO_ExtInterfaceDef_i (repo),
    TAO_InterfaceAttrExtension_i (repo)
{
}

TAO_ComponentDef_i::~TAO_ComponentDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ComponentDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Component;
}

void
TAO_ComponentDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ComponentDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Contained::Description *
TAO_ComponentDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ComponentDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr
TAO_ComponentDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ComponentDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->supported_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDefSeq *
TAO_ComponentDef_i::supported_interfaces_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ComponentDef_i::supported_interfaces (
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->supported_interfaces_i (supported_interfaces
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ComponentDef_i::supported_interfaces_i (
    const CORBA::InterfaceDefSeq & /* supported_interfaces */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ComponentIR::ComponentDef_ptr
TAO_ComponentDef_i::base_component (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::ComponentDef::_nil ());

  return this->base_component_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ComponentIR::ComponentDef_ptr
TAO_ComponentDef_i::base_component_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ComponentDef_i::base_component (
    CORBA::ComponentIR::ComponentDef_ptr base_component
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->base_component_i (base_component
                          ACE_ENV_ARG_PARAMETER);
}

void
TAO_ComponentDef_i::base_component_i (
    CORBA::ComponentIR::ComponentDef_ptr base_component
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ComponentIR::ProvidesDef_ptr
TAO_ComponentDef_i::create_provides (
    const char *id,
    const char *name,
    const char *version,
    CORBA::InterfaceDef_ptr interface_type
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::ProvidesDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::ProvidesDef::_nil ());

  return this->create_provides_i (id,
                                  name,
                                  version,
                                  interface_type
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::ProvidesDef_ptr
TAO_ComponentDef_i::create_provides_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    CORBA::InterfaceDef_ptr  /* interface_type */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ComponentIR::UsesDef_ptr
TAO_ComponentDef_i::create_uses (const char *id,
                                 const char *name,
                                 const char *version,
                                 CORBA::InterfaceDef_ptr interface_type,
                                 CORBA::Boolean is_multiple
                                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::UsesDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::UsesDef::_nil ());

  return this->create_uses_i (id,
                              name,
                              version,
                              interface_type,
                              is_multiple
                              ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::UsesDef_ptr
TAO_ComponentDef_i::create_uses_i (const char * /* id */,
                                   const char * /* name */,
                                   const char * /* version */,
                                   CORBA::InterfaceDef_ptr  /* interface_type */,
                                   CORBA::Boolean /* is_multiple */
                                   ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */ )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ComponentIR::EmitsDef_ptr
TAO_ComponentDef_i::create_emits (const char *id,
                                  const char *name,
                                  const char *version,
                                  CORBA::ValueDef_ptr value
                                  ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::EmitsDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::EmitsDef::_nil ());

  return this->create_emits_i (id,
                               name,
                               version,
                               value
                                ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::EmitsDef_ptr
TAO_ComponentDef_i::create_emits_i (const char * /* id */,
                                    const char * /* name */,
                                    const char * /* version */,
                                    CORBA::ValueDef_ptr  /* value */
                                    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ComponentIR::PublishesDef_ptr
TAO_ComponentDef_i::create_publishes (const char *id,
                                      const char *name,
                                      const char *version,
                                      CORBA::ValueDef_ptr value
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::PublishesDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::PublishesDef::_nil ());

  return this->create_publishes_i (id,
                                   name,
                                   version,
                                   value
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::PublishesDef_ptr
TAO_ComponentDef_i::create_publishes_i (const char * /* id */,
                                        const char * /* name */,
                                        const char * /* version */,
                                        CORBA::ValueDef_ptr  /* value */
                                        ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ComponentIR::ConsumesDef_ptr
TAO_ComponentDef_i::create_consumes (const char *id,
                                     const char *name,
                                     const char *version,
                                     CORBA::ValueDef_ptr value
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ComponentIR::ConsumesDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ComponentIR::ConsumesDef::_nil ());

  return this->create_consumes_i (id,
                                  name,
                                  version,
                                  value
                                  ACE_ENV_ARG_PARAMETER);
}

CORBA::ComponentIR::ConsumesDef_ptr
TAO_ComponentDef_i::create_consumes_i (const char * /* id */,
                                       const char * /* name */,
                                       const char * /* version */,
                                       CORBA::ValueDef_ptr  /* value */
                                       ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

