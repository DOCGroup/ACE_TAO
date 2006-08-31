#include "DeploymentPlanFramework/Injector.h"

using CQML::Injector;

Injector::Injector () {}

Injector::~Injector () {}

std::map<std::string, CQML::Component> Injector::add_monolith_instances (const std::string& plan_name)
  {
    // Return empty.
    return std::map <std::string, CQML::Component> ();
  }

std::map<std::string, CQML::Component> Injector::add_assembly_instances (const std::string& plan_name)
  {
    // Return empty.
    return std::map <std::string, CQML::Component> ();
  }

Injector::ConnectionMap Injector::add_connections (const std::string& plan_name, const ConnectionMap &)
  {
    // Return empty.
    return Injector::ConnectionMap ();
  }

std::map <std::string, std::string> Injector::assign_node_mappings (const std::string& plan_name, const std::map <std::string, std::string> &)
  {
    // Return empty.
    return std::map <std::string, std::string> ();
  }


