// -*- C++ -*-
//
// $Id$

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::Strategy_Proxy::Strategy_Proxy()
  : strategy_impl_(0)
{
}

ACE_INLINE
TAO::CSD::Strategy_Proxy::~Strategy_Proxy()
{
  strategy_impl_ = 0; // don't delete it! The var will do it for us.
}

ACE_INLINE
void
TAO::CSD::Strategy_Proxy::dispatch_request
                                (TAO_ServerRequest& server_request,
                                 TAO::Portable_Server::Servant_Upcall& upcall)
{

  if (this->strategy_impl_ == 0)
    {
      // This is the "default" strategy implementation.
      upcall.servant()->_dispatch(server_request, &upcall);
    }
  else
    {
      // Delegate to the custom strategy object.
      this->strategy_impl_->dispatch_request(server_request, upcall);
    }
}

ACE_INLINE
bool
TAO::CSD::Strategy_Proxy::poa_activated_event(TAO_ORB_Core& orb_core)
{
  // Delegate to the custom strategy object (or return true if this proxy
  // is not holding a custom strategy).
  return (this->strategy_impl_ == 0) ? true
    : this->strategy_impl_->poa_activated_event(orb_core);
}


ACE_INLINE
void
TAO::CSD::Strategy_Proxy::poa_deactivated_event()
{
  // We only need to do something if this proxy holds a custom strategy.
  if (this->strategy_impl_)
    {
      // Delegate to the custom strategy object.
      this->strategy_impl_->poa_deactivated_event();
    }
}

ACE_INLINE
void
TAO::CSD::Strategy_Proxy::servant_activated_event
                                  (PortableServer::Servant servant,
                                   const PortableServer::ObjectId& oid)
{
  // We only need to do something if this proxy holds a custom strategy.
  if (this->strategy_impl_)
    {
      // Delegate to the custom strategy object.
      this->strategy_impl_->servant_activated_event(servant, oid);
    }
}

ACE_INLINE
void
TAO::CSD::Strategy_Proxy::servant_deactivated_event
                                  (PortableServer::Servant servant,
                                   const PortableServer::ObjectId& oid)
{
  // We only need to do something if this proxy holds a custom strategy.
  if (this->strategy_impl_)
    {
      // Delegate to the custom strategy object.
      this->strategy_impl_->servant_deactivated_event(servant, oid);
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
