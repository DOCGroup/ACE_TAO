/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PI_Server_Loader.h
 *
 *  $Id$
 *
 *   Header file for Loading RTScheduler.
 *
 *
 *  @author  Yamuna Krishnamurthy <yamuna@oomworks.com>
 */
//=============================================================================

#ifndef TAO_PI_SERVER_LOADER_H
#define TAO_PI_SERVER_LOADER_H
#include /**/ "ace/pre.h"

#include "pi_server_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Service_Object.h"
#include "ace/Service_Config.h"

class TAO_PI_Server_Export TAO_PI_Server_Loader : public ACE_Service_Object
{
public:
  /// Constructor.
  TAO_PI_Server_Loader (void);

  /// Destructor.
  virtual ~TAO_PI_Server_Loader (void);

  /// Initialize the PI_Server loader hooks.
  virtual int init (int argc,
                    ACE_TCHAR* []);
};

ACE_STATIC_SVC_DECLARE_EXPORT (TAO_PI_Server, TAO_PI_Server_Loader)
ACE_FACTORY_DECLARE (TAO_PI_Server, TAO_PI_Server_Loader)

#include /**/ "ace/post.h"
#endif /* TAO_PI_SERVER_LOADER_H */
