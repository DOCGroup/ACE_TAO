//==============================================================
/**
*  @file  PSPE_Handler.h
*
*  $Id$
*
*  @author Jules White <jules@dre.vanderbilt.edu>
*/
//================================================================

#ifndef CIAO_CONFIG_HANDLERS_PSPE_HANDLER_H
#define CIAO_CONFIG_HANDLERS_PSPE_HANDLER_H
#include /**/ "ace/pre.h"

#include "Config_Handlers_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

namespace Deployment
{
class PlanSubcomponentPortEndpoints;
struct PlanSubcomponentPortEndpoint;
}

namespace DAnCE
{

namespace Config_Handlers
{
class PlanConnectionDescription;
class PlanSubcomponentPortEndpoint;

/*
* @class PSPE_Handler
*
* @brief Handler class for <PlanSubcomponentPortEndpoint> types.
*
* This class defines handler methods to map values from
* XSC PlanSubcomponentPortEndpoint objects, parsed from
* the descriptor files, to the corresponding CORBA IDL type.
*
*/
class Config_Handlers_Export PSPE_Handler
{
public:
static void sub_component_port_endpoints (
const PlanConnectionDescription &src,
::Deployment::PlanSubcomponentPortEndpoints &dest);

static PlanSubcomponentPortEndpoint
sub_component_port_endpoint (
const Deployment::PlanSubcomponentPortEndpoint &src);

private:
static void sub_component_port_endpoint (
const PlanSubcomponentPortEndpoint &src,
::Deployment::PlanSubcomponentPortEndpoint &dest);
};
}
}

#include /**/ "ace/post.h"
#endif /* CIAO_CONFIG_HANDLERS_PSPE_HANDLER_H*/
