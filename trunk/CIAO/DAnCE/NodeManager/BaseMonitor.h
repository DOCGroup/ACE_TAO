// $Id$
//----------------------------------------------------------------------------------
/// Monitor Interface
/*
 * @file BaseMonitor.h
 * @brief The BaseMonitor.h file.
 * This is an abstract class containing the interface to the Monitor plug-in
 *
 * @author Nilabja R <nilabjar@dre.vanderbilt.edu>
 */
//----------------------------------------------------------------------------------

#ifndef MONITOR_BASEH
#define MONITOR_BASEH
#include "tao/ORB.h"
#include "ciao/Deployment_TargetManagerC.h"

namespace CIAO
{

  class MonitorBase
    {
    public:
      virtual ~MonitorBase () {};

      /** @function initialize_params
       *  @param  domain The Initital domain for this host
       *  @param  target_manager  TargetManager_ptr
       *  @param  interval The time interval after whic updates need to be send.
       *  @description  This function is called by the controller to initialize
       *                parameters.
       */
      virtual int initialize_params (
                                     ::Deployment::Domain& domain,
                                     ::Deployment::TargetManager_ptr target_manager,
                                     int interval
                                     )=0;
      /** @function start
       *  @param  ORB_ptr The ORB pointer
       *  @description  This function is called by the controller
       *                to start up the monitor.
       */
      virtual int start (CORBA::ORB_ptr)=0;
      /** @function stop
       *  @description  This function is called by the controller
       *                to stop the monitor.
       */
      virtual int stop ()=0;
      /** @function get_current_data
       *  @return ::Deployment::Domain* The current Domain data
       *  @description  This function is called by the controller
       *                to get the current data.
       */
      virtual ::Deployment::Domain* get_current_data ()=0;
    };

  extern "C" ACE_Proper_Export_Flag CIAO::MonitorBase *
    createMonitor (void);

} // CIAO


#endif /* MONITOR_BASEH */
