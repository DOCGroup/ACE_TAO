// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file LWFT_Client_Init.h
 *
 * $Id$
 *
 * Encapsulates LWFT replicated application initialization steps.
 *
 * @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_LWFT_SERVER_INIT_H
#define TAO_LWFT_SERVER_INIT_H

#include /**/ "ace/pre.h"

#include "lwft_server_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class LWFT_Server_Export LWFT_Server_Init
{
public:
  LWFT_Server_Init (void);
  ~LWFT_Server_Init (void);
  
  CORBA::ORB_ptr pre_init (int &argc, char *argv[]);
  
  /// Used to force the registration of interceptors.
  static int Initializer (void);

private:
  static bool initialized_;
};

static int
TAO_Requires_LWFT_Server_Initializer = LWFT_Server_Init::Initializer ();

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DECLARE (LWFT_Server_Init)
ACE_FACTORY_DECLARE (LWFT_Server, LWFT_Server_Init)

#include /**/ "ace/post.h"

#endif // TAO_LWFT_SERVER_INIT_H
