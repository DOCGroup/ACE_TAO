// -*- C++ -*-

//=============================================================================
/**
 * @file Monitor_Point_Registry.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITOR_POINT_REGISTRY_H
#define MONITOR_POINT_REGISTRY_H

#include /**/ "ace/pre.h"

#include "ace/Synch_Traits.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (ACE_ENABLE_MONITORS)

#include "ace/Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/MonitorControl_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    class Monitor_Base;

    /**
     * @class Monitor_Point_Registry
     *
     * @brief Storage for instantiated monitor points.
     *
     */
    class ACE_Export Monitor_Point_Registry
    {
    public:
      /// Used to help ensure that there is only a single instance
      /// per process of Monitor_Point_Registry.
      static Monitor_Point_Registry* instance (void);
      
      Monitor_Point_Registry (void);

      /// Adds a Statistic or ControlAction to its corresponding registry.
      bool add (Monitor_Base* type);
      
      /// Removes a Statistic or ControlAction from its registry.
      bool remove (const char* name);

      /// Returns a list of names stored in the registry
      MonitorControl_Types::NameList names (void);

      /// The lookup operation.
      Monitor_Base* get (const ACE_CString& name) const;
      
      /// Returns a unique id for a constraint when it is created.
      long constraint_id (void);
      
      /// Needed since ACE_Singleton doesn't call the destructor.
      void cleanup (void);

    private:
      /// Underlying container for the registry.
      typedef ACE_Hash_Map_Manager<ACE_CString,
                                   Monitor_Base*,
                                   ACE_SYNCH_NULL_MUTEX> Map;

      mutable ACE_SYNCH_MUTEX mutex_;
      Map map_;
      
      /// Since we're accessed as a singleton, we can keep track of
      /// dispensing unique ids for constraints.
      long constraint_id_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#endif /* ACE_ENABLE_MONITORS */

#include /**/ "ace/post.h"

#endif // MONITOR_POINT_REGISTRY_H
