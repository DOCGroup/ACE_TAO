/* -*- C++ -*- */

// ============================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Trader_Base.cpp
//
// = AUTHOR
//    Irfan Pyarali <irfan@cs.wustl.edu>
// 
// ============================================================================

#define ACE_BUILD_DLL

#include "Trader.h"
#include "ace/OS.h"
#include <iostream.h>

TAO_Trader_Base::TAO_Trader_Base (void)
  : trading_components_ (*this),
    import_attributes_ (*this),
    support_attributes_ (*this),
    link_attributes_ (*this)
{
}

TAO_Import_Attributes_Impl &
TAO_Trader_Base::import_attributes (void)
{
  return this->import_attributes_;
}

const TAO_Import_Attributes_Impl &
TAO_Trader_Base::import_attributes (void) const
{
  return this->import_attributes_;
}

TAO_Support_Attributes_Impl &
TAO_Trader_Base::support_attributes (void) 
{
  return this->support_attributes_;
}

const TAO_Support_Attributes_Impl &
TAO_Trader_Base::support_attributes (void) const
{
  return this->support_attributes_;
}

TAO_Link_Attributes_Impl &
TAO_Trader_Base::link_attributes (void) 
{
  return this->link_attributes_;
}

const TAO_Link_Attributes_Impl &
TAO_Trader_Base::link_attributes (void) const
{
  return this->link_attributes_;
}

TAO_Trading_Components_Impl &
TAO_Trader_Base::trading_components (void)
{
  return this->trading_components_;
}

const TAO_Trading_Components_Impl &
TAO_Trader_Base::trading_components (void) const
{
  return this->trading_components_;
}


CORBA::Boolean
TAO_Trader_Base::is_valid_identifier_name (const char* ident)
{
  int return_value = CORBA::B_FALSE;

  if (ident == 0)
    return return_value;
  
  int length = ACE_OS::strlen (ident);
  if (length >= 1 && isalpha (ident[0]))
    {
      return_value = 1;
      for (int i = 0; i < length; i++)
	{
	  if (! (isalnum (ident[i]) || ident[i] == '_'))
	    {
	      return_value = 0;
	      break;
	    }
	}     
    }

  return return_value;
}

#include "Trader_T.h"

TAO_Trader_Factory::TAO_TRADER*
TAO_Trader_Factory::TAO_create_linked_trader (void)
{
  TAO_TRADER::Trader_Components linked_trader =
    (TAO_TRADER::Trader_Components)
    (TAO_TRADER::LOOKUP |
     TAO_TRADER::REGISTER |
     TAO_TRADER::ADMIN |
     TAO_TRADER::LINK);
  return new TAO_TRADER (linked_trader);
}

#ifdef ACE_HAS_THREADS

TAO_Trader_Factory::TAO_MT_TRADER*
TAO_Trader_Factory::TAO_MT_create_linked_trader (void)
{
  TAO_MT_TRADER::Trader_Components linked_trader =
    (TAO_MT_TRADER::Trader_Components)
    (TAO_MT_TRADER::LOOKUP |
     TAO_MT_TRADER::REGISTER |
     TAO_MT_TRADER::ADMIN |
     TAO_MT_TRADER::LINK);
  return new TAO_MT_TRADER (linked_trader);
}

#endif /* ACE_HAS_THREADS */

