#include <ostream>
#include <utility>

#include "XercesString.h"
#include "xercesc/util/PlatformUtils.hpp"

namespace XML
{
// On unicode windows, ACE_TCHAR == XMLCh
#if !defined(_MSC_VER) || !defined(ACE_USES_WCHAR)
  XStr::XStr (ACE_TCHAR const *str)
    : _wstr (xercesc::XMLString::transcode (ACE_TEXT_ALWAYS_CHAR (str)))
  {
  }
#endif

  XStr::XStr (XMLCh *wstr)
    : _wstr (wstr)
  {
  }

  XStr::XStr (XMLCh const *wstr)
    : _wstr (xercesc::XMLString::replicate (wstr))
  {
  }

  XStr::XStr (XStr const &right)
    : _wstr (xercesc::XMLString::replicate (right._wstr.get ()))
  {
  }

  XStr::XStr (XStr &&other) noexcept
    : _wstr (std::move (other._wstr))
  {
  }

  XStr &XStr::operator= (XStr const &rhs)
  {
    if (std::addressof (rhs) != this)
    {
      XStr temp (rhs);
      std::swap (this->_wstr, temp._wstr);
    }

    return *this;
  }

  XStr &XStr::operator= (XStr &&rhs) noexcept
  {
    if (this != std::addressof (rhs))
    {
      _wstr = std::move (rhs._wstr);
    }

    return *this;
  }

  XStr::~XStr () = default;

  XMLCh const *XStr::begin () const
  {
    return _wstr.get ();
  }

  XMLCh const *XStr::end () const
  {
    XMLCh const *const wstr = _wstr.get ();
    return wstr == nullptr ? nullptr : wstr + size ();
  }

  bool XStr::append (XMLCh const *tail)
  {
    if (tail == nullptr)
      return false;

    XMLSize_t const iTailLen (xercesc::XMLString::stringLen (tail));
    XMLSize_t const iWorkLen (
      _wstr == nullptr ? 0 : xercesc::XMLString::stringLen (_wstr.get ()));

    XMLSize_t const bytes ((iWorkLen + iTailLen + 1) * sizeof (XMLCh));
    detail::Xerces_String result {
      static_cast<XMLCh *> (xercesc::XMLPlatformUtils::fgMemoryManager->allocate (bytes))
    };

    if (!result)
      return false;

    XMLCh *target = result.get ();
    if (iWorkLen != 0)
      xercesc::XMLString::moveChars (target, _wstr.get (), iWorkLen);
    target += iWorkLen;
    xercesc::XMLString::moveChars (target, tail, iTailLen);
    target += iTailLen;
    *target = 0;
    _wstr = std::move (result);
    return true;
  }

  bool XStr::erase (XMLCh const *head, XMLCh const *tail)
  {
    bool const bOK = head <= tail && head >= begin () && tail <= end ();
    if (!bOK)
      return false;

    XMLSize_t const bytes ((size () - (tail - head) + 1) * sizeof (XMLCh));
    detail::Xerces_String result {
      static_cast<XMLCh *> (xercesc::XMLPlatformUtils::fgMemoryManager->allocate (bytes))
    };

    if (!result)
      return false;

    XMLCh *target = result.get ();
    XMLCh const *cursor = begin ();

    while (cursor != head)
      *target++ = *cursor++;
    cursor = tail;
    while (cursor != end ())
      *target++ = *cursor++;
    *target = 0;

    _wstr = std::move (result);
    return true;
  }

  XMLSize_t XStr::size () const
  {
    if (_wstr == nullptr)
      return XMLSize_t{};

    return xercesc::XMLString::stringLen (_wstr.get ());
  }

  XMLCh XStr::operator[] (XMLSize_t i)
  {
    return _wstr.get ()[i];
  }

  XMLCh XStr::operator[] (XMLSize_t i) const
  {
    return _wstr.get ()[i];
  }

  XMLCh *XStr::release ()
  {
    return this->_wstr.release ();
  }

  void XStr::reset ()
  {
    this->_wstr.reset ();
  }

  bool operator== (XStr const &lhs, XStr const &rhs)
  {
    return xercesc::XMLString::compareIString (lhs, rhs) == 0;
  }

  bool operator!= (XStr const &lhs, XStr const &rhs)
  {
    return !(lhs == rhs);
  }

  std::ostream &operator<< (std::ostream &o, XStr const &str)
  {
    detail::Transcoded_String const s (xercesc::XMLString::transcode (str));
    o << s.get ();
    return o;
  }

}
