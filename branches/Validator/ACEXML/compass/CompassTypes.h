// $Id$

#ifndef COMPASS_TYPES_H
#define COMPASS_TYPES_H

#include "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ACEXML/common/XML_Types.h"

// *************** Packaging and Deployment ***************

namespace Deployment
{
  typedef ACEXML_String UUID;
  typedef ACEXML_String Location;
  typedef unsigned long FailureReason;
  typedef ACEXML_String Cookie;

  enum AssemblyState
    {
      INACTIVE,
      INSERVICE
    };

  typedef ACEXML_String FeatureName;

  struct UnknownImplId {};
  struct InvalidLocation {};
  struct InstallationFailure { FailureReason reason; };
  struct InvalidAssembly {};
  struct RemoveFailure { FailureReason reason; };

  struct ImplEntryPointNotFound {};

}

#include "ace/post.h"

#endif /* COMPASS_TYPES_H */
