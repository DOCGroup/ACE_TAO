/* -*- C++ -*- */
// $Id$

#include "ComponentModuleDef_i.h"

ACE_RCSID (IFRService, 
           ComponentModuleDef_i, 
           "$Id$")

TAO_ComponentModuleDef_i::TAO_ComponentModuleDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_ComponentContainer_i (repo)
{
}

TAO_ComponentModuleDef_i::~TAO_ComponentModuleDef_i (void)
{
}
