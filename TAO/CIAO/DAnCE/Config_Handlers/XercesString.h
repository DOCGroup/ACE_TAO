// $Id$
//==============================================================
/**
 *  @file  XercesString.h
 *
 *  $Id$
 *
 *  @brief Helper method for XerceString
 *
 *  @author Bala Natarajan <bala@dre.vanderbilt.edu>
 */
//================================================================
#ifndef CIAO_CONFIG_HANLDERS_XERCES_STRING_H
#define CIAO_CONFIG_HANLDERS_XERCES_STRING_H
#include /**/ "ace/pre.h"

#include "ace/Swap.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <iosfwd>

#include <xercesc/util/XMLString.hpp>

namespace CIAO
{
  namespace Config_Handlers
  {
    /**
     * @class XStr
     *
     * @brief
     *
     */
    class Config_Handlers_Export XStr
    {
    public:
      XStr() : _wstr(0L) { };

      XStr (const char* str);

      XStr (XMLCh* wstr);

      XStr (const XMLCh* wstr);

      XStr (const XStr& copy);

      XStr& operator= (const XStr& rhs);

      ~XStr();

      const XMLCh* begin () const;

      const XMLCh* end () const;

      bool append (const XMLCh* tail);

      bool erase (const XMLCh* head, const XMLCh* tail);

      int size () const;

      XMLCh operator [] (const int i);

      const XMLCh operator [] (const int i) const;

      operator const XMLCh* () const { return _wstr; };

      bool operator== (const XMLCh* wstr) const;

    private:

      XMLCh* _wstr; // Internal representation
    };

    bool operator== (const XStr& lhs, const XStr& rhs);
    bool operator!= (const XStr& lhs, const XStr& rhs);

    std::ostream& operator<< (std::ostream& o, XStr const& str);
  }
}

#include /**/ "ace/post.h"

#endif /* _XERCESSTRING_H */
