// -*- C++ -*-

// ===================================================================
/**
 *  @file   PI_Server.h
 *
 *  $Id$
 *
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_PI_SERVER_H
#define TAO_PI_SERVER_H

#include /**/ "ace/pre.h"

#include "pi_server_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_PI_Init
 *
 */
class TAO_PI_Server_Export TAO_PI_Server_Init
{
public:

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

#if defined(ACE_HAS_BROKEN_STATIC_CONSTRUCTORS)

typedef int (*TAO_Module_Initializer) (void);

static TAO_Module_Initializer
TAO_Requires_PI_Server_Initializer =
  &TAO_PI_Server_Init::Initializer;

#else

static int
TAO_Requires_PI_Server_Initializer =
  TAO_PI_Server_Init::Initializer ();

#endif /* ACE_HAS_BROKEN_STATIC_CONSTRUCTORS */

#define TAO_PI_SERVER_SAFE_INCLUDE
#include "PS_CurrentC.h"
#include "ServerRequestInterceptorC.h"
#undef TAO_PI_SERVER_SAFE_INCLUDE

#include /**/ "ace/post.h"

#endif  /* TAO_PI_SERVER_H */
