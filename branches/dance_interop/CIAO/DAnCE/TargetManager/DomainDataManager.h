// $Id$
//===============================================================
/**
 * @file DomainDataManager.h
 *
 * @brief Maintains the Domain Information
 *
 * It contains the entire Domain information.Both the
 * initial domain as well as the current available domain.
 *
 * @author Nilabja Roy nilabjar@dre.vanderbilt.edu
 */
//===============================================================
#ifndef DOMAIN_DATA_MGRH
#define DOMAIN_DATA_MGRH

#include "TargetManagerC.h"
#include "Deployment_Configuration.h"

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
         * @operation updateDomain
         * @brief     This function is called by the other classes to update
         *            current domain data.
         * @param     elements The string sequence of elements
         *            being updated
         * @param     domainSubset The subset of the actual Domain to be updated
         * @param     updateKind Specifies the update type eg. add, delete, update
         *
         */
        int update_domain (const ::CORBA::StringSeq & elements,
                           const ::Deployment::Domain & domainSubset,
                           ::Deployment::DomainUpdateKind updateKind
                           );
      /**
       * @operation    getInitialDomain
       * @brief        This function is called the Executor code
       *               to get the Original Domain data.
       * @return       Domain* The Initial Domain
       *
       */
      ::Deployment::Domain* get_initial_domain ();

      /**
       * @operation   getCurrentDomain
       * @brief       This function is called the Executor code
       *              to get the Current Domain data.
       * @return      Domain* The Current Domain
       */
      ::Deployment::Domain* get_current_domain ();


      /**
       * @operation create
       * @brief     This function is called to create the Datamanager
       * @param orb The orb pointer
       * @param target The Target Manager Object Reference
       *
       * @description This function calls the constructor of the
       *              class Domain Data Manager
       */
      static DomainDataManager * create (CORBA::ORB_ptr orb,
                                  ::Deployment::TargetManager_ptr target
                                  );

      /**
       * @operation get_data_manager
       * @brief Returns the static pointer to the
       *        data manager.
       * @return DomainDataManager*
       * @description The staic get_data_manger function returning
       *              the data_manager pointer
       */
      static DomainDataManager* get_data_manager ();

      /**
       * @operation delete_data_manager
       * @brief deletes the data manager
       */

      static void delete_data_manger ();

      /**
       * @operation get_node_managers
       * @brief returns the sequence of node managers
       * object reference
       */

      CIAO::Host_NodeManager_seq *
        get_node_managers ();

      /**
       * @operation get_cpu_info
       * @brief returns the node specific cpu utilization
       *
       * @return CIAO::Host_Infos*
       */

       CIAO::Host_Infos* get_cpu_info ();

       /**
        * @operation get_pid
        * @brief returns the pid of the component id submitted
        * @param cmp The component id
        *
        * @return process id
        */
       CORBA::Long get_pid (ACE_CString cmp);

       /**
        * @operation commitResources
        * @brief commits the resources that are specified
        * in the plan.
        * @param plan ::Deployment::DeploymentPlan
        * @exception  ::Deployment::ResourceNotAvailable thrown
        *             when the resources mentioned in the plan exceeds
        *             the current resource.
        * @exception  ::Deployment::PlanError thrown if the plan has any
        *              error
        *
        */
       void commitResources (
       const ::Deployment::DeploymentPlan & plan);

       /**
        * @operation releaseResources
        * @brief The function releases the resources held by a plan
        *
        * @param argname ::Deployment::DeploymentPlan the plan whose
        *                resources are to be released
        */
       void releaseResources (
       const ::Deployment::DeploymentPlan& plan);

       /**
        * @operation stop_monitors
        * @brief The function makes a call on the leaveDomain on the
        *        NodeManager
        *
        * @description The node manager in turn stops the monitor
        *
        */

       void stop_monitors ();

    protected:


      /**
       * @operation   Constructor
       * @param orb The orb pointer
       * @param target The Target Manager Object Reference
       * @description The constructor made protected so that no one can create
       *              it.
       */
      DomainDataManager (CORBA::ORB_ptr orb,
                         ::Deployment::TargetManager_ptr target
                         );

      /**
       * @operation readinDomainData
       * @brief   It will read the initial Domain data from
       *          XML files.
       */
      int readin_domain_data ();

      /**
       * @operation match_requirement_resource
       * @brief Match the deployed resources to the
       * available resource
       */
      void match_requirement_resource (
         ::Deployment::InstanceResourceDeploymentDescriptions deployed,
         ::Deployment::Resources& available
         );

      /**
       * @operation match_properties
       * @brief Match the properties of a Requirement to the
       * properties of available resource
       * @param deployed The deployed Properties
       * @param available The available Properties
       */

      void match_properties (
                        ::Deployment::Properties deployed,
                        ::Deployment::SatisfierProperties& available);


        /// The different actiona that can take place
        enum  Action {commit , release};

      /**
       * @operation commit_release_resource
       * @brief Either commits or releases the given resource
       * based on the current Action set.
       * @param deployed ::Deployment::Property is the resource
       * to be commited/released
       * @param available ::Deployment::SatisfierProperty is the
       * available resource from which committed/released.
       * @exception  ::Deployment::ResourceNotAvailable thrown
       *             when the deployed resources exceeds
       *             the available resource.
       */

        void commit_release_resource (  ::Deployment::Property & deployed,
                          ::Deployment::SatisfierProperty & available);

      /**
       * @operation call_all_node_managers
       * @brief This function calls all NM and gives them
       * the sub-domain
       */
      int call_all_node_managers ();

      /// The ORB pointer
      CORBA::ORB_var orb_;

      /// The Deployment Configuration
      CIAO::Deployment_Configuration deployment_config_;

      /// The Initial Domain - contains resources
      /// at total capacity
      ::Deployment::Domain initial_domain_;


      /// The Current Domain - contains resources
      /// at current capacity
      ::Deployment::Domain current_domain_;

      /// The Target Manager Context
      ::Deployment::TargetManager_var target_mgr_;

      /**
       * The staic data manager pointer implementing
       * singleton pattern
       */
      static DomainDataManager* global_data_manager_;

      /**
       * The static provisioned Domain data
       */
      ::Deployment::Domain provisioned_data_;

      ///The current action
      Action current_action_;
    };

} // CIAO

#endif /* DOMAIN_DATA_MGRH */
