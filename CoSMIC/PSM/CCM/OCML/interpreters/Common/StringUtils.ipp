/*
 *  XMLUnicodeString inline methods
 */

#include <cctype>

inline
XMLUnicodeString::XMLUnicodeString()
  : buf_(NULL)
{
}

inline
XMLUnicodeString::XMLUnicodeString(const XMLUnicodeString& value)
  : buf_(xercesc::XMLString::replicate(value))
{
}

inline
XMLUnicodeString::XMLUnicodeString(const char* const value)
  : buf_(xercesc::XMLString::transcode(value))
{
}

inline
XMLUnicodeString::XMLUnicodeString(const XMLCh* value)
  : buf_(xercesc::XMLString::replicate(value))
{
}

inline
XMLUnicodeString::~XMLUnicodeString()
{
  if (buf_)
      xercesc::XMLString::release(&buf_);
}

inline std::string
XMLUnicodeString::str() const
{
  if (buf_ == NULL)
    return std::string();
  
  char* tmp = xercesc::XMLString::transcode(buf_);
  std::string result(tmp);
  xercesc::XMLString::release(&tmp);
  return result;
}

inline 
XMLUnicodeString::operator const XMLCh*() const
{
  return buf_;
}

inline
XMLUnicodeString::operator const XMLByte* () const
{
  return (XMLByte*) buf_;
}

inline XMLUnicodeString&
XMLUnicodeString::operator=(const XMLUnicodeString& value)
{
  XMLCh* new_buf = xercesc::XMLString::replicate(value);
  
  if (buf_)
    xercesc::XMLString::release(&buf_);
  
  buf_ = new_buf;
  return *this;
}

inline 
XMLUnicodeString::operator std::string () const
{
  return this->str();
}

inline bool
XMLUnicodeString::operator==(const XMLUnicodeString& obj) const
{
  return (xercesc::XMLString::compareIString(buf_, obj.buf_) == 0);
}

inline bool
XMLUnicodeString::operator==(const char* str) const
{
  return *this == XMLUnicodeString(str);
}

inline size_t
XMLUnicodeString::size() const
{
  return xercesc::XMLString::stringLen(buf_);
}

/*
 *  Helper inline methods
 */
inline std::string
delete_begin_end_spaces(const std::string& str)
{
  std::string::const_iterator begin = str.begin();
  std::string::const_iterator end = str.end();

  // Skip every character which is not alpha, digit or underscore
  while (!((begin == end) || isalpha(*begin) || isdigit(*begin) ||
           (*begin == '_')))
    ++begin;

  // Skip every character which is not alpha, digit or underscore
  --end;
  while (!((begin == end) || isalpha(*end) || isdigit(*end) ||
           (*end == '_')))
    --end;

  // Create a new string and return it
  return std::string(begin, ++end);
}
