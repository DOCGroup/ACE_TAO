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

#ifndef TAO_TRADER_C
#define TAO_TRADER_C

#include "Trader_T.h"
#include "Trader_Interfaces.h"

ACE_RCSID(Trader, Trader_T, "$Id$")

  // *************************************************************
  // TAO_Trader
  // *************************************************************

template <class TRADER_LOCK_TYPE, class MAP_LOCK_TYPE>
TAO_Trader<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>::
TAO_Trader (TAO_Trader_Base::Trader_Components components)
{
  CORBA::Environment env;
  for (int i = LOOKUP_IF; i <= LINK_IF; i++)
    this->ifs_[i] = 0;

  if (ACE_BIT_ENABLED (components, LOOKUP))
    {
      TAO_Lookup<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* lookup =
        new TAO_Lookup<TRADER_LOCK_TYPE,MAP_LOCK_TYPE> (*this);
      this->trading_components ().lookup_if (lookup->_this (env));
      this->ifs_[LOOKUP_IF] = lookup;
    }
  if (ACE_BIT_ENABLED (components, REGISTER))
    {
      TAO_Register<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* reg =
        new TAO_Register<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> (*this);
      this->trading_components ().register_if (reg->_this (env));
      this->ifs_[REGISTER_IF] = reg;
    }
  if (ACE_BIT_ENABLED (components, ADMIN))
    {
      TAO_Admin<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* admin =
        new TAO_Admin<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> (*this);
      this->trading_components ().admin_if (admin->_this (env));
      this->ifs_[ADMIN_IF] = admin;
    }
  if (ACE_BIT_ENABLED (components, PROXY))
    {
      TAO_Proxy<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* proxy =
        new TAO_Proxy<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> (*this);
      this->trading_components ().proxy_if (proxy->_this (env));
      this->ifs_[PROXY_IF] = proxy;
    }
  if (ACE_BIT_ENABLED (components, LINK))
    {
      TAO_Link<TRADER_LOCK_TYPE, MAP_LOCK_TYPE>* link =
        new TAO_Link<TRADER_LOCK_TYPE, MAP_LOCK_TYPE> (*this);
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

  // *************************************************************
  // Template Attribute Classes
  // *************************************************************

template <class IF>
TAO_Trader_Components<IF>::
TAO_Trader_Components (const TAO_Trading_Components_i& comps)
  : comps_ (comps)
{
}

template <class IF> CosTrading::Lookup_ptr
TAO_Trader_Components<IF>::lookup_if (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Lookup::_duplicate (this->comps_.lookup_if ());
}

template <class IF> CosTrading::Register_ptr
TAO_Trader_Components<IF>::register_if (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Register::_duplicate (this->comps_.register_if ());
}

template <class IF> CosTrading::Admin_ptr
TAO_Trader_Components<IF>::admin_if (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Admin::_duplicate (this->comps_.admin_if ());
}

template <class IF> CosTrading::Proxy_ptr
TAO_Trader_Components<IF>::proxy_if (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return CosTrading::Proxy::_duplicate (this->comps_.proxy_if ());
}

template <class IF> CosTrading::Link_ptr
TAO_Trader_Components<IF>::link_if (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
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
TAO_Support_Attributes<IF>::supports_modifiable_properties (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_modifiable_properties ();
}

template <class IF> CORBA::Boolean
TAO_Support_Attributes<IF>::supports_dynamic_properties (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_dynamic_properties ();
}

template <class IF> CORBA::Boolean
TAO_Support_Attributes<IF>::supports_proxy_offers (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.supports_proxy_offers ();
}

template <class IF> CosTrading::TypeRepository_ptr
TAO_Support_Attributes<IF>::type_repos (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
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
TAO_Import_Attributes<IF>::def_search_card (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_search_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_search_card (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_search_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::def_match_card (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_match_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_match_card (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_match_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::def_return_card (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_return_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_return_card (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_return_card ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_list (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_list ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::def_hop_count (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_hop_count ();
}

template <class IF> CORBA::ULong
TAO_Import_Attributes<IF>::max_hop_count (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_hop_count ();
}

template <class IF> CosTrading::FollowOption
TAO_Import_Attributes<IF>::def_follow_policy (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.def_follow_policy ();
}

template <class IF> CosTrading::FollowOption
TAO_Import_Attributes<IF>::max_follow_policy (CORBA::Environment& TAO_IN_ENV)
  TAO_THROW_SPEC ((CORBA::SystemException))
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
TAO_Link_Attributes<IF>::max_link_follow_policy (CORBA::Environment &env)
  TAO_THROW_SPEC ((CORBA::SystemException))
{
  return this->attrs_.max_link_follow_policy ();
}


  // *************************************************************
  // TAO_Sequence_Extracter
  // *************************************************************

template <class OPERAND_TYPE> CORBA::Boolean
TAO_find (const CORBA::Any& sequence, const OPERAND_TYPE& element)
{
  CORBA::Boolean return_value = 0;
  TAO_Element_Equal<OPERAND_TYPE> functor;
  TAO_DynSequence_i dyn_seq (sequence);

  TAO_TRY
    {
      CORBA::ULong length = dyn_seq.length (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      
      for (CORBA::ULong i = 0; i < length && ! return_value; i++)
        {
          if (functor (dyn_seq, element))
            {
              //ACE_DEBUG ((LM_DEBUG, "TAO_find::success\n"));
              return_value = 1;
            }
        }
    }
  TAO_CATCHANY {} TAO_ENDTRY;

  return return_value;
}

template <class ELEMENT_TYPE> int
TAO_Element_Equal<ELEMENT_TYPE>::
operator () (TAO_DynSequence_i& dyn_any,
             const ELEMENT_TYPE& element)
{
  return 1;
}

#endif /* TAO_TRADER_C */
