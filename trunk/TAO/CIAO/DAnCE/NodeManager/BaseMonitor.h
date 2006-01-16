// $Id$
//----------------------------------------------------------------------------------
/// Monitor Interface
/*
 * @file Base_Monitor.h
 * @brief The Base_Monitor.h file.
 * This is an abstract class containing the interface to the Monitor plug-in
 */
//----------------------------------------------------------------------------------

#ifndef MONITOR_BASEH
#define MONITOR_BASEH
//#include "ace/config-all.h"
#include "tao/ORB.h"
#include "TargetManagerC.h"

class MonitorBase
{
  public:
  /** @function :- initialize_params
   *  @param   :- TargetManager_ptr
   *  @param  interval : The time interval after whic updates need to be send.
   *  Description :- This function is called by the controller to initialize
   *                 parameters.
   */
    virtual int initialize_params (
                                   ::Deployment::Domain& domain,
                                   ::Deployment::TargetManager_ptr target_manager,
                                   int interval
                                   )=0;
    virtual int start (CORBA::ORB_ptr)=0;
    virtual int stop ()=0;
    virtual ::Deployment::Domain* get_current_data ()=0;
};


extern "C" /*ACE_Proper_Export_Flag*/ MonitorBase *
createMonitor (void);

#endif /* MONITOR_BASEH */
