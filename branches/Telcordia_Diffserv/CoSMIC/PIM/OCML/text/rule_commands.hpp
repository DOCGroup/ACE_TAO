#ifndef _RULE_COMMANDS_HPP_
#define _RULE_COMMANDS_HPP_

#include "option_commands.hpp"

class Define_Equality_Rule_Command: public Option_Command_Base
{
public:
  Define_Equality_Rule_Command(Environment& environment);

  virtual void execute(const std::vector<std::string>& params);

private:
  template <typename OPTION_TYPE_INFO>
  OCML::Option_Value* try_create_value(OCML::Option_Definition* definition,
                                       const char* str);
 
  OCML::Option_Value* create_value(OCML::Option_Definition* definition,
                                   const char* str);
};

class Define_If_Rule_Command: public Option_Command_Base
{
public:
  Define_If_Rule_Command(Environment& environment);

  virtual void execute(const std::vector<std::string>& params);
};

class Commit_Rule_Command: public Option_Command_Base
{
public:
  Commit_Rule_Command(Environment& environment);

  virtual void execute(const std::vector<std::string>& params);
};

class Validate_Rules_Command: public Option_Command_Base
{
public:
  Validate_Rules_Command(Environment& environment)
    : Option_Command_Base(environment)
  {
  }

  virtual void execute(const std::vector<std::string>&)
  {
    environment().validate();
  }
};

#endif // _RULE_COMMANDS_HPP_
