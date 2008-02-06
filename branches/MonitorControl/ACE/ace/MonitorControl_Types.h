// -*- C++ -*-

//=============================================================================
/**
 * @file MonitorControl_Types.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef MONITORCONTROL_TYPES_H
#define MONITORCONTROL_TYPES_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Vector_T.h"
#include "ace/SString.h"
#include "ace/Time_Value.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    struct ACE_Export MonitorControl_Types
    {
      /**
       * @brief An instance is contained by each enabled monitor point.
       */
      struct Data
      {
        ACE_Time_Value tv_;
        double number_;
      };
        
      /**
       * @brief Used by the admin class as a container for the data from
       *        a group of monitor points.
       */
      typedef ACE_Vector<Data> DataList;
      
      /**
       * @brief Used in various places to pass around a set of string names.
       */
      typedef ACE_Vector<ACE_CString> NameList;
       
      /**
       * @brief Used internally by a monitor point to tell which operations
       *        on its data are valid or invalid, and to give it some limited
       *        smarts about the semantics of its data.
       */
      enum InfoType
      {
        IT_COUNTER,
        IT_NUMBER,
        IT_TIME,
        IT_INTERVAL,
        IT_LIST,
        IT_DISABLED
      };
    };
  
    /**
     * @class MC_Generic
     *
     * @brief Base class of Monitor_Base and ControlAction.
     */
    /// Allows us to combine
    /// operations of the respective registries into a common base class.
    class ACE_Export MC_Generic
    {
    public:
      const char* name (void) const;
      
    protected:
      MC_Generic (void);
      MC_Generic (const char* name);
      ~MC_Generic (void);
      
    private:
      ACE_CString name_;
    };
  }
}    

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // MONITORCONTROL_TYPES_H
