// -*- C++ -*-

//=============================================================================
/**
 *  @file    PortableServer_Functions.h
 *
 *  PortableServer helper functions
 *
 *  @author  Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_PORTABLESERVER_FUNCTIONS_H
#define TAO_PORTABLESERVER_FUNCTIONS_H

#include /**/ "ace/pre.h"

#include "tao/PortableServer/portableserver_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#include "tao/PortableServer/PS_ForwardC.h"
#include "ace/OS_NS_wchar.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableServer
{
  /// Utility functions for the other
  extern TAO_PortableServer_Export char* ObjectId_to_string (
    const PortableServer::ObjectId &id);

  extern TAO_PortableServer_Export PortableServer::ObjectId *string_to_ObjectId (
    const char *id);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_FUNCTIONS_H */
