// $Id$

#ifndef ACEXML_PROPERTY_H
#define ACEXML_PROPERTY_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"
#include "ace/ARGV.h"

class ACEXML_Property
{
public:
  ACEXML_Property(void);
  virtual ~ACEXML_Property();
  virtual set (const ACEXML_String& property, const ACEXML_String& value) = 0;
  virtual ACE_ARGV* dump() const = 0;
}

#include "ace/post.h"

#endif /* ACEXML_PROPERTY_H */
