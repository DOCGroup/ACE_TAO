// Attributes.h
// $Id$

#if !defined (TAO_ATTRIBUTES_C)
#define TAO_ATTRIBUTES_C

#include "Attributes.h"
#include "Trader_Base.h"

TAO_Support_Attributes_Impl::TAO_Support_Attributes_Impl (TAO_Trader_Base &trader)
  : trader_ (trader),
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
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CORBA::B_FALSE);
  return this->supports_modifiable_properties_;
}

void
TAO_Support_Attributes_Impl::supports_modifiable_properties (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->supports_modifiable_properties_ = new_value;
}

CORBA::Boolean
TAO_Support_Attributes_Impl::supports_dynamic_properties (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CORBA::B_FALSE);

  return this->supports_dynamic_properties_;
}
    
void
TAO_Support_Attributes_Impl::supports_dynamic_properties (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->supports_dynamic_properties_ = new_value;
}
    
CORBA::Boolean
TAO_Support_Attributes_Impl::supports_proxy_offers (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CORBA::B_FALSE);
    
  return this->supports_proxy_offers_;
}

void
TAO_Support_Attributes_Impl::supports_proxy_offers (CORBA::Boolean new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->supports_proxy_offers_ = new_value;
}

CosTrading::TypeRepository_ptr
TAO_Support_Attributes_Impl::type_repos (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CosTrading::TypeRepository::_nil ());
  return this->type_repos_->_duplicate (type_repos_);
}
  
void
TAO_Support_Attributes_Impl::type_repos (CosTrading::TypeRepository_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->type_repos_ = new_value;
}
  

TAO_Import_Attributes_Impl::TAO_Import_Attributes_Impl (TAO_Trader_Base &trader)
  : trader_ (trader),
    def_search_card_ (10),
    max_search_card_ (10),
    def_match_card_ (10),
    max_match_card_ (10), 
    def_return_card_ (10),
    max_return_card_ (10), 
    max_list_ (0),
    def_hop_count_ (0),
    max_hop_count_ (0),
    def_follow_policy_ (CosTrading::local_only),
    max_follow_policy_ (CosTrading::local_only)
{
}
 

TAO_Import_Attributes_Impl::~TAO_Import_Attributes_Impl (void)
{
}
  
CORBA::ULong
TAO_Import_Attributes_Impl::def_search_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 10);
  return this->def_search_card_;
}

void
TAO_Import_Attributes_Impl::def_search_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->def_search_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_search_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 10);
  return this->max_search_card_;
}

void
TAO_Import_Attributes_Impl::max_search_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->max_search_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::def_match_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 10);
  return this->def_match_card_;
}

void
TAO_Import_Attributes_Impl::def_match_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->def_match_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_match_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 10);
  return this->max_match_card_;
}

void
TAO_Import_Attributes_Impl::max_match_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->max_match_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::def_return_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 10);
  return this->def_return_card_;
}

void
TAO_Import_Attributes_Impl::def_return_card (CORBA::ULong new_value)
{  
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->def_return_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_return_card (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 10); 
  return this->max_return_card_;
}

void
TAO_Import_Attributes_Impl::max_return_card (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->max_return_card_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_list (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 0); 
  return this->max_list_;
}

void
TAO_Import_Attributes_Impl::max_list (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->max_list_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::def_hop_count (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 0);  
  return this->def_hop_count_;
}

void
TAO_Import_Attributes_Impl::def_hop_count (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->def_hop_count_ = new_value;
}

CORBA::ULong
TAO_Import_Attributes_Impl::max_hop_count (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), 0);  
  return this->max_hop_count_;
}

void
TAO_Import_Attributes_Impl::max_hop_count (CORBA::ULong new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->max_hop_count_ = new_value;
}

CosTrading::FollowOption
TAO_Import_Attributes_Impl::def_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), CosTrading::local_only); 
  return this->def_follow_policy_;
}

void
TAO_Import_Attributes_Impl::def_follow_policy (FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->def_follow_policy_ = new_value;
}

FollowOption
TAO_Import_Attributes_Impl::max_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (), CosTrading::local_only); 
  return this->max_follow_policy_;
}

void
TAO_Import_Attributes_Impl::max_follow_policy (FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->max_follow_policy_ = new_value;
}


TAO_Trading_Components_Impl::TAO_Trading_Components_Impl (TAO_Trader_Base &trader)
  : trader_ (trader),
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
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CosTrading::Lookup::_nil ());  
  return this->lookup_;
}

void
TAO_Trading_Components_Impl::lookup_if (CosTrading::Lookup_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->lookup_ = new_value;
}

CosTrading::Register_ptr
TAO_Trading_Components_Impl::register_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CosTrading::Register::_nil ());  
  return this->register_;
}

void
TAO_Trading_Components_Impl::register_if (CosTrading::Register_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->register_ = new_value;
}

