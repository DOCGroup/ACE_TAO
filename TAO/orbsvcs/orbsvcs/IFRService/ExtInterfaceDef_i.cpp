#include "ExtInterfaceDef_i.h"
#include "Repository_i.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_ExtInterfaceDef_i::TAO_ExtInterfaceDef_i (TAO_Repository_i *repo)
  : TAO_IRObject_i (repo),
    TAO_Container_i (repo),
    TAO_Contained_i (repo),
    TAO_IDLType_i (repo),
    TAO_InterfaceDef_i (repo),
    TAO_InterfaceAttrExtension_i (repo)
{
}

TAO_ExtInterfaceDef_i::~TAO_ExtInterfaceDef_i ()
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
