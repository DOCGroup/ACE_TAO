// $Id$

#ifndef COMPASS_DEPLOYMENT_H
#define COMPASS_DEPLOYMENT_H

#include "ace/OS_String.h"
#include <vector>

// *************** Packaging and Deployment ***************

namespace Deployment
{
  typedef string UUID;
  typedef string Location;
  typedef unsigned long FailureReason;
  typedef string Cookie;

  enum AssemblyState
    {
      INACTIVE,
      INSERVICE
    };

  typedef string FeatureName;

  struct ConfigValue
  {
    FeatureName name;
    string value;
  };
  typedef vector<ConfigValue*> ConfigValues;

  struct UnknownImplId {};
  struct InvalidLocation {};
  struct InstallationFailure { FailureReason reason; };
  struct InvalidAssembly {};
  struct RemoveFailure { FailureReason reason; };

  struct ImplEntryPointNotFound {};

};

#endif /* COMPASS_DEPLOYMENT_H */
