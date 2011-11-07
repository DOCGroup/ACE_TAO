// $Id$

#include "tao/CSD_ThreadPool/CSD_TP_Cancel_Visitor.h"
#include "tao/CSD_ThreadPool/CSD_TP_Request.h"

#if !defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Cancel_Visitor.inl"
#endif /* ! __ACE_INLINE__ */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO::CSD::TP_Cancel_Visitor::~TP_Cancel_Visitor()
{
}


bool
TAO::CSD::TP_Cancel_Visitor::visit_request(TP_Request* request,
                                           bool&       remove_flag)
{
  // If our servant_ data member is in the 'nil' state, then
  // we are supposed to cancel *ALL* requests that we visit.
  //
  // Otherwise, if our servant_ data member is not in the 'nil' state,
  // we are supposed to cancel only requests that target our specific
  // servant_.

  if ((this->servant_.in() == 0) || (request->is_target(this->servant_.in())))
    {
      // Set the remove_flag to true so that this request is removed
      // (and released) from the queue when we finish our visit.
      remove_flag = true;

      // Cancel the request
      request->cancel();
    }

   // Since we are either cancelling requests to any servant or a
   // specific servant, always continue visitation.
   return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
