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

#include "tao/PI_Server/pi_server_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PI_Server_Init
 *
 */
class TAO_PI_Server_Export TAO_PI_Server_Init
{
public:

  /// Used to force the initialization of the ORB code.
  static int Initializer (void);
};

static int
TAO_Requires_PI_Server_Initializer =
  TAO_PI_Server_Init::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

#define TAO_PI_SERVER_SAFE_INCLUDE
#include "tao/PI_Server/ServerRequestInterceptorC.h"
#include "tao/PI_Server/ServerRequestInfoC.h"
#undef TAO_PI_SERVER_SAFE_INCLUDE

#include /**/ "ace/post.h"

#endif  /* TAO_PI_SERVER_H */
