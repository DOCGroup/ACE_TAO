#ifndef _CATEGORY_PARSER_HPP_
#define _CATEGORY_PARSER_HPP_

#include <vector>
#include <string>

inline std::vector<std::string>
parse_category_string(const std::string& val)
{
  std::string::const_iterator iter1 = val.begin();
  std::string::const_iterator iter2 = val.begin();
  do {
    iter1 = std::find(iter2, val.end(), '/');
  } while (iter1 != val.end());
}

#endif // _CATEGORY_PARSER_HPP_
