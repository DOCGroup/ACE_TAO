// $Id$

#include "test.hpp"
#include <sstream>

template <class OPTION_TRAIT>
void write_info(OCML::Basic_Option_Reference<OPTION_TRAIT>* option)
{
  bool assigned = option->assigned();
  bool required = option->required();
  bool allowed  = option->allowed();
  bool flagged  = option->flagged();

  std::cout << "Reference: " << std::endl
            << "    type:     " << OPTION_TRAIT::type_string() << std::endl
            << "    value:    " << OPTION_TRAIT::as_string(option->value())
            << std::endl
            << "    assigned: " << bool_as_string(assigned) << std::endl
            << "    required: " << bool_as_string(required) << std::endl
            << "    allowed:  " << bool_as_string(allowed)  << std::endl
            << "    flagged:  " << bool_as_string(flagged)  << std::endl;
}

template <class OPTION_TRAIT>
std::string str_info(OCML::Basic_Option_Reference<OPTION_TRAIT>* option)
{
  bool assigned = option->assigned();
  bool required = option->required();
  bool allowed  = option->allowed();
  bool flagged  = option->flagged();

  std::stringstream s;
  s << '<' << OPTION_TRAIT::type_string() << "> = ";

  if (assigned)
    s << OPTION_TRAIT::as_string(option->value());
  else
    s << "null";

  s  << " (" << "re: " << bool_as_string(required) << ", "
             << "al: " << bool_as_string(allowed) << ", "
             << "fl: " << bool_as_string(flagged) << ')';

  return s.str();
}
