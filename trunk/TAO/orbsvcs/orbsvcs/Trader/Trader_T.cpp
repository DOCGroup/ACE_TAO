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

#include "Trader_T.h"

#if defined(_MSC_VER)
#pragma warning (disable:4250)
#endif /* _MSC_VER */

#include "Lookup.h"
#include "Register.h"
#include "Link.h"
#include "Proxy.h"
#include "Admin.h"

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> 
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::
TAO_Trader (Trader_Components components)
{
  CORBA::Environment env;
  for (int i = LOOKUP_IF; i <= LINK_IF; i++)
    this->ifs_[i] = 0;
  
  if (ACE_BIT_ENABLED (components, LOOKUP))
    {
      TAO_Lookup<TRADER_SELF>* lookup = new TAO_Lookup<TRADER_SELF> (*this);
      this->trading_components ().lookup_if (lookup->_this (env));
      this->ifs_[LOOKUP_IF] = lookup;
    }
  if (ACE_BIT_ENABLED (components, REGISTER))
    {
      TAO_Register<TRADER_SELF>* reg = new TAO_Register<TRADER_SELF> (*this);
      this->trading_components ().register_if (reg->_this (env));
      this->ifs_[REGISTER_IF] = reg;
    }
  if (ACE_BIT_ENABLED (components, ADMIN))
    {
      TAO_Admin<TRADER_SELF>* admin = new TAO_Admin<TRADER_SELF> (*this);
      this->trading_components ().admin_if (admin->_this (env));
      this->ifs_[ADMIN_IF] = admin;
    }
  if (ACE_BIT_ENABLED (components, PROXY))
    {
      TAO_Proxy<TRADER_SELF>* proxy = new TAO_Proxy<TRADER_SELF> (*this);
      this->trading_components ().proxy_if (proxy->_this (env));
      this->ifs_[PROXY_IF] = proxy;
    }
  if (ACE_BIT_ENABLED (components, LINK))
    {
      TAO_Link<TRADER_SELF, MAP_LOCK_TYPE>* link =
	new TAO_Link<TRADER_SELF, MAP_LOCK_TYPE> (*this);
      this->trading_components ().link_if (link->_this (env));
      this->ifs_[LINK_IF] = link;
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> 
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::~TAO_Trader (void)
{
  // Remove Trading Components from POA
  //
  // Note that there is no real error checking here as we can't do
  // much about errors here anyway
  //

  for (int i = LOOKUP_IF; i <= LINK_IF; i++)
    {
      if (this->ifs_[i] != 0)
	{
	  TAO_TRY
	    {
	      PortableServer::POA_var poa =
		this->ifs_[i]->_default_POA (TAO_TRY_ENV);
	      TAO_CHECK_ENV;
	      PortableServer::ObjectId_var id =
		poa->servant_to_id (this->ifs_[i], TAO_TRY_ENV);
	      TAO_CHECK_ENV;
	      poa->deactivate_object (id.in (), TAO_TRY_ENV);
	    }
	  TAO_CATCHANY
	    {      
	    }
	  TAO_ENDTRY;
	  
	  delete this->ifs_[i];	  
	}
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::Service_Type_Map&
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::service_type_map (void) 
{
  return this->service_type_map_;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> ACE_Lock &
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::lock (void)
{
  return this->lock_;
}

template <class SEQ, class OPERAND_TYPE> CORBA::Boolean
TAO_find (SEQ& sequence, const OPERAND_TYPE element)
{
  int length = sequence.length(),
    return_value = 0;

  for (int i = 0; i < length; i++)
    {
      if (sequence[i] == element)
	{	  
	  return_value = 1;
	  break;
	}
    }

  return (CORBA::Boolean) return_value;
}

#endif /* TAO_TRADER_C */
