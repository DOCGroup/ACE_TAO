// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Remote_Request.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_REMOTE_REQUEST_H
#define TAO_CSD_TP_REMOTE_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Corba_Request.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Remote_Request;
    typedef TAO_Intrusive_Ref_Count_Handle<TP_Remote_Request>
                                             TP_Remote_Request_Handle;

    /**
     * @class TP_Remote_Request
     *
     * @brief Represents a "queue-able", remote, CORBA request.
     *        Both syncronous and asynchronous remote CORBA requests
     *        are represented by the class.
     *
     * TBD - Go over the following comments and clean up.
     *
     * Since this class derives from the TP_Request class, it can be
     * added to a TP_Queue (ie, it is a "queueable" request).  It
     * represents a servant request that has been made by a remote
     * CORBA client (as opposed to a collocated CORBA client).  The
     * term "CORBA client" is being used here to distinguish CORBA
     * servant requests (those made thru a CORBA object reference), and
     * "Custom" servant requests that can be "dispatched" to the strategy
     * directly by the client application code (ie, not thru a CORBA
     * object reference).  Thus, there are "CORBA clients" and
     * "Direct clients".
     *
     * In summary, this class represents a servant request made when a
     * remote client invokes a method on a CORBA object reference.
     *
     */
    class TAO_CSD_TP_Export TP_Remote_Request : public TP_Corba_Request
    {
    public:

      /// Constructor.
      TP_Remote_Request(TAO_ServerRequest&              server_request,
                        const PortableServer::ObjectId& object_id,
                        PortableServer::POA_ptr         poa,
                        const char*                     operation,
                        PortableServer::Servant         servant,
                        TP_Servant_State*               servant_state);

      /// Virtual Destructor.
      virtual ~TP_Remote_Request();


    protected:

      /// Prepare this TP_Remote_Request object to be placed into the
      /// request queue.  This will cause the underlying TAO_ServerRequest
      /// object to be cloned.
      virtual void prepare_for_queue_i();

      /// Dispatch the request to the servant.
      virtual void dispatch_i();

      /// Cancel the request.
      virtual void cancel_i();
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Remote_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_REMOTE_REQUEST_H */
