#include "PlanManager.h"
#include "ace/INET_Addr.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Naming/Naming_Client.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>
#include <functional>

using namespace mil::darpa::arms::mlrm;
using namespace mil::darpa::arms::mlrm::BandwidthBroker;

/*
Named loops idiom
http://www.csse.monash.edu.au/~damian/Idioms/Topics/07.SB.NamedLoops/html/text.html
*/
#define NAMED(blockname) goto blockname; \
 blockname##_skip: if (0) blockname:
#define BREAK(blockname) goto blockname##_skip;

namespace CIAO
{
  namespace RACE
  {
    namespace CIDL_NetQoSPlanner_Impl
    {
      PlanManager::Invalid_NetQoS::Invalid_NetQoS () 
        : runtime_error("Invalid_NetQoS") {};
                                                     
      PlanManager::PlanManager (::Deployment::DeploymentPlan &dep_plan, 
                                size_t net_qos_index,
                                std::string const &filename)
        : dep_plan_ (dep_plan),
          net_qos_index_ (net_qos_index),
          node_map_filename_ (filename)
      { 
        ::CIAO::DAnCE::NetworkQoS::NetQoSRequirement *net_qos;
        if (this->dep_plan_.infoProperty [this->net_qos_index_].value >>= net_qos)
        {
          this->net_qos_req_ = *net_qos;
          this->build_instance_node_map ();
          // Build an in memory map of logical nodes to the physical hosts.
          this->build_node_map (this->node_map_filename_);
        }
        else
          throw Invalid_NetQoS();
      }


      PlanManager::~PlanManager ()
      {
        this->teardown ();
      }

      void PlanManager::del (const FlowRequest *c) throw ()
      {
        delete c;
      }
      
      void PlanManager::setNetQoS (::CIAO::DAnCE::NetworkQoS::NetQoSRequirement
                       *net_qos_req)
      {
        this->net_qos_req_ = *net_qos_req;
      }

      ::CIAO::DAnCE::NetworkQoS::NetQoSRequirement const * PlanManager::getNetQoS ()
      {
        return &this->net_qos_req_;
      }
/* 
      size_t PlanManager::get_net_qos_length ()
      {
        return this->net_qos_req_.conn_qos_set.length();
      }
*/
      void PlanManager::remove_netqos ()
      {
          // Remove CIAONetworkQoS infoProperty
          CORBA::ULong length = this->dep_plan_.infoProperty.length();
          size_t j = this->net_qos_index_;
          //ACE_DEBUG ((LM_ERROR, "(%N:%l): Length of dep_plan.infoProperty before removal = %u\n",length));
          if (length > j+1)
          {
            for (size_t k = j + 1; k < length; ++k)
              {
                this->dep_plan_.infoProperty[k-1] = this->dep_plan_.infoProperty[k];
              }
          }
          this->dep_plan_.infoProperty.length(length - 1);
          // Removal code ends
      }

      void PlanManager::build_instance_node_map ()
      {
        for (size_t i = 0;i < this->dep_plan_.instance.length (); ++i)
          {
            const Deployment::InstanceDeploymentDescription &instance = 
              this->dep_plan_.instance[i];
            this->instance_node_map_.insert 
               (std::make_pair (instance.name.in (),instance.node.in()));
          }
      }
      void PlanManager::push_dscp_info (Deployment::DiffservInfo const &endpoint)
      {
              size_t len = this->dscp_infos_.length ();
              this->dscp_infos_.length (len + 1);
              this->dscp_infos_ [len] = endpoint;
      }

      void PlanManager::push_flow (auto_ptr<FlowRequest> flow_request)
      {
        this->command_list_.push_back (flow_request.release ());
      }

      CommandList::iterator PlanManager::flows_begin ()
      {
        return this->command_list_.begin ();
      }
      
      CommandList::iterator PlanManager::flows_end ()
      {
        return this->command_list_.end ();
      }
      
      Deployment::DiffservInfos const * PlanManager::get_dscp_infos () const
      {
        return &this->dscp_infos_;
      }

