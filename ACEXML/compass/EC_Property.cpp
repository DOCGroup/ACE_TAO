// $Id$

#include "ACEXML/compass/EC_Property.h"
#include "ace/OS_String.h"

ACEXML_EC_Property::ACEXML_EC_Property()
  : ec_consumer_control_period_ (0), ec_supplier_control_period_ (0)
{}

ACEXML_EC_Property::~ACEXML_EC_Property()
{}

int
ACEXML_EC_Property::set (const ACEXML_String& property,
                         const ACEXML_String& value)
{
  const ACEXML_Char* prop = property.c_str();
  if (ACE_OS::strcmp (prop, "ECDispatching") == 0)
    this->ec_dispatching_ = value;
  else if (ACE_OS::strcmp (prop, "ECFiltering") == 0)
    this->ec_filtering_ = value;
  else if (ACE_OS::strcmp (prop, "ECSupplierFiltering") == 0)
    this->ec_supplier_filtering_ = value;
  else if (ACE_OS::strcmp (prop, "ECProxyConsumerLock") == 0)
    this->ec_proxy_consumer_lock_ = value;
  else if (ACE_OS::strcmp (prop, "ECProxySupplierLock") == 0)
    this->ec_proxy_supplier_lock_ = value;
  else if (ACE_OS::strcmp (prop, "ECConsumerControl") == 0)
    this->ec_consumer_control_ = value;
  else if (ACE_OS::strcmp (prop, "ECSupplierControl") == 0)
    this->ec_supplier_control_ = value;
  else
    {
      ACE_ERROR ((LM_ERROR, "Unknown property %s in property file", prop));
      return -1;
    }
  return 0;
}

int
ACEXML_EC_Property::set (const ACEXML_String& property, const long value)
{
  const ACEXML_Char* prop = property.c_str();
  if (ACE_OS::strcmp (prop, "ECConsumerControlPeriod") == 0)
    this->ec_consumer_control_period_ = value;
  else if (ACE_OS::strcmp (prop, "ECSupplierControlPeriod") == 0)
    this->ec_supplier_control_period_ = value;
  else
    {
      ACE_ERROR ((LM_ERROR, "Unknown property %s in property file", prop));
      return -1;
    }
  return 0;
}

ACEXML_Char*
ACEXML_EC_Property::dump() const
{
  ACEXML_String argv ("-ECDispatching");
  argv += " ";
  argv += this->ec_dispatching_;
  argv += " ";
  argv += "-ECFiltering";
  argv += " ";
  argv += this->ec_filtering_;
  argv += " ";
  argv += "-ECSupplierFiltering";
  argv += " ";
  argv += this->ec_supplier_filtering_;
  argv += " ";
  argv += "-ECProxyConsumerLock";
  argv += " ";
  argv += this->ec_proxy_consumer_lock_;
  argv += " ";
  argv += "-ECProxySupplierLock";
  argv += " ";
  argv += this->ec_proxy_supplier_lock_;
  argv += " ";
  argv += "-ECConsumerControl";
  argv += " ";
  argv += this->ec_consumer_control_;
  argv += " ";
  argv += "-ECSupplierControl";
  argv += " ";
  argv += this->ec_supplier_control_;
  argv += " ";
  argv += "-ECConsumerControlPeriod";
  argv += " ";
  char temp[20] = {0};
  int pos = ACE_OS::sprintf (temp, "%ld", this->ec_consumer_control_period_);
  temp[pos] = 0;
  argv += temp;
  argv += " ";
  argv += "-ECSupplierControlPeriod";
  argv += " ";
  pos = ACE_OS::sprintf (temp, "%ld", this->ec_supplier_control_period_);
  temp[pos] = 0;
  argv += temp;
  argv += " ";
  return argv.rep();
}
