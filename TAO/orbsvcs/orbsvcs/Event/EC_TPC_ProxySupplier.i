// $Id$

extern unsigned long EC_TPC_debug_level;

ACE_INLINE
TAO_EC_TPC_ProxyPushSupplier::TAO_EC_TPC_ProxyPushSupplier (TAO_EC_Event_Channel_Base* ec, int validate_connection)
: TAO_EC_Default_ProxyPushSupplier (ec, validate_connection)
{
  if (EC_TPC_debug_level > 0)        
    ACE_DEBUG ((LM_DEBUG, "(%P|%t) EC_TPC_ProxyPushSupplier::CTOR (%@)\n", this));
}

