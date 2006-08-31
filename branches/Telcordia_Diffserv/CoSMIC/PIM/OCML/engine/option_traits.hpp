/**
 * @file option_traits.hpp
 *
 * $Id$
 *
 * Definition of the option type traits.
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#ifndef OPTIONS_TYPE_INFO_HPP
#define OPTIONS_TYPE_INFO_HPP

#include <string>

namespace OCML
{

  /**
   * Defines type information for the boolean option which maches the
   * requested template argument by the basic option class.
  */
  struct Boolean_Option_Trait
  {
    /// The value type.
    typedef bool value_type;
    /// The default value type.
    typedef bool default_value_type;

    /// Export to a string.
    static std::string as_string(value_type value);

    /// Parse a string and return value.
    static value_type parse_string(const char* str);

    /// Parse a string and return value.
    static value_type parse_string(const char* first, const char* last);

    static const char* type_string();

    static value_type read(std::istream& is);

    static void write(std::ostream& os, const value_type& value);
  };
 
  /**
   * Defines type information for the integer option which maches the
   * requested template argument by the basic option class.
   */
  struct Integer_Option_Trait
  {
    /// The value type.
    typedef int value_type;
    /// The default value type.
    typedef int default_value_type;

    /// Export to a string.
    static std::string as_string(value_type value);

    /// Parse a string and return value.
    static value_type parse_string(const char* str);

    /// Parse a string and return value.
    static value_type parse_string(const char* first, const char* last);

    static const char* type_string();
 
    static value_type read(std::istream& is);

    static void write(std::ostream& os, const value_type& value);
  };
 
  /**
   * Defines type information for the string option which maches the
   * requested template argument by the basic option class.
   */
  struct String_Option_Trait
  {
    /// The value type.
    typedef std::string value_type;
    /// The default value type.
    typedef std::string default_value_type;

    /// Export to a string.
    static std::string as_string(value_type value);

    /// Parse a string and return value.
    static value_type parse_string(const char* str);

    /// Parse a string and return value.
    static value_type parse_string(const char* first, const char* last);

    static const char* type_string();

    static value_type read(std::istream& is);

    static void write(std::ostream& os, const value_type& value);
  };
 
} // namespace OCML

#include "option_traits.ipp"

#endif // OPTIONS_TYPE_INFO_HPP
