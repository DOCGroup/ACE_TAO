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

