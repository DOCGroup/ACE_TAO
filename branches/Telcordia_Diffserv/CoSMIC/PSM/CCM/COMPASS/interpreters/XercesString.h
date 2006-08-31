#ifndef _XERCESSTRING_H
#define _XERCESSTRING_H

#include <xercesc/util/XMLString.hpp>

#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

class XStr
{
  XMLCh *_wstr;
 public:
  XStr() : _wstr(0L) { };
  XStr(const char *str);
  XStr(XMLCh *wstr);
  XStr(const XMLCh *wstr);
  XStr(const XStr &copy);
  ~XStr();
  bool append(const XMLCh *tail);
  bool erase(const XMLCh *head, const XMLCh *tail);
  const XMLCh* begin() const;
  const XMLCh* end() const;
  int size() const;
  XMLCh & operator [] (const int i);
  const XMLCh operator [] (const int i) const;
  operator const XMLCh * () const { return _wstr; };
  operator const char* () const { return XMLString::transcode (_wstr); };
};

#endif