CosTrading::Link_ptr
TAO_Trading_Components_Impl::link_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CosTrading::Link::_nil ());  
  return this->link_;
}

void
TAO_Trading_Components_Impl::link_if (CosTrading::Link_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->link_ = new_value;
}

CosTrading::Proxy_ptr
TAO_Trading_Components_Impl::proxy_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CosTrading::Proxy::_nil ());  
  return this->proxy_;
}
  
void
TAO_Trading_Components_Impl::proxy_if (CosTrading::Proxy_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->proxy_ = new_value;
}
  
CosTrading::Admin_ptr
TAO_Trading_Components_Impl::admin_if (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CosTrading::Admin::_nil ());  
  return this->admin_;
}

void
TAO_Trading_Components_Impl::admin_if (CosTrading::Admin_ptr new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->admin_ = new_value;
}

  
TAO_Link_Attributes_Impl::TAO_Link_Attributes_Impl (TAO_Trader_Base &trader)
  :trader_ (trader),
   max_link_follow_policy_ (CosTrading::local_only)
{
}
 
TAO_Link_Attributes_Impl::~TAO_Link_Attributes_Impl (void)
{
}

CosTrading::FollowOption 
TAO_Link_Attributes_Impl::max_link_follow_policy (void) const
{
  ACE_READ_GUARD_RETURN (ACE_Lock, ace_mon, this->trader_.lock (),
			 CosTrading::local_only);  
  return this->max_link_follow_policy_;
}
  
void  
TAO_Link_Attributes_Impl::max_link_follow_policy (CosTrading::FollowOption new_value)
{
  ACE_WRITE_GUARD (ACE_Lock, ace_mon, this->trader_.lock ());  
  this->max_link_follow_policy_ = new_value;
}

template <class IF>
TAO_Trader_Components<IF>::
TAO_Trader_Components (const TAO_Trading_Components_Impl& comps) 
  : comps_ (comps)
{
}

template <class IF> CosTrading::Lookup_ptr
TAO_Trader_Components<IF>::lookup_if (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Lookup::_duplicate (this->comps_.lookup_if ());
}

template <class IF> CosTrading::Register_ptr
TAO_Trader_Components<IF>::register_if (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Register::_duplicate (this->comps_.register_if ());
}

template <class IF> CosTrading::Admin_ptr
TAO_Trader_Components<IF>::admin_if (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Admin::_duplicate (this->comps_.admin_if ());
}

template <class IF> CosTrading::Proxy_ptr
TAO_Trader_Components<IF>::proxy_if (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Proxy::_duplicate (this->comps_.proxy_if ());
}

template <class IF> CosTrading::Link_ptr
TAO_Trader_Components<IF>::link_if (CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Link::_duplicate (this->comps_.link_if ());
}
template <class IF> 
TAO_Support_Attributes<IF>::
TAO_Support_Attributes (const TAO_Support_Attributes_Impl& attrs) 
  : attrs_ (attrs)
{
}

template <class IF> CORBA::Boolean 
TAO_Support_Attributes<IF>::supports_modifiable_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_modifiable_properties ();
}

template <class IF> CORBA::Boolean 
TAO_Support_Attributes<IF>::supports_dynamic_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_dynamic_properties ();
}

template <class IF> CORBA::Boolean 
TAO_Support_Attributes<IF>::supports_proxy_offers (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_proxy_offers ();
}

template <class IF> CosTrading::TypeRepository_ptr 
TAO_Support_Attributes<IF>::type_repos (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::TypeRepository::_duplicate (this->attrs_.type_repos ());
}

template <class IF> 
TAO_Import_Attributes<IF>::
TAO_Import_Attributes (const TAO_Import_Attributes_Impl& attrs)
  : attrs_ (attrs)
{
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::def_search_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_search_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_search_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_search_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::def_match_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_match_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_match_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_match_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::def_return_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_return_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_return_card (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_return_card ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_list (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_list ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::def_hop_count (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_hop_count ();
}

template <class IF> CORBA::ULong 
TAO_Import_Attributes<IF>::max_hop_count (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_hop_count ();
}

template <class IF> CosTrading::FollowOption 
TAO_Import_Attributes<IF>::def_follow_policy (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_follow_policy ();
}

template <class IF> CosTrading::FollowOption 
TAO_Import_Attributes<IF>::max_follow_policy (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_follow_policy ();
}

template <class IF> 
TAO_Link_Attributes<IF>::
TAO_Link_Attributes (const TAO_Link_Attributes_Impl& attrs)
  : attrs_ (attrs)
{
}

template <class IF> CosTrading::FollowOption
TAO_Link_Attributes<IF>::max_link_follow_policy (CORBA::Environment &env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_link_follow_policy ();
}

#endif /* TAO_ATTRIBUTES_C */
