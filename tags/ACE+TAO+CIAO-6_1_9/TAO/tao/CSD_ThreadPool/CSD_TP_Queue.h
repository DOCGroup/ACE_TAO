// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Queue.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_QUEUE_H
#define TAO_CSD_TP_QUEUE_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"
#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Request;
    class TP_Queue_Visitor;

    /**
     * @class TP_Queue
     *
     * @brief Queue of servant requests that need to be dispatched.
     *
     * This is the queue of pending servant requests that is "owned"
     * by a TP_Strategy object.  When an ORB thread dispatches
     * a servant request to the strategy object, it will create the
     * appropriate (subclass of) TP_Request object to "wrap"
     * the servant request in a "queue-friendly" wrapper.  The ORB thread
     * will then place the TP_Request object on to the queue.  Note that
     * this scenario pertains to what are being called "remote requests".
     * There are other scenarios in which other types of requests can
     * get added to this queue.
     *
     * The strategy object will employ a set of worker threads that are
     * responsible for "servicing" the servant requests in the queue.
     *
     * Note: In the future, support will be added to allow the client
     *       application inject "custom" TP_Request objects into
     *       a TP_Strategy object, causing them to be placed in
     *       the queue.
     */
    class TAO_CSD_TP_Export TP_Queue
    {
    public:

      /// Default Constructor.
      TP_Queue();

      /// Destructor.
      ~TP_Queue();

      /// Place a request at the end of the queue.
      void put(TP_Request* request);

      /// Returns true if the queue is empty.  Returns false otherwise.
      bool is_empty() const;

      /// Visitors will visit each request in the queue, from front to back,
      /// and have the ability to stop visiting at any time (ie, before
      /// visiting every request).
      void accept_visitor(TP_Queue_Visitor& visitor);

    private:

      /// The request at the front of the queue.
      TP_Request* head_;

      /// The request at the end of the queue.
      TP_Request* tail_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Queue.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_QUEUE_H */
