// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_FW_Server_Request_Wrapper.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_FW_SERVER_REQUEST_WRAPPER_H
#define TAO_CSD_FW_SERVER_REQUEST_WRAPPER_H

#include /**/ "ace/pre.h"

#include "tao/CSD_Framework/CSD_FW_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Server_Request.h"
#include "tao/operation_details.h"
#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    /**
     * @class FW_Server_Request_Wrapper
     *
     * @brief Wrapper around a TAO_Server_Request that will be cloned
     *        at the proper time.
     *
     * This CSD Framework class is used to provide an interface to a
     * TAO_ServerRequest object such that it can be used for CSD strategies
     * that need to clone TAO_Server_Requests (ie, so that they may be
     * placed on a queue to be dispatched by another thread).
     *
     * There are several purposes for this class, with the main thing being
     * that the TAO_ServerRequest object is not used directly by the
     * CSD Strategies since it is, after all, an internal TAO class that was
     * never meant to be exposed.  Future changes to TAO's internal design
     * may cause disruption in the TAO_ServerRequest class.  These changes
     * would only need to be accounted for here in this
     * FW_Server_Request_Wrapper class' implementation, and all CSD
     * Strategies will work again.  It's a maintenance issue.
     *
     * @note: The CSD namespace is inside of TAO namespace in current
     *        implementation. This can be changed but, at least for now, it's
     *        already been delivered to some customs, we leave it as-is. If it
     *        needs to change, we will make this change.
     */
    class TAO_CSD_FW_Export FW_Server_Request_Wrapper
    {
    public:

      /// Constructor.
      FW_Server_Request_Wrapper(TAO_ServerRequest& server_request);

      /// Destructor.
      ~FW_Server_Request_Wrapper();

      /// Perform the clone operation.
      void clone();

      /// Dispatch the request to the servant.
      void dispatch(
        PortableServer::Servant servant,
        TAO::Portable_Server::Servant_Upcall *servant_upcall);

      /// Cancel the request.
      void cancel();

    private:

      /// Create a deep copy of the request_ object.
      /// The other clone methods are used to support the TAO_ServerRequest clone.
      TAO_ServerRequest* clone (TAO_ServerRequest*& from);

      /// Clone an input cdr stream.
      TAO_InputCDR* clone (TAO_InputCDR*& from);

      /// Create a deep copy of a TAO_Operation_Details object and marshall
      /// the arguments into a TAO_InputCDR stream. The cloned TAO_Operation_Details
      /// object is created without any arguments. This would help the skeleton
      /// code to determine whether the arguments are in the operation_details_
      /// object or should be demarshalled from the incoming_ stream in the request_
      /// object.
      bool clone (TAO_Operation_Details const *& from,
                  TAO_Operation_Details const *& to,
                  TAO_InputCDR* & cdr);

      /// Clone the TAO_Tagged_Profile object.
      void clone (TAO_Tagged_Profile& from, TAO_Tagged_Profile& to);

      /// Clone the TAO_Service_Context object.
      void clone (TAO_Service_Context& from, TAO_Service_Context& to);

      /// Create a TAO_OutputCDR stream initialized with a heap allocated
      /// buffer.
      TAO_OutputCDR* create_new_output_cdr ();

      /// A flag that indicates that the TAO_ServerRequest object
      /// is already cloned.
      bool is_clone_;

      /// Pointer to the underlying TAO_ServerRequest object.
      TAO_ServerRequest* request_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_Framework/CSD_FW_Server_Request_Wrapper.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_FW_SERVER_REQUEST_WRAPPER_H */
