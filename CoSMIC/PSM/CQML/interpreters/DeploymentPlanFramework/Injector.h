#ifndef INJECTOR_H
#define INJECTOR_H

#include <map>
#include <set>
#include "CQML/CQML.h"
#include "DeploymentPlanFramework/DeploymentPlanFramework_Export.h"

namespace CQML
  {
    struct DeploymentPlanFramework_Export InstanceConnection
      {
        std::string instance_name;
        std::string port_name;
        std::string port_kind;
      };
    
    class DeploymentPlanFramework_Export Injector
      {
        public:
          typedef std::map <std::string, std::pair <InstanceConnection, InstanceConnection> > ConnectionMap;
          typedef std::set <CQML::Property> PropertySet;

          Injector ();
          virtual ~Injector ();
          virtual std::map<std::string, CQML::Component> add_monolith_instances (const std::string& plan_name);
          virtual std::map<std::string, CQML::Component> add_assembly_instances (const std::string& plan_name);
          virtual ConnectionMap add_connections (const std::string& plan_name, const ConnectionMap &);
          virtual std::map <std::string, std::string> assign_node_mappings (const std::string& plan_name, const std::map <std::string, std::string> &);
      };
  } // namespace CQML

#endif // INJECTOR_H
