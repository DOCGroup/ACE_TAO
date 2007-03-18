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
#include "DAnCE/Deployment/Deployment_TargetManagerC.h"

namespace CIAO
{

  class MonitorBase
    {
    public:
      virtual ~MonitorBase () {};

      /**
       * This function is called by the controller to initialize
       * parameters.
       * @param  domain The Initital domain for this host
       * @param  target_manager  TargetManager_ptr
       * @param  interval The time interval after which updates need to be send.
       */
      virtual int initialize_params (
                                     ::Deployment::Domain& domain,
                                     ::Deployment::TargetManager_ptr target_manager,
                                     int interval
                                     )=0;
      /**
       * This function is called by the controller
       *  to start up the monitor.
       * @param orb The ORB pointer
       */
      virtual int start (CORBA::ORB_ptr orb) = 0;
      /**
       * This function is called by the controller
       * to stop the monitor.
       */
      virtual int stop ()=0;
      /**
       * This function is called by the controller
       * to get the current data.
       * @return The current Domain data
       */
      virtual ::Deployment::Domain* get_current_data ()=0;
    };

  extern "C" ACE_Proper_Export_Flag CIAO::MonitorBase *
    createMonitor (void);

} // CIAO


#endif /* MONITOR_BASEH */
