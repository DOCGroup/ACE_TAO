/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ValueDef_i.h"

ACE_RCSID(IFR_Service, ValueDef_i, "$Id$")

TAO_ValueDef_i::TAO_ValueDef_i (TAO_Repository_i *repo,
                                ACE_Configuration_Section_Key section_key)
  : TAO_IRObject_i (repo, section_key),
    TAO_Container_i (repo, section_key),
    TAO_Contained_i (repo, section_key),
    TAO_IDLType_i (repo, section_key)
{
}

TAO_ValueDef_i::~TAO_ValueDef_i (void)
{
}

CORBA::DefinitionKind
TAO_ValueDef_i::def_kind (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::dk_Value;
}

void
TAO_ValueDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ValueDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Contained::Description *
TAO_ValueDef_i::describe (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Contained::Description *
TAO_ValueDef_i::describe_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr
TAO_ValueDef_i::type (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::TypeCode_ptr
TAO_ValueDef_i::type_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->supported_interfaces_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::supported_interfaces (
    const CORBA::InterfaceDefSeq &supported_interfaces
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->supported_interfaces_i (supported_interfaces
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::supported_interfaces_i (
    const CORBA::InterfaceDefSeq & /* supported_interfaces */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::InitializerSeq *
TAO_ValueDef_i::initializers (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->initializers_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InitializerSeq *
TAO_ValueDef_i::initializers_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::initializers (const CORBA::InitializerSeq &initializers
                              ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->initializers_i (initializers
                        ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::initializers_i (const CORBA::InitializerSeq & /* initializers */
                                ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ValueDef_ptr
TAO_ValueDef_i::base_value (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::ValueDef::_nil ());

  return this->base_value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDef_ptr
TAO_ValueDef_i::base_value_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::base_value (CORBA::ValueDef_ptr base_value
                            ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->base_value_i (base_value
                      ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::base_value_i (CORBA::ValueDef_ptr /* base_value */
                              ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->abstract_base_values_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::abstract_base_values (
    const CORBA::ValueDefSeq &abstract_base_values
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->abstract_base_values_i (abstract_base_values
                                ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::abstract_base_values_i (
    const CORBA::ValueDefSeq & /* abstract_base_values */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean
TAO_ValueDef_i::is_abstract (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_abstract_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_abstract_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::is_abstract (CORBA::Boolean is_abstract
                             ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->is_abstract (is_abstract
                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_abstract_i (CORBA::Boolean /* is_abstract */
                             ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean
TAO_ValueDef_i::is_custom (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_custom_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_custom_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::is_custom (CORBA::Boolean is_custom
                           ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->is_custom_i (is_custom
                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_custom_i (CORBA::Boolean /* is_custom */
                             ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean
TAO_ValueDef_i::is_truncatable (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_truncatable_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_truncatable_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void
TAO_ValueDef_i::is_truncatable (CORBA::Boolean is_truncatable
                                ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->is_truncatable_i (is_truncatable
                          ACE_ENV_ARG_PARAMETER);
}

void
TAO_ValueDef_i::is_truncatable_i (CORBA::Boolean /* is_truncatable */
                                  ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean
TAO_ValueDef_i::is_a (const char *id
                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_a_i (id
                       ACE_ENV_ARG_PARAMETER);
}

CORBA::Boolean
TAO_ValueDef_i::is_a_i (const char *id
                        ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (id, "IDL:omg.org/CORBA/ValueBase:1.0") == 0)
    {
      return 1;
    }

  // TODO
  return 0;
}

CORBA::ValueDef::FullValueDescription *
TAO_ValueDef_i::describe_value (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_value_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ValueDef::FullValueDescription *
TAO_ValueDef_i::describe_value_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ValueMemberDef_ptr
TAO_ValueDef_i::create_value_member (const char *id,
                                     const char *name,
                                     const char *version,
                                     CORBA::IDLType_ptr type,
                                     CORBA::Visibility access
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ValueMemberDef::_nil ());

  return this->create_value_member_i (id,
                                      name,
                                      version,
                                      type,
                                      access
                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::ValueMemberDef_ptr
TAO_ValueDef_i::create_value_member_i (const char * /* id */,
                                       const char * /* name */,
                                       const char * /* version */,
                                       CORBA::IDLType_ptr /* type */,
                                       CORBA::Visibility /* access */
                                       ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::AttributeDef_ptr
TAO_ValueDef_i::create_attribute (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::AttributeDef::_nil ());

  return this->create_attribute_i (id,
                                   name,
                                   version,
                                   type,
                                   mode
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::AttributeDef_ptr
TAO_ValueDef_i::create_attribute_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    CORBA::IDLType_ptr /* type */,
    CORBA::AttributeMode /* mode */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::OperationDef_ptr
TAO_ValueDef_i::create_operation (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr result,
    CORBA::OperationMode mode,
    const CORBA::ParDescriptionSeq &params,
    const CORBA::ExceptionDefSeq &exceptions,
    const CORBA::ContextIdSeq &contexts
    ACE_ENV_ARG_DECL
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::OperationDef::_nil ());

  return this->create_operation_i (id,
                                   name,
                                   version,
                                   result,
                                   mode,
                                   params,
                                   exceptions,
                                   contexts
                                   ACE_ENV_ARG_PARAMETER);
}

CORBA::OperationDef_ptr
TAO_ValueDef_i::create_operation_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    CORBA::IDLType_ptr /* result */,
    CORBA::OperationMode /* mode */,
    const CORBA::ParDescriptionSeq & /* params */,
    const CORBA::ExceptionDefSeq & /* exceptions */,
    const CORBA::ContextIdSeq & /* contexts */
    ACE_ENV_ARG_DECL_NOT_USED /* ACE_ENV_SINGLE_ARG_PARAMETER */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}
