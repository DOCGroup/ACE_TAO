// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Synch_Helper.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_SYNCH_HELPER_H
#define TAO_CSD_TP_SYNCH_HELPER_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"
#include "tao/Condition.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    /**
     * @class TP_Synch_Helper
     *
     * @brief Helper class for synchronous requests to block the requesting
     *        thread until the appropriate time (when it will be un-blocked).
     *
     * TBD - Description here
     *
     */
    class TAO_CSD_TP_Export TP_Synch_Helper
    {
    public:

      /// Constructor.  Sets initial state to PENDING.
      TP_Synch_Helper();

      /// Destructor.
      ~TP_Synch_Helper();

      /// Returns true if the helper state is DISPATCHED, and false if
      /// the helper state is CANCELLED.  However, if the helper state
      /// is PENDING, then this method will block the calling thread
      /// until the state changes to something other than PENDING
      /// (ie, DISPATCHED or CANCELLED).
      bool wait_while_pending();

      /// Change the state of this helper to DISPATCHED, which will cause
      /// wait_while_pending() to unblock.
      void dispatched();

      /// Change the state of this helper to CANCELLED, which will cause
      /// wait_while_pending() to unblock.
      void cancelled();


    private:

      /// Enumeration Type for all possible states of this helper object.
      enum HelperState
      {
        PENDING,
        DISPATCHED,
        CANCELLED
      };

      /// Thread lock type
      typedef TAO_SYNCH_MUTEX LockType;

      /// Thread condition type
      typedef TAO_Condition<LockType> ConditionType;

      /// Lock used to protect the state and condition.
      LockType lock_;

      /// Used to denote the state of the request dispatching.
      HelperState state_;

      /// The condition used to block the calling thread until the
      /// state is something other than the PENDING state.
      ConditionType condition_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Synch_Helper.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_SYNCH_HELPER_H */
