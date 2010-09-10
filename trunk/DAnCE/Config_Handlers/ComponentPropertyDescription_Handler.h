//==============================================================
/**
*  @file  ComponentPropertyDescription_Handler.h
*
*  $Id$
*
*  @author Jules White <jules@dre.vanderbilt.edu>
*/
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_ComponentPropertyDescription_Handler_H
#define CIAO_CONFIG_HANDLERS_ComponentPropertyDescription_Handler_H
#include /**/ "ace/pre.h"


#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
struct ComponentPropertyDescription;
}

namespace DAnCE
{
namespace Config_Handlers
{
class ComponentPropertyDescription;

/*
* @class ComponentPropertyDescription_Handler
*
* @brief Handler class for <ComponentPortDescription> types.
*
* This class defines handler methods to map values from
* XSC objects, parsed from the descriptor files, to the
* corresponding CORBA IDL type for the schema element.
*
*/
class Config_Handlers_Export ComponentPropertyDescription_Handler
{
public:

/// This method maps the values from the XSC object
/// <ComponentPropertyDescription> to the CORBA IDL type
/// <Deployment::ComponentPropertyDescription>.
static void   component_property_description (
const ComponentPropertyDescription& desc,
::Deployment::ComponentPropertyDescription& toconfig);
static ComponentPropertyDescription
component_property_description (
const ::Deployment::ComponentPropertyDescription &src);
};
}
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_ComponentPropertyDescription_Handler_H */
