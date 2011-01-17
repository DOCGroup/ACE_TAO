// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_Strategy_Proxy.h"
#include "tao/TAO_Server_Request.h"
#include "tao/debug.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_Framework/CSD_Strategy_Proxy.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

bool
TAO::CSD::Strategy_Proxy::custom_strategy
                          (CSD_Framework::Strategy_ptr strategy)
{
  if (this->strategy_impl_)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("(%P|%t) Error - TAO::CSD::Strategy_Proxy ")
                   ACE_TEXT("object already has a custom strategy.\n")));

      return false;
    }

  if (CORBA::is_nil(strategy))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR((LM_ERROR,
                   ACE_TEXT("(%P|%t) Error - TAO::CSD::Strategy_Proxy ")
                   ACE_TEXT("supplied with a NIL custom strategy.\n")));

      return false;
    }

  // We need to bump up the reference count of the strategy before saving
  // it off into our handle (smart pointer) data member.
  this->strategy_ = CSD_Framework::Strategy::_duplicate(strategy);
  this->strategy_impl_ = dynamic_cast <TAO::CSD::Strategy_Base*> (strategy);

  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
