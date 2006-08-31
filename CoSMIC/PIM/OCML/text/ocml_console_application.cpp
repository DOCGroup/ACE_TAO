#include "ocml_console_application.hpp"
#include "category_commands.hpp"
#include "option_commands.hpp"
#include "rule_commands.hpp"

OCML_Console_Application::OCML_Console_Application(std::istream& is)
  : Application(is),
    configurator_(OCML::Configurator::instance("orb_tree.xml")),
    env_(configurator_)
{
  register_command("list", "List all the options",
                   new List_Command(env_));
  register_command("cd", "Changes the current category",
                   new Change_Category_Command(env_));
  register_command("show", "Show the value of an option",
                   new Show_Option_Command(env_));
  register_command("set", "Set the value of an option",
                   new Set_Option_Command(env_));
  register_command("clear", "Clear the value of an option",
                   new Clear_Option_Command(env_));
  register_command("define_equality_rule", "Define a new equality rule",
                   new Define_Equality_Rule_Command(env_));
  register_command("define_if_rule", "Define a new if rule",
                   new Define_If_Rule_Command(env_));
  register_command("apply", "Validate and apply the inserted rules",
                   new Validate_Rules_Command(env_));
  register_command("commit", "Add the given rule to the constraint engine",
                   new Commit_Rule_Command(env_));
  register_command("write", "Print the values in XML format",
                   new Write_Command(configurator_));
  register_command("load", "Load the values from a given file",
                   new Load_Command(configurator_));
}

OCML_Console_Application::~OCML_Console_Application()
{
  delete unregister_command("clear");
  delete unregister_command("set");
  delete unregister_command("show");
  delete unregister_command("cd");
  delete unregister_command("list");

  delete configurator_;
}

