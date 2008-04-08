// -*- C++ -*-

//=============================================================================
/**
 * @file MC_Generic_Registry.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MC_GENERIC_REGISTRY_H
#define MC_GENERIC_REGISTRY_H

#include /**/ "ace/pre.h"

#include "ace/Synch_Traits.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Thread_Mutex.h"
#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/MonitorControl_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class MC_Generic_Registry
     *
     * @brief Common base class for the statistic and control registries.
     *
     * Giving the Statistic and ControlAction classes a common,
     * albeit empty, base class enables us to factor most of
     * the registry operations into the class below. There will
     * be one global registry each for monitor points and
     * control actions.
     */
    class ACE_Export MC_Generic_Registry
    {
    public:

      /// Adds a Statistic or ControlAction to its corresponding registry.
      bool add (MC_Generic* type);
      
      /// Removes a Statistic or ControlAction from its registry.
      bool remove (const char* name);

      /// Returns a list of names stored in the registry
      MonitorControl_Types::NameList names (void);

    protected:
      MC_Generic_Registry (void);
      virtual ~MC_Generic_Registry (void);

      /// Retrieves a monitor point or a control action, which is
      /// then downcast by the calling subclass.
      MC_Generic* getobj (const ACE_CString& name) const;

    private:
      /// Underlying container for the registries.
      typedef ACE_Hash_Map_Manager<ACE_CString,
                                   MC_Generic*,
                                   ACE_SYNCH_NULL_MUTEX> Map;

      mutable ACE_SYNCH_MUTEX mutex_;
      Map map_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MC_GENERIC_REGISTRY_H
