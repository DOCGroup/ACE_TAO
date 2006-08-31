#ifndef _ENVIRONMENT_HPP_
#define _ENVIRONMENT_HPP_

#include <stack>
#include <map>
#include <set>
#include <engine/configurator.hpp>
#include <engine/definitions.hpp>
#include <engine/option_interface.hpp>

class Environment
{
public:
  Environment(OCML::Configurator* configurator);

  void go_root();

  void go_up();

  void category(OCML::Option_Category* category);

  OCML::Option_Category* category();

  OCML::Option_Interface*
  option_interface(OCML::Option_Definition* option) const;

  OCML::Constraint_Engine* constraint_engine();

  OCML::Constraint_Engine::rule_index rule(const char* name);
  
  void add_rule(const char* name, OCML::Constraint_Engine::rule_index rule);

  void delete_rule(const char* name);

  void validate();

protected:
  void initialize_option_wrappers(OCML::Option_Category* category);

private:
  OCML::Configurator* configurator_;
  std::stack<OCML::Option_Category*> parents_;
  std::map<std::string, OCML::Constraint_Engine::rule_index> rules_;
  std::set<std::string> deleted_rules_;
};

#include "environment.ipp"

#endif // _ENVIRONMENT_HPP_
