// $Id$

#include "tao/PortableServer/Network_Priority_Hook.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/POA_Cached_Policies.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Network_Priority_Hook::~TAO_Network_Priority_Hook(void)
{
}

void
TAO_Network_Priority_Hook::update_network_priority (
  TAO_Root_POA &poa, TAO_POA_Policy_Set &)
{
  poa.cached_policies ().network_priority_model
    (TAO::Portable_Server::Cached_Policies::NO_NETWORK_PRIORITY);
  poa.cached_policies ().reply_diffserv_codepoint (0);
  poa.cached_policies ().request_diffserv_codepoint (0);
}

void
TAO_Network_Priority_Hook::set_dscp_codepoint (
  TAO_ServerRequest &, TAO_Root_POA &)
{
}

int
TAO_Network_Priority_Hook::initialize (void)
{
  return ACE_Service_Config::process_directive
    (ace_svc_desc_TAO_Network_Priority_Hook);
}


ACE_FACTORY_DEFINE (TAO_PortableServer, TAO_Network_Priority_Hook)
ACE_STATIC_SVC_DEFINE (TAO_Network_Priority_Hook,
                       ACE_TEXT ("TAO_Network_Priority_Hook"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Network_Priority_Hook),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)
TAO_END_VERSIONED_NAMESPACE_DECL
