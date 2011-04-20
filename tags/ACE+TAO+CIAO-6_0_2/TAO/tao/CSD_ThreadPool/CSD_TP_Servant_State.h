// -*- C++ -*-

//=============================================================================
/**
 *  @file    CSD_TP_Servant_State.h
 *
 *  $Id$
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_CSD_TP_SERVANT_STATE_H
#define TAO_CSD_TP_SERVANT_STATE_H

#include /**/ "ace/pre.h"

#include "tao/CSD_ThreadPool/CSD_TP_Export.h"
#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Intrusive_Ref_Count_Base_T.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"
#include "ace/Synch.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  namespace CSD
  {

    /**
     * @class TP_Servant_State
     *
     * @brief Maintains state information for a particular servant.
     *
     * This TP_Servant_State class is an intrusively reference-counted
     * class.  This allows it to be held in a "smart pointer" (aka, handle)
     * object that will manage the reference-counting automagically.
     *
     * One TP_Servant_State object is created for each servant object for
     * which a request is to be dispatched.  The servant state objects are
     * held (via smart pointers) in a TP_Servant_State_Map object.  In turn,
     * the TP_Servant_State_Map object is a data member of the TP_Stategy
     * class.  Each request placed on to the request queue will hold a
     * reference (via a smart pointer) to the servant state object.
     *
     * Currently, the only "state" info held in this TP_Servant_State class
     * is the servant's busy flag.
     *
     */
    class TAO_CSD_TP_Export TP_Servant_State
                      : public TAO_Intrusive_Ref_Count_Base<TAO_SYNCH_MUTEX>
    {
    public:

      /// Handle Type (aka, Smart Pointer Type).
      typedef TAO_Intrusive_Ref_Count_Handle<TP_Servant_State> HandleType;

      /// Default Constructor.
      TP_Servant_State();

      /// Virtual Destructor.
      virtual ~TP_Servant_State();

      /// Accessor for the servant busy flag.
      bool busy_flag() const;

      /// Mutator for the servant busy flag.
      void busy_flag(bool new_value);

    private:

      /// The servant's current "busy" state (true == busy, false == not busy)
      bool busy_flag_;
    };

  }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/CSD_ThreadPool/CSD_TP_Servant_State.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CSD_TP_SERVANT_STATE_H */
