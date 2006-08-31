/* -*- C++ -*- */

#ifndef _BASIC_COMMANDS_HPP_
#define _BASIC_COMMANDS_HPP_

#include <vector>
#include <string>

class Command
{
public:
  virtual void execute(const std::vector<std::string>& params) = 0;
};

class Application;

class Terminator: public Command
{
public:
  Terminator(Application* app);

  virtual void execute(const std::vector<std::string>&);

private:
  Application* app_;
};

class Helper: public Command
{
public:
  Helper(Application* app);
  
  virtual void execute(const std::vector<std::string>&);

private:
  Application* app_;
};

class Echo: public Command
{
public:
  virtual void execute(const std::vector<std::string>& params);
};


#endif // _BASIC_COMMANDS_HPP_
