// $Id$

#include <ostream>
#include <memory>


#include "PICML/XercesString.h"

using xercesc::XMLString;

namespace PICML
{

  XStr::XStr (const char* str)
    : _wstr(0)
  {
    _wstr = XMLString::transcode(str);
  }

  XStr::XStr (std::string& str)
    : _wstr (0)
  {
    _wstr = XMLString::transcode (str.c_str());
  }

  XStr::XStr (XMLCh *wstr)
    : _wstr(wstr)
  {

  }

  XStr::XStr (const XMLCh* wstr)
    : _wstr(0)
  {
    _wstr = XMLString::replicate(wstr);
  }

  XStr::XStr (const XStr &right)
    : _wstr(0)
  {
    _wstr = XMLString::replicate(right._wstr);
  }

  XStr& XStr::operator= (const XStr& rhs)
  {
    if (&rhs == this)
      return *this;
    XStr temp (rhs);
    std::swap (this->_wstr, temp._wstr);
    return *this;
  }

  XStr::~XStr ()
  {
    if (_wstr)
      XMLString::release(&_wstr);
  }

  const XMLCh* XStr::begin () const
  {
    return _wstr;
  }

  const XMLCh* XStr::end () const
  {
    return _wstr + size();
  }

  bool XStr::append(const XMLCh *tail)
  {
    int iTailLen = XMLString::stringLen(tail);
    int iWorkLen = XMLString::stringLen(_wstr);
    XMLCh *result = new XMLCh[ iWorkLen + iTailLen + 1 ];
    bool bOK = result != 0;
    if (bOK)
      {
        XMLCh *target = result;
        XMLString::moveChars(target, _wstr, iWorkLen);
        target += iWorkLen;
        XMLString::moveChars(target, tail, iTailLen);
        target += iTailLen;
        *target++ = 0;
        XMLString::release(&_wstr);
        _wstr = result;
      }
    return bOK;
  }

  bool XStr::erase(const XMLCh *head, const XMLCh *tail)
  {
    bool bOK = head <= tail && head >= begin() && tail <= end();
    if (bOK)
      {
        XMLCh *result = new XMLCh[ size() - (tail - head) + 1 ];
        XMLCh *target = result;
        bOK = target != NULL;
        if (bOK)
          {
            const XMLCh *cursor = begin();

            while (cursor != head) *target++ = *cursor++;
            cursor = tail;
            while ( cursor != end() ) *target++ = *cursor++;
            *target ++ = 0;
            XMLString::release(&_wstr);
            _wstr = result;
          }
      }
    return bOK;
  }

  int XStr::size () const
  {
    return XMLString::stringLen(_wstr);
  }

  XMLCh XStr::operator [] (const int i)
  {
    return _wstr[i];
  }

  const XMLCh XStr::operator [] (const int i) const
  {
    return _wstr[i];
  }

  bool XStr::operator== (const XMLCh* wstr) const
  {
    return XMLString::compareIString (wstr, this->_wstr) == 0;
  }

  bool operator== (const XStr& lhs, const XStr& rhs)
  {
    return XMLString::compareIString (lhs,rhs) == 0;
  }

  bool operator!= (const XStr& lhs, const XStr& rhs)
  {
    return !operator==(lhs, rhs);
  }

  char* XStr::c_str() const
  {
	return XMLString::transcode (_wstr);
  }

  XStr::operator const XMLCh* () const
  {
	return _wstr;
  };

  std::ostream&
  operator<< (std::ostream& o, XStr const& str)
  {
    std::string conv (XMLString::transcode (str));
    o << conv;
    return o;
  }
}
