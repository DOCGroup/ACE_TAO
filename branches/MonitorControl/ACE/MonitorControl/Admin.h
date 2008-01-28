// -*- C++ -*-

//=============================================================================
/**
 * @file Admin.h
 *
 * $Id$
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef ADMIN_H
#define ADMIN_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Event_Handler.h"

#include "MonitorControl/ControlAction.h"
#include "MonitorControl/Statistic.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE
{
  namespace MonitorControl
  {
    /**
     * @class MonitorPointAutoUpdater
     *
     * @brief Automates periodic updating of monitor point classes.
     *
     * A single instance of this class is held by the Admin
     * class below. For a monitor point that must periodically
     * check what they are monitoring, the admin class registers
     * the monitor point with the ACE reactor. Each time the
     * interval times out, the reactor will call the 
     * handle_timeout() method below, and pass in the appropriate
     * monitor point. The handle_timeout() method will in turn
     * call the appropriate method on the monitor point to get
     * it to update its data.
     */
    struct MonitorPointAutoUpdater : ACE_Event_Handler
    {
      /// Override of ACE base class method.
      virtual int handle_timeout (const ACE_Time_Value& interval,
                                  const void* monitor_point);
    };
    
    /**
     * @class Admin
     *
     * @brief Accesses monitor points or groups and manages the registries.
     *
     * Responsible for adding and removing monitor points and control
     * actions to/form their respective global registries. If 
     * add_monitor_point() is called with a non-zero auto_update_msec
     * arg, the monitor point will get registered with the reactor, which
     * will prompt it to automatically update its data.
     */
    class MONITORCONTROL_Export Admin
    {
    public:
      /**
       * @struct InvalidName
       *
       * @brief Exception, thrown when an operation below taking a
       *        single name or list of names has a problem because of
       *        an invalid name.
       *
       */
      struct InvalidName
      {
        /// Constructor stores the list of names for which there was a
        /// problem in the operation where the exception was thrown.
        InvalidName (const Datatypes::NameList& names);
        
        ~InvalidName (void);
        
        /// Class member which holds the namelist passed to the constructor.
        Datatypes::NameList names_;
      };
      
      Admin (void);
      ~Admin (void);
      
      /// Add or remove monitor points from a global registry
      /// (see Registry.h). If the auto_update_msec arg is non-zero,
      /// the monitor point, the auto updater member, and the given time
      /// interval are passed to the reactor singleton's register_timeout()
      /// method.
      
      bool add_monitor_point (Statistic* monitor_point,
                              unsigned long auto_update_msec);
                              
      /// Same as for monitor points, except that a different registry
      /// is used, and there is no registration with the reactor. These
      /// are actions that may be triggered by a constraint or called
      /// from the application directly.
      
      bool add_control_action (ControlAction* control_action);
                              
      /// Resets the monitor points found in the registry by
      /// lookup of the passed list of names. Throws InvalidName
      void clear_statistics (const Datatypes::NameList& names);
      
      /// Called from the application to manually update the monitor points
      /// (or group) specified in <names>. The boolean parameter specifies
      /// whether or not some action should be triggered (constraint check,
      /// logging, or some other action). Throws InvalidName.
      void update_monitors (const Datatypes::NameList& names, bool notify);
      
      /// This mutator allows the application to create its own reactor
      /// and substitute it for the default reactor.
      void reactor (ACE_Reactor* new_reactor);
      ACE_Reactor* reactor (void) const;
      
    private:
      MonitorPointAutoUpdater auto_updater_;
      ACE_Reactor* reactor_;
      bool delete_reactor_;
    };
  }
}

ACE_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif // ADMIN_H
