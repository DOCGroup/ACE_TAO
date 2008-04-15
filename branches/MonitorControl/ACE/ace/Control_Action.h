// -*- C++ -*-

//=============================================================================
/**
 * @file Control_Action.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef CONTROL_ACTION_H
#define CONTROL_ACTION_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Refcountable.h"
#include "ace/Thread_Mutex.h"
#include "ace/Synch_Traits.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class ControlAction
     *
     * @brief Base class for control actions initiated by the
     * application or by constraint evaluation trigger.
     *
     * This class is extended by the application developer or
     * by the MC service itself to create a concrete class.
     */
    class ACE_Export Control_Action : private ACE_Refcountable
    {
    public:
      /// To be implemented by the concrete derived class.
      virtual void execute (const char* command = "") = 0;
      
      /// Refcounting methods.
      void add_ref (void);
      void remove_ref (void);
      int refcount (void) const;
      
    protected:
      Control_Action (void);
      virtual ~Control_Action (void);  
      
    protected:    
      mutable ACE_SYNCH_MUTEX mutex_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // CONTROL_ACTION_H
