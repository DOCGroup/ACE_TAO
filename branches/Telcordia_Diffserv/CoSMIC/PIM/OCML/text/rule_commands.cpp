#include "rule_commands.hpp"

/*
 * Define_Equality_Rule_Command implementation.
 */

Define_Equality_Rule_Command::
Define_Equality_Rule_Command(Environment& environment)
  : Option_Command_Base(environment)
{
}

// This is private, so it can be in a cpp file.
template <typename OPTION_TYPE_INFO>
OCML::Option_Value*
Define_Equality_Rule_Command::
try_create_value(OCML::Option_Definition* definition, const char* str)
{
  typedef OCML::Basic_Option_Definition<OPTION_TYPE_INFO> definition_type;
  typedef OCML::Basic_Option_Value<OPTION_TYPE_INFO> value_type;

  definition_type* option = dynamic_cast<definition_type*>(definition);

  value_type* value = 0;

  if (option != 0)
    value = new value_type(OPTION_TYPE_INFO::parse_string(str));

  return value;
}

OCML::Option_Value*
Define_Equality_Rule_Command::
create_value(OCML::Option_Definition* definition, const char* str)
{
  OCML::Option_Value* result = 0;

  if (result == 0)
    result =
      try_create_value<OCML::Boolean_Option_Trait>(definition, str);
  if (result == 0)
    result =
      try_create_value<OCML::Integer_Option_Trait>(definition, str);
  if (result == 0)
    result =
      try_create_value<OCML::String_Option_Trait>(definition, str);

  return result;
}

void
Define_Equality_Rule_Command::
execute(const std::vector<std::string>& params)
{
  OCML::Option_Interface* interface = option(params[1].c_str());

  std::auto_ptr<OCML::Option_Value>
    value(create_value(interface->definition(), params[2].c_str()));

  if (value.get() != 0)
  {
    OCML::Constraint_Engine::rule_index index =
      engine()->define_equality_rule(interface->definition(), value.get());
    environment().add_rule(params[0].c_str(), index);
  }
  else
  {
    std::cout << "Unable to parse value: " << params[1] << std::endl;
  }
}

Define_If_Rule_Command::
Define_If_Rule_Command(Environment& environment)
  : Option_Command_Base(environment)
{
}

/*
 * Commit_If_Command implementation.
 */

void
Define_If_Rule_Command::
execute(const std::vector<std::string>& params)
{
  OCML::Constraint_Engine::rule_index index1 =
    environment().rule(params[1].c_str());
  OCML::Constraint_Engine::rule_index index2 =
    environment().rule(params[2].c_str());

  OCML::Constraint_Engine::rule_index index =
    engine()->define_if_rule(index1, index2);

  environment().add_rule(params[0].c_str(), index);
}

/*
 * Commit_Rule_Command implementation.
 */

Commit_Rule_Command::
Commit_Rule_Command(Environment& environment)
  : Option_Command_Base(environment)
{
}

void
Commit_Rule_Command::
execute(const std::vector<std::string>& params)
{
  OCML::Constraint_Engine::rule_index index = environment().rule(params[0].c_str());
  engine()->add_rule(index);
}

