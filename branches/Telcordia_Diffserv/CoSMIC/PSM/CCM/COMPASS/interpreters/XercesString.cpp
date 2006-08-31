#include "XercesString.h"
#include <stddef.h>
#include <stdlib.h>

#ifdef XERCES_CPP_NAMESPACE_USE
XERCES_CPP_NAMESPACE_USE
#endif

XStr::XStr(const char *str) : _wstr(NULL)
{
  _wstr = XMLString::transcode(str);
}

XStr::XStr(XMLCh *wstr) : _wstr(wstr) { };

XStr::XStr(const XMLCh *wstr) : _wstr(NULL)
{
  _wstr = XMLString::replicate(wstr);
}

XStr::XStr(const XStr &right) : _wstr(NULL)
{
  _wstr = XMLString::replicate(right._wstr);
}

XStr::~XStr()
{
  // thanks tinny!!
  if (_wstr) XMLString::release(&_wstr);
}

bool XStr::append(const XMLCh *tail)
{
  int iTailLen = XMLString::stringLen(tail);
  int iWorkLen = XMLString::stringLen(_wstr);
  XMLCh *result = new XMLCh[ iWorkLen + iTailLen + 1 ];
  bool bOK = result != NULL;
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

const XMLCh* XStr::begin() const
{
  return _wstr;
}

const XMLCh* XStr::end() const
{
  return _wstr + size();
}

int XStr::size() const
{
  return XMLString::stringLen(_wstr);
}

XMLCh & XStr::operator [] (const int i)
{
  return _wstr[i];
}

const XMLCh XStr::operator [] (const int i) const
{
  return _wstr[i];
}
