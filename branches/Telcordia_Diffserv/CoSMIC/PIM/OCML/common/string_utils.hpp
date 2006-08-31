#ifndef STRING_UTILS_HPP
#define STRING_UTILS_HPP

/**
 * @file string_utils.hpp
 *
 * $Id$
 *
 * @author Emre Turkay <turkaye@dre.vanderbilt.edu>
 */

#include <string>
#include <xercesc/util/XMLString.hpp>

class XMLUnicodeString
{
public:
  // Constructors
  XMLUnicodeString();

  XMLUnicodeString(const XMLUnicodeString& value);

  XMLUnicodeString(const char* const value);

  XMLUnicodeString(const XMLCh* value);

  /// Destructor.
  ~XMLUnicodeString();

  // Methods
  std::string str() const;

  operator const XMLCh* () const;
  operator const XMLByte* () const;

  XMLUnicodeString& operator=(const XMLUnicodeString& value);

  operator std::string () const;

  bool operator==(const XMLUnicodeString& obj) const;

  bool operator==(const char* str) const;

  size_t size() const;

private:
  XMLCh* buf_;
};

std::string
delete_begin_end_spaces(const std::string& str);

#include "string_utils.ipp"

#endif // STRING_UTILS_HPP
