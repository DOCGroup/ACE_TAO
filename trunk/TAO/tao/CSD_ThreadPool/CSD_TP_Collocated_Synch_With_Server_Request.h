// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Collocated_Synch_With_Server_Request.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_COLLOCATED_SYNCH_WITH_SERVER_REQUEST_H
#define TAO_CSD_TP_COLLOCATED_SYNCH_WITH_SERVER_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Corba_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Synch_Helper.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Collocated_Synch_With_Server_Request;
    typedef TAO_Intrusive_Ref_Count_Handle
            <TP_Collocated_Synch_With_Server_Request>
                              TP_Collocated_Synch_With_Server_Request_Handle;

    /**
     * @class TP_Collocated_Synch_With_Server_Request
     *
     * @brief Represents a "queue-able", one-way, collocated, CORBA
     *        request with a "Synch Scope" policy of SYNC_WITH_SERVER.
     *
     * This kind of request is one-way request with the SYNC_WITH_SERVER
     * policy applied. It is cloned before enqueuing and the "enqueuing"
     * thread will block until it is signalled by the TP_Task thread that
     * will happen just before the request is dispatched or the request
     * is cancelled.
     */
    class TAO_CSD_TP_Export TP_Collocated_Synch_With_Server_Request
                                                  : public TP_Corba_Request
    {
    public:

      /// Constructor.
      TP_Collocated_Synch_With_Server_Request
                           (TAO_ServerRequest&              server_request,
                            const PortableServer::ObjectId& object_id,
                            PortableServer::POA_ptr         poa,
                            const char*                     operation,
                            PortableServer::Servant         servant,
                            TP_Servant_State*               servant_state);

      /// Virtual Destructor.
      virtual ~TP_Collocated_Synch_With_Server_Request();

      /// Wait until the request has been dispatched (but not completed), or
      /// until it has been cancelled.  Note that this will wait until just
      /// *before* the request is dispatched by a worker thread.  This is
      /// different than the TP_Collocated_Synch_Request which waits until
      /// just *after* the request is dispatched by a worker thread.
      /// Returns true if the request has been dispatched, and returns
      /// false if the request has been cancelled.
      bool wait(void);


    protected:

      /// Prepare this TP_Collocated_Synch_With_Server_Request object to be
      /// placed into the request queue.  This will cause the underlying
      /// TAO_ServerRequest object to be cloned.
      virtual void prepare_for_queue_i();

      /// Dispatch the request to the servant.
      virtual void dispatch_i();

      /// Cancel the request.
      virtual void cancel_i();


    private:

      /// Helper used to block and unblock the thread that invokes our
      /// wait() method.
      TP_Synch_Helper synch_helper_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Collocated_Synch_With_Server_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_COLLOCATED_SYNCH_WITH_SERVER_REQUEST_H */
