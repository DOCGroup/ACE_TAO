// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Custom_Synch_Request.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_CUSTOM_SYNCH_REQUEST_H
#define TAO_CSD_TP_CUSTOM_SYNCH_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Custom_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Synch_Helper.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Custom_Synch_Request;
    typedef TAO_Intrusive_Ref_Count_Handle<TP_Custom_Synch_Request>
                                             TP_Custom_Synch_Request_Handle;

    /**
     * @class TP_Custom_Synch_Request
     *
     * @brief Base class for "queue-able", Custom (non-CORBA),
     *        Synchronous requests.
     *
     * TBD - Add description
     *
     */
    class TAO_CSD_TP_Export TP_Custom_Synch_Request : public TP_Custom_Request
    {
    public:

      /// Constructor.
      TP_Custom_Synch_Request(TP_Custom_Request_Operation* op,
                              TP_Servant_State*            servant_state);

      /// Virtual Destructor.
      virtual ~TP_Custom_Synch_Request();

      /// Wait until the request has been executed (and completes), or
      /// until it has been cancelled.  Returns true if the request has
      /// been executed/completed, and returns false if the request has
      /// been cancelled.
      bool wait();


    protected:

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
# include "tao/CSD_ThreadPool/CSD_TP_Custom_Synch_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_CUSTOM_SYNCH_REQUEST_H */
