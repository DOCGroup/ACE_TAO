// $Id$

#ifndef _XERCESSTRING_H
#define _XERCESSTRING_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <iosfwd>

#include <xercesc/util/XMLString.hpp>

// Utility class that provides a std::string like facade to XMLString.
// Doesn't implement all of the methods of std::string.
#include "CIAO_XML_Utils_Export.h"
namespace CIAO
{
namespace Config_Handlers
{

  class CIAO_XML_Utils_Export XStr
  {
  public:
    XStr() : _wstr(0L) { };

    XStr (const char* str);

    XStr (XMLCh* wstr);

    XStr (const XMLCh* wstr);

    XStr (const XStr& copy);

    XStr& operator= (const XStr& rhs);

    ~XStr();

    const XMLCh* begin() const;

    const XMLCh* end() const;

    bool append(const XMLCh* tail);

    bool erase (const XMLCh* head, const XMLCh* tail);

    int size() const;

    XMLCh operator [] (const int i);

    XMLCh operator [] (const int i) const;

    operator const XMLCh* () const { return _wstr; };

  private:

    XMLCh* _wstr; // Internal representation

  };

  CIAO_XML_Utils_Export bool operator== (const XStr& lhs, const XStr& rhs);
  CIAO_XML_Utils_Export bool operator!= (const XStr& lhs, const XStr& rhs);

  CIAO_XML_Utils_Export std::ostream&
  operator<< (std::ostream& o, XStr const& str);

}
}
#include /**/ "ace/post.h"

#endif /* _XERCESSTRING_H */
