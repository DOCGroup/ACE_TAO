// $Id$

//=============================================================================
/**
 *  @file RTScheduler.h
 *
 *  $Id$
 *
 *   Takes care of the RT Scheduler loading on static build.
 *
 *
 *  @author  Yamuna Krishnamurthy <yamuna@oomworks.com>
 */
//=============================================================================

#ifndef TAO_RTSCHEDULER_H_
#define TAO_RTSCHEDULER_H_
#include "ace/pre.h"

#include "rtscheduler_export.h"
#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_RTScheduler_Export TAO_RTScheduler_Initializer
{
public:
  // Constructor.
  TAO_RTScheduler_Initializer (void);
};

static TAO_RTScheduler_Initializer TAO_RTScheduler_initializer;

#define TAO_RTSCHEDULER_SAFE_INCLUDE
#include "RTSchedulerC.h"
#undef TAO_RTSCHEDULER_SAFE_INCLUDE

#include "ace/post.h"
#endif /* TAO_RTSCHEDULER_H_ */


