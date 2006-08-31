#include <algorithm>
#include <functional>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <iterator>

#include "DeploymentPlanFramework/Injector.h"

#include "UmlExt.h"
#include "UdmUtil.h"

#include "FaultTolerance/FTInjector.h"

namespace CQML
{

  FTInjector::FTInjector (ComponentAdder *comp_add,
                          ConnectionAdder *conn_add,
                          NodeAssigner *node_add,
                          FTRequirementsVisitor *ft_req)
    : comp_addr_ (comp_add),
      conn_addr_ (conn_add),
      node_assgn_ (node_add),
      ft_req_visitor_ (ft_req)
    {}

  FTInjector::~FTInjector ()
    {}

  const ComponentAdder * FTInjector::get_component_adder(void) const
    {
      return this->comp_addr_.get();
    }
  const ConnectionAdder * FTInjector::get_connection_adder (void) const
    {
      return this->conn_addr_.get();
    }
  const NodeAssigner * FTInjector::get_node_assigner (void) const
    {
      return this->node_assgn_.get();
    }

  std::map<std::string, CQML::Component> FTInjector::add_monolith_instances (const std::string& plan_name)
    {
      return this->comp_addr_->get_all_monolith_components ();
    }

  std::map<std::string, CQML::Component> FTInjector::add_assembly_instances (const std::string& plan_name)
    {
      return this->comp_addr_->get_all_assembly_components ();
    }

  Injector::ConnectionMap FTInjector::add_connections (const std::string& plan_name, const Injector::ConnectionMap &conn_map)
    {
      return this->conn_addr_->get_new_connections (plan_name, conn_map);
    }

  std::map <std::string, std::string> FTInjector::assign_node_mappings (const std::string& plan_name, const std::map <std::string, std::string> &known_mapping)
    {
      this->node_assgn_->compute_assignment (known_mapping);
      const std::map <std::string, Node> compname_node_map = 
        this->node_assgn_->get_node_assignment ();
      std::map <std::string, std::string> compname_nodename_map;
      for (std::map <std::string, Node>::const_iterator itr = compname_node_map.begin();
           itr != compname_node_map.end();
           ++itr)
        {
          compname_nodename_map.insert (std::make_pair(itr->first, std::string(itr->second.name ())));
        }
      return compname_nodename_map;
    }

} // namespace CQML

