// file      : CCF/CodeGenerationKit/Regex.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_RUNTIME_REGEX_HPP
#define CCF_RUNTIME_REGEX_HPP

#include <string>
#include <boost/regex.hpp>

namespace regex
{
  using boost::regex;
  using boost::regex_merge;

  inline std::string
  perl_s (std::string src, std::string e)
  {
    char delimiter = '/';

    std::string::size_type first = e.find (delimiter);
    std::string::size_type middle = e.find (delimiter, first + 1);
    std::string::size_type last = e.find (delimiter, middle + 1);

    std::string pattern (e, first + 1, middle - first - 1);
    std::string format (e, middle + 1, last - middle - 1);

    //std::cout << pattern << "  " << format << std::endl;

    regex expr (pattern);

    return regex_merge (
      src,
      expr,
      format,
      boost::match_default | boost::format_all );
  }
}

#endif  // CCF_RUNTIME_REGEX_HPP
