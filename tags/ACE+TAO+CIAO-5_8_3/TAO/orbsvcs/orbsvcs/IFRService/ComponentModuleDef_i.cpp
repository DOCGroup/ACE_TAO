// $Id$

#include "orbsvcs/IFRService/ComponentModuleDef_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ComponentModuleDef_i::TAO_ComponentModuleDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_ComponentContainer_i (repo)
{
}

TAO_ComponentModuleDef_i::~TAO_ComponentModuleDef_i (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
