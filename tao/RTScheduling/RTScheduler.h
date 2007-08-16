// -*- C++ -*-

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
#include /**/ "ace/pre.h"

#include "tao/RTScheduling/rtscheduler_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_RTScheduler_Export TAO_RTScheduler_Initializer
{
public:
  // Constructor.
  TAO_RTScheduler_Initializer (void);
};

static TAO_RTScheduler_Initializer TAO_RTScheduler_initializer;

TAO_END_VERSIONED_NAMESPACE_DECL

#define TAO_RTSCHEDULER_SAFE_INCLUDE
#include "tao/RTScheduling/RTSchedulerC.h"
#undef TAO_RTSCHEDULER_SAFE_INCLUDE

#include /**/ "ace/post.h"
#endif /* TAO_RTSCHEDULER_H_ */