      bool PlanManager::get_endpoints (::Deployment::DiffservInfo &diffserv_conn, 
                                       const std::string &conn_name)
      {
        size_t total_connections = this->dep_plan_.connection.length();
        for (size_t num_conn = 0; num_conn < total_connections; ++num_conn)
          {
            if (conn_name == this->dep_plan_.connection[num_conn].name.in())
              {
                const ::Deployment::PlanConnectionDescription & conn_desc =
                      this->dep_plan_.connection [num_conn];
                for (size_t i = 0; i < 2; ++i)
                  {
                    switch (conn_desc.internalEndpoint[i].kind)
                      {
                        case Deployment::Facet:
                        case Deployment::EventConsumer:
                        case Deployment::ecEventConsumer:
                        case Deployment::rtecEventConsumer:
                        case Deployment::nsEventConsumer:
                          {
                            unsigned long index = 
                              conn_desc.internalEndpoint[i].instanceRef;
                            
                            diffserv_conn.server_instance_name =
                              this->dep_plan_.instance[index].name;
                            diffserv_conn.server_port_name = 
                              conn_desc.internalEndpoint[i].portName;
                            diffserv_conn.server_port_kind = 
                              conn_desc.internalEndpoint[i].kind;
                            break;
                          }
                        case Deployment::SimplexReceptacle:
                        case Deployment::MultiplexReceptacle:
                        case Deployment::EventEmitter:
                        case Deployment::EventPublisher:
                        case Deployment::ecEventEmitter:
                        case Deployment::ecEventPublisher:
                        case Deployment::rtecEventEmitter:
                        case Deployment::rtecEventPublisher:
                        case Deployment::nsEventEmitter:
                        case Deployment::nsEventPublisher:
                          {
                            unsigned long index = 
                              conn_desc.internalEndpoint[i].instanceRef;
                            
                            diffserv_conn.client_instance_name = 
                              this->dep_plan_.instance[index].name;
                            diffserv_conn.client_port_name = 
                              conn_desc.internalEndpoint[i].portName;
                            diffserv_conn.client_port_kind = 
                              conn_desc.internalEndpoint[i].kind;
                            break;
                          }
                      }
                  }
                return true;
              } 
          }
        return false;
      }

      int PlanManager::get_ip_address (CommonDef::IPAddress & ip,
                                       const char *instance_name) 
      {
        //const int FAKE_PORT = 20000;
        std::string inst_name (instance_name);
        if (this->instance_node_map_.find (inst_name) != this->instance_node_map_.end ())
        {
          const std::string &logical_node = this->instance_node_map_[inst_name];
          //ACE_DEBUG ((LM_DEBUG, 
          //            "In PlanManager::process_netqos_req: logical_node = %s\n",
          //            logical_node.c_str()));
          //const std::string &physical_node = this->get_physical_host (logical_node);
          ip.dottedDecimal = CORBA::string_dup (logical_node.c_str());
          ip.subnetMask = CORBA::string_dup ("255.255.255.255");
          return 0;
/*          ACE_INET_Addr addr (FAKE_PORT, physical_node.c_str (), AF_INET);
          if (const char *ip_addr = addr.get_host_addr ())
            {
              ip.dottedDecimal = CORBA::string_dup (ip_addr);
              ip.subnetMask = CORBA::string_dup ("255.255.255.255");
              return 0;
            }
          else
            return -1;
*/        }
        else
          return -1;
      }

      void PlanManager::build_node_map (std::string const &node_map_filename)
      { 
          //std::cerr << "Node_map Filename: " << this->node_map_file () << std::endl;
          std::ifstream input_file (node_map_filename.c_str());
          std::istream_iterator <std::string> begin (input_file), end;
          for (; begin != end; )
            {
               std::string logical_node = *begin++;
               if (begin == end)
                break;
               //std::cerr << "Logical node name = " << logical_node << std::endl;
               std::string corbaloc_url = *begin++;
               //std::cerr << "Physical corbaloc URL = " << corbaloc_url << std::endl;
               //TAO_CORBALOC_Parser corbaloc_parser;
               //std:: cerr << "Is it a valid corbaloc URL? = " << corbaloc_parser.match_prefix (corbaloc_url.c_str()) << std::endl;
               if (1/*corbaloc_parser.match_prefix (corbaloc_url.c_str())*/)
                 {
                    size_t first = corbaloc_url.find (':', 0); 
                    size_t second = corbaloc_url.find (':', first + 1);
                    size_t third = corbaloc_url.find (':', second + 1);
                    std::string hostname = corbaloc_url.substr (second + 1, third - second - 1);
                    //std::cerr << "Hostname = " << hostname << std::endl;
                    this->node_map_.insert (make_pair (logical_node, hostname));
                 }
               else
                 {
                    ACE_DEBUG ((LM_ERROR, "In PlanManager::process_netqos_req(): Incorrect corbaloc URL"));
                 }
            }
      }

      std::string PlanManager::get_physical_host (const std::string &logical_node)
      {
        if (this->node_map_.find (logical_node) != this->node_map_.end ())
        {
           return this->node_map_[logical_node];
        }
        else
           return std::string ("");
      }
      
