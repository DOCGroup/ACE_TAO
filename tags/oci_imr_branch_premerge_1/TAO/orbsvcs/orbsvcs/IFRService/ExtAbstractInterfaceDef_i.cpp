/* -*- C++ -*- */
// $Id$

#include "ExtAbstractInterfaceDef_i.h"
#include "Repository_i.h"

ACE_RCSID (IFRService, 
           ExtAbstractInterfaceDef_i, 
           "$Id$")

TAO_ExtAbstractInterfaceDef_i::TAO_ExtAbstractInterfaceDef_i (
    TAO_Repository_i *repo
  )
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceDef_i (repo),
    TAO_AbstractInterfaceDef_i (repo),
    TAO_InterfaceAttrExtension_i (repo)
{
}

TAO_ExtAbstractInterfaceDef_i::~TAO_ExtAbstractInterfaceDef_i (void)
{
}

