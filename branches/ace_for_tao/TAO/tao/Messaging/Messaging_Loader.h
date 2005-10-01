/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Messaging_Loader.h
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

#include "messaging_export.h"

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Messaging_Export TAO_Messaging_Loader : public ACE_Service_Object
{
public:
  /// Constructor.
  TAO_Messaging_Loader (void);

  /// Destructor.
  virtual ~TAO_Messaging_Loader (void);

  /// Initialize the Messaging loader hooks.
  virtual int init (int argc,
                    ACE_TCHAR* []);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_Messaging, TAO_Messaging_Loader)
ACE_FACTORY_DECLARE (TAO_Messaging, TAO_Messaging_Loader)

#include /**/ "ace/post.h"
#endif /* TAO_RTSCHEDULER_LOADER_H */
