#include "application.hpp"

#include <iostream>
#include <algorithm>
#include <iterator>
#include <common/line_parser.hpp>
#include <ace/Log_Msg.h>

#include <ace/Log_Msg.h>

Application::Application(std::istream& is)
  : is_(is),
    quit_(false)
{
  ACE_DEBUG((LM_TRACE, "ENTER: Application::Application(istream&)\n"));
  register_command("echo", "Echo the given parameters",
                   new Echo);
  register_command("quit", "Exit the application.",
                   new Terminator(this));
  register_command("help", "Show this help screen.",
                   new Helper(this));
  ACE_DEBUG((LM_TRACE, "EXIT: Application::Application(istream&)\n"));
}

Application::~Application()
{
  ACE_DEBUG((LM_TRACE, "ENTER: Application::~Application()\n"));
  delete unregister_command("echo");
  delete unregister_command("quit");
  delete unregister_command("help");
  ACE_DEBUG((LM_TRACE, "EXIT: Application::~Application()\n"));
}

int
Application::run()
{
  ACE_DEBUG((LM_TRACE, "ENTER: Application::run()\n"));
  std::string command_line;
  command_kind command;
  
  while (!quit_)
    {
      try {
        command_line = read_command();
        command = parse_command(command_line);
        if (command.empty_ != true)
        {
          ACE_DEBUG((LM_DEBUG, "Processing: %s\n", command.command_.c_str()));
          process_command(command);
        }
      }
      catch (Parse_Error& e) {
        std::cerr << e.what() << std::endl;
        ACE_DEBUG((LM_TRACE, "EXIT: Application::run()\n"));
        return 2;
      }
    }

  ACE_DEBUG((LM_TRACE, "EXIT: Application::run()\n"));
  return 0;
}

void
Application::quit()
{
  ACE_DEBUG((LM_TRACE, "Application::quit()\n"));
  quit_ = true;
}

std::string
Application::read_command()
{
  ACE_DEBUG((LM_TRACE, "ENTER: Application::read_command()\n"));
  std::string result;
  std::getline(is_, result);
  return result;
  ACE_DEBUG((LM_TRACE, "EXIT: Application::read_command()\n"));
}

Application::command_kind
Application::parse_command(const std::string& command_line)
{
  command_kind result;
  result.empty_ = true;

  std::vector<std::string> words;
  common::parse_line(command_line.begin(), command_line.end(),
                     std::back_inserter(words));

  std::vector<std::string>::iterator iter = words.begin();
  if (iter != words.end())
  {
    result.empty_ = false;
    result.command_ = *iter;
    ++iter;

    std::copy(iter, words.end(), std::back_inserter(result.parameters_));
  }

  return result;
}

void Application::process_command(const command_kind& command)
{
  ACE_DEBUG ((LM_DEBUG, "Processing: %s\n", command.command_.c_str()));
  std::map<std::string, Command*>::iterator iter =
    commands_.find(command.command_);

  if (iter != commands_.end())
  {
    iter->second->execute(command.parameters_);
  }
  else
  {
    std::string message = std::string("Unknown command: ")+command.command_; 
    throw Parse_Error(message.c_str());
  }
}

std::map<std::string, std::string>
Application::descriptions()
{
  std::map<std::string, std::string> result;
  
  for (std::map<std::string, Command*>::const_iterator iter=commands_.begin();
       iter != commands_.end(); ++iter)
    {
      result[iter->first] = descriptions_[iter->first];
    }

  return result;
}

void
Application::register_command(const char* name, const char* descr,
                              Command* command)
{
  commands_[name] = command;
  descriptions_[name] = descr;
}

Command*
Application::unregister_command(const char* name)
{
  std::map<std::string, Command*>::iterator iter = commands_.find(name);

  if (iter != commands_.end())
    {
      Command* command = iter->second;
      commands_.erase(iter);
      descriptions_.erase(descriptions_.find(name));

      return command;
    }

  return 0;
}
