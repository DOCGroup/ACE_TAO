// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Custom_Request.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_CUSTOM_REQUEST_H
#define TAO_CSD_TP_CUSTOM_REQUEST_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CSD_ThreadPool/CSD_TP_Request.h"
#include "tao/CSD_ThreadPool/CSD_TP_Custom_Request_Operation.h"
#include "tao/PortableServer/Servant_Base.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    class TP_Custom_Request;
    typedef TAO_Intrusive_Ref_Count_Handle<TP_Custom_Request>
                                             TP_Custom_Request_Handle;

    /**
     * @class TP_Custom_Request
     *
     * @brief Base class for "queue-able" Custom (non-CORBA) requests.
     *
     * TBD - Add description
     *
     */
    class TAO_CSD_TP_Export TP_Custom_Request : public TP_Request
    {
    public:

      /// Virtual Destructor.
      virtual ~TP_Custom_Request();


    protected:

      /// Constructor.
      TP_Custom_Request(TP_Custom_Request_Operation* op,
                        TP_Servant_State*            servant_state);

      void execute_op();
      void cancel_op();


    private:

      TP_Custom_Request_Operation_Handle op_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Custom_Request.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_CUSTOM_REQUEST_H */
