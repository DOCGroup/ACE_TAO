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
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ============================================================================

#include "Trader.h"

  // *************************************************************
  // TAO_Trader_Base
  // *************************************************************

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

  // *************************************************************
  // Attribute Classes
  // *************************************************************

TAO_Support_Attributes_Impl::
TAO_Support_Attributes_Impl (TAO_Lockable &locker)
  : locker_ (locker),
    supports_modifiable_properties_ (CORBA::B_TRUE),
    supports_dynamic_properties_ (CORBA::B_TRUE),
    supports_proxy_offers_ (CORBA::B_FALSE),
    type_repos_ (CosTrading::TypeRepository::_nil ())
{
}

TAO_Support_Attributes_Impl::~TAO_Support_Attributes_Impl (void)
{
}
  
CORBA::Boolean
TAO_Support_Attributes_Impl::supports_modifiable_properties (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CORBA::B_FALSE);
  return this->supports_modifiable_properties_;
}

void
TAO_Support_Attributes_Impl::
supports_modifiable_properties (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->supports_modifiable_properties_ = new_value;
}

CORBA::Boolean
TAO_Support_Attributes_Impl::supports_dynamic_properties (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CORBA::B_FALSE);

  return this->supports_dynamic_properties_;
}
    
void
TAO_Support_Attributes_Impl::
supports_dynamic_properties (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->supports_dynamic_properties_ = new_value;
}
    
CORBA::Boolean
TAO_Support_Attributes_Impl::supports_proxy_offers (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CORBA::B_FALSE);
    
  return this->supports_proxy_offers_;
}

void
TAO_Support_Attributes_Impl::
supports_proxy_offers (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->supports_proxy_offers_ = new_value;
}

CosTrading::TypeRepository_ptr
TAO_Support_Attributes_Impl::type_repos (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CosTrading::TypeRepository::_nil ());
  return this->type_repos_;
}
  
void
TAO_Support_Attributes_Impl::
type_repos (CosTrading::TypeRepository_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->type_repos_ = new_value;
}
  

TAO_Import_Attributes_Impl::
TAO_Import_Attributes_Impl (TAO_Lockable &locker)
  : locker_ (locker),
    def_search_card_ (10),
    max_search_card_ (100),
    def_match_card_ (10),
    max_match_card_ (100), 
    def_return_card_ (10),
    max_return_card_ (100), 
    max_list_ (0),
    def_hop_count_ (5),
    max_hop_count_ (5),
    def_follow_policy_ (CosTrading::if_no_local),
    max_follow_policy_ (CosTrading::always)
{
}
 

TAO_Import_Attributes_Impl::~TAO_Import_Attributes_Impl (void)
{
}
  
CORBA::ULong
TAO_Import_Attributes_Impl::def_search_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->def_search_card_;
}

void
TAO_Import_Attributes_Impl::def_search_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_search_card_)
    this->def_search_card_ = this->max_search_card_;
  else
    this->def_search_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_search_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->max_search_card_;
}

void
TAO_Import_Attributes_Impl::max_search_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  this->max_search_card_ = new_value;

  if (this->def_search_card_ > this->max_search_card_)
    this->def_search_card_ = this->max_search_card_;
}

CORBA::ULong
TAO_Import_Attributes_Impl::def_match_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->def_match_card_;
}

void
TAO_Import_Attributes_Impl::def_match_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_match_card_)
    this->def_match_card_ = this->max_match_card_;
  else
    this->def_match_card_ = new_value;  
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_match_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->max_match_card_;
}

void
TAO_Import_Attributes_Impl::max_match_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->max_match_card_ = new_value;

  if (this->def_match_card_ > this->max_match_card_)
    this->def_match_card_ = this->max_match_card_;
}

CORBA::ULong
TAO_Import_Attributes_Impl::def_return_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10);
  return this->def_return_card_;
}

void
TAO_Import_Attributes_Impl::def_return_card (CORBA::ULong new_value)
{  
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_return_card_)
    this->def_return_card_ = this->max_return_card_;
  else
    this->def_return_card_ = new_value;  
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_return_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 10); 
  return this->max_return_card_;
}

