#ifndef _CATEGORY_COMMANDS_HPP_
#define _CATEGORY_COMMANDS_HPP_

#include <vector>
#include <string>
#include <iostream>
#include "environment.hpp"
#include "basic_commands.hpp"

class List_Command: public Command
{
public:
  List_Command(Environment& environment);

  template <typename InputIterator>
  void list(const char* type, InputIterator begin, InputIterator end);

  virtual void execute(const std::vector<std::string>&);

private:
  Environment& env_;
};

class Change_Category_Command: public Command
{
public:
  Change_Category_Command(Environment& environment);

  virtual void execute(const std::vector<std::string>& params);

private:
  Environment& env_;
};

#include "category_commands.tpp"

#endif // _CATEGORY_COMMANDS_HPP_
