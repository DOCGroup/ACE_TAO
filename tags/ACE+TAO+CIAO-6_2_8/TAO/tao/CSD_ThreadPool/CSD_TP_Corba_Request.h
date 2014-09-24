// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Corba_Request.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_CORBA_REQUEST_H
#define TAO_CSD_TP_CORBA_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Request.h"
#include "tao/CSD_Framework/CSD_FW_Server_Request_Wrapper.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ace/SString.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Corba_Request;
    typedef TAO_Intrusive_Ref_Count_Handle<TP_Corba_Request>
                                             TP_Corba_Request_Handle;

    /**
     * @class TP_Corba_Request
     *
     * @brief Base class for "queue-able" CORBA requests.
     *
     * TBD - Add description
     *
     */
    class TAO_CSD_TP_Export TP_Corba_Request : public TP_Request
    {
    public:

      /// Virtual Destructor.
      virtual ~TP_Corba_Request();


    protected:

      /// Constructor.
      TP_Corba_Request(const PortableServer::ObjectId& object_id,
                       PortableServer::POA_ptr         poa,
                       const char*                     operation,
                       PortableServer::Servant         servant,
                       TP_Servant_State*               servant_state,
                       TAO_ServerRequest&              server_request);

      /// Delegate to the FW_Server_Request_Wrapper clone() method.
      void do_clone();

      /// Delegate to the FW_Server_Request_Wrapper dispatch() method.
      void do_dispatch(void);

      /// Delegate to the FW_Server_Request_Wrapper cancel() method.
      void do_cancel();

    private:
      /// The ObjectId for the target servant.
      PortableServer::ObjectId object_id_;

      /// The POA.
      PortableServer::POA_var poa_;

      /// The name of the IDL operation.
      ACE_CString operation_;

      /// The TAO_ServerRequest object wrapper.
      FW_Server_Request_Wrapper server_request_;
    };
  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Corba_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_CORBA_REQUEST_H */
