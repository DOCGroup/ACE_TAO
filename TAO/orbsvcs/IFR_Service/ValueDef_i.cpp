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

IR::DefinitionKind
TAO_ValueDef_i::def_kind (CORBA::Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  return IR::dk_Value;
}

void 
TAO_ValueDef_i::destroy (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->destroy_i (ACE_TRY_ENV);
}

void 
TAO_ValueDef_i::destroy_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_ValueDef_i::describe (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->describe_i (ACE_TRY_ENV);
}

IR::Contained::Description *
TAO_ValueDef_i::describe_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr 
TAO_ValueDef_i::type (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (CORBA::TypeCode::_nil ());

  return this->type_i (ACE_TRY_ENV);
}

CORBA::TypeCode_ptr 
TAO_ValueDef_i::type_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->supported_interfaces_i (ACE_TRY_ENV);
}

IR::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::supported_interfaces (
    const IR::InterfaceDefSeq &supported_interfaces,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->supported_interfaces_i (supported_interfaces,
                                ACE_TRY_ENV);
}

void 
TAO_ValueDef_i::supported_interfaces_i (
    const IR::InterfaceDefSeq & /* supported_interfaces */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::InitializerSeq *
TAO_ValueDef_i::initializers (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->initializers_i (ACE_TRY_ENV);
}

IR::InitializerSeq *
TAO_ValueDef_i::initializers_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::initializers (const IR::InitializerSeq &initializers,
                              CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->initializers_i (initializers,
                        ACE_TRY_ENV);
}

void 
TAO_ValueDef_i::initializers_i (const IR::InitializerSeq & /* initializers */,
                                CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::ValueDef_ptr 
TAO_ValueDef_i::base_value (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (IR::ValueDef::_nil ());

  return this->base_value_i (ACE_TRY_ENV);
}

IR::ValueDef_ptr 
TAO_ValueDef_i::base_value_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::base_value (IR::ValueDef_ptr base_value,
                            CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->base_value_i (base_value,
                      ACE_TRY_ENV);
}

void 
TAO_ValueDef_i::base_value_i (IR::ValueDef_ptr /* base_value */,
                              CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->abstract_base_values_i (ACE_TRY_ENV);
}

IR::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::abstract_base_values (
    const IR::ValueDefSeq &abstract_base_values,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;
  
  this->abstract_base_values_i (abstract_base_values,
                                ACE_TRY_ENV);
}

void 
TAO_ValueDef_i::abstract_base_values_i (
    const IR::ValueDefSeq & /* abstract_base_values */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean 
TAO_ValueDef_i::is_abstract (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_abstract_i (ACE_TRY_ENV);
}

CORBA::Boolean 
TAO_ValueDef_i::is_abstract_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::is_abstract (CORBA::Boolean is_abstract,
                             CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->is_abstract (is_abstract,
                     ACE_TRY_ENV);
}

void 
TAO_ValueDef_i::is_abstract_i (CORBA::Boolean /* is_abstract */,
                             CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean 
TAO_ValueDef_i::is_custom (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_custom_i (ACE_TRY_ENV);
}

CORBA::Boolean 
TAO_ValueDef_i::is_custom_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::is_custom (CORBA::Boolean is_custom,
                           CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->is_custom_i (is_custom,
                     ACE_TRY_ENV);
}

void 
TAO_ValueDef_i::is_custom_i (CORBA::Boolean /* is_custom */,
                             CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean 
TAO_ValueDef_i::is_truncatable (CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_truncatable_i (ACE_TRY_ENV);
}

CORBA::Boolean 
TAO_ValueDef_i::is_truncatable_i (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::is_truncatable (CORBA::Boolean is_truncatable,
                                CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->is_truncatable_i (is_truncatable,
                          ACE_TRY_ENV);
}

void 
TAO_ValueDef_i::is_truncatable_i (CORBA::Boolean /* is_truncatable */,
                                  CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean 
TAO_ValueDef_i::is_a (const char *id,
                      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  return this->is_a_i (id,
                       ACE_TRY_ENV);
}

CORBA::Boolean 
TAO_ValueDef_i::is_a_i (const char * /* id */,
                        CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ValueMemberDef_ptr 
TAO_ValueDef_i::create_value_member (const char *id,
                                     const char *name,
                                     const char *version,
                                     IR::IDLType_ptr type,
                                     CORBA::Visibility access,
                                     CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR::ValueMemberDef::_nil ());

  return this->create_value_member_i (id,
                                      name,
                                      version,
                                      type,
                                      access,
                                      ACE_TRY_ENV);
}

IR::ValueMemberDef_ptr 
TAO_ValueDef_i::create_value_member_i (const char * /* id */,
                                       const char * /* name */,
                                       const char * /* version */,
                                       IR::IDLType_ptr /* type */,
                                       CORBA::Visibility /* access */,
                                       CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::AttributeDef_ptr 
TAO_ValueDef_i::create_attribute (
    const char *id,
    const char *name,
    const char *version,
    IR::IDLType_ptr type,
    IR::AttributeMode mode,
    const IR::ExceptionDefSeq &get_exceptions,
    const IR::ExceptionDefSeq &put_exceptions,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR::AttributeDef::_nil ());

  return this->create_attribute_i (id,
                                   name,
                                   version,
                                   type,
                                   mode,
                                   get_exceptions,
                                   put_exceptions,
                                   ACE_TRY_ENV);
}

IR::AttributeDef_ptr 
TAO_ValueDef_i::create_attribute_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    IR::IDLType_ptr /* type */,
    IR::AttributeMode /* mode */,
    const IR::ExceptionDefSeq & /* get_exceptions */,
    const IR::ExceptionDefSeq & /* put_exceptions */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::OperationDef_ptr 
TAO_ValueDef_i::create_operation (
    const char *id,
    const char *name,
    const char *version,
    IR::IDLType_ptr result,
    IR::OperationMode mode,
    const IR::ParDescriptionSeq &params,
    const IR::ExceptionDefSeq &exceptions,
    const IR::ContextIdSeq &contexts,
    CORBA::Environment &ACE_TRY_ENV
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (IR::OperationDef::_nil ());

  return this->create_operation_i (id,
                                   name,
                                   version,
                                   result,
                                   mode,
                                   params,
                                   exceptions,
                                   contexts,
                                   ACE_TRY_ENV);
}

IR::OperationDef_ptr 
TAO_ValueDef_i::create_operation_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    IR::IDLType_ptr /* result */,
    IR::OperationMode /* mode */,
    const IR::ParDescriptionSeq & /* params */,
    const IR::ExceptionDefSeq & /* exceptions */,
    const IR::ContextIdSeq & /* contexts */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

