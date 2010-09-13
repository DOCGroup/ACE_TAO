// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Queue_Visitor.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_QUEUE_VISITOR_H
#define TAO_CSD_TP_QUEUE_VISITOR_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Request;


    /**
     * @class TP_Queue_Visitor
     *
     * @brief Base class for vistors of the elements in the TP_Queue.
     *
     * Provides a way to perform thread-safe iteration over the
     * TP_Request objects contained within a TP_Queue object.
     *
     * This also provides a means to encapsulate each distinct algorithm
     * within a distinct subclass of TP_Queue_Visitor.
     *
     */
    class TAO_CSD_TP_Export TP_Queue_Visitor
    {
    public:

      /// Virtual Destructor.
      virtual ~TP_Queue_Visitor();

      /// Returns true to continue visitation.  Return false to stop
      /// visitation.  Sets the remove_flag to true if the request should
      /// be removed from the queue as a result of the visit.  Leaves the
      /// remove_flag alone otherwise.
      virtual bool visit_request(TP_Request* request, bool& remove_flag) = 0;


    protected:

      /// Default Constructor.
      TP_Queue_Visitor();
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Queue_Visitor.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_QUEUE_VISITOR_H */
