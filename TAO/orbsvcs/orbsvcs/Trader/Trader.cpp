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

#include "Trader.h"

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

// = Methods to deal with ACE_Hash_Map_Manager.

TAO_String_Hash_Key::TAO_String_Hash_Key (void)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (char * name)
  : CORBA::String_var (name)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (const char * name)
  : CORBA::String_var (name)
{
}

TAO_String_Hash_Key::TAO_String_Hash_Key (const CORBA::String_var &src)
  : CORBA::String_var (src)
{
}

int
TAO_String_Hash_Key::operator == (const TAO_String_Hash_Key &hash_key) const
{
  return ACE_OS::strcmp (this->in (), hash_key.in ()) == 0;
}

int
operator < (const TAO_String_Hash_Key &left,
	    const TAO_String_Hash_Key &right)
{
  return ACE_OS::strcmp (left.in (), right.in ()) < 0;
}


u_long
TAO_String_Hash_Key::hash (void) const
{
  u_long ret = ACE::hash_pjw (this->in ());
  return ret; 
}

TAO_String_Hash_Key::~TAO_String_Hash_Key (void)
{
}

int
operator< (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
	   const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r)
{
  if (l.high < r.high)
    return 1;
  else if (l.high == r.high) 
    return (l.low < r.low);
  else
    return 0;
}

int
operator> (const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &l,
	   const CosTradingRepos::ServiceTypeRepository::IncarnationNumber &r)
{
  return (r < l);
}

#include "Trader_T.h"

TAO_Trader_Factory::TAO_TRADER*
TAO_Trader_Factory::create_linked_trader (void)
{
  typedef TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex>  NULL_TRADER;
  
  NULL_TRADER::Trader_Components linked_trader =
    (NULL_TRADER::Trader_Components)
    (NULL_TRADER::LOOKUP |
     NULL_TRADER::REGISTER |
     NULL_TRADER::ADMIN |
     NULL_TRADER::LINK);
  return new NULL_TRADER (linked_trader);
}

#ifdef ACE_HAS_THREADS
/*
TAO_Trader_Factory::TAO_TRADER*
TAO_Trader_Factory::create_MT_linked_trader (void)
{
  typedef TAO_Trader<ACE_Thread_Mutex, ACE_RW_Mutex>  MT_TRADER;
  MT_TRADER::Trader_Components linked_trader =
    (MT_TRADER::Trader_Components)
    (MT_TRADER::LOOKUP |
     MT_TRADER::REGISTER |
     MT_TRADER::ADMIN |
     MT_TRADER::LINK);
  return new MT_TRADER (linked_trader);
}
*/
#endif /* ACE_HAS_THREADS */

