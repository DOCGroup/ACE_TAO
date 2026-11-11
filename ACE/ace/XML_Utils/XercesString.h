#ifndef _XERCESSTRING_H
#define _XERCESSTRING_H

#include /**/ "ace/pre.h"

#if !defined(ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <iosfwd>
#include <memory>
#include <xercesc/util/XMLString.hpp>

// Utility class that provides a std::string like facade to XMLString.
// Doesn't implement all of the methods of std::string.
#include "XML_Utils_Export.h"

namespace XML
{
  namespace detail
  {
    struct Transcoded_String_Deleter
    {
      void operator() (char *str) const noexcept
      {
        xercesc::XMLString::release (&str);
      }
    };

    struct Xerces_String_Deleter
    {
      void operator() (XMLCh *str) const noexcept
      {
        xercesc::XMLString::release (&str);
      }
    };

    using Transcoded_String = std::unique_ptr<char, Transcoded_String_Deleter>;
    using Xerces_String = std::unique_ptr<XMLCh, Xerces_String_Deleter>;
  }

  class XML_Utils_Export XStr
  {
  public:
    XStr () = default;

// On unicode windows, ACE_TCHAR == XMLCh
#if !defined(_MSC_VER) || !defined(ACE_USES_WCHAR)
    explicit XStr (ACE_TCHAR const *str);
#endif

    explicit XStr (XMLCh *wstr);

    explicit XStr (XMLCh const *wstr);

    XStr (XStr const &copy);
    XStr (XStr &&other) noexcept;

    XStr &operator= (XStr const &rhs);
    XStr &operator= (XStr &&rhs) noexcept;

    ~XStr ();

    XMLCh const *begin () const;

    XMLCh const *end () const;

    bool append (XMLCh const *tail);

    bool erase (XMLCh const *head, XMLCh const *tail);

    XMLSize_t size () const;

    XMLCh operator[] (XMLSize_t i);

    XMLCh operator[] (XMLSize_t i) const;

    operator XMLCh const * () const
    {
      return _wstr.get ();
    }

    XMLCh *release ();

    /// Release the held string and free its memory.
    void reset ();

  private:
    detail::Xerces_String _wstr {}; // Internal representation
  };

  XML_Utils_Export bool operator== (XStr const &lhs, XStr const &rhs);
  XML_Utils_Export bool operator!= (XStr const &lhs, XStr const &rhs);

  XML_Utils_Export std::ostream &operator<< (std::ostream &o, XStr const &str);
}

#include /**/ "ace/post.h"

#endif /* _XERCESSTRING_H */
