// $Id$

#ifndef ACEXML_EC_PROPERTY_H
#define ACEXML_EC_PROPERTY_H

#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"
#include "ACEXML/compass/Property.h"

class Compass_Export ACEXML_EC_Property : public ACEXML_Property
{
public:
  ACEXML_EC_Property();
  virtual ~ACEXML_EC_Property();
  virtual int set (const ACEXML_String& property, const ACEXML_String& value);
  virtual int set (const ACEXML_String& property, const long value);
  virtual ACEXML_Char* dump() const;
private:
  ACEXML_String ec_dispatching_;
  ACEXML_String ec_filtering_;
  ACEXML_String ec_supplier_filtering_;
  ACEXML_String ec_proxy_consumer_lock_;
  ACEXML_String ec_proxy_supplier_lock_;
  ACEXML_String ec_consumer_control_;
  ACEXML_String ec_supplier_control_;
  long ec_consumer_control_period_;
  long ec_supplier_control_period_;
};

#include "ace/post.h"

#endif /* ACEXML_EC_PROPERTY_H */
