/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//    Link.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#if !defined (TAO_LINK_C)
#define TAO_LINK_C

#include "Link.h"


template <class TRADER>  
TAO_Link<TRADER>::TAO_Link (const TRADER &trader)
  : trader_ (trader)
{
}

template <class TRADER>
TAO_Link<TRADER>::~TAO_Link (void)
{
}

template <class TRADER> void
TAO_Link<TRADER>::
add_link (const char *name, 
	  CosTrading::Lookup_ptr target, 
	  CosTrading::FollowOption def_pass_on_follow_rule, 
	  CosTrading::FollowOption limiting_follow_rule,
	  CORBA::Environment& _env) 
  TAO_THROW_SPEC (CORBA::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::DuplicateLinkName, 
		  CosTrading::InvalidLookupRef, 
		  CosTrading::Link::DefaultFollowTooPermissive, 
		  CosTrading::Link::LimitingFollowTooPermissive)
{
  TAO_THROW (CORBA::SystemException ());
}

template <class TRADER> void
TAO_Link<TRADER>::remove_link (const char *name,
			       CORBA::Environment& _env) 
  TAO_THROW_SPEC (CORBA::SystemException, 
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName)
{
  TAO_THROW (CORBA::SystemException ());
}

template <class TRADER> CosTrading::Link::LinkInfo * 
TAO_Link<TRADER>::describe_link (const char *name,
				 CORBA::Environment& _env) 
  TAO_THROW_SPEC (CosTrading::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName)
{
  TAO_THROW (CORBA::SystemException ());
}

template <class TRADER> CosTrading::LinkNameSeq * 
TAO_Link<TRADER>::list_links (CosTrading::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  TAO_THROW (CORBA::SystemException ());
}

template <class TRADER> void
TAO_Link<TRADER>::modify_link (const char *name, 
			       CosTrading::FollowOption def_pass_on_follow_rule, 
			       CosTrading::FollowOption limiting_follow_rule,
			       CORBA::Environment& _env) 
  TAO_THROW_SPEC (CORBA::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName, 
		  CosTrading::Link::DefaultFollowTooPermissive, 
		  CosTrading::Link::LimitingFollowTooPermissive)
{
  TAO_THROW (CORBA::SystemException ());
}

template <class TRADER>
CosTrading::Lookup_ptr
TAO_Lookup<TRADER>::lookup_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Lookup::_duplicate (this->trader_.trading_components ().lookup_if ());
}

template <class TRADER>
CosTrading::Register_ptr
TAO_Lookup<TRADER>::register_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Register::_duplicate (this->trader_.trading_components ().register_if ());
}

template <class TRADER>
CosTrading::Link_ptr
TAO_Lookup<TRADER>::link_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Link::_duplicate (this->trader_.trading_components ().link_if ());
}

template <class TRADER>
CosTrading::Proxy_ptr
TAO_Lookup<TRADER>::proxy_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Proxy::_duplicate (this->trader_.trading_components ().proxy_if ());
}

template <class TRADER>
CosTrading::Admin_ptr
TAO_Lookup<TRADER>::admin_if (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::Admin::_duplicate (this->trader_.trading_components ().admin_if ());
}

template <class TRADER>
CORBA::Boolean 
TAO_Lookup<TRADER>::supports_modifiable_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return this->trader_.support_attributes ().supports_modifiable_properties ();
}
  
template <class TRADER>
CORBA::Boolean 
TAO_Lookup<TRADER>::supports_dynamic_properties (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return this->trader_.support_attributes ().supports_dynamic_properties ();
}

template <class TRADER>
CORBA::Boolean 
TAO_Lookup<TRADER>::supports_proxy_offers (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return this->trader_.support_attributes ().supports_proxy_offers ();
}
  
template <class TRADER>
CosTrading::TypeRepository_ptr 
TAO_Lookup<TRADER>::type_repos (CORBA::Environment& _env)
  TAO_THROW_SPEC (CORBA::SystemException)
{
  return CosTrading::TypeRepository::_duplicate 
    (this->trader_.support_attributes ().type_repos ());
}

template <class TRADER>
CosTrading::FollowOption
TAO_Link<TRADER>::max_link_follow_policy (CORBA::Environment &env)
{
  return this->trader_.link_attributes ().max_link_follow_policy ();
}

#endif /* TAO_LINK_C */
