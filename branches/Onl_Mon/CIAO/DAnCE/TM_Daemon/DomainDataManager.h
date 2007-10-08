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
 * @author Nilabja Roy <nilabjar@dre.vanderbilt.edu>
 * @quthor Nishanth Shanakran <nshankar@dre.vanderbilt.edu>
 */
//===============================================================
#ifndef DOMAIN_DATA_MGRH
#define DOMAIN_DATA_MGRH

#include "DAnCE/DomainApplicationManager/Deployment_Configuration.h"

#include "NM_MonitorC.h"

#include <vector>
#include <map>
#include <string>
#include "utils/Timer.h"
#include "ace/Mutex.h"
#include "ace/Condition_T.h"

using namespace ::CIAO::TM_Daemon;

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
       * Constructor
       *
       * @param orb the orb pointer.
       *
       * @param dat_file file that contains the objec references to the
       * node managers.
       */
      DomainDataManager (CORBA::ORB_ptr orb,
                         const char *dat_file,
                         const char *domain_file,
                         ::Deployment::DeploymentPlan& plan);

      ~DomainDataManager ();


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
       * @brief signals all the nodes to send in data
       */
      std::map<std::string, ::Deployment::Node>
      get_all_data (Onl_Monitor::AMI_NM_MonitorHandler_ptr handler);

      /**
       * @brief tells all the node managers to start the monitoring of QoS
       */
      int start_monitor_qos (Onl_Monitor::AMI_NM_MonitorHandler_ptr handler);

      void stop_monitors ();

    private:

      /**
       * Extracts the Obj Refs from the NS
       */
      int get_monitor_obj_ref ();


      /**
       * Read the initial Domain data from XML files.
       */
      int readin_domain_data ();

      /**
       * Invokes all NMs giving them the sub-domain.
       */
      int call_all_node_managers ();

      /**
       * Updates the node status by reading it from a file
       */
      bool update_node_status ();

      /**
       *  updates the snapshot of the system.
       */
      int update_dynamic (const ::Deployment::Domain &domainSubset);


      /// The ORB pointer
      CORBA::ORB_var orb_;

      /// The Deployment Configuration
      CIAO::Deployment_Configuration deployment_config_;

      /// The Initial Domain - contains resources at total capacity
      ::Deployment::Domain initial_domain_;

      /// The Current Domain - contains resourcesat current capacity
      ::Deployment::Domain current_domain_;

      /// The monitor obj references
      std::vector<Onl_Monitor::NM_Monitor_var> node_monitors_;

      /// The count of responses returned
      int response_count_;

      /// Stores the time taken to obtain the snapshot of the entire domain.
      utils::Timer delay_;

      /// Exception occured
      bool ex_occur_;

      /// Timers for each node to measure data collection delay latency for
      /// each node
      std::map<std::string, utils::Timer*> node_timers_;

      /// Map used to store "dynamic information" of each node.
      std::map<std::string, ::Deployment::Node> node_info_map_;

      /// Deployment plan.
      ::Deployment::DeploymentPlan plan_;

      /// the Data file which contains the Node Manager references
      std::string dat_file_;

      /// Condition varibale.
      ACE_Thread_Condition <ACE_Thread_Mutex> condition_;

      /// Mutex used by the condition varibale.
      ACE_Thread_Mutex condition_mutex_;

      /// Thread mutex.
      ACE_Thread_Mutex mutex_;
    };
  }


#endif /* DOMAIN_DATA_MGRH */
