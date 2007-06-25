// MonitorController.h,v 1.9 2006/02/06 00:49:53 wotte Exp
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

#include "ace/Task.h"
#include "ace/Auto_Ptr.h"

#include "ace/Synch_Traits.h"
#include "ace/Synch.h"

#include "ace/Vector_T.h"
#include "NM_Monitor_Impl.h"

#include "NodeManager_Impl.h"

/**
 * @namespace CIAO
 *
 * @brief The top level CIAO namespace
 *
 */

namespace CIAO
{

  class MonitorBase;

  struct MonitorElement
  {
    ACE_DLL dll_;
    ACE_CString lib_name_;
    MonitorBase* monitor_;
  };

  /// forward declarations ..

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
       * @function svc.
       * @brief The main function containing the
       * entire activity.
       *
       * This function runs in the context of the
       * thread
       * @return int The status of the function
       */
      int svc (void);
      /**
       * @function terminate.
       * @brief The terminate function
       *
       * This function is called to terminate the
       * thread
       */
      void terminate ();


      /// The Constructor.
      MonitorController (CORBA::ORB_ptr orb,
                         const ::Deployment::Domain& domain,
                         ::CIAO::NodeManager_Impl_Base* node_mgr
                         );

      ~MonitorController ();


      /** @function get_component_process
       *
       *  @brief returns the component to process if mapping
       */
      NodeManager_Impl_Base::Component_Ids
        get_component_process ();

      /** @function init
       *
       *  @brief  Initiates the class with all the monitors
       */
      int init ();

      /** @function update_data_for_TM
       *
       *  @brief  Updates data to the TM
       */
      ::Deployment::Domain* update_data_for_TM ();

      /** @function get_this_domain
       *
       * @brief returns the initial domain
       */
      auto_ptr<Deployment::Domain> get_initial_domain ();

    protected:

      /**
       * @function terminating.
       * @brief returns the terminating flag
       * @return bool The terminting state of the thread
       */
      bool terminating ();

      /**
       * @function parse_initial_domain
       *
       * @brief It parses the initial domain
       * and sets the options
       *
       */
      void parse_initial_domain ();


      /**
       * @function populate_monitor_list
       *
       * @brief populates the monitor list vector
       */

      void populate_monitor_list ();

      /**
       * @function create_monitor_domain
       *
       * @brief Extracts the domain element specific
       *        to the monitor, so that it can be send
       *        to the monitor.
       * @param const char* resource The resource name
       *        mentioned in the Domain.cdd file
       *
       */
      ::Deployment::Domain create_monitor_domain
          (const char* resource);

      /**
       * @function add_resource_to_domain
       *
       * @brief Adds new resources to the current domain
       *
       * @param new_domain The main domain to be return to TM
       *        resource The new resource to be added
       *
       */
      void add_resource_to_domain (::Deployment::Domain& new_domain,
                              ::Deployment::Domain monitored_domain);


      /**
       * @function create_the_servant
       *
       * @brief Creates the servant object and the obj ref
       */
      void create_the_servant ();

      /**
       * @function upload_obj_ref
       *
       * @brief Uploads the obj ref to the NameService
       */
      void upload_obj_ref ();
      /// The vector of pointers to monitor objects
      ACE_Vector <MonitorElement*> monitor_list_;

      /// The monitor object
      auto_ptr <MonitorBase>  monitor_;


      /// The terminate flag_
      bool terminate_flag_;

      //Thread Mutex for synchronizing call
      ACE_SYNCH_MUTEX lock_;

      // the ORB pointer ..
      CORBA::ORB_ptr orb_;

      /// The Poa var
      PortableServer::POA_var poa_;

      /// The initial domain
      ::Deployment::Domain initial_domain_;

      /// The Node Manager
      ::CIAO::NodeManager_Impl_Base* node_mgr_;

      /// flag tells ; what to monitor
      bool monitor_cpu_usage_;

      /// flag tells ; what to monitor
      bool monitor_NA_usage_;

      /// TO add component pid or not ..
      bool add_component_pid_;

      /// The Monitor object servant ...
      Onl_Monitor_NM_Monitor_i* monitor_i_;

      /// The monitor var
      Onl_Monitor::NM_Monitor_var monitorv_;
    };

} // CIAO

#endif
