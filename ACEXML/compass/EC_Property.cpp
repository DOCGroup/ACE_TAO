// $Id$

#include "ACEXML/compass/EC_Property.h"
#include "ace/OS_String.h"

ACEXML_EC_Property::ACEXML_EC_Property()
  : ec_consumer_control_period_ (0), ec_supplier_control_period_ (0),
    ec_dispatching_threads_ (0)
{}

ACEXML_EC_Property::~ACEXML_EC_Property()
{}

int
ACEXML_EC_Property::set (const ACEXML_String& property,
                         const ACEXML_String& value)
{
  const ACEXML_Char* prop = property.c_str();
  const ACEXML_Char* val = value.c_str();
  if (ACE_OS::strcasecmp (prop, "ECDispatching") == 0)
    {
      if (ACE_OS::strcasecmp (val, "reactive") == 0 ||
          ACE_OS::strcasecmp (val, "mt") == 0)
        {
          this->ec_dispatching_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECFiltering") == 0)
    {
      if (ACE_OS::strcasecmp (val, "null") == 0 ||
          ACE_OS::strcasecmp (val, "basic") == 0 ||
          ACE_OS::strcasecmp (val, "prefix") == 0)
        {
          this->ec_filtering_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECSupplierFiltering") == 0 ||
           ACE_OS::strcasecmp (prop, "ECSupplierFilter") == 0)
    {
      if (ACE_OS::strcasecmp (val, "null") == 0 ||
          ACE_OS::strcasecmp (val, "per-supplier") == 0)
        {
          this->ec_supplier_filtering_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECTimeout") == 0)
    {
      if (ACE_OS::strcasecmp (val, "reactive") == 0)
        {
          this->ec_timeout_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECObserver") == 0)
    {
      if (ACE_OS::strcasecmp (val, "null") == 0 ||
          ACE_OS::strcasecmp (val, "basic") == 0)
        {
          this->ec_observer_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECScheduling") == 0)
    {
      if (ACE_OS::strcasecmp (val, "null") == 0 ||
          ACE_OS::strcasecmp (val, "group") == 0)
        {
          this->ec_scheduling_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECProxyPushConsumerCollection") == 0)
    {
      if (this->check_collection (val) == 0)
        {
          this->ec_proxy_push_consumer_collection_  = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECProxyPushSupplierCollection") == 0)
    {
      if (this->check_collection (val) == 0)
        {
          this->ec_proxy_push_supplier_collection_  = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECProxyConsumerLock") == 0)
    {
      if (ACE_OS::strcasecmp (val, "null") == 0 ||
          ACE_OS::strcasecmp (val, "thread") == 0 ||
          ACE_OS::strcasecmp (val, "recursive") == 0)
        {
          this->ec_proxy_consumer_lock_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECProxySupplierLock") == 0)
    {
      if (ACE_OS::strcasecmp (val, "null") == 0 ||
          ACE_OS::strcasecmp (val, "thread") == 0 ||
          ACE_OS::strcasecmp (val, "recursive") == 0)
        {
          this->ec_proxy_supplier_lock_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECUseORBId") == 0)
    {
      this->ec_orbid_ = value;
      return 0;
    }
  else if (ACE_OS::strcasecmp (prop, "ECConsumerControl") == 0)
    {
      if (ACE_OS::strcasecmp (val, "null") == 0 ||
          ACE_OS::strcasecmp (val, "reactive") == 0)
        {
          this->ec_consumer_control_ = value;
          return 0;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECSupplierControl") == 0)
    {
      if (ACE_OS::strcasecmp (val, "null") == 0 ||
          ACE_OS::strcasecmp (val, "reactive") == 0)
        {
          this->ec_supplier_control_ = value;
          return 0;
        }
    }
  ACE_ERROR ((LM_ERROR, "Invalid property %s = %s\n", prop, val));
  return -1;
}

int
ACEXML_EC_Property::check_collection (const ACEXML_Char* str)
{
  ACEXML_Char* opt = ACE_OS::strdup (str);
  ACE_Auto_Basic_Array_Ptr<ACEXML_Char> cleanup_opt (opt);
  ACEXML_Char* aux;
  for (ACE_TCHAR* arg = ACE_OS::strtok_r (opt, ACE_TEXT(":"), &aux);
       arg != 0;
       arg = ACE_OS::strtok_r (0, ACE_TEXT(":"), &aux))
    {
      if (ACE_OS::strcasecmp (arg, ACE_TEXT("mt")) == 0
          || ACE_OS::strcasecmp (arg, ACE_TEXT("st")) == 0
          || ACE_OS::strcasecmp (arg, ACE_TEXT("list")) == 0
          || ACE_OS::strcasecmp (arg, ACE_TEXT("rb_tree")) == 0
          || ACE_OS::strcasecmp (arg, ACE_TEXT("immediate")) == 0
          || ACE_OS::strcasecmp (arg, ACE_TEXT("copy_on_read")) == 0
          || ACE_OS::strcasecmp (arg, ACE_TEXT("copy_on_write")) == 0
          || ACE_OS::strcasecmp (arg, ACE_TEXT("delayed")) == 0)
        continue;
      else
        return -1;
    }
  return 0;
}

int
ACEXML_EC_Property::set (const ACEXML_String& property, const long value)
{
  const ACEXML_Char* prop = property.c_str();
  if (ACE_OS::strcasecmp (prop, "ECConsumerControlPeriod") == 0)
    {
      this->ec_consumer_control_period_ = value;
      if (this->ec_consumer_control_period_ != 0 &&
          this->ec_consumer_control_period_ < 1000)
        {
          ACE_ERROR ((LM_ERROR, "Invalid configuration ECConsumerControlPeriod"
                      " = %d\n", this->ec_consumer_control_period_));
          return -1;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECSupplierControlPeriod") == 0)
    {
      this->ec_supplier_control_period_ = value;
      if (this->ec_supplier_control_period_ != 0 &&
          this->ec_supplier_control_period_ < 1000)
        {
          ACE_ERROR ((LM_ERROR, "Invalid configuration ECSupplierControlPeriod"
                      " = %d\n", this->ec_supplier_control_period_));
          return -1;
        }
    }
  else if (ACE_OS::strcasecmp (prop, "ECDispatchingThreads") == 0)
    {
      this->ec_dispatching_threads_ = value;
      if (this->ec_dispatching_threads_ <= 0)
        {
          ACE_ERROR ((LM_ERROR, "Invalid value ECDispatchingThreads = %d "
                      "when ECDispatching = mt\n",
                      this->ec_dispatching_threads_));
          return -1;
        }
    }
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
  ACEXML_String argv (" ");
  if (this->ec_dispatching_.length())
    {
      argv += " -ECDispatching ";
      argv += this->ec_dispatching_;
    }
  if (this->ec_filtering_.length())
    {
      argv += " -ECFiltering ";
      argv += this->ec_filtering_;
    }
  if (this->ec_supplier_filtering_.length())
    {
      argv += " -ECSupplierFilter ";
      argv += this->ec_supplier_filtering_;
    }
  if (this->ec_timeout_.length())
    {
      argv += " -ECTimeout ";
      argv += this->ec_timeout_;
    }
  if (this->ec_observer_.length())
    {
      argv += " -ECObserver ";
      argv += this->ec_observer_;
    }
  if (this->ec_scheduling_.length())
    {
      argv += " -ECScheduling ";
      argv += this->ec_scheduling_;
    }
  if (this->ec_proxy_push_consumer_collection_.length())
    {
      argv += " -ECProxyPushSupplierCollection ";
      argv += this->ec_proxy_push_consumer_collection_;
    }
  if (this->ec_proxy_push_supplier_collection_.length())
    {
      argv += " -ECProxyPushSupplierCollection ";
      argv += this->ec_proxy_push_supplier_collection_;
    }
  if (this->ec_proxy_consumer_lock_.length())
    {
      argv += " -ECProxyConsumerLock ";
      argv += this->ec_proxy_consumer_lock_;
    }
  if (this->ec_proxy_supplier_lock_.length())
    {
      argv += " -ECProxySupplierLock ";
      argv += this->ec_proxy_supplier_lock_;
    }
  if (this->ec_orbid_.length())
    {
      argv += " -ECUseORBId ";
      argv += this->ec_orbid_;
    }
  if (this->ec_consumer_control_.length())
    {
      argv += " -ECConsumerControl ";
      argv += this->ec_consumer_control_;
    }
  if (this->ec_supplier_control_.length())
    {
      argv += " -ECSupplierControl ";
      argv += this->ec_supplier_control_;
    }
  char temp[20] = {0};
  int pos = 0;
  if (this->ec_consumer_control_period_ > 0)
    {
      argv += " -ECConsumerControlPeriod ";
      pos += ACE_OS::sprintf (temp, "%ld", this->ec_consumer_control_period_);
      temp[pos] = 0;
      argv += temp;
    }
  if (this->ec_supplier_control_period_ > 0)
    {
      argv += " -ECSupplierControlPeriod ";
      pos += ACE_OS::sprintf (temp, "%ld", this->ec_consumer_control_period_);
      temp[pos] = 0;
      argv += temp;
    }
  if (this->ec_dispatching_threads_ > 0)
    {
      argv += " -ECDispatchingThreads ";
      pos += ACE_OS::sprintf (temp, "%ld", this->ec_dispatching_threads_);
      temp[pos] = 0;
      argv += temp;
    }
  return argv.rep();
}