      void PlanManager::build_instance_index_map ()
      {
        CORBA::ULong instance_len = this->dep_plan_.instance.length ();
        for (CORBA::ULong j = 0; j < instance_len; ++j)
          {
            const char *instance_name = this->dep_plan_.instance[j].name.in ();
            this->instance_map_.bind (instance_name, j);
          }
      }
      
      std::string PlanManager::push_policy 
           (CIAO::DAnCE::ServerResource &server_resource, 
            const std::string & instance_name,
            NWPriorityModel model,
            int request_dscp, int reply_dscp)
      {
        static long policy_count = 0;
        CORBA::ULong policy_set_len = server_resource.orb_config.policy_set.length ();
        CORBA::ULong new_policy_set_len = policy_set_len + 1;
        server_resource.orb_config.policy_set.length (new_policy_set_len);
        ::CIAO::DAnCE::PolicySet &policy_set =
            server_resource.orb_config.policy_set[policy_set_len];

        std::ostringstream policy_set_id;
        policy_set_id << "test_policy_set_id" <<  policy_count++ 
                      <<  "_" << instance_name;

        policy_set.Id = CORBA::string_dup (policy_set_id.str().c_str ());

        CORBA::ULong policy_def_len = policy_set.policies.length ();
        CORBA::ULong new_policy_def_len = policy_def_len + 1;
        policy_set.policies.length (new_policy_def_len);

        if (SERVER == model)
          {
            ::CIAO::DAnCE::NWPriorityModelPolicyDef npmd;
            npmd.nw_priority_model = ::CIAO::DAnCE::CLIENT_PROPAGATED_NWPRIORITY;
            npmd.request_dscp = request_dscp;
            npmd.reply_dscp = reply_dscp;
     
            policy_set.policies[policy_def_len].NWPriorityModelDef (npmd); 
          }
        else if (CLIENT == model)
          {
            ::CIAO::DAnCE::CNWPriorityModelPolicyDef cnpmd;
            cnpmd.request_dscp = request_dscp;
            cnpmd.reply_dscp = reply_dscp;

            policy_set.policies[policy_def_len].CNWPriorityModelDef (cnpmd);
          }
        
        return policy_set_id.str();
      } 

      void PlanManager::push_deployed_resource 
             (Deployment::DeploymentPlan &dep_plan,
              const std::string &server_resource_id,
              size_t server_instance_iter,
              const std::string &policy_set_id)
      {
          //static long server_resource_count = 0;
          CORBA::ULong dep_res_len = dep_plan.instance[server_instance_iter].deployedResource.length ();
          CORBA::ULong new_dep_res_len = dep_res_len + 1;
          dep_plan.instance[server_instance_iter].deployedResource.length (new_dep_res_len);

          Deployment::InstanceResourceDeploymentDescription &resource_desc =
            dep_plan.instance[server_instance_iter].deployedResource[dep_res_len]; 

          resource_desc.resourceUsage = Deployment::InstanceUsesResource;
          resource_desc.requirementName = CORBA::string_dup ("CIAO:PolicySet");
          //std::ostringstream res_name;
          //res_name <<  "test_server_resource_name" << server_resource_count++;
          //resource_desc.resourceName = CORBA::string_dup (res_name.str().c_str());
          resource_desc.resourceName = CORBA::string_dup (server_resource_id.c_str());

          CORBA::ULong pro_len = resource_desc.property.length ();
          CORBA::ULong new_pro_len = pro_len + 1;
          resource_desc.property.length (new_pro_len);
          resource_desc.property[pro_len].name = CORBA::string_dup ("CIAO:InstancePolicy");
          resource_desc.property[pro_len].value <<= policy_set_id.c_str ();
    }

