#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include "basic_commands.hpp"

#include <string>
#include <vector>
#include <map>

class Parse_Error
{
public:
  Parse_Error(const char* msg): what_(msg) { }
  const char* what() const { return what_.c_str(); }
  
private:
  std::string what_;
};

class Application
{
public:
  struct command_kind
  {
    std::string command_;
    std::vector<std::string> parameters_;
    bool empty_;
  };

public:
  Application(std::istream& is);
  ~Application();

  int run();
  void quit();
  std::map<std::string, std::string> descriptions();

  std::string read_command();
  command_kind parse_command(const std::string& command_line);
  void process_command(const command_kind& command);

  void register_command(const char* name, const char* descr,
                        Command* command);

  Command* unregister_command(const char* name);

private:
  std::istream& is_;
  bool quit_;

  std::map<std::string, Command*> commands_;
  std::map<std::string, std::string> descriptions_;
};

#endif // _APPLICATION_HPP_
