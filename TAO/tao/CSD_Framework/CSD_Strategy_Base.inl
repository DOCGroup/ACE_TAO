// -*- C++ -*-
//
// $Id$

#include "tao/debug.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_INLINE
TAO::CSD::Strategy_Base::Strategy_Base()
  : poa_activated_(false)
{
}

ACE_INLINE
void
TAO::CSD::Strategy_Base::dispatch_request
                         (TAO_ServerRequest& server_request,
                          TAO::Portable_Server::Servant_Upcall& upcall)
{
  DispatchResult result;

  if (server_request.collocated())
    {
      result = this->dispatch_collocated_request_i(server_request,
                                                   upcall.user_id(),
                                                   this->poa_.in(),
                                                   server_request.operation(),
                                                   upcall.servant());
    }
  else
    {
      result = this->dispatch_remote_request_i(server_request,
                                               upcall.user_id(),
                                               this->poa_.in(),
                                               server_request.operation(),
                                               upcall.servant());
    }

  switch (result)
    {
      case DISPATCH_HANDLED:
        // Do nothing.  Everything has been handled.
        break;

      case DISPATCH_REJECTED:
        if (server_request.collocated ())
          {
             CORBA::NO_IMPLEMENT ex;
             ex._raise ();
          }
        else
          {
            // Raise an appropriate SystemException if the request is expecting
            // a reply.
            if (!server_request.sync_with_server() &&
                server_request.response_expected() &&
                !server_request.deferred_reply())
              {
                CORBA::NO_IMPLEMENT ex;
                server_request.tao_send_reply_exception(ex);
              }
          }
        break;

      case DISPATCH_DEFERRED:
        // Perform the "default" dispatching strategy logic for this request
        // right now, using the current thread.
        upcall.servant()->_dispatch(server_request, &upcall);
        break;

      default:
        if (TAO_debug_level > 0)
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT("(%P|%t) Unknown result (%d) from call to ")
                     ACE_TEXT("dispatch_remote_request_i().\n"), result));
        // Since we do not know what to do here, just do the minimum, which
        // treats this case just like the DISPATCH_HANDLED case, for better
        // or worse.  Hitting this default case means a coding error.
        break;
    }
}


ACE_INLINE
bool
TAO::CSD::Strategy_Base::poa_activated_event(TAO_ORB_Core& orb_core)
{
  // Notify the subclass of the event, saving the result.
  this->poa_activated_ = this->poa_activated_event_i(orb_core);

  // Return the result
  return this->poa_activated_;
}

ACE_INLINE
void
TAO::CSD::Strategy_Base::poa_deactivated_event()
{
  if (this->poa_activated_)
    {
      this->poa_activated_ = false;

      // Notify the subclass of the event.
      this->poa_deactivated_event_i();

      // Reset the poa to nil to decrement the reference count.
      // This will break the circular dependency of the deletion
      // of the CSD POA.
      this->poa_ = 0;
    }
}

ACE_INLINE
void
TAO::CSD::Strategy_Base::servant_activated_event
                                (PortableServer::Servant servant,
                                 const PortableServer::ObjectId& oid)
{
  this->servant_activated_event_i(servant, oid);
}

ACE_INLINE
void
TAO::CSD::Strategy_Base::servant_deactivated_event
                                (PortableServer::Servant servant,
                                 const PortableServer::ObjectId& oid)
{
  this->servant_deactivated_event_i(servant, oid);
}

TAO_END_VERSIONED_NAMESPACE_DECL
