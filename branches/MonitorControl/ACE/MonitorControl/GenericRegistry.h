// -*- C++ -*-

//=============================================================================
/**
 * @file GenericRegistry.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef GENERICREGISTRY_H
#define GENERICREGISTRY_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_Traits.h"
#include "ace/Null_Mutex.h"
#include "ace/Hash_Map_Manager_T.h"

#include "MonitorControl/Datatypes.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class GenericRegistry
     *
     * @brief Common base class for the statistic and control registries.
     *
     * Giving the Statistic and ControlAction classes a common,
     * albeit empty, base class enables us to factor most of
     * the registry operations into the class below. There will
     * be one global registry each for monitor points and
     * control actions.
     */
    class GenericRegistry
    {
    public:
      /**
       * @class MapError
       *
       * @brief Exception class, thrown by the add() operation
       * in class GenericRegistry.
       *
       * Used when items are added to the registry,
       * to give more info about an insertion error than we get
       * from the boolean return type.
       */
      class MapError
      {
      public:
        /// Info that the MapError exception might contain.
        enum ErrorReason
        {
          MAP_ERROR_BIND_FAILURE,
          MAP_ERROR_INVALID_VALUE
        };
        
        MapError (ErrorReason why);
        ~MapError (void);
      
        ErrorReason why_;
      };
      
      GenericRegistry (void);
      ~GenericRegistry (void);
      
      /// Adds a Statistic or ControlAction to its respecive registry.
      /// Throws MapError.
      bool add (Generic* type);
      
      /// Removes a Statistic or ControlAction specified by name.
      bool remove (const ACE_CString& name);
      
      /// Retrieves a list of the names added to the registry so far.
      const Datatypes::NameList& names (void) const;
      
    private:
      /// Underlying container for the registries.
      typedef ACE_Hash_Map_Manager<ACE_CString,
                                   Generic*,
                                   ACE_SYNCH_NULL_MUTEX> Map;
        
      mutable ACE_SYNCH_RW_MUTEX mutex_;
      Map map_;
      Datatypes::NameList name_cache_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // GENERICREGISTRY_H
