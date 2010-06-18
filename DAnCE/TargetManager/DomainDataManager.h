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

#include "Deployment/Deployment_TargetDataC.h"
#include "Deployment/Deployment_TargetManagerC.h"

namespace DAnCE
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
     * @param     domainSubset The subset of the actual Domain to be updated
     * @param     updateKind Specifies the update type eg. add, delete, update
     *
     */
    int update_domain (const ::CORBA::StringSeq & elements,
                       const ::Deployment::Domain & domainSubset,
                       ::Deployment::DomainUpdateKind updateKind);
    /**
     * @brief        This function is called from the Executor code
     *               to get the Original Domain data.
     * @return       Domain* The Initial Domain
     */
    ::Deployment::Domain* get_initial_domain (void);

    /**
     * @brief       This function is called from the Executor code
     *              to get the Current Domain data.
     * @return      Domain* The Current Domain
     */
    ::Deployment::Domain* get_current_domain (void);

    /**
     * @brief returns the sequence of node managers
     * object reference
     */
    ::Deployment::ResourceCommitmentManager_ptr
    commitResources (const ::Deployment::ResourceAllocations & resources);

    /**
     * @brief The function releases the resources held by a plan
     */
    void releaseResources (const ::Deployment::ResourceCommitmentManager_ptr manager);

    /**
     * @brief The function allocates resources specified in the
     * parameter
     *
     * This function is for the ResourceCommitmentManager
     *
     */
    void commitResourceAllocation (
                                   const ::Deployment::ResourceAllocations & resources);

    /**
     * @brief The function releases resources specified in the
     * parameter
     *
     * This function is for the ResourceCommitmentManager
     *
     */
    void releaseResourceAllocation (
                                    const ::Deployment::ResourceAllocations & resources);

    /**
     * @brief Initializing DomainDataManager
     *
     * This function inits the DomainDataManager
     *
     */
    void init (CORBA::ORB_ptr orb,
               ::Deployment::TargetManager_ptr target,
               const ACE_TCHAR *domain_name);

  private:
    /// The different action that can take place
    enum  Action {commit , release};

    /**
     * @brief   It will read the initial Domain data from
     *          XML files.
     */
    int readin_domain_data (void);

    /**
     * @brief Match the deployed resources to the
     * available resource
     */
    void match_requirement_resource (
                                     ::Deployment::InstanceResourceDeploymentDescriptions deployed,
                                     ::Deployment::Resources& available);

    /**
     * @brief Match the properties of a Requirement to the
     * properties of available resource
     * @param deployed The deployed Properties
     * @param available The available Properties
     */
    void match_properties (
                           ::Deployment::Properties deployed,
                           ::Deployment::SatisfierProperties& available);


    /**
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
     * @brief This function calls all NM and gives them
     * the sub-domain
     */
    int call_all_node_managers (void);

    /**
     * @brief This function add new elements to the
     * already existing domain
     *
     * @param domain Deployment::Domain contians the new
     * elements
     */
    int add_to_domain (const ::Deployment::Domain& domain);

    /**
     *  @brief This function deletes elements from the domain
     *
     *  @param domain ::Deployment::Domain contains the new elements
     *  in the domain
     */
    int delete_from_domain (const ::Deployment::Domain& domain);

    /**
     *  @brief This function intimates the planner about a domain
     *  change
     *
     *  @param domain ::Deployment::Domain contains the new elements
     *  in the domain
     */
    int intimate_planner (const ::Deployment::Domain& domain);

    /**
     *  @brief This function finds a new node in the initial_domain
     *
     *  @param node The name of the node which is to be searched
     */

    bool find_in_initial_domain (const char* node_name,
                                 ::Deployment::Node& node);
    /**
     *  @brief This function finds a new node in the proviosiond_domain
     *
     *  @param node The name of the node which is to be searched
     */
    bool find_in_provisioned_domain (const char* node_name,
                                     ::Deployment::Node& node);

    /**
     *  @brief updates the node status by reading it from a file
     */
    bool update_node_status ();

    /**
     * @function find_resource
     * @brief It finds the Resource structure which is respresents the
     * ResourceAllocation
     */
    ::Deployment::Resource& find_resource (
                                           const ::Deployment::ResourceAllocation& resource);

    int commit_release_RA (
                           const ::Deployment::ResourceAllocations& resources);

    /// The ORB pointer
    CORBA::ORB_var orb_;

    /// The Deployment Configuration
    //      CIAO::Deployment_Configuration deployment_config_;

    /// The Initial Domain - contains resources
    /// at total capacity
    ::Deployment::Domain initial_domain_;

    /// The Current Domain - contains resources at current capacity
    ::Deployment::Domain current_domain_;

    /// The Target Manager Context
    ::Deployment::TargetManager_var target_mgr_;

    /**
     * The static provisioned Domain data
     */
    ::Deployment::Domain provisioned_data_;

    /// temporary domain used in commit/release to
    /// guard against exceptions
    ::Deployment::Domain temp_provisioned_data_;

    /// The current action
    Action current_action_;
  };

  typedef ACE_Singleton <DomainDataManager, ACE_SYNCH_RECURSIVE_MUTEX>
  DomainDataManager_Singleton;
#define DOMAIN_DATA_MANAGER DomainDataManager_Singleton::instance ()
} // CIAO

#endif /* DOMAIN_DATA_MGRH */
