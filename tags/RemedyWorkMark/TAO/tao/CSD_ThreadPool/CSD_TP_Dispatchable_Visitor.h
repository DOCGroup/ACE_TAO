// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Dispatchable_Visitor.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_DISPATCHABLE_VISITOR_H
#define TAO_CSD_TP_DISPATCHABLE_VISITOR_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Queue_Visitor.h"
#include "tao/CSD_ThreadPool/CSD_TP_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    /**
     * @class TP_Dispatchable_Visitor
     *
     * @brief Used to extract the first "dispatchable" request from the queue.
     *
     * An instance of this visitor class is used by one of the worker
     * threads to locate the first "dispatchable" request in the queue.  If
     * such a request is visited, then this visitor will save a "copy" of
     * the request, indicate that the request should be removed from the
     * queue, and indicate that visitation should stop.
     *
     * An method is provided to retrieve a "copy" of the "dispatchable"
     * request that was saved off during visitation.  A nil reference
     * (ie, a NULL pointer) will be returned if no dispatchable request
     * was found.
     *
     */
    class TAO_CSD_TP_Export TP_Dispatchable_Visitor : public TP_Queue_Visitor
    {
    public:

      /// Default Constructor.
      TP_Dispatchable_Visitor();

      /// Virtual Destructor.
      virtual ~TP_Dispatchable_Visitor();

      /// Reset this visitor object in order to re-use it for another
      /// visitation of the request queue.  This sets the vistor's "result"
      /// (the TP_Request* data member) to its default value (a nil handle).
      void reset();

      /// Returns true to continue visitation.  Returns false to stop
      /// visitation.  Sets the remove_flag to true if the request should
      /// be removed from the queue as a result of the visit.  Leaves the
      /// remove_flag alone otherwise.
      virtual bool visit_request(TP_Request* request, bool& remove_flag);

      /// This returns a "copy" of the located request, or 0 if no request
      /// was located.
      TP_Request* request();


    private:

      /// A handle to the located request.
      TP_Request_Handle request_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Dispatchable_Visitor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_DISPATCHABLE_VISITOR_H */
