#include "basic_commands.hpp"
#include "application.hpp"

#include <iostream>
#include <algorithm>
#include <iterator>

Terminator::Terminator(Application* app)
  : app_(app)
{
}

void Terminator::execute(const std::vector<std::string>&)
{
  app_->quit();
}

Helper::Helper(Application* app)
  : app_(app)
{
}

void Helper::execute(const std::vector<std::string>&)
{
  std::map<std::string, std::string> descr = app_->descriptions();

  for (std::map<std::string, std::string>::const_iterator iter = descr.begin();
       iter != descr.end(); ++iter)
    {
      std::cout << iter->first << " : \t" << iter->second << std::endl;
    }
}

void Echo::execute(const std::vector<std::string>& params)
{
  std::vector<std::string>::const_iterator iter = params.begin();
  std::cout << *iter;
  ++iter;
  while (iter != params.end())
  {
    std::cout << ' ' << *iter;
    ++iter;
  }
}
