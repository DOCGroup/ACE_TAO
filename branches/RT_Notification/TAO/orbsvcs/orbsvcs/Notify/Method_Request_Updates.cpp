// $Id$

#include "Method_Request_Updates.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Updates.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Notify, TAO_NS_Method_Request_Updates, "$Id$")

#include "tao/debug.h"
#include "Proxy.h"
#include "Peer.h"

TAO_NS_Method_Request_Updates::TAO_NS_Method_Request_Updates (const TAO_NS_EventTypeSeq& added, const TAO_NS_EventTypeSeq& removed, TAO_NS_Proxy* proxy)
  : TAO_NS_Method_Request_Updates_Base (added, removed, proxy)
{
}

TAO_NS_Method_Request_Updates::~TAO_NS_Method_Request_Updates ()
{
}

int
TAO_NS_Method_Request_Updates::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}

/********************************************************************************************************/

TAO_NS_Method_Request_Updates_No_Copy::TAO_NS_Method_Request_Updates_No_Copy (const TAO_NS_EventTypeSeq& added
                                                                              , const TAO_NS_EventTypeSeq& removed, TAO_NS_Proxy* proxy)
  : TAO_NS_Method_Request_Updates_No_Copy_Base (added, removed, proxy)
{
}

TAO_NS_Method_Request_Updates_No_Copy::~TAO_NS_Method_Request_Updates_No_Copy ()
{
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Updates_No_Copy::copy (void)
{
  TAO_NS_Method_Request* request;

  ACE_NEW_THROW_EX (request,
                    TAO_NS_Method_Request_Updates (this->added_, this->removed_, this->proxy_),
                    CORBA::INTERNAL ());

  return request;
}

int
TAO_NS_Method_Request_Updates_No_Copy::execute (ACE_ENV_SINGLE_ARG_DECL)
{
  return this->execute_i (ACE_ENV_SINGLE_ARG_PARAMETER);
}
