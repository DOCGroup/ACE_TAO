// $Id$

//---------------------------------------------------------------------------------
/**
 *  @file MonitorController.h
 *
 *  @brief The Monitor Controller class.
 *
 *  This class is invoked by the Nodemanager object and it
 *  in turn loads the proper Monitor which is strategised.
 *
 *  @author Nilabja Roy
 */
//----------------------------------------------------------------------------------
#ifndef MONITOR_CONTROLLER_H
#define MONITOR_CONTROLLER_H

#include "NodeManager_svnt_export.h"
#include "DAnCE/Deployment/Deployment_BaseC.h"

#include "DAnCE/TargetManager/TargetManagerImplC.h"
#include "ace/Task.h"
#include "ace/Auto_Ptr.h"

#include "ace/Synch_Traits.h"
#include "ace/Synch.h"


/**
 * @namespace CIAO
 *
 * @brief The top level CIAO namespace
 *
 */

namespace CIAO
{

  class MonitorBase;

  class NodeManager_Impl_Base;

  /**
   *  @class MonitorController
   *
   *  @brief The Controller class
   *
   *  Acts as a facade to the Monitor
   *  module
   *
   */
  class NodeManager_svnt_Export MonitorController : public ACE_Task_Base
    {
    public:
      /**
       * @brief The main function containing the
       * entire activity.
       *
       * This function runs in the context of the
       * thread
       * @return int The status of the function
       */
      int svc (void);
      /**
       * @brief The terminate function
       *
       * This function is called to terminate the
       * thread
       */
      void terminate ();


      /// The Constructor.
      MonitorController (CORBA::ORB_ptr orb,
                         ::Deployment::Domain& domain,
                         ::Deployment::TargetManager_ptr target,
                         ::CIAO::NodeManager_Impl_Base* node_mgr
                         );

      ~MonitorController ();
    protected:

      /**
       * @brief returns the terminating flag
       * @return bool The terminating state of the thread
       */
      bool terminating ();

      /// The monitor object
      auto_ptr <MonitorBase>  monitor_;

      /// The TargetManagerImpl object
      CIAO::TargetManagerImpl_var target_impl_cmp_;

      /// The TargetManager Facet ....
      Deployment::TargetManager_var target_facet_i_;

      /// The terminate flag_
      bool terminate_flag_;

      /// Thread Mutex for synchronizing call
      TAO_SYNCH_MUTEX lock_;

      /// the ORB pointer ..
      /// @todo Make this a _var
      CORBA::ORB_ptr orb_;

      /// The initial domain
      ::Deployment::Domain initial_domain_;

      /// The Node Manager
      ::CIAO::NodeManager_Impl_Base* node_mgr_;

      /// Flag tells ; what to monitor
      bool monitor_cpu_usage_;

      /// To add component pid or not ..
      bool add_component_pid_;
    };

} // CIAO

#endif
