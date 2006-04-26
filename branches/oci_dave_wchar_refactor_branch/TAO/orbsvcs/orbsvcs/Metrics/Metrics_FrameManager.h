// $Id$

#ifndef METRICS_FRAME_MANAGER_H
#define METRICS_FRAME_MANAGER_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "ace/Singleton.h"

#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/RtecSchedulerC.h"

#include "orbsvcs/Metrics/Metrics_Utils.h"

#if defined (__ACE_INLINE__)
#include "orbsvcs/Metrics/Metrics_FrameManager.i"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/Metrics/Metrics_FrameManager_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

typedef TAO_Metrics_FrameManager<ACE_SYNCH_MUTEX>
TAO_METRICS_FRAME_MGR_TYPE;

typedef ACE_Singleton<TAO_METRICS_FRAME_MGR_TYPE, ACE_SYNCH_MUTEX>
TAO_METRICS_FRAME_MGR_SINGLETON;

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* METRICS_FRAME_MANAGER_H */

