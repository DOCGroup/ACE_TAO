// $Id$
//==============================================================
/**
 * @file MonitorCB.h
 *
 * @brief The Monitor Callback Function
 *
 * This class updates data back to the Target Manager
 *
 * @author Nilabja Roy <nilabjar.vanderbilt.edu>
*/
//==============================================================

#ifndef MONITOR_CBH
#define MONITOR_CBH

#include "TargetManagerC.h"



/**
 * @namespace CIAO
 *
 * @brief The top level CIAO namespace
 *
 */

namespace CIAO
{
  /**
   * @class MonitorCB
   *
   * @brief Updates data back to the TM.
   *
   */

  class MonitorCB
    {
    public:
      /**
       *  @constructor
       *  @param _orb The ORB pointer
       *  @param The TargetManager reference
       *  @param interval The time interval to sent update
       */
      MonitorCB (CORBA::ORB_ptr _orb,Deployment::TargetManager_ptr,int interval);
      /**
       *  @function update_data
       *  @param  data Contains the updated Domain data
       *  @return int indicates success.
       *  @description This function is called by the monitor to
       *               update Domain data, which is then sent to
       *               TM.
       */
      int update_data (::Deployment::Domain& data);
    private:
      /// The ORB pointer
      CORBA::ORB_ptr orb_;

      /// The Target Manager pointer to send back the
      /// update
      ::Deployment::TargetManager_ptr target_mgr_;

      /// The interval after which updates need to be send
      int interval_;

    };

  /// The Callback function pointer
  typedef int (MonitorCB::*CallBack) (::Deployment::Domain &);

} // CIAO namespace

#endif /* MONITOR_CBH */
