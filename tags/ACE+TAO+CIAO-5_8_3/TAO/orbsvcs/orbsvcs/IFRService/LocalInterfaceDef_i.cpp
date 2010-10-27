// $Id$

#include "orbsvcs/IFRService/LocalInterfaceDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LocalInterfaceDef_i::TAO_LocalInterfaceDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceDef_i (repo)
{
}

TAO_LocalInterfaceDef_i::~TAO_LocalInterfaceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_LocalInterfaceDef_i::def_kind (void)
{
  return CORBA::dk_LocalInterface;
}

CORBA::Boolean
TAO_LocalInterfaceDef_i::is_a (const char *interface_id)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->is_a_i (interface_id);
}

CORBA::Boolean
TAO_LocalInterfaceDef_i::is_a_i (const char *interface_id)
{
  if (ACE_OS::strcmp (interface_id, "IDL:omg.org/CORBA/LocalBase:1.0") == 0)
    {
      return 1;
    }

  return this->TAO_InterfaceDef_i::is_a_i (interface_id);
}

TAO_END_VERSIONED_NAMESPACE_DECL
