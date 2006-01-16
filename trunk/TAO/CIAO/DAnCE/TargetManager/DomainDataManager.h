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
     * @brief   : This function is called by the other classes to update
     *            current domain data.
     * @param     elements The string sequence of elements
     *            being updated
     * @param     domainsubset The subset of the actual Domain to be updated
     * @param     updatekind Specifies the update type eg. add, delete, update
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
   * @operation   Constructor
   * @param orb The orb pointer
   * @param target The Target Manager Object Reference
   */
  DomainDataManager (CORBA::ORB_ptr orb,
                     ::Deployment::TargetManager_ptr target
                     );

 protected:

  /**
   * @operation readinDomainData
   * @brief   It will read the initial Domain data from
   *          XML files.
   */
  int readin_domain_data ();

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
};

#endif /* DOMAIN_DATA_MGRH */
