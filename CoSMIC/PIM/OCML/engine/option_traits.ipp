/**
 * @file option_traits.ipp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#include <sstream>

namespace OCML
{
  /*
   * Boolean Option Trait inline methods.
   */
  
  inline std::string
  Boolean_Option_Trait::as_string(value_type value)
  {
    if (value)
    {
      return "true";
    }
    else
    {
      return "false";
    }
  }
  
  inline Boolean_Option_Trait::value_type
  Boolean_Option_Trait::parse_string(const char* str)
  {
    if (std::string(str) == "true")
    {
      return true;
    }
    else
    {
      /// @todo Some kind of error mechanism may be implemented here.
      return false;
    }
  }
 
  inline Boolean_Option_Trait::value_type
  Boolean_Option_Trait::parse_string(const char* first, const char* end)
  {
    if (std::string(first, end) == "true")
    {
      return true;
    }
    else
    {
      /// @todo Some kind of error mechanism may be implemented here.
      return false;
    }
  }
  
  inline const char*
  Boolean_Option_Trait::type_string()
  {
    return "BooleanOption";
  }
  
  inline bool
  Boolean_Option_Trait::read(std::istream& is)
  {
    std::string s;
    is >> s;
    return (s == "true");
  }
  
  inline void
  Boolean_Option_Trait::write(std::ostream& os,
                              const Boolean_Option_Trait::value_type& value)
  {
    if (value)
      os << "true";
    else
      os << "false";
  }
  
  /*
   * Integer Option Trait inline methods.
   */
  
  inline std::string
  Integer_Option_Trait::as_string(Integer_Option_Trait::value_type value)
  {
    std::ostringstream os;
    os << value;
    return os.str();
  }
  
  inline Integer_Option_Trait::value_type
  Integer_Option_Trait::parse_string(const char* str)
  {
    value_type value;
    std::istringstream is(str);
    is >> value;
    return value;
  }
 
  inline Integer_Option_Trait::value_type
  Integer_Option_Trait::parse_string(const char* first, const char* end)
  {
    std::string str(first, end);
    value_type value;
    std::istringstream is(str.c_str());
    is >> value;
    return value;
  }
  
  inline const char*
  Integer_Option_Trait::type_string()
  {
    return "IntegerOption";
  }
  
  inline Integer_Option_Trait::value_type
  Integer_Option_Trait::read(std::istream& is)
  {
    int value;
    is >> value;
    return value;
  }
  
  inline void
  Integer_Option_Trait::write(std::ostream& os,
                              const Integer_Option_Trait::value_type& value)
  {
    os << value;
  }
  
  /*
   * String Option Trait inline methods.
   */
  
  inline std::string
  String_Option_Trait::as_string(String_Option_Trait::value_type value)
  {
    return value;
  }
  
  inline String_Option_Trait::value_type
  String_Option_Trait::parse_string(const char* str)
  {
    return std::string(str);
  }
 
  inline String_Option_Trait::value_type
  String_Option_Trait::parse_string(const char* first, const char* last)
  {
    return std::string(first, last);
  }
  
  inline const char*
  String_Option_Trait::type_string()
  {
    return "StringOption";
  }
  
  inline String_Option_Trait::value_type
  String_Option_Trait::read(std::istream& is)
  {
    std::string value;
    is >> value;
    return value;
  }
  
  inline void
  String_Option_Trait::write(std::ostream& os,
                             const String_Option_Trait::value_type& value)
  {
    os << value;
  }
  
} // namespace OCML
