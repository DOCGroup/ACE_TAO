/* -*- C++ -*- */
// $Id$

#include "InterfaceAttrExtension_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           InterfaceAttrExtension_i, 
           "$Id$")

// =====================================================================

TAO_InterfaceAttrExtension_i::TAO_InterfaceAttrExtension_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo)
{
}

TAO_InterfaceAttrExtension_i::~TAO_InterfaceAttrExtension_i (void)
{
}

void
TAO_InterfaceAttrExtension_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_InterfaceAttrExtension_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  /// TODO.
}

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
TAO_InterfaceAttrExtension_i::describe_ext_interface (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_ext_interface_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::InterfaceAttrExtension::ExtFullInterfaceDescription *
TAO_InterfaceAttrExtension_i::describe_ext_interface_i (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

CORBA::ExtAttributeDef_ptr 
TAO_InterfaceAttrExtension_i::create_ext_attribute (
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

  return this->create_ext_attribute_i (id,
                                       name,
                                       version,
                                       type,
                                       mode,
                                       get_exceptions,
                                       set_exceptions
                                       ACE_ENV_ARG_PARAMETER);
}
  
CORBA::ExtAttributeDef_ptr 
TAO_InterfaceAttrExtension_i::create_ext_attribute_i (
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

