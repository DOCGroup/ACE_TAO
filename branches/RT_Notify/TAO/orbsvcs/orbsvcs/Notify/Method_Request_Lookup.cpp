// $Id$

#include "Method_Request_Lookup.h"
#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Event_Map_T.h"
#include "ProxySupplier.h"
#include "ProxyConsumer.h"

#if ! defined (__ACE_INLINE__)
#include "Method_Request_Lookup.inl"
#endif /* __ACE_INLINE__ */

ACE_RCSID(RT_Notify, TAO_NS_Method_Request_Lookup, "$Id$")

TAO_NS_Method_Request_Lookup::TAO_NS_Method_Request_Lookup (TAO_NS_Event_var& event, TAO_NS_ProxyConsumer* proxy_consumer, TAO_NS_Consumer_Map* map)
  : TAO_NS_Method_Request (event), proxy_consumer_ (proxy_consumer), map_ (map)
{
}

TAO_NS_Method_Request_Lookup::~TAO_NS_Method_Request_Lookup ()
{
}

TAO_NS_Method_Request*
TAO_NS_Method_Request_Lookup::copy (void)
{
  /// @@use factory
  return new TAO_NS_Method_Request_Lookup (this->event_, this->proxy_consumer_, this->map_);
}

int
TAO_NS_Method_Request_Lookup::call (void)
{
  ACE_DECLARE_NEW_CORBA_ENV; 
  
  TAO_NS_Consumer_Collection* consumers = map_->find (this->event_->type () ACE_ENV_ARG_PARAMETER);
      
  consumers->for_each (this ACE_ENV_ARG_PARAMETER);

  return 0;
}

void
TAO_NS_Method_Request_Lookup::work (TAO_NS_ProxySupplier* proxy_supplier ACE_ENV_ARG_DECL)
{
  proxy_supplier->push (this->event_ ACE_ENV_ARG_PARAMETER);
}
