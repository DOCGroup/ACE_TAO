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
 * @class MonitorCB
 *
 * @brief Updates data back the TM.
 *
 */

class MonitorCB
{
  public:
    /**
     *     Constructor .. accepts the ORB pointer as the parameter..
     */
    MonitorCB (CORBA::ORB_ptr _orb,Deployment::TargetManager_ptr,int interval);
    /**
     *   Function Name : UpdateData
     *  Param         : Domain - contains the updated Domain data
     *  Return Value  : int indicates success.
     *  Description   : This function is called by the monitor to
     *                  update Domain data, which is then sent to
     *                  TM.
     */
      int update_data (::Deployment::Domain& data);
  private:
    // The ORB pointer
    CORBA::ORB_ptr orb_;

    // The Target Manager pointer to send back the
    // update
    ::Deployment::TargetManager_ptr target_mgr_;

    // The interval after which updates need to be send
    int interval_;

};

typedef int (MonitorCB::*CallBack) (::Deployment::Domain &);

#endif /* MONITOR_CBH */
