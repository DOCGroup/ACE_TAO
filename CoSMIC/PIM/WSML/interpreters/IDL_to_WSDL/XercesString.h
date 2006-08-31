// $Id$

#ifndef _XERCESSTRING_H
#define _XERCESSTRING_H

#include /**/ "ace/pre.h"

#include "ace/Swap.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <xercesc/util/XMLString.hpp>

// Utility class that provides a std::string like facade to XMLString.
// Doesn't implement all of the methods of std::string.

class XStr
{
public:
  XStr (void) : _wstr (0L) { };

  XStr (const char* str);

  XStr (XMLCh* wstr);

  XStr (const XMLCh* wstr);

  XStr (const XStr& copy);

  XStr& operator= (const XStr& rhs);

  ~XStr (void);

  const XMLCh* begin (void) const;

  const XMLCh* end (void) const;

  bool append (const XMLCh* tail);

  bool erase (const XMLCh* head, const XMLCh* tail);

  int size (void) const;
  
  XMLCh operator [] (const int i);

  const XMLCh operator [] (const int i) const;

  operator const XMLCh* () const { return _wstr; };

  bool operator== (const XMLCh* wstr) const;


private:

  XMLCh* _wstr; // Internal representation

};

bool operator== (const XStr& lhs, const XStr& rhs);
bool operator!= (const XStr& lhs, const XStr& rhs);

#include /**/ "ace/post.h"

#endif /* _XERCESSTRING_H */
