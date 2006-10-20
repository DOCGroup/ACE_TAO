#ifndef PLAN_MANAGER_H
#define PLAN_MANAGER_H

#include "ace/Hash_Map_Manager_T.h"
#include "ciao/DeploymentS.h"
#include "ciao/NetQoSC.h"
#include "ciao/CIAO_common.h"
#include "BandwidthBroker/BandwidthBrokerC.h"
#include "BB_proxy.h"
#include <map>
#include <string>
#include <sstream>
#include <memory>

using namespace mil::darpa::arms::mlrm;
using namespace mil::darpa::arms::mlrm::BandwidthBroker;

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_NetQoSPlanner_Impl
    {
      class PlanManager 
      {
        public:

        PlanManager (::Deployment::DeploymentPlan &dep_plan, size_t index,
                     ::CIAO::DAnCE::NetworkQoS::NetQoSRequirement const
                     *net_qos_req);
        ~PlanManager ();
        static void del (const FlowRequest *c) throw ();
        
        void set_NetQoS (::CIAO::DAnCE::NetworkQoS::NetQoSRequirement
                         *net_qos_req);
        
        ::CIAO::DAnCE::NetworkQoS::NetQoSRequirement const *
          getNetQoS ();
      

        void build_node_map ();
        void build_instance_node_map ();
        void build_instance_index_map ();
        void push_dscp_info (Deployment::DiffservInfo const *endpoint);
        void push_flow (auto_ptr<FlowRequest> flow_request);

        Deployment::DiffservInfos const * get_dscp_infos () const;
        
        bool get_endpoints (::Deployment::DiffservInfo&,
                            const std::string &conn_name);
        
        std::string get_physical_host (const std::string &logical_node);
        
        int get_ip_address (CommonDef::IPAddress & srcIP, 
                            const char *);
        
        void dump_policies (const ::Deployment::DeploymentPlan &dep_plan);
        void dump_deployed_resources (const ::Deployment::DeploymentPlan &dep_plan);

        bool resolve_BB ();
        bool deploy_plan (::Deployment::DeploymentPlan &dep_plan);
        bool teardown_plan (::Deployment::DeploymentPlan &dep_plan);
        void remove_netqos (::Deployment::DeploymentPlan &dep_plan, size_t property_index);

        bool process_netqos_req (PlannerState *state, 
                                 const ::Deployment::DeploymentPlan &dep_plan);

        int make_flow_request (const CommonDef::IPAddress &srcIP,
                               const CommonDef::IPAddress &destIP,
                               int bandwidth,
                               CommonDef::QOSRequired qos_req,
                               long &dscp);

        void add_network_priorities (Deployment::DeploymentPlan & temp_plan,
                                     PlannerState *state);

        std::string push_policy(CIAO::DAnCE::ServerResource&, 
                                const std::string &,
                                NWPriorityModel model,
                                int request_dscp,
                                int reply_dscp);

        void push_deployed_resource(Deployment::DeploymentPlan&,
                                    const std::string &server_resource_id,
                                    size_t, const std::string &);
        
        void push_deployed_resource(Deployment::DeploymentPlan &dep_plan, 
                                    const std::string &server_resource_id,
                                    size_t client_instance_iter, 
                                    const std::string & policy_set_id, 
                                    const std::string & client_port_name,
                                    Deployment::CCMComponentPortKind client_port_kind);

        protected:

        typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                          int, ACE_Hash<ACE_CString>,
                                          ACE_Equal_To<ACE_CString>,
                                          ACE_Null_Mutex> Instance_Map;
        enum NWPriorityModel { UNDEF_NWPM = -1, CLIENT = 0 , SERVER = 1 };
        
        std::map <std::string, std::string> node_map_;
        std::map <std::string, std::string> instance_node_map_;
        Instance_Map instance_map_;
        BB_Proxy::CommandList command_list_;
        ::Deployment::DiffservInfos dscp_infos_;
        ::CIAO::DAnCE::ServerResource server_resource;
        ::CIAO::DAnCE::NetworkQoS::NetQoSRequirement net_qos_req_;
        ::Deployment::DeploymentPlan &dep_plan_;
        size_t net_qos_index_;

      };
    }
  }
}


#endif /* PLAN_MANAGER_H */

