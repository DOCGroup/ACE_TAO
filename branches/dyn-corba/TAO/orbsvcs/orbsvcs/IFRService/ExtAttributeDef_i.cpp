/* -*- C++ -*- */
// $Id$

#include "ExtAttributeDef_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           ExtAttributeDef_i, 
           "$Id$")

TAO_ExtAttributeDef_i::TAO_ExtAttributeDef_i (TAO_Repository_i *repo)
  : TAO_AttributeDef_i (repo),
    TAO_Contained_i (repo),
    TAO_IRObject_i (repo)
{
}

TAO_ExtAttributeDef_i::~TAO_ExtAttributeDef_i (void)
{
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::get_exceptions (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->get_exceptions_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::get_exceptions_i (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ExtAttributeDef_i::get_exceptions (
    const CORBA::ExcDescriptionSeq &get_exceptions
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->get_exceptions_i (get_exceptions
                          ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ExtAttributeDef_i::get_exceptions_i (
    const CORBA::ExcDescriptionSeq &get_exceptions
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::set_exceptions (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->set_exceptions_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExcDescriptionSeq *
TAO_ExtAttributeDef_i::set_exceptions_i (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}

void 
TAO_ExtAttributeDef_i::set_exceptions (
    const CORBA::ExcDescriptionSeq &set_exceptions
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->set_exceptions_i (set_exceptions
                          ACE_ENV_ARG_PARAMETER);
}

void 
TAO_ExtAttributeDef_i::set_exceptions_i (
    const CORBA::ExcDescriptionSeq &set_exceptions
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
}

CORBA::ExtAttributeDescription *
TAO_ExtAttributeDef_i::describe_attribute (
    ACE_ENV_SINGLE_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return this->describe_attribute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

CORBA::ExtAttributeDescription *
TAO_ExtAttributeDef_i::describe_attribute_i (
    ACE_ENV_SINGLE_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // TODO
  return 0;
}


