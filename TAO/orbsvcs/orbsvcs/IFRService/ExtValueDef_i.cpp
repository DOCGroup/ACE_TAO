/* -*- C++ -*- */
// $Id$

#include "Repository_i.h"
#include "ExtValueDef_i.h"

ACE_RCSID (IFRService, 
           ExtValueDef_i, 
           "$Id$")

TAO_ExtValueDef_i::TAO_ExtValueDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_ValueDef_i (repo)
{
}

TAO_ExtValueDef_i::~TAO_ExtValueDef_i (void)
{
}

CORBA::ExtInitializerSeq *
TAO_ExtValueDef_i::ext_initializers (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->ext_initializers (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExtInitializerSeq *
TAO_ExtValueDef_i::ext_initializers_i (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ExtValueDef_i::ext_initializers (
    const CORBA::ExtInitializerSeq &ext_initializers
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->ext_initializers_i (ext_initializers
                            ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ExtValueDef_i::ext_initializers_i (
    const CORBA::ExtInitializerSeq & /* ext_initializers */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ExtValueDef::ExtFullValueDescription *
TAO_ExtValueDef_i::describe_ext_value (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_ext_value (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExtValueDef::ExtFullValueDescription *
TAO_ExtValueDef_i::describe_ext_value_i (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ExtAttributeDef_ptr 
TAO_ExtValueDef_i::create_ext_attribute (
    const char *id,
    const char *name,
    const char *version,
    CORBA::IDLType_ptr type,
    CORBA::AttributeMode mode,
    const CORBA::ExceptionDefSeq &get_exceptions,
    const CORBA::ExceptionDefSeq &set_exceptions
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD_RETURN (CORBA::ExtAttributeDef::_nil ());

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::ExtAttributeDef::_nil ());

  return 
    this->create_ext_attribute_i (
        id,
        name,
        version,
        type,
        mode,
        get_exceptions,
        set_exceptions
        ACE_ENV_ARG_PARAMETER
      );
}

CORBA::ExtAttributeDef_ptr 
TAO_ExtValueDef_i::create_ext_attribute_i (
    const char * /* id */,
    const char * /* name */,
    const char * /* version */,
    CORBA::IDLType_ptr /* type */,
    CORBA::AttributeMode /* mode */,
    const CORBA::ExceptionDefSeq & /* get_exceptions */,
    const CORBA::ExceptionDefSeq & /* set_exceptions */
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}


