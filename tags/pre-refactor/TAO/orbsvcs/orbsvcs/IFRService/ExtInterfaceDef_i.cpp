/* -*- C++ -*- */
// $Id$

#include "ExtInterfaceDef_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           ExtInterfaceDef_i, 
           "$Id$")

TAO_ExtInterfaceDef_i::TAO_ExtInterfaceDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceAttrExtension_i (repo),
    TAO_InterfaceDef_i (repo)
{
}

TAO_ExtInterfaceDef_i::~TAO_ExtInterfaceDef_i (void)
{
}

void
TAO_ExtInterfaceDef_i::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_IFR_WRITE_GUARD;

  this->update_key (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_ExtInterfaceDef_i::destroy_i (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  /// Call destroy() on our base classes.
  this->TAO_InterfaceAttrExtension_i::destroy_i (
      ACE_ENV_SINGLE_ARG_PARAMETER
    );
  ACE_CHECK;

  this->TAO_InterfaceDef_i::destroy_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