void
TAO_Import_Attributes_Impl::max_return_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->max_return_card_ = new_value;

  if (this->def_return_card_ > this->max_return_card_)
    this->def_return_card_ = this->max_return_card_;
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_list (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 0); 
  return this->max_list_;
}

void
TAO_Import_Attributes_Impl::max_list (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->max_list_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::def_hop_count (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 0);  
  return this->def_hop_count_;
}

void
TAO_Import_Attributes_Impl::def_hop_count (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_hop_count_)
    this->def_hop_count_ = this->max_hop_count_;
  else
    this->def_hop_count_ = new_value;  
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_hop_count (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), 0);  
  return this->max_hop_count_;
}

void
TAO_Import_Attributes_Impl::max_hop_count (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->max_hop_count_ = new_value;

  if (this->def_hop_count_ > this->max_hop_count_)
    this->def_hop_count_ = this->max_hop_count_;
}

CosTrading::FollowOption
TAO_Import_Attributes_Impl::def_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), CosTrading::local_only); 
  return this->def_follow_policy_;
}

void
TAO_Import_Attributes_Impl::def_follow_policy (CosTrading::FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());

  if (new_value > this->max_follow_policy_)
    this->def_follow_policy_ = this->max_follow_policy_;
  else
    this->def_follow_policy_ = new_value;  
}

CosTrading::FollowOption
TAO_Import_Attributes_Impl::max_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (), CosTrading::local_only); 
  return this->max_follow_policy_;
}

void
TAO_Import_Attributes_Impl::max_follow_policy (CosTrading::FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->max_follow_policy_ = new_value;

  if (this->def_follow_policy_ > this->max_follow_policy_)
    this->def_follow_policy_ = this->max_follow_policy_;
}


TAO_Trading_Components_Impl::TAO_Trading_Components_Impl (TAO_Lockable &locker)
  : locker_ (locker),
    lookup_ (CosTrading::Lookup::_nil ()),
    register_ (CosTrading::Register::_nil ()),
    link_ (CosTrading::Link::_nil ()),
    proxy_ (CosTrading::Proxy::_nil ()),
    admin_ (CosTrading::Admin::_nil ())  
{
}

TAO_Trading_Components_Impl::~TAO_Trading_Components_Impl (void)
{
}

CosTrading::Lookup_ptr
TAO_Trading_Components_Impl::lookup_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CosTrading::Lookup::_nil ());  
  return this->lookup_;
}

void
TAO_Trading_Components_Impl::lookup_if (CosTrading::Lookup_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->lookup_ = new_value;
}

CosTrading::Register_ptr
TAO_Trading_Components_Impl::register_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CosTrading::Register::_nil ());  
  return this->register_;
}

void
TAO_Trading_Components_Impl::register_if (CosTrading::Register_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->register_ = new_value;
}

CosTrading::Link_ptr
TAO_Trading_Components_Impl::link_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CosTrading::Link::_nil ());  
  return this->link_;
}

void
TAO_Trading_Components_Impl::link_if (CosTrading::Link_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->link_ = new_value;
}

CosTrading::Proxy_ptr
TAO_Trading_Components_Impl::proxy_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CosTrading::Proxy::_nil ());  
  return this->proxy_;
}
  
void
TAO_Trading_Components_Impl::proxy_if (CosTrading::Proxy_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->proxy_ = new_value;
}
  
CosTrading::Admin_ptr
TAO_Trading_Components_Impl::admin_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CosTrading::Admin::_nil ());  
  return this->admin_;
}

void
TAO_Trading_Components_Impl::admin_if (CosTrading::Admin_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->admin_ = new_value;
}

  
TAO_Link_Attributes_Impl::TAO_Link_Attributes_Impl (TAO_Lockable &locker)
  :locker_ (locker),
   max_link_follow_policy_ (CosTrading::local_only)
{
}
 
TAO_Link_Attributes_Impl::~TAO_Link_Attributes_Impl (void)
{
}

CosTrading::FollowOption 
TAO_Link_Attributes_Impl::max_link_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->locker_.lock (),
			 CosTrading::local_only);  
  return this->max_link_follow_policy_;
}
  
