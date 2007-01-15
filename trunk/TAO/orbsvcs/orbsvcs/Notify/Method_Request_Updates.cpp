// $Id$

#include "orbsvcs/Notify/Method_Request_Updates.h"

ACE_RCSID(Notify, TAO_Notify_Method_Request_Updates, "$Id$")

#include "tao/debug.h"
#include "orbsvcs/Notify/Proxy.h"
#include "orbsvcs/Notify/Peer.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Notify_Method_Request_Updates::TAO_Notify_Method_Request_Updates (const TAO_Notify_EventTypeSeq& added, const TAO_Notify_EventTypeSeq& removed, TAO_Notify_Proxy* proxy)
  : TAO_Notify_Method_Request_Updates_Base (added, removed, proxy)
{
}

TAO_Notify_Method_Request_Updates::~TAO_Notify_Method_Request_Updates ()
{
}

int
TAO_Notify_Method_Request_Updates::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/*****************************************************************************/

TAO_Notify_Method_Request_Updates_No_Copy::TAO_Notify_Method_Request_Updates_No_Copy (const TAO_Notify_EventTypeSeq& added
                                                                              , const TAO_Notify_EventTypeSeq& removed, TAO_Notify_Proxy* proxy)
  : TAO_Notify_Method_Request_Updates_No_Copy_Base (added, removed, proxy)
{
}

TAO_Notify_Method_Request_Updates_No_Copy::~TAO_Notify_Method_Request_Updates_No_Copy ()
{
}

TAO_Notify_Method_Request_Queueable*
TAO_Notify_Method_Request_Updates_No_Copy::copy (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Method_Request_Queueable* request = 0;

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Updates (this->added_, this->removed_, this->proxy_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (request);

  return request;
}

int
TAO_Notify_Method_Request_Updates_No_Copy::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

TAO_END_VERSIONED_NAMESPACE_DECL
