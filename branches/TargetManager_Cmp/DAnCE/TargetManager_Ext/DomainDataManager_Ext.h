
// $Id$
//===============================================================
/**
 * @file DomainDataManager_Ext.h
 *
 * @brief An extension to the DomainDataManager
 *
 * This class extends the functionality of the DomainDataManager
 * 
 * @author Nilabja Roy nilabjar@dre.vanderbilt.edu
 */
//===============================================================
#ifndef DOMAIN_DATA_MGR_EXTH
#define DOMAIN_DATA_MGR_EXTH

#include "TargetManagerExt_ImplC.h"
#include "DAnCE/DomainApplicationManager/Deployment_Configuration.h"
#include "DAnCE/TargetManager/DomainDataManager.h"

/**
 * @namespace CIAO
 *
 * @brief The main CIAO namespace
 *
 */
namespace CIAO
{
  /**
   * @class DomainDataManager_Ext
   *
   * @brief The Extension to the DomainDataManager
   *
   */
  class DomainDataManager_Ext : public DomainDataManager
    {

      public :
 
        DomainDataManager_Ext (CORBA::ORB_ptr orb,
            ::Deployment::TargetManager_ptr target);
        
      /**
       * @brief     This function is called to create the Datamanager
       * @param orb The orb pointer
       * @param target The Target Manager Object Reference
       *
       * This function calls the constructor of the
       *              class DomainDataManager_Ext
       */
      static DomainDataManager * create (CORBA::ORB_ptr orb,
                                  ::Deployment::TargetManager_ptr target
                                  );
      
      /**
       * @brief returns the sequence of node managers
       * object reference
       */
      CIAO::Host_NodeManager_seq *
        get_node_managers ();

      /**
       * @brief returns the node specific cpu utilization
       *
       * @return CIAO::Host_Infos*
       */

       CIAO::Host_Infos* get_cpu_info ();

       /**
        * @brief returns the pid of the component id submitted
        * @param cmp The component id
        *
        * @return process id
        */
       CORBA::Long get_pid (const ACE_CString& cmp);

       /**
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

    };
} // CIAO

#endif
