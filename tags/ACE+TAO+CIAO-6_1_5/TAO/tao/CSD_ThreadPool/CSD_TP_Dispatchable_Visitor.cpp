// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Dispatchable_Visitor.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Dispatchable_Visitor.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::TP_Dispatchable_Visitor::~TP_Dispatchable_Visitor()
{
}


bool
TAO::CSD::TP_Dispatchable_Visitor::visit_request(TP_Request* request,
                                                 bool&       remove_flag)
{
  // Ask the request object if the target servant is "ready" to accept
  // a request being dispatched to it.
  if (request->is_ready())
    {
      // Ok.  This request is a "dispatchable" request.  It is what we were
      // hoping to find.

      // Save a copy of the request in our handle data member.
      request->_add_ref();
      this->request_ = request;

      // Make sure that the queue will extract the request from the queue
      // upon our return.
      remove_flag = true;

      // Mark the target servant as being "busy".
      request->mark_as_busy();

      // Stop the visitation by returning false.
      return false;
    }

  // The target servant object of the request isn't ready, so the request
  // is not considered to be a "dispatchable" request.

  // Return true to visit the next request in the queue (if there is one).
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
