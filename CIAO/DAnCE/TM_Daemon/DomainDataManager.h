// $Id$
//===============================================================
/**
 * @file DomainDataManager.h
 *
 * @brief Maintains the Domain Information
 *
 * It contains the entire Domain information. Both the
 * initial domain as well as the current available domain.
 *
 * @author Nilabja Roy nilabjar@dre.vanderbilt.edu
 */
//===============================================================
#ifndef DOMAIN_DATA_MGRH
#define DOMAIN_DATA_MGRH

#include "DAnCE/DomainApplicationManager/Deployment_Configuration.h"
//#include "DAnCE/Deployment/Deployment_ResourceCommitmentManagerC.h"

#include "NM_MonitorC.h"

#include <vector>
#include <map>
#include <string>
#include "Profile_Code.h"
#include "ace/Mutex.h"

/**
 * @namespace CIAO
 *
 * @brief The main CIAO namespace
 *
 */
namespace CIAO
{
  /**
   * @class DomainDataManager
   *
   * @brief Responsible for maintaining the Domain Information
   *
   * It maintains both the Current Domain Information as well
   * as the Initial domain at full capacity.
   */
  class DomainDataManager
    {

      public :
        /**
         * @brief     This function is called by the other classes to update
         *            current domain data.
         * @param     elements The string sequence of elements
         *            being updated
         * @param     domainSubset The subset of the actual Domain to
         *            be updated
         * @param     updateKind Specifies the update type eg. add,
         *            delete, update
         *
         */
        int update_domain (const ::CORBA::StringSeq & elements,
                           const ::Deployment::Domain & domainSubset,
                           ::Deployment::DomainUpdateKind updateKind);
      /**
       * This function calls the constructor of the
       * class Domain Data Manager
       * @brief This function is called to create the Datamanager
       * @param orb The orb pointer
       * @param target The Target Manager Object Reference
       *
       */
      static DomainDataManager * create (CORBA::ORB_ptr orb,
                                  ::Deployment::TargetManager_ptr target,
				  const char* dat_file);

      /**
       * @brief Returns the static pointer to the
       *        data manager.
       * @return DomainDataManager*
       * The staic get_data_manger function returning
       *              the data_manager pointer
       */
      static DomainDataManager* get_data_manager ();

      /**
       * @brief deletes the data manager
       */
      static void delete_data_manger ();

      /**
       * @brief Extracts the Obj Refs from the NS
       */
      int get_monitor_obj_ref ();

      /**
       * @brief checks if all the responses have reached or not
       */
      bool got_all_response ();

      /**
       * @brief signals all the nodes to send in data
       */
      void get_all_data (Onl_Monitor::AMI_NM_MonitorHandler_ptr handler);

      /**
       * @brief tells all the node managers to start the monitoring of QoS
       */
      int start_monitor_qos (Onl_Monitor::AMI_NM_MonitorHandler_ptr handler,
                             ::Deployment::DeploymentPlan& plan);

      void stop_monitors ();


      /**
       * @brief Writes snapshot of the application
       */
      void write_snapshot ();

      private:

      /**
       * The constructor made protected so that no one can create
       * it.
       * @param orb The orb pointer
       * @param target The Target Manager Object Reference
       */
      DomainDataManager (CORBA::ORB_ptr orb,
                         ::Deployment::TargetManager_ptr target,
			 const char* dat_file
                         );

      /**
       * @brief   It will read the initial Domain data from
       *          XML files.
       */
      int readin_domain_data ();

      /**
       * @brief This function calls all NM and gives them
       * the sub-domain
       */
      int call_all_node_managers ();

      /**
       *  @brief updates the node status by reading it from a file
       */
      bool update_node_status ();

      /**
       *  @brief updates the snapshot of the system.
       */
      int update_dynamic (const ::Deployment::Domain &domainSubset);

      /// The ORB pointer
      CORBA::ORB_var orb_;

      /// The Deployment Configuration
      CIAO::Deployment_Configuration deployment_config_;

      /// The Initial Domain - contains resources
      /// at total capacity
      ::Deployment::Domain initial_domain_;

      /// The staic data member , replacing a global variable
      static DomainDataManager* global_data_manager_;

      /// The Current Domain - contains resources
      /// at current capacity
      ::Deployment::Domain current_domain_;

      /// The monitor obj references
      std::vector<Onl_Monitor::NM_Monitor_var> node_monitors_;

      /// The count of responses returned
      int response_count_;

      /// denotes if all responses are there or not
      bool got_all_response_;

      /// The profiler class to time code
      Profile_Code * profile_;

      /// Exception occured
      bool ex_occur;

     /// guard
     ACE_Thread_Mutex lock;

     /// profiles for each node to measure end-to-end for each node
     std::map<std::string, Profile_Code*> profile_nodes_;

      /// Map used to store "dynamic information" of each node.
     std::map<std::string, ::Deployment::Node> node_info_map_;

     /// the Data file which contains the Node Manager references
     std::string dat_file_;

      /// Thread mutex.
      ACE_Mutex mutex_;

    };
} // CIAO


#endif /* DOMAIN_DATA_MGRH */
