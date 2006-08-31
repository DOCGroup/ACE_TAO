#ifndef _OPTION_COMMANDS_HPP_
#define _OPTION_COMMANDS_HPP_

#include <vector>
#include <string>
#include <iostream>
#include "basic_commands.hpp"
#include "environment.hpp"

class Option_Command_Base: public Command
{
public:
  Option_Command_Base(Environment& environment);

protected:
  OCML::Option_Interface* option(const char* name);
  OCML::Constraint_Engine* engine()
  { return env_.constraint_engine(); }
  Environment& environment()
  { return env_; }

private:
  Environment& env_;
};

class Show_Option_Command: public Option_Command_Base
{
public:
  Show_Option_Command(Environment& environment);

  virtual void execute(const std::vector<std::string>& params);
};

class Set_Option_Command: public Option_Command_Base
{
public:
  Set_Option_Command(Environment& environment);

  virtual void execute(const std::vector<std::string>& params);
};

class Clear_Option_Command: public Option_Command_Base
{
public:
  Clear_Option_Command(Environment& environment);

  virtual void execute(const std::vector<std::string>& params);
};

class Write_Command: public Command
{
public:
  Write_Command(OCML::Configurator* configurator);

  virtual void execute(const std::vector<std::string>&);

private:
  OCML::Configurator* configurator_;
};

class Load_Command: public Command
{
public:
  Load_Command(OCML::Configurator* configurator);

  virtual void execute(const std::vector<std::string>& params);

private:
  OCML::Configurator* configurator_;
};

#endif // _OPTION_COMMANDS_HPP_
