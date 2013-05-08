// -*- C++ -*-
//
// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

extern unsigned long TAO_EC_TPC_debug_level;

ACE_INLINE
TAO_EC_TPC_ProxyPushSupplier::TAO_EC_TPC_ProxyPushSupplier (TAO_EC_Event_Channel_Base* ec, int validate_connection)
: TAO_EC_Default_ProxyPushSupplier (ec, validate_connection)
{
  if (TAO_EC_TPC_debug_level > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "(%P|%t) EC_TPC_ProxyPushSupplier::CTOR (%@)\n", this));
}

TAO_END_VERSIONED_NAMESPACE_DECL
