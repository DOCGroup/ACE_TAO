/* -*- C++ -*- */
// ============================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Trader.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
// 
// ============================================================================

#if !defined (TAO_TRADER_C)
#define TAO_TRADER_C

#include "Trader.h"
#include "Lookup.h"
#include "Register.h"
#include "Link.h"
#include "Proxy.h"
#include "Admin.h"

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> 
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::TAO_Trader (Trader_Components components)
{
  typedef TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> TRADER_SELF;
  
  if (ACE_BIT_ENABLED (components, ADMIN))
    this->trading_components ().admin_if (new TAO_Admin<TRADER_SELF> (*this));   
  if (ACE_BIT_ENABLED (components, LOOKUP))
    this->trading_components ().lookup_if (new TAO_Lookup<TRADER_SELF> (*this));
  if (ACE_BIT_ENABLED (components, REGISTER))
    this->trading_components ().register_if (new TAO_Register<TRADER_SELF> (*this));
  if (ACE_BIT_ENABLED (components, LINK))
    this->trading_components ().link_if (new TAO_Link<TRADER_SELF> (*this));
  if (ACE_BIT_ENABLED (components, PROXY))
    this->trading_components ().proxy_if (new TAO_Proxy<TRADER_SELF> (*this));
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> 
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::~TAO_Trader ()
{
  CORBA::release (this->trading_components ().lookup_if ());
  CORBA::release (this->trading_components ().register_if ());
  CORBA::release (this->trading_components ().link_if ());
  CORBA::release (this->trading_components ().proxy_if ());
  CORBA::release (this->trading_components ().admin_if ());
  CORBA::release (this->support_attributes ().type_repos ());
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::SERVICE_TYPE_MAP&
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::service_type_map (void) 
{
  return this->service_type_map_;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> TAO_Lock &
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::lock (void)
{
  return this->lock_;
}
  
#endif /* TAO_TRADER_C */
