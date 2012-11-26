// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Cancel_Visitor.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_CANCEL_VISITOR_H
#define TAO_CSD_TP_CANCEL_VISITOR_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"
#include "tao/PortableServer/Servant_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Queue_Visitor.h"


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {
    /**
     * @class TP_Cancel_Visitor
     *
     * @brief Used to extract/cancel request(s) from the queue.
     *
     * This visitor is used to cancel certain requests in the queue
     * as they are visited.
     *
     * Note that this is currently implemented to cancel *all*
     * requests in the queue, or requests that are targeted for a specific
     * servant. This could be extended in the future to perhaps
     * cancel all requests that have the same operation name, or something
     * else.
     *
     */
    class TAO_CSD_TP_Export TP_Cancel_Visitor : public TP_Queue_Visitor
    {
    public:

      /// Default Constructor - cancel *all* requests.
      TP_Cancel_Visitor();

      /// Constructor with provided servant - cancel requests that
      /// target the supplied servant.
      TP_Cancel_Visitor(PortableServer::Servant servant);

      /// Virtual Destructor.
      virtual ~TP_Cancel_Visitor();

      /// Returns true to continue visitation.  Returns false to stop
      /// visitation.  Sets the remove_flag to true if the request should
      /// be removed from the queue as a result of the visit.  Leaves the
      /// remove_flag alone otherwise.
      virtual bool visit_request(TP_Request* request, bool& remove_flag);

    private:

      /// Left as nil if we are to cancel all requests, or set to a specific
      /// servant if only requests targeting that servant should be cancelled.
      PortableServer::ServantBase_var servant_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Cancel_Visitor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_DISPATCHABLE_VISITOR_H */
