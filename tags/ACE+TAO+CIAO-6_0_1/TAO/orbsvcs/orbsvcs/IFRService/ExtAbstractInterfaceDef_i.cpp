// $Id$

#include "orbsvcs/IFRService/ExtAbstractInterfaceDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ExtAbstractInterfaceDef_i::TAO_ExtAbstractInterfaceDef_i (
    TAO_Repository_i *repo)
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

// Just call the base class version, this is here only to
// disambiguate multiple inheritance.
void
TAO_ExtAbstractInterfaceDef_i::destroy (void)
{
  TAO_IFR_WRITE_GUARD;

  this->update_key ();

  TAO_AbstractInterfaceDef_i::destroy_i ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
