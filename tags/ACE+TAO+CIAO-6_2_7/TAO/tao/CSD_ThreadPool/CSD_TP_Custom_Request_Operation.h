// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Custom_Request_Operation.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_CUSTOM_REQUEST_OPERATION_H
#define TAO_CSD_TP_CUSTOM_REQUEST_OPERATION_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Intrusive_Ref_Count_Base_T.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include "tao/PortableServer/Servant_Base.h"
#include "ace/Synch.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Custom_Request_Operation;
    typedef TAO_Intrusive_Ref_Count_Handle<TP_Custom_Request_Operation>
                                          TP_Custom_Request_Operation_Handle;


    /**
     * @class TP_Custom_Request_Operation
     *
     * @brief Base class for all custom request operations.
     *
     * @note  The caller that creates a new TP_Custom_Request_Operation
     *        object needs call _add_ref () on the servant before
     *        constructing it and the TP_Custom_Request_Operation object
     *        is responsible to decrement the reference count.
     *
     * TBD - Add description
     */
    class TAO_CSD_TP_Export TP_Custom_Request_Operation
                      : public TAO_Intrusive_Ref_Count_Base<TAO_SYNCH_MUTEX>
    {
    public:

      /// Virtual Destructor.
      virtual ~TP_Custom_Request_Operation();

      /// Invoked by a worker thread to perform the operation.
      void execute();

      /// Invoked when the request has been cancelled.
      void cancel();

      /// Used by the TP_Strategy to obtain the target servant in order
      /// to construct the custom request object.  Returns the servant as
      /// an "in" argument (the caller does not get a new 'copy').  This
      /// is useful for chaining.
      PortableServer::Servant servant();


    protected:

      /// Constructor.
      TP_Custom_Request_Operation(PortableServer::Servant servant);

      virtual void execute_i() = 0;
      virtual void cancel_i() = 0;


    private:

      PortableServer::ServantBase_var servant_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Custom_Request_Operation.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_CUSTOM_REQUEST_OPERATION_H */
