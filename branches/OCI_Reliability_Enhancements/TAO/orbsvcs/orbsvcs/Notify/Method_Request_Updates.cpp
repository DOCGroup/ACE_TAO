// $Id$

#include "orbsvcs/Notify/Method_Request_Updates.h"
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
TAO_Notify_Method_Request_Updates::execute (void)
{
  return this->execute_i ();
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
TAO_Notify_Method_Request_Updates_No_Copy::copy (void)
{
  TAO_Notify_Method_Request_Queueable* request = 0;

  ACE_NEW_THROW_EX (request,
                    TAO_Notify_Method_Request_Updates (this->added_, this->removed_, this->proxy_),
                    CORBA::NO_MEMORY ());

  return request;
}

int
TAO_Notify_Method_Request_Updates_No_Copy::execute (void)
{
  return this->execute_i ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
