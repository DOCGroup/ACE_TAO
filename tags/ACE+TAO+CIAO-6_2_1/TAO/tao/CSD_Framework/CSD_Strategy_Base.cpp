// -*- C++ -*-
// $Id$

#include "tao/CSD_Framework/CSD_Strategy_Base.h"
#include "tao/CSD_Framework/CSD_POA.h"
#include "tao/CSD_Framework/CSD_Strategy_Proxy.h"
#include "tao/PortableServer/Root_POA.h"
#include "tao/PortableServer/POAManager.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/TAO_Server_Request.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_Framework/CSD_Strategy_Base.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::Strategy_Base::~Strategy_Base()
{
}

CORBA::Boolean
TAO::CSD::Strategy_Base::apply_to (PortableServer::POA_ptr poa)
{
  if (CORBA::is_nil(poa))
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR((LM_ERROR,
                   ACE_TEXT("(%P|%t) CSD Strategy cannot ")
                   ACE_TEXT("be applied to a nil POA.\n")));
      return false;
    }

  if (!CORBA::is_nil(this->poa_.in()))
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR((LM_ERROR,
                   ACE_TEXT("(%P|%t) CSD Strategy already ")
                   ACE_TEXT("applied to a POA.\n")));
      return false;
    }

  // The POA is a local interface (IDL terminology), and thus we know that
  // we can downcast the POA_ptr to its (TAO) implementation type.
  TAO_CSD_POA* poa_impl = dynamic_cast<TAO_CSD_POA*>(poa);

  if (poa_impl == 0)
    {
      if (TAO_debug_level > 0)
        TAOLIB_ERROR((LM_ERROR,
                   ACE_TEXT("(%P|%t) CSD Strategy cannot ")
                   ACE_TEXT("be applied to a non CSD POA.\n")));
      return false;
    }

  // We need to check to see if the POA is already "active".  If this is
  // the case, then we need to handle the poa_activated_event() right now.
  // If the POA is not already "active", then we can just wait until it
  // does get activated, and we (the strategy) will be informed of the
  // poa_activated_event() at that time.
  if (poa_impl->tao_poa_manager().get_state() ==
                                       PortableServer::POAManager::ACTIVE)
    {
      // The POA is already "active" (since its POAManager is active).
      // We need to "raise" the poa_activated_event() now.  Otherwise,
      // the event will be raised when the POAManager does become active.
      if (!this->poa_activated_event( poa_impl->orb_core() ))
        {
          // An error has been already been reported to the log with
          // the detailed reason for the failure to handle the event.
          return false;
        }
    }

  // Set the CSD Strategy_Base on the strategy proxy object owned by the POA.
  bool strategy_set = false;
  try
    {
      poa_impl->set_csd_strategy (this);
      strategy_set = true;
    }
  catch (const ::CORBA::Exception&)
    {
    }

  if (! strategy_set)
    {
      // We need to make sure that we raise a poa_deactivated_event() if
      // we earlier raised a poa_activated_event().
      this->poa_deactivated_event();

      // An error has been already been reported to the log with
      // the detailed reason why the proxy will not accept the
      // custom strategy.
      return false;
    }

  // Save a duplicate of the poa into our data member.
  this->poa_ = PortableServer::POA::_duplicate (poa);

  // Success
  return true;
}


void
TAO::CSD::Strategy_Base::servant_activated_event_i
                                (PortableServer::Servant ,
                                 const PortableServer::ObjectId&)
{
  // do nothing.
}


void
TAO::CSD::Strategy_Base::servant_deactivated_event_i
                                (PortableServer::Servant,
                                 const PortableServer::ObjectId&)
{
  // do nothing.
}

TAO_END_VERSIONED_NAMESPACE_DECL
