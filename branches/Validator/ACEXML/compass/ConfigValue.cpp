// $Id$

#include "ACEXML/compass/ConfigValue.h"

using namespace Deployment;

ConfigValue::ConfigValue()
  : name_(), value_()
{

}

ConfigValue::~ConfigValue()
{
  delete this->name_;
  delete this->value_;
}

ConfigValue::ConfigValue (const ConfigValue& config)
{
  if (*this != config)
    {
      this->name_ = config.name_;
      this->value_ = config.value_;
    }
  return *this;
}

ConfigValue&
ConfigValue::operator= (const ConfigValue& config)
{
  this->name_ = config.name_;
  this->value_ = config.value_;
  return *this;
}

int
ConfigValue::operator== (const ConfigValue& config)
{
  return (this->name_ == config.name_ && this->value_ == config.value_);
}
