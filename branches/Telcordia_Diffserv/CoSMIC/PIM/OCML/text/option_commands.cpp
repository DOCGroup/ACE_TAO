#include "option_commands.hpp"
#include "application.hpp"

#include <fstream>
#include <string>
#include <algorithm>
#include <iterator>

/*
 * class Option_Command_Base implementation.
 */

Option_Command_Base::Option_Command_Base(Environment& environment)
  : env_(environment)
{
}

OCML::Option_Interface* Option_Command_Base::option(const char* name)
{
  OCML::Option_Category* root = env_.category();

  for (OCML::Option_Category::option_iterator iter = root->begin_options();
       iter != root->end_options(); ++iter)
  {
    if ((*iter)->name() == std::string(name))
    {
      OCML::Option_Interface* result = env_.option_interface(*iter);
      return result;
    }
  }

  return 0;
}

/*
 * class Show_Option_Command implementation.
 */

Show_Option_Command::Show_Option_Command(Environment& environment)
  : Option_Command_Base(environment)
{
}

void Show_Option_Command::execute(const std::vector<std::string>& params)
{
  OCML::Option_Interface* interface = option(params[0].c_str());

  if (interface != 0)
  {
    if (interface->assigned())
    {
      std::cout << interface->str_value();
    }
    else
    {
      std::cout << "NULL";
    }
  }
  else
  {
    std::string message = std::string("Unknown option: ")+params[0];
    throw Parse_Error(message.c_str());
  }
}

/*
 * class Set_Option_Command implementation.
 */

Set_Option_Command::Set_Option_Command(Environment& environment)
  : Option_Command_Base(environment)
{
}

void Set_Option_Command::execute(const std::vector<std::string>& params)
{
  OCML::Option_Interface* interface = option(params[0].c_str());

  if (interface != 0)
  {
    if (interface->str_value(params[1].c_str()) == false)
    {
      std::cout << "\n Constraint violation \n";
    }
  }
  else
  {
    std::string message = std::string("Unknown option: ")+params[0];
    throw Parse_Error(message.c_str());
  }
}

/*
 * class Clear_Option_Command implementation.
 */

Clear_Option_Command::Clear_Option_Command(Environment& environment)
  : Option_Command_Base(environment)
{
}

void Clear_Option_Command::execute(const std::vector<std::string>& params)
{
  OCML::Option_Interface* interface = option(params[0].c_str());

  if (interface != 0)
  {
    if (interface->clear() == false)
    {
      std::cout << "\n Constraint violation \n";
    }
  }
  else
  {
    std::string message = std::string("Unknown option: ")+params[0];
    throw Parse_Error(message.c_str());
  }
}

/*
 * class Write_Command implementation.
 */

Write_Command::Write_Command(OCML::Configurator* configurator)
  : configurator_(configurator)
{
}

void
Write_Command::execute(const std::vector<std::string>&)
{
  std::cout << configurator_->write();
}

/*
 * class Load_Command implementation.
 */

Load_Command::Load_Command(OCML::Configurator* configurator)
  : configurator_(configurator)
{
}

void
Load_Command::execute(const std::vector<std::string>& params)
{
  std::ifstream inf(params[0].c_str());
  inf.unsetf(std::ios_base::skipws);
  if (!inf)
  {
	  std::cerr << "Cannot open file.\n";
	  return;
  }
  std::string value_data;
  std::copy(std::istream_iterator<char>(inf), std::istream_iterator<char>(),
            std::back_inserter(value_data));
//  configurator_->initialize_options(value_data.c_str(), value_data.size());
}

