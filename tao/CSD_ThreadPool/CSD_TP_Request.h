// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Request.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_REQUEST_H
#define TAO_CSD_TP_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Servant_State.h"
#include "tao/PortableServer/Servant_Base.h"
#include "tao/Intrusive_Ref_Count_Base_T.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Request;
    typedef TAO_Intrusive_Ref_Count_Handle<TP_Request> TP_Request_Handle;

    class TP_Queue;


    /**
     * @class TP_Request
     *
     * @brief Base class for "queue-able" requests.
     *
     * This class serves as the abstract base class for all types of
     * "servant requests" that can be inserted into a TP_Queue
     * object.
     */
    class TAO_CSD_TP_Export TP_Request
      : public TAO_Intrusive_Ref_Count_Base<TAO_SYNCH_MUTEX>
    {
    public:

      /// Virtual Destructor.
      virtual ~TP_Request();

      /// Prepare the request to be placed into the request queue.
      void prepare_for_queue();

      /// Invoked to dispatch the request to the servant.
      void dispatch();

      /// Invoked to cancel the request.
      void cancel();

      /// Is the target servant ready to accept a request?
      bool is_ready() const;

      /// Mark the target servant as being busy.
      void mark_as_busy();

      /// Mark the target servant as being ready (ie, not busy).
      void mark_as_ready();

      /// This method returns true if this request targets the supplied
      /// servant object.
      bool is_target(PortableServer::Servant servant);


    protected:

      /// Constructor.
      TP_Request(PortableServer::Servant servant,
                 TP_Servant_State*       servant_state);

      /// Accessor for the servant.  Does not return a new (ref counted)
      /// reference!  This is used for chaining.
      PortableServer::Servant servant();

      /// The subclass knows if it needs to do anything in preparation
      /// of being placed into the request queue.  The default implementation
      /// does nothing, so only subclasses that have something to do
      /// need to provide their own implementation.
      virtual void prepare_for_queue_i();

      /// The subclass knows how to carry out its own way of dispatching
      /// the request to the servant.
      virtual void dispatch_i() = 0;

      /// Ask the subclass to perform its duties to carry out the cancellation.
      virtual void cancel_i() = 0;


    private:

      /// The TP_Queue class is our friend since it needs access to
      /// the prev_ and next_ (private) data members.
      friend class TP_Queue;

      /// The previous TP_Request object (in the queue).
      TP_Request* prev_;

      /// The next TP_Request object (in the queue).
      TP_Request* next_;

      /// Reference to the servant object.
      PortableServer::ServantBase_var servant_;

      /// Reference to the servant "state" object (contains the busy flag).
      TP_Servant_State::HandleType servant_state_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_REQUEST_H */
