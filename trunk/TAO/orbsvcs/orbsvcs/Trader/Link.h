/* -*- C++ -*- */

// ========================================================================
// $Id$
// 
// = LIBRARY
//    orbsvcs
// 
// = FILENAME
//   Link.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>,
//    Irfan Pyarali <irfan@cs.wustl.edu>,
//    Seth Widoff <sbw1@cs.wustl.edu>
// 
// ========================================================================

#ifndef TAO_LINK_H
#define TAO_LINK_H

#include "Attributes.h"
#include "Monitor.h" 
#include "stl.h"

template <class TRADER, class MAP_LOCK_TYPE>
class TAO_Link : 
  public TAO_Trader_Components <POA_CosTrading::Link>,
  public TAO_Support_Attributes <POA_CosTrading::Link>,
  public TAO_Link_Attributes <POA_CosTrading::Link>
{
public:

  TAO_Link (TRADER &trader);

  ~TAO_Link (void);
		
  virtual void add_link (const char *name, 
			 CosTrading::Lookup_ptr target, 
			 CosTrading::FollowOption def_pass_on_follow_rule, 
			 CosTrading::FollowOption limiting_follow_rule,
			 CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException,
		    CosTrading::Link::IllegalLinkName, 
		    CosTrading::Link::DuplicateLinkName, 
		    CosTrading::InvalidLookupRef, 
		    CosTrading::Link::DefaultFollowTooPermissive, 
		    CosTrading::Link::LimitingFollowTooPermissive));
  // BEGIN SPEC
  // The add_link operation allows a trader subsequently to use the
  // service of another trader in the performance of its own trading
  // service operations. 

  // The "name" parameter is used in subsequent link management
  // operations to identify the intended link. If the parameter is not
  // legally formed, then the IllegalLinkName exception is raised. An
  // exception of DuplicateLinkName is raised if the link name already
  // exists. The link name is also used as a component in a sequence
  // of name components in naming a trader for resolving or forwarding
  // operations. The sequence of context relative link names provides
  // a path to a trader.

  // The "target" parameter identifies the Lookup interface at which
  // the trading service provided by the target trader can be
  // accessed. Should the Lookup interface parameter be nil, then an
  // exception of InvalidLookupRef is raised. The target interface is
  // used to obtain the associated Register interface, which will be
  // subsequently returned as part of a describe_link operation and
  // invoked as part of a resolve operation.

  // The "def_pass_on_follow_rule" parameter specifies the default
  // link behavior for the link if no link behavior is specified on an
  // importer's query request. If the "def_pass_on_follow_rule"
  // exceeds the "limiting_follow_rule" specified in the next
  // parameter, then a DefaultFollowTooPermissive exception is
  // raised.

  // The "limiting_follow_rule" parameter specifies the most
  // permissive link follow behavior that the link is willing to
  // tolerate. The exception LimitingFollowTooPermissive is raised if
  // this parameter exceeds the trader's attribute of
  // "max_link_follow_policy" at the time of the link's creation. Note
  // it is possible for a link's "limiting_follow_rule" to exceed the
  // trader's "max_link_follow_policy" later in the life of a link, as
  // it is possible that the trader could set its
  // "max_link_follow_policy" to a more restrictive value after the
  // creation of the link. 
  // END SPEC
  
  virtual void remove_link (const char *name,
			    CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CORBA::SystemException,
		    CosTrading::Link::IllegalLinkName, 
		    CosTrading::Link::UnknownLinkName));

  // BEGIN SPEC
  // The remove_link operation removes all knowledge of the target
  // trader. The target trader cannot be used subsequently to resolve,
  // forward, or propagate trading operations from this trader.

  // The "name" parameter identifies the link to be removed. The
  // exception IllegalLinkName is raised if the link is formed poorly
  // and the UnknownLinkName exception is raised if the named link is
  // not in the trader. 
  // END SPEC
  
  virtual CosTrading::Link::LinkInfo* describe_link (const char *name,
						     CORBA::Environment&) 
    TAO_THROW_SPEC ((CORBA::SystemException, 
		    CosTrading::Link::IllegalLinkName, 
		    CosTrading::Link::UnknownLinkName));

  // BEGIN SPEC
  // The describe_link operation returns information on a link held in
  // the trader.

  // The "name" parameter identifies the link whose description is
  // required. For a malformed link name, the exception
  // IllegalLinkName is raised. An UnknownLinkName exception is raised
  // if the named link is not found in the trader.

  // The operation returns a LinkInfo structure comprising: ° the
  // Lookup interface of the target trading service, ° the Register
  // interface of the target trading service, and ° the default, as
  // well as the limiting follow behavior of the named link.

  // If the target service does not support the Register interface,
  // then that field of the LinkInfo structure is nil. Given the
  // description of the Register::resolve() operation in "Resolve
  // Operation" on page 16-45, most implementations will opt for
  // determining the Register interface when add_link is called and
  // storing that information statically with the rest of the link
  // state. 
  // END SPEC
  
  virtual CosTrading::LinkNameSeq* list_links (CORBA::Environment& _env)
    TAO_THROW_SPEC ((CORBA::SystemException));

  // BEGIN SPEC
  // The list_links operation returns a list of the names of all
  // trading links within the trader. The names can be used
  // subsequently for other management operations, such as
  // describe_link or remove_link. 
  // END SPEC
  
  virtual void modify_link (const char *name, 
			    CosTrading::FollowOption def_pass_on_follow_rule, 
			    CosTrading::FollowOption limiting_follow_rule,
			    CORBA::Environment& _env) 
    TAO_THROW_SPEC ((CosTrading::Link::IllegalLinkName, 
		    CosTrading::Link::UnknownLinkName, 
		    CosTrading::Link::DefaultFollowTooPermissive, 
		    CosTrading::Link::LimitingFollowTooPermissive));

  // BEGIN SPEC
  // The modify_link operation is used to change the existing link
  // follow behaviors of an identified link. The Lookup interface
  // reference of the target trader and the name of the link cannot be
  // changed.

  // The "name" parameter identifies the link whose follow behaviors
  // are to be changed. A poorly formed "name" raises the
  // IllegalLinkName exception. An UnknownLinkName exception is raised
  // if the link name is not known to the trader.

  // The "def_pass_on_follow_rule" parameter specifies the new default
  // link behavior for this link. If the "def_pass_on_follow_rule"
  // exceeds the "limiting_follow_rule" specified in the next
  // parameter, then a DefaultFollowTooPermissive exception is
  // raised.

  // The "limiting_follow_rule" parameter specifies the new limit for
  // the follow behavior of this link. The exception
  // LimitingFollowTooPermissive is raised if the value exceeds the
  // current "max_link_follow_policy" of the trader. 
  // END SPEC
  		
private:

  typedef TAO_Monitor
  <
  map <string, CosTrading::Link::LinkInfo, less <string> >, 
  MAP_LOCK_TYPE
  >
  LINKS;

  LINKS links_;
  // The collection of link connecting this trader to others in the
  // federation. 
  
  TRADER &trader_;
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Link.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* TAO_LINK_H */
