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
TAO_ValueDef_i::destroy (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::Contained::Description *
TAO_ValueDef_i::describe (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::TypeCode_ptr 
TAO_ValueDef_i::type (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::InterfaceDefSeq *
TAO_ValueDef_i::supported_interfaces (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::supported_interfaces (
    const IR::InterfaceDefSeq & /* supported_interfaces */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::InitializerSeq *
TAO_ValueDef_i::initializers (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::initializers (const IR::InitializerSeq & /* initializers */,
                              CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::ValueDef_ptr 
TAO_ValueDef_i::base_value (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::base_value (IR::ValueDef_ptr /* base_value */,
                            CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

IR::ValueDefSeq *
TAO_ValueDef_i::abstract_base_values (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::abstract_base_values (
    const IR::ValueDefSeq & /* abstract_base_values */,
    CORBA::Environment & /* ACE_TRY_ENV */
  )
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean 
TAO_ValueDef_i::is_abstract (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::is_abstract (CORBA::Boolean /* is_abstract */,
                             CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean 
TAO_ValueDef_i::is_custom (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::is_custom (CORBA::Boolean /* is_custom */,
                           CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean 
TAO_ValueDef_i::is_truncatable (CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ValueDef_i::is_truncatable (CORBA::Boolean /* is_truncatable */,
                                CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::Boolean 
TAO_ValueDef_i::is_a (const char * /* id */,
                      CORBA::Environment & /* ACE_TRY_ENV */)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

IR::ValueMemberDef_ptr 
TAO_ValueDef_i::create_value_member (const char * /* id */,
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

