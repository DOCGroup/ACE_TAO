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
#include "Locking.h"
#include "Trader.h"

template <class TRADER, class MAP_LOCK_TYPE>  
TAO_Link<TRADER,MAP_LOCK_TYPE>::TAO_Link (TRADER &trader)
  : trader_ (trader),
    TAO_Trader_Components <POA_CosTrading::Link> (trader.trading_components ()),
    TAO_Link_Attributes <POA_CosTrading::Link> (trader.link_attributes ()),
    TAO_Support_Attributes <POA_CosTrading::Link> (trader.support_attributes ())
{
}

template <class TRADER, class MAP_LOCK_TYPE>
TAO_Link<TRADER,MAP_LOCK_TYPE>::~TAO_Link (void)
{
}

template <class TRADER, class MAP_LOCK_TYPE> void
TAO_Link<TRADER,MAP_LOCK_TYPE>::
add_link (const char *name, 
	  CosTrading::Lookup_ptr target, 
	  CosTrading::FollowOption def_pass_on_follow_rule, 
	  CosTrading::FollowOption limiting_follow_rule,
	  CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::DuplicateLinkName, 
		  CosTrading::InvalidLookupRef, 
		  CosTrading::Link::DefaultFollowTooPermissive, 
		  CosTrading::Link::LimitingFollowTooPermissive))
{
  TAO_WRITE_GUARD (MAP_LOCK_TYPE, 
		   ace_mon, 
		   this->links_.lock ());
  
  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::Link::IllegalLinkName (name));

  // Ensure this isn't a duplicate link name.
  string link_name (name);
  LINKS::iterator links_iter = this->links_.find (link_name);
  if (links_iter != this->links_.end ())
    TAO_THROW (CosTrading::Link::DuplicateLinkName (name));

  // Ensure the lookup_ptr isn't nil.
  if (target == CosTrading::Lookup::_nil())
    TAO_THROW (CosTrading::InvalidLookupRef (target));

  // Ensure that the default link behavior isn't stronger than the
  // limiting link behavior.
  if (def_pass_on_follow_rule > limiting_follow_rule)
    TAO_THROW (CosTrading::Link::DefaultFollowTooPermissive
	       (def_pass_on_follow_rule, limiting_follow_rule));

  // Ensure that the limiting link behavior for this link doesn't
  // exceed the maximum allowed for a link.
  if (limiting_follow_rule < this->max_link_follow_policy (_env))
    TAO_THROW (CosTrading::Link::LimitingFollowTooPermissive
	       (limiting_follow_rule, this->max_link_follow_policy (_env)));

  // Create a link info structure for this link of the federation.
  CosTrading::Link::LinkInfo link_info;
  link_info.target = target;
  link_info.target_reg = target->register_if (_env);
  link_info.def_pass_on_follow_rule = def_pass_on_follow_rule;
  link_info.limiting_follow_rule = limiting_follow_rule;
  TAO_CHECK_ENV_RETURN_VOID (_env);

  // Insert this link into the collection of links.
  this->links_.insert (link_name, link_info);
}

template <class TRADER, class MAP_LOCK_TYPE> void
TAO_Link<TRADER,MAP_LOCK_TYPE>::remove_link (const char *name,
			       CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException, 
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName))
{
  TAO_WRITE_GUARD (MAP_LOCK_TYPE, 
		   ace_mon, 
		   this->links_.lock ());

  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::Link::IllegalLinkName (name));

  // Ensure this isn't a duplicate link name.
  string link_name (name);
  LINKS::iterator links_iter = this->links_.find (link_name);
  if (links_iter == this->links_.end ())
    TAO_THROW (CosTrading::Link::UnknownLinkName (name));

  // Erase the link state from the map.
  this->links_.erase (links_iter);
}

