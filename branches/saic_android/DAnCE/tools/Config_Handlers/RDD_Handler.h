
//==============================================================
/**
*  @file  RDD_Handler.h
*
*  $Id$
*
*  @author Jules White <jules@dre.vanderbilt.edu>
*/
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_RDD_Handler_H
#define CIAO_CONFIG_HANDLERS_RDD_Handler_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
struct ResourceDeploymentDescription;
}


namespace DAnCE
{

namespace Config_Handlers
{
class ResourceDeploymentDescription;

/*
* @class RDD_Handler
*
* @brief Handler class for <ResourceDeploymentDescription> types.
*
* This class defines handler methods to map values from XSC
* ResourceDeploymentDescription objects, parsed from the
* descriptor files, to the  corresponding CORBA IDL Any type.
*
*/

class Config_Handlers_Export RDD_Handler
{
public:
static void resource_deployment_descr (
const ResourceDeploymentDescription& desc,
::Deployment::ResourceDeploymentDescription& toconfig);
static ResourceDeploymentDescription
resource_deployment_descr (
const ::Deployment::ResourceDeploymentDescription &src);

};
}
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_RDD_Handler_H */