void  
TAO_Link_Attributes_Impl::max_link_follow_policy (CosTrading::FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->max_link_follow_policy_ = new_value;
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

int
operator== (const CosTrading::Admin::OctetSeq& left,
            const CosTrading::Admin::OctetSeq& right)
{
  int return_value = 0;
  CORBA::ULong left_length = left.length (),
    right_length = right.length ();
  
  if (left_length == right_length) 
    {
      return_value = 1;
      for (CORBA::ULong i = 0; i < left_length; i++)
	{
	  if (left[i] != right[i])
	    {
	      return_value = 0;
	      break;
	    }
	}
    }

  return return_value;
}

  // *************************************************************
  // TAO_Trader_Factory
  // *************************************************************

#include "ace/Arg_Shifter.h"
#include "Trader_T.h"

TAO_Trader_Factory::TAO_TRADER*
TAO_Trader_Factory::create_trader (int& argc, char** argv)
{
  TAO_Trader_Factory trader_factory (argc, argv);
  return trader_factory.manufacture_trader ();
}

TAO_Trader_Factory::TAO_Trader_Factory (int& argc, char** argv)
  : conformance_ (LINKED),
    threadsafe_ (CORBA::B_FALSE),
    supports_dynamic_properties_ (CORBA::B_TRUE),
    supports_modifiable_properties_ (CORBA::B_TRUE),
    def_search_card_ (20),
    max_search_card_ (50),
    def_match_card_ (20),
    max_match_card_ (50),
    def_return_card_ (20),
    max_return_card_ (50),
    def_follow_policy_ (CosTrading::if_no_local),
    max_follow_policy_ (CosTrading::always)
{
  this->parse_args (argc, argv);
}

TAO_TRADER*
TAO_Trader_Factory::manufacture_trader (void)
{
  typedef TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex> TRADER;

#if defined ACE_HAS_THREADS
  typedef TAO_Trader<ACE_Thread_Mutex, ACE_RW_Mutex>  MT_TRADER;
#else
  typedef TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex>  MT_TRADER;
#endif /* ACE_HAS_THREADS */
  
  TAO_TRADER* return_value = 0;
  int components = ACE_static_cast (int, TAO_TRADER::LOOKUP);

  if (this->conformance_ >= SIMPLE)
    components |= ACE_static_cast (int, TAO_TRADER::REGISTER);
  
  if (this->conformance_ >= STANDALONE)
    components |= ACE_static_cast (int, TAO_TRADER::ADMIN);

  if (this->conformance_ >= LINKED)
    components |= ACE_static_cast (int, TAO_TRADER::LINK);

  if (this->threadsafe_)
    {
      ACE_NEW_RETURN (return_value,
                      MT_TRADER (ACE_static_cast (TAO_TRADER::Trader_Components,
                                                  components)),
                      0);
    }
  else
    {
      ACE_NEW_RETURN (return_value,
                      TRADER (ACE_static_cast (TAO_TRADER::Trader_Components,
                                               components)),
                      0);
    }

  TAO_Import_Attributes_Impl import_attributes =
    return_value->import_attributes ();
  TAO_Support_Attributes_Impl support_attributes =
    return_value->support_attributes ();

  import_attributes.def_search_card (this->def_search_card_);
  import_attributes.max_search_card (this->max_search_card_);
  import_attributes.def_match_card (this->def_match_card_);
  import_attributes.max_match_card (this->max_match_card_);
  import_attributes.def_return_card (this->def_return_card_);
  import_attributes.max_return_card (this->max_return_card_);
  import_attributes.def_hop_count (this->def_hop_count_);
  import_attributes.max_hop_count (this->max_hop_count_);
  import_attributes.def_follow_policy (this->def_follow_policy_);
  import_attributes.max_follow_policy (this->max_follow_policy_);
  support_attributes.supports_modifiable_properties (this->supports_modifiable_properties_);
  support_attributes.supports_dynamic_properties (this->supports_dynamic_properties_);

  return return_value;
}

void
TAO_Trader_Factory::parse_args (int& argc, char** argv)
{  
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      char *current_arg = arg_shifter.get_current ();
      
      if (ACE_OS::strcmp (current_arg, "-TSthreadsafe") == 0)
        {
          arg_shifter.consume_arg ();
          this->threadsafe_ = CORBA::B_TRUE;
        }
      else if (ACE_OS::strcmp (current_arg, "-TSconformance") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* conformance_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (conformance_str, "Linked") == 0)
                this->conformance_ = LINKED;
              else if (ACE_OS::strcasecmp (conformance_str, "Query") == 0)
                this->conformance_ = QUERY;
              else if (ACE_OS::strcasecmp (conformance_str, "Simple") == 0)
                this->conformance_ = SIMPLE;
              else if (ACE_OS::strcasecmp (conformance_str, "Standalone") == 0)
                this->conformance_ = STANDALONE;

              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-TSsupports_dynamic_properties") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* arg_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (arg_str, "true") == 0)
                this->supports_dynamic_properties_ = CORBA::B_TRUE;
              else if (ACE_OS::strcasecmp (arg_str, "false") == 0)
                this->supports_dynamic_properties_ = CORBA::B_FALSE;

              arg_shifter.consume_arg ();
            }
          
        }
      else if (ACE_OS::strcmp (current_arg, "-TSsupports_modifiable_properties") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* arg_str = arg_shifter.get_current ();

              if (ACE_OS::strcasecmp (arg_str, "true") == 0)
                this->supports_modifiable_properties_ = CORBA::B_TRUE;
              else if (ACE_OS::strcasecmp (arg_str, "false") == 0)
                this->supports_modifiable_properties_ = CORBA::B_FALSE;

              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-TSdef_search_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_search_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSdef_match_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_match_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSdef_return_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_return_card") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSdef_hop_count") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_hop_count") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              CORBA::ULong value =
                ACE_static_cast (CORBA::ULong,
                                 ACE_OS::atoi (arg_shifter.get_current ()));
              arg_shifter.consume_arg ();

              if (ACE_OS::strstr (current_arg, "card"))
                {
                  if (ACE_OS::strstr (current_arg, "max"))
                    {
                      if (ACE_OS::strstr (current_arg, "search"))
                        this->max_search_card_ = value;
                      else if (ACE_OS::strstr (current_arg, "match"))
                        this->max_match_card_ = value;
                      else
                        this->max_return_card_ = value;
                    }
                  else
                    {
                      if (ACE_OS::strstr (current_arg, "search"))
                        this->def_search_card_ = value;
                      else if (ACE_OS::strstr (current_arg, "match"))
                        this->def_match_card_ = value;
                      else 
                        this->def_return_card_ = value;
                    }
                }
              else
                {
                  if (ACE_OS::strstr (current_arg, "max"))
                    this->max_hop_count_ = value;
                  else
                    this->def_hop_count_ = value;
                }
            }
        }
      else if (ACE_OS::strcmp (current_arg, "-TSdef_follow_policy") == 0 ||
               ACE_OS::strcmp (current_arg, "-TSmax_follow_policy") == 0)
        {
          arg_shifter.consume_arg ();
          if (arg_shifter.is_parameter_next ())
            {
              char* arg_str = arg_shifter.get_current ();
              CosTrading::FollowOption follow_option;

              if (ACE_OS::strcasecmp (arg_str, "always") == 0)
                follow_option = CosTrading::always;
              else if (ACE_OS::strcasecmp (arg_str, "if_no_local") == 0)
                follow_option = CosTrading::if_no_local;
              else if (ACE_OS::strcasecmp (arg_str, "local_only") == 0)
                follow_option = CosTrading::local_only;
              else if (ACE_OS::strstr (current_arg, "def"))
                follow_option = this->def_follow_policy_;
              else
                follow_option = this->max_follow_policy_;

              if (ACE_OS::strstr (current_arg, "def"))
                this->def_follow_policy_ = follow_option;
              else
                this->max_follow_policy_ = follow_option;
              
              arg_shifter.consume_arg ();              
            }
        }
      else
        arg_shifter.ignore_arg ();
    }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
ACE_MT (template class TAO_Trader<ACE_Thread_Mutex, ACE_RW_Mutex>);
template class TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#if defined (ACE_MT_SAFE) && (ACE_MT_SAFE != 0)
#pragma instantiate TAO_Trader<ACE_Thread_Mutex, ACE_RW_Mutex>;
#endif /* ACE_MT_SAFE */ 
#pragma instantiate TAO_Trader<ACE_Null_Mutex, ACE_Null_Mutex>;
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */ 

