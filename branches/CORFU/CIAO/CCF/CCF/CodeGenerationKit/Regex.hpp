// file      : CCF/CodeGenerationKit/Regex.hpp
// author    : Boris Kolpackov <boris@dre.vanderbilt.edu>
// cvs-id    : $Id$

#ifndef CCF_RUNTIME_REGEX_HPP
#define CCF_RUNTIME_REGEX_HPP

#include <string>
#include <boost/regex.hpp>

namespace regex
{
  template <typename C>
  std::basic_string<C>
  perl_s (std::basic_string<C> const& src, std::basic_string<C> const& e)
  {
    typedef std::basic_string<C> string;
    typedef typename string::size_type size;

    if (e.empty ())
      return src;

    C delimiter (e[0]);

    size first = e.find (delimiter);
    size middle = e.find (delimiter, first + 1);
    size last = e.find (delimiter, middle + 1);

    string pattern (e, first + 1, middle - first - 1);
    string format (e, middle + 1, last - middle - 1);

    //std::cout << pattern << "  " << format << std::endl;

    boost::basic_regex<C> expr (pattern);

    return regex_merge (
      src,
      expr,
      format,
      boost::match_default | boost::format_all );
  }

  template <typename C>
  std::basic_string<C>
  perl_s (std::basic_string<C> const& src, C const* e)
  {
    return perl_s (src, std::basic_string<C> (e));
  }
}

#endif  // CCF_RUNTIME_REGEX_HPP