template <class TRADER, class MAP_LOCK_TYPE> CosTrading::Link::LinkInfo * 
TAO_Link<TRADER,MAP_LOCK_TYPE>::describe_link (const char *name,
					       CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CosTrading::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName))
{
  TAO_READ_GUARD_RETURN (MAP_LOCK_TYPE, 
			 ace_mon, 
			 this->links_.lock (),
			 (CosTrading::Link::LinkInfo*) 0);

  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW_RETURN (CosTrading::Link::IllegalLinkName (name), 0);
  
  // Ensure this isn't a duplicate link name.
  string link_name (name);
  LINKS::iterator links_iter = this->links_.find (link_name);
  if (links_iter == this->links_.end ())
    TAO_THROW_RETURN (CosTrading::Link::UnknownLinkName (name), 0);

  // return the link infor for this link name.
  return &(*links_iter).second;
}

template <class TRADER, class MAP_LOCK_TYPE> CosTrading::LinkNameSeq* 
TAO_Link<TRADER,MAP_LOCK_TYPE>::list_links (CORBA::Environment& _env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  TAO_READ_GUARD_RETURN (MAP_LOCK_TYPE, 
			 ace_mon, 
			 this->links_.lock (),
			 (CosTrading::LinkNameSeq*) 0);

  // Allocate space for the link names.
  CORBA::ULong size = this->links_.size (), i = 0;
  CosTrading::LinkName* link_seq =
    CosTrading::LinkNameSeq::allocbuf (size);

  // Copy the link names into the buffer.
  for (LINKS::iterator links_iter = this->links_.begin ();
       links_iter != this->links_.end ();
       links_iter++)
    link_seq[i++] = CORBA::string_dup ((*links_iter).first.data ());

  // Return a sequence of the buf names.
  return new CosTrading::LinkNameSeq (i, i, link_seq, 1);
}

template <class TRADER, class MAP_LOCK_TYPE> void
TAO_Link<TRADER,MAP_LOCK_TYPE>::
modify_link (const char *name, 
	     CosTrading::FollowOption def_pass_on_follow_rule, 
	     CosTrading::FollowOption limiting_follow_rule,
	     CORBA::Environment& _env) 
  TAO_THROW_SPEC ((CORBA::SystemException,
		  CosTrading::Link::IllegalLinkName, 
		  CosTrading::Link::UnknownLinkName, 
		  CosTrading::Link::DefaultFollowTooPermissive, 
		  CosTrading::Link::LimitingFollowTooPermissive))
{
  TAO_WRITE_GUARD (MAP_LOCK_TYPE, 
		   ace_mon, 
		   this->links_.lock ());

  // Ensure the link name is valid.
  if (! TAO_Trader_Base::is_valid_identifier_name (name))
    TAO_THROW (CosTrading::Link::IllegalLinkName (name));

  // Ensure this isn't a duplicate link name.
  string link_name (name);
  LINKS::iterator links_iter = this->links_.find (link_name);
  if (links_iter == this->links_.end ())
    TAO_THROW (CosTrading::Link::UnknownLinkName (name));

  // Ensure that the default link behavior isn't stronger than the
  // limiting link behavior.
  if (def_pass_on_follow_rule > limiting_follow_rule)
    TAO_THROW (CosTrading::Link::DefaultFollowTooPermissive
	       (def_pass_on_follow_rule, limiting_follow_rule));

  // Ensure that the limiting link behavior for this link doesn't
  // exceed the maximum allowed for a link.
  if (limiting_follow_rule < this->max_link_follow_policy (_env))
    TAO_THROW (CosTrading::Link::LimitingFollowTooPermissive
	       (limiting_follow_rule, this->max_link_follow_policy (_env)));

  // Adjust the link settings
  CosTrading::Link::LinkInfo& link_info = (*links_iter).second;
  link_info.def_pass_on_follow_rule = def_pass_on_follow_rule;
  link_info.limiting_follow_rule = limiting_follow_rule;
}

#endif /* TAO_LINK_C */
