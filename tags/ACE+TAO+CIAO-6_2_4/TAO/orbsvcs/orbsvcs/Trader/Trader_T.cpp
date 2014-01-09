// $Id$

#ifndef TAO_TRADER_CPP
#define TAO_TRADER_CPP

#include "orbsvcs/Trader/Trader_T.h"
#include "orbsvcs/Trader/Trader_Interfaces.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::
TAO_Trader (TAO_Trader_Base::Trader_Components components)
{

  // @@ Seth, we need a way to propagate the exception out.  This will
  // not work on platforms using environment variable.
  for (int i = LOOKUP_IF; i <= LINK_IF; i++)
    this->ifs_[i] = 0;

  if (ACE_BIT_ENABLED (components, LOOKUP))
    {
      TAO_Lookup<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* lookup;

      ACE_NEW (lookup,
               (TAO_Lookup<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>) (*this));

      this->trading_components ().lookup_if (lookup->_this ());
      lookup->_remove_ref ();

      this->ifs_[LOOKUP_IF] = lookup;
    }
  if (ACE_BIT_ENABLED (components, REGISTER))
    {
      TAO_Register<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* reg;

      ACE_NEW (reg,
               (TAO_Register<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>) (*this));

      this->trading_components ().register_if (reg->_this ());
      reg->_remove_ref ();

      this->ifs_[REGISTER_IF] = reg;
    }
  if (ACE_BIT_ENABLED (components, ADMIN))
    {
      TAO_Admin<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* admin;

      ACE_NEW (admin,
               (TAO_Admin<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>) (*this));

      this->trading_components ().admin_if (admin->_this ());
      admin->_remove_ref ();

      this->ifs_[ADMIN_IF] = admin;
    }
  if (ACE_BIT_ENABLED (components, PROXY))
    {
      TAO_Proxy<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* proxy;

      ACE_NEW (proxy,
               (TAO_Proxy<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>) (*this));

      this->trading_components ().proxy_if (proxy->_this ());
      proxy->_remove_ref ();

      this->ifs_[PROXY_IF] = proxy;
    }
  if (ACE_BIT_ENABLED (components, LINK))
    {
      TAO_Link<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* link;

      ACE_NEW (link,
               (TAO_Link<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>) (*this));

      this->trading_components ().link_if (link->_this ());
      link->_remove_ref ();

      this->ifs_[LINK_IF] = link;
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::~TAO_Trader (void)
{
  // Remove Trading Components from POA


  for (int i = LOOKUP_IF; i <= LINK_IF; i++)
    {
      if (this->ifs_[i] != 0)
        {
          try
            {
              PortableServer::POA_var poa =
                this->ifs_[i]->_default_POA ();

              PortableServer::ObjectId_var id =
                poa->servant_to_id (this->ifs_[i]);

              poa->deactivate_object (id.in ());
            }
          catch (const CORBA::Exception&)
            {
              // Don't let exceptions propagate out of this call since
              // it's the destructor!
            }
        }
    }
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Offer_Database<MAP_LOCK_TYPE>&
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::offer_database (void)
{
  return this->offer_database_;
}

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE> ACE_Lock &
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::lock (void)
{
  return this->lock_;
}

template <class IF>
TAO_Trader_Components<IF>::
TAO_Trader_Components (const TAO_Trading_Components_i& comps)
  : comps_ (comps)
{
}

template <class IF> CosTrading::Lookup_ptr
TAO_Trader_Components<IF>::lookup_if (void)
{
  return CosTrading::Lookup::_duplicate (this->comps_.lookup_if ());
}

template <class IF> CosTrading::Register_ptr
TAO_Trader_Components<IF>::register_if (void)
{
  return CosTrading::Register::_duplicate (this->comps_.register_if ());
}

template <class IF> CosTrading::Admin_ptr
TAO_Trader_Components<IF>::admin_if (void)
{
  return CosTrading::Admin::_duplicate (this->comps_.admin_if ());
}

template <class IF> CosTrading::Proxy_ptr
TAO_Trader_Components<IF>::proxy_if (void)
{
  return CosTrading::Proxy::_duplicate (this->comps_.proxy_if ());
}

template <class IF> CosTrading::Link_ptr
TAO_Trader_Components<IF>::link_if (void)
{
  return CosTrading::Link::_duplicate (this->comps_.link_if ());
}
template <class IF>
TAO_Support_Attributes<IF>::
TAO_Support_Attributes (const TAO_Support_Attributes_i& attrs)
  : attrs_ (attrs)
{
}

template <class IF> CORBA::Boolean
TAO_Support_Attributes<IF>::supports_modifiable_properties (void)
{
  return this->attrs_.supports_modifiable_properties ();
}

template <class IF> CORBA::Boolean
TAO_Support_Attributes<IF>::supports_dynamic_properties (void)
{
  return this->attrs_.supports_dynamic_properties ();
}

template <class IF> CORBA::Boolean
TAO_Support_Attributes<IF>::supports_proxy_offers (void)
{
  return this->attrs_.supports_proxy_offers ();
}

template <class IF> CosTrading::TypeRepository_ptr
TAO_Support_Attributes<IF>::type_repos ( )
{
  return CosTrading::TypeRepository::_duplicate (this->attrs_.type_repos ());
}

template <class IF>
TAO_Import_Attributes<IF>::
TAO_Import_Attributes (const TAO_Import_Attributes_i& attrs)
  : attrs_ (attrs)
{
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::def_search_card (void)
{
  return this->attrs_.def_search_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_search_card (void)
{
  return this->attrs_.max_search_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::def_match_card (void)
{
  return this->attrs_.def_match_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_match_card (void)
{
  return this->attrs_.max_match_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::def_return_card (void)
{
  return this->attrs_.def_return_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_return_card (void)
{
  return this->attrs_.max_return_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_list (void)
{
  return this->attrs_.max_list ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::def_hop_count (void)
{
  return this->attrs_.def_hop_count ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_hop_count (void)
{
  return this->attrs_.max_hop_count ();
}

template <class IF> CosTrading::FollowOption
TAO_Import_Attributes<IF>::def_follow_policy (void)
{
  return this->attrs_.def_follow_policy ();
}

template <class IF> CosTrading::FollowOption
TAO_Import_Attributes<IF>::max_follow_policy (void)
{
  return this->attrs_.max_follow_policy ();
}

template <class IF>
TAO_Link_Attributes<IF>::
TAO_Link_Attributes (const TAO_Link_Attributes_i& attrs)
  : attrs_ (attrs)
{
}

template <class IF> CosTrading::FollowOption
TAO_Link_Attributes<IF>::max_link_follow_policy (void)
{
  return this->attrs_.max_link_follow_policy ();
}

template <class ELEMENT_TYPE> int
TAO_Element_Equal<ELEMENT_TYPE>::
operator () (TAO_DynSequence_i& ,
             const ELEMENT_TYPE&)
{
  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_TRADER_CPP */
