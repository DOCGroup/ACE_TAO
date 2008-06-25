// $Id$

#ifndef TIMESTAMP_MONITOR_H
#define TIMESTAMP_MONITOR_H

#include /**/ "ace/pre.h"

#include "ace/Monitor_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined (TAO_HAS_MONITOR_FRAMEWORK) && (TAO_HAS_MONITOR_FRAMEWORK == 1)

#include "tao/Versioned_Namespace.h"

#include "orbsvcs/orbsvcs/Notify/MonitorControl/notify_mc_export.h"

using namespace ACE_VERSIONED_NAMESPACE_NAME::ACE::Monitor_Control;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Notify_MC_Export Timestamp_Monitor : public Monitor_Base
{
public:
  Timestamp_Monitor (const char *name);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_MONITOR_FRAMEWORK==1 */

#include /**/ "ace/post.h"

#endif /* TIMESTAMP_MONITOR_H */
