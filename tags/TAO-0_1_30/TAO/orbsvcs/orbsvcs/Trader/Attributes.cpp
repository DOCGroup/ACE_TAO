// Attributes.cpp
// $Id$

#include "Attributes.h"

TAO_Support_Attributes_Impl::TAO_Support_Attributes_Impl (TAO_Lockable &locker)
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
TAO_Support_Attributes_Impl::supports_modifiable_properties (CORBA::Boolean new_value)
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
TAO_Support_Attributes_Impl::supports_dynamic_properties (CORBA::Boolean new_value)
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
TAO_Support_Attributes_Impl::supports_proxy_offers (CORBA::Boolean new_value)
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
TAO_Support_Attributes_Impl::type_repos (CosTrading::TypeRepository_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->locker_.lock ());  
  this->type_repos_ = new_value;
}
  

TAO_Import_Attributes_Impl::TAO_Import_Attributes_Impl (TAO_Lockable &locker)
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
