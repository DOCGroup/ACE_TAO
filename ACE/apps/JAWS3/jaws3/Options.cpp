#include "ace/OS_NS_stdlib.h"

#ifndef JAWS_BUILD_DLL
#define JAWS_BUILD_DLL
#endif /*JAWS_BUILD_DLL*/

#include "jaws3/Options.h"
#include <memory>

JAWS_Options::JAWS_Options ()
{
  this->cf_ = std::make_unique<JAWS_Config_File> ("jaws.conf");
}

char const *
JAWS_Options::getenv (char const *key)
{
  char const *value {};
  if (this->cf_ == nullptr || this->cf_->find (key, value) < 0)
  {
    value = ACE_OS::getenv (key);
  }

  return value;
}

