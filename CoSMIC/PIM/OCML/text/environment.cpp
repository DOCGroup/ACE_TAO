#include "environment.hpp"

#include <ace/Log_Msg.h>

void Environment::go_root()
{
  ACE_DEBUG((LM_TRACE, "ENTER: Environment::go_root()\n"));
  while (!parents_.empty())
    parents_.pop();
  ACE_DEBUG((LM_TRACE, "EXIT: Environment::go_root()\n"));
}

void Environment::go_up()
{
  ACE_DEBUG((LM_TRACE, "ENTER: Environment::go_up()\n"));
  if (!parents_.empty())
    {
      parents_.pop();
    }
  ACE_DEBUG((LM_TRACE, "EXIT: Environment::go_up()\n"));
}

void Environment::category(OCML::Option_Category* category)
{
  ACE_DEBUG((LM_TRACE, "ENTER: Environment::category(Option_Category*)\n"));
  parents_.push(category);
  ACE_DEBUG((LM_TRACE, "EXIT: Environment::category(Option_Category*)\n"));
}

OCML::Option_Category* Environment::category()
{
  ACE_DEBUG((LM_TRACE, "ENTER: Environment::category()\n"));
  if (parents_.empty())
  {
    return configurator_->root_category();
  }

  OCML::Option_Category* result = parents_.top();
  ACE_DEBUG((LM_TRACE, "EXIT: Environment::category()\n"));
  return result;
}

OCML::Option_Interface*
Environment::option_interface(OCML::Option_Definition* option) const
{
  ACE_DEBUG((LM_TRACE,
            "ENTER: Environment::option_interface(Option_Definition*)\n"));
  OCML::Option_Interface* result = configurator_->interface(option);
  ACE_DEBUG((LM_TRACE,
            "EXIT: Environment::option_interface(Option_Definition*)\n"));
  return result;
}

void Environment::validate()
{
  if (configurator_->constraint_engine()->validate() == false)
  {
    std::cout << "Constraint violation. " << std::endl;
  }
  else
  {
    // Delete the deleted rules.
    for (std::set<std::string>::iterator iter = deleted_rules_.begin();
         iter != deleted_rules_.end(); ++iter)
    {
      rules_.erase(rules_.find(*iter));
    }
    deleted_rules_.clear();
  }
}

