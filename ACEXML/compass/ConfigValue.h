// $Id$

#ifndef COMPASS_CONFIG_VALUE_H
#define COMPASS_CONFIG_VALUE_H

#include "ace/pre.h"
#include "ACEXML/compass/Compass_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

namespace Deployment
{
  class Compass_Export ConfigValue
  {
  public:
    ConfigValue();
    ~ConfigValue();
    ConfigValue (const ConfigValue& config);
    ConfigValue& operator= (const ConfigValue& config);
    int operator== (const ConfigValue& config);

    FeatureName name_;
    ACEXML_String value_;
  };

  typedef ACE_Unbounded_Set<ConfigValue> ConfigValues;
}

#include "ace/post.h"

#endif /* COMPASS_CONFIG_VALUE_H */