     void PlanManager::push_deployed_resource
              (Deployment::DeploymentPlan &dep_plan, 
               const std::string &server_resource_id,
               size_t client_instance_iter, 
               const std::string & policy_set_id, 
               const std::string & client_port_name,
               Deployment::CCMComponentPortKind client_port_kind)
     { 
          //static long client_resource_count = 0;
          CORBA::ULong dep_res_len = dep_plan.instance[client_instance_iter].deployedResource.length ();
          CORBA::ULong new_dep_res_len = dep_res_len + 1;
          dep_plan.instance[client_instance_iter].deployedResource.length (new_dep_res_len);

          Deployment::InstanceResourceDeploymentDescription &resource_desc = 
             dep_plan.instance[client_instance_iter].deployedResource[dep_res_len];

          resource_desc.resourceUsage = Deployment::InstanceUsesResource;
          resource_desc.requirementName = CORBA::string_dup ("CIAO:PolicySet");
          //std::ostringstream res_name;
          //res_name << "test_client_resource_name" << client_resource_count++;
          //resource_desc.resourceName = CORBA::string_dup (res_name.str().c_str());
          resource_desc.resourceName = CORBA::string_dup (server_resource_id.c_str());

          CORBA::ULong pro_len = resource_desc.property.length ();
          CORBA::ULong new_pro_len = pro_len + 1;
          resource_desc.property.length (new_pro_len);
          if (Deployment::SimplexReceptacle == client_port_kind ||  
              Deployment::MultiplexReceptacle == client_port_kind)
            {
                resource_desc.property[pro_len].name = CORBA::string_dup ("CIAO:ReceptaclePolicy");
            }
          else if (Deployment::EventPublisher == client_port_kind)
            {
                resource_desc.property[pro_len].name = CORBA::string_dup ("CIAO:PublisherPolicy");
            }
            
          resource_desc.property[pro_len].value <<= policy_set_id.c_str ();

          pro_len = resource_desc.property.length ();
          new_pro_len = pro_len + 1;
          resource_desc.property.length (new_pro_len);
          resource_desc.property[pro_len].name = client_port_name.c_str();
          resource_desc.property[pro_len].value <<= policy_set_id.c_str ();
     }

     bool PlanManager::teardown()
     {
/*        std::for_each (this->command_list_.begin (), 
                       this->command_list_.end (),
                       std::mem_fun(&FlowRequest::rollback));
*/        
        bool retval = true;
        for (CommandList::iterator iter = this->command_list_.begin();
            iter != this->command_list_.end();
            ++iter)
        { 
          retval &= ((*iter)->rollback () == -1) ? false : true;
        }
        std::for_each (this->command_list_.begin (), 
                       this->command_list_.end (),
                       PlanManager::del);
        
        this->command_list_.clear();
        return retval;
     }

      // *********************************************************
      // code that creates the deployment plan populating the network
      // priority policies using the diffserv codepoint decisions.

      void
      PlanManager::add_network_priorities ()
      {
        static long server_resource_count = 0;
        this->build_instance_index_map();

        CORBA::ULong len = this->dscp_infos_.length ();
        //CIAO::DAnCE::ServerResource server_resource;
        std::ostringstream server_resource_id;
        server_resource_id << "test_server_resource_id" << server_resource_count++;
        this->server_resource.Id = CORBA::string_dup (server_resource_id.str().c_str());

        for (CORBA::ULong i = 0; i < len; ++i)
          {
            std::string server_instance_name = this->dscp_infos_[i].server_instance_name.in ();
            std::string client_instance_name = this->dscp_infos_[i].client_instance_name.in ();
            std::string client_port_name = this->dscp_infos_[i].client_port_name.in ();
            Deployment::CCMComponentPortKind client_port_kind =
              this->dscp_infos_[i].client_port_kind; 

            CORBA::Long request_dscp = this->dscp_infos_[i].request_dscp;
            CORBA::Long reply_dscp = this->dscp_infos_[i].reply_dscp;

            int server_instance_iter = 0;
            if (this->instance_map_.find (server_instance_name.c_str(), server_instance_iter) == 0)
              {
                std::string policy_set_id = this->push_policy (this->server_resource, 
                                                               server_instance_name, 
                                                               SERVER, 0, 0);
                this->push_deployed_resource (this->dep_plan_, server_resource_id.str(), 
                                              server_instance_iter, policy_set_id);    
              }
    
            int client_instance_iter = 0;
            if (this->instance_map_.find (client_instance_name.c_str(), client_instance_iter) == 0)
              {
                std::string policy_set_id = this->push_policy (this->server_resource, 
                                                               client_instance_name, 
                                                               CLIENT, request_dscp, reply_dscp);
                this->push_deployed_resource (this->dep_plan_, server_resource_id.str(), 
                                              client_instance_iter, policy_set_id, 
                                              client_port_name, client_port_kind);
              }
          }
    
        CORBA::ULong info_prop_len = this->dep_plan_.infoProperty.length ();
        CORBA::ULong new_info_prop_len = info_prop_len + 1;
        this->dep_plan_.infoProperty.length (new_info_prop_len);
        this->dep_plan_.infoProperty[info_prop_len].name = CORBA::string_dup ("CIAOServerResources");
        this->dep_plan_.infoProperty[info_prop_len].value <<= this->server_resource;
      }

    }
  }
}

