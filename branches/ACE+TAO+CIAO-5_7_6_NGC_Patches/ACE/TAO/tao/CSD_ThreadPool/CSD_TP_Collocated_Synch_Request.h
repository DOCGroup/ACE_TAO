// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Collocated_Synch_Request.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_COLLOCATED_SYNCH_REQUEST_H
#define TAO_CSD_TP_COLLOCATED_SYNCH_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Corba_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Synch_Helper.h"
#include "tao/Exception.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Collocated_Synch_Request;
    typedef TAO_Intrusive_Ref_Count_Handle<TP_Collocated_Synch_Request>
                                          TP_Collocated_Synch_Request_Handle;

    /**
     * @class TP_Collocated_Synch_Request
     *
     * @brief Represents a "queue-able", synchronous, collocated,
     *        CORBA request.
     *
     * This kind request is the two-way or oneway(with SYNC_WITH_TARGET
     * policy applied) collocated request. It is NOT cloned before
     * enqueuing and the "enqueuing" thread will block until the request
     * is dispatched/handled or cancelled.
     */
    class TAO_CSD_TP_Export TP_Collocated_Synch_Request
                                                  : public TP_Corba_Request
    {
    public:

      /// Constructor.
      TP_Collocated_Synch_Request
                           (TAO_ServerRequest&              server_request,
                            const PortableServer::ObjectId& object_id,
                            PortableServer::POA_ptr         poa,
                            const char*                     operation,
                            PortableServer::Servant         servant,
                            TP_Servant_State*               servant_state);

      /// Virtual Destructor.
      virtual ~TP_Collocated_Synch_Request();

      /// Wait until the request has been dispatched (and completed), or
      /// until it has been cancelled.
      /// Returns true if the request has been dispatched, and returns
      /// false if the request has been cancelled.
      bool wait(void);


    protected:

      /// Note that we do not override our base class implementation of
      /// prepare_for_queue_i() (which does nothing), because we don't
      /// need to clone the Server Request object.

      /// Dispatch the request to the servant.
      virtual void dispatch_i();

      /// Cancel the request.
      virtual void cancel_i();


    private:

      /// Helper used to block and unblock the thread that invokes our
      /// wait() method.
      TP_Synch_Helper synch_helper_;

      /// Set to NULL initially, and will only be set thereafter if an
      /// exception is raised from the dispatch() call on the server_request_.
      CORBA::Exception* exception_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_COLLOCATED_SYNCH_REQUEST_H */
