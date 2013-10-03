// $Id$

#include "orbsvcs/IFRService/AbstractInterfaceDef_i.h"
#include "orbsvcs/IFRService/Repository_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_AbstractInterfaceDef_i::TAO_AbstractInterfaceDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceDef_i (repo)
{
}

TAO_AbstractInterfaceDef_i::~TAO_AbstractInterfaceDef_i (void)
{
}

CORBA::DefinitionKind
TAO_AbstractInterfaceDef_i::def_kind (void)
{
  return CORBA::dk_AbstractInterface;
}

CORBA::Boolean
TAO_AbstractInterfaceDef_i::is_a (const char *interface_id)
{
  TAO_IFR_READ_GUARD_RETURN (0);

  this->update_key ();

  return this->is_a_i (interface_id);
}

CORBA::Boolean
TAO_AbstractInterfaceDef_i::is_a_i (const char *interface_id)
{
  if (ACE_OS::strcmp (interface_id, "IDL:omg.org/CORBA/AbstractBase:1.0") == 0)
    {
      return true;
    }

  return this->TAO_InterfaceDef_i::is_a_i (interface_id);
}

TAO_END_VERSIONED_NAMESPACE_DECL
