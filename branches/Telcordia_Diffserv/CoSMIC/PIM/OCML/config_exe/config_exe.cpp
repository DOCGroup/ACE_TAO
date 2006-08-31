#include <ui/configurator_ui.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <iterator>

std::string tree_file;
std::string value_file;
std::string rule_file;
std::string output_file;

bool usage(const char* bin_name)
{
  std::cerr << "Usage: " << bin_name
            << " tree_file [-v value_file] [-r rule_file]"
            << std::endl;
  return false;
}

bool parse_params(int argc, char** argv)
{
  std::vector<std::string> args(argv, argv + argc);
  std::vector<std::string>::iterator iter = args.begin();

  if (iter == args.end())
    return true;

  if ((*iter)[0] != '-')
  {
    tree_file = *iter;
    ++iter;
  }

  if (*iter == "-v")
  {
    ++iter;
    value_file = *iter;
    ++iter;
  }

  if (*iter == "-r")
  {
    ++iter;
    rule_file = *iter;
    ++iter;
  }

  if (*iter == "-o")
  {
    ++iter;
    output_file = *iter;
    ++iter;
  }

  if (iter != args.end())
    return usage(argv[0]);

  return true;
}

int main(int argc, char* argv[])
{
  if (parse_params(argc, argv) == false)
    return 1;

  std::string values;
  std::ifstream f(value_file.c_str());
  std::copy(std::istream_iterator<char>(f), std::istream_iterator<char>(),
            std::back_inserter(values));
  Configurator_Dialog dlg;
  std::string new_values = dlg.show(tree_file, values, rule_file);

  std::ofstream of(output_file.c_str());
  of << new_values;
  return 0;
}
