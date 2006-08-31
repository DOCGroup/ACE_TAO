/* -*- C++ -*- */

/**
 * @file line_parser.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef _LINEPARSER_HPP_
#define _LINEPARSER_HPP_

#include <string>
#include <cctype>
#include <cstring>

namespace common
{

  /// Functor used to parse the command line into space seperated words.
  template <typename OutputIterator>
  class line_parser
  {
  public:
    // exception thrown when there is an escape character at the end
    // of the input stream  -if escape is true in destructor-
    class open_escape { };
  
  public:
    line_parser(const line_parser<OutputIterator>& lp);
  
    line_parser(OutputIterator iter);

    /// Append the last word, check if escape is true
    ~line_parser();
  
    /// appends words into the container by also handling escape character
    void operator()(char ch);

  private:
    OutputIterator iter_;
    bool escape_;
    std::string word_;
  };

  template <typename InputIterator, typename OutputIterator>
  void parse_line(InputIterator begin, InputIterator end, OutputIterator to);

} // namespace common

#include "line_parser.ipp"
#include "line_parser.tpp"

#endif // _LINEPARSER_HPP_
