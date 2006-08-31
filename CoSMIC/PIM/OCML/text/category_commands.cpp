#include "application.hpp"
#include "category_commands.hpp"
#include <ace/Log_Msg.h>

/*
 * class List_Rules_Command implementation.
 */

List_Command::List_Command(Environment& environment)
  : env_(environment)
{
}

void List_Command::execute(const std::vector<std::string>&)
{
  ACE_DEBUG ((LM_DEBUG, "Fetching the root category\n"));
  OCML::Option_Category* root = env_.category();
  ACE_DEBUG ((LM_DEBUG, "Name of the root category: %s\n", root->name().c_str()));
 
  ACE_DEBUG ((LM_DEBUG, "Printing contained categories\n"));
  this->list("categories", root->begin_categories(), root->end_categories());
  ACE_DEBUG ((LM_DEBUG, "Printing contained options\n"));
  this->list("options", root->begin_options(), root->end_options());
}

/*
 * class Change_Category_Command implementation.
 */
Change_Category_Command::Change_Category_Command(Environment& environment)
  : env_(environment)
{
}

void Change_Category_Command::execute(const std::vector<std::string>& params)
{
  if (params.size() == 0)
    {
      env_.go_root();
      return;
    }
  
  if (params[0] == std::string(".."))
    {
      env_.go_up();
      return;
    }
  
  OCML::Option_Category* root = env_.category();
  
  for (OCML::Option_Category::category_iterator iter=root->begin_categories();
       iter != root->end_categories(); ++iter)
    {
      if ((*iter)->name() == params[0])
        {
          env_.category(*iter);
          return;
        }
    }
  
  std::string message = std::string("Unknown category: ")+params[0];
  throw Parse_Error(message.c_str());
}
