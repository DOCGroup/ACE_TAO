/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    RTScheduler_Loader.h
 *
 *  $Id$
 *
 *   Header file for Loading RTScheduler.
 *
 *
 *  @author  Yamuna Krishnamurthy <yamuna@oomworks.com>
 */
//=============================================================================

#ifndef TAO_RTSCHEDULER_LOADER_H
#define TAO_RTSCHEDULER_LOADER_H
#include /**/ "ace/pre.h"

#include "rtscheduler_export.h"


#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORB_Core;

class TAO_RTScheduler_Export TAO_RTScheduler_Loader : public ACE_Service_Object
{
public:
  /// Constructor.
  TAO_RTScheduler_Loader (void);

  /// Destructor.
  virtual ~TAO_RTScheduler_Loader (void);

  /// Initialize the RTScheduler loader hooks.
  virtual int init (int argc,
                    ACE_TCHAR* []);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_RTScheduler, TAO_RTScheduler_Loader)
ACE_FACTORY_DECLARE (TAO_RTScheduler, TAO_RTScheduler_Loader)

#include /**/ "ace/post.h"
#endif /* TAO_RTSCHEDULER_LOADER_H */
