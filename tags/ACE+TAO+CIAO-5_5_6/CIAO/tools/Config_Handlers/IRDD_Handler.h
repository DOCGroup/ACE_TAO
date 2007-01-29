//==============================================================
/**
*  @file  IRDD_Handler.h
*
*  $Id$
*
*  @author Jules White <jules@dre.vanderbilt.edu>
*/
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_IRDD_Handler_H
#define CIAO_CONFIG_HANDLERS_IRDD_Handler_H
#include /**/ "ace/pre.h"

#include "Common.h"
#include "Config_Handlers_Export.h"
#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */



namespace Deployment
{
struct InstanceResourceDeploymentDescription;
}


namespace CIAO
{
namespace Config_Handlers
{

class InstanceResourceDeploymentDescription;


/*
* @class IRDD_Handler
*
* @brief Handler class for <InstanceResourceDeploymentDescription> types.
*
* This class defines handler methods to map values from
* XSC InstanceResourceDeploymentDescription objects, parsed from the descriptor files, to the
* corresponding CORBA IDL Any type.
*
*/
class Config_Handlers_Export IRDD_Handler {
public:
IRDD_Handler (void);
virtual ~IRDD_Handler (void);

static void instance_resource_deployment_descr (
const InstanceResourceDeploymentDescription& desc,
Deployment::InstanceResourceDeploymentDescription& toconfig)
throw (Config_Error);

static InstanceResourceDeploymentDescription
instance_resource_deployment_descr (
const Deployment::InstanceResourceDeploymentDescription& src)
throw (Config_Error);


};
}
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_IRDD_Handler_H */

