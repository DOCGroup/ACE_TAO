// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Collocated_Asynch_Request.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_COLLOCATED_ASYNCH_REQUEST_H
#define TAO_CSD_TP_COLLOCATED_ASYNCH_REQUEST_H

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

    class TP_Collocated_Asynch_Request;
    typedef TAO_Intrusive_Ref_Count_Handle<TP_Collocated_Asynch_Request>
                                        TP_Collocated_Asynch_Request_Handle;

    /**
     * @class TP_Collocated_Asynch_Request
     *
     * @brief Represents a "queue-able", collocated, asynchronous,
     *        CORBA request.
     *
     * This kind request is one-way collocated request with the default
     * SYNC_SCOPE policy (SYNC_WITH_TRANSPORT) applied. It is cloned
     * before enqueuing and the "enqueuing" thread never blocks.
     */
    class TAO_CSD_TP_Export TP_Collocated_Asynch_Request
                                                 : public TP_Corba_Request
    {
    public:

      /// Constructor.
      TP_Collocated_Asynch_Request
                           (TAO_ServerRequest&              server_request,
                            const PortableServer::ObjectId& object_id,
                            PortableServer::POA_ptr         poa,
                            const char*                     operation,
                            PortableServer::Servant         servant,
                            TP_Servant_State*               servant_state);

      /// Virtual Destructor.
      virtual ~TP_Collocated_Asynch_Request();


    protected:

      /// Prepare this TP_Collocated_Asynch_Request object to be placed
      /// into the request queue.  This will cause the underlying
      /// TAO_ServerRequest object to be cloned.
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
# include "tao/CSD_ThreadPool/CSD_TP_Collocated_Asynch_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_COLLOCATED_ASYNCH_REQUEST_H */
