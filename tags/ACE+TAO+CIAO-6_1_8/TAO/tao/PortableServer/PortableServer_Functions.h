// -*- C++ -*-

//=============================================================================
/**
 *  @file    PortableServer_Functions.h
 *
 *  $Id$
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

#if defined (TAO_EXPORT_MACRO)
#undef TAO_EXPORT_MACRO
#endif
#define TAO_EXPORT_MACRO TAO_PortableServer_Export

#if defined (TAO_EXPORT_NESTED_CLASSES)
#  if defined (TAO_EXPORT_NESTED_MACRO)
#    undef TAO_EXPORT_NESTED_MACRO
#  endif /* defined (TAO_EXPORT_NESTED_MACRO) */
#  define TAO_EXPORT_NESTED_MACRO TAO_PortableServer_Export
#endif /* TAO_EXPORT_NESTED_CLASSES */

#include "tao/PortableServer/PS_ForwardC.h"
#include "ace/OS_NS_wchar.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableServer
{
  /// Utility functions for the other
  TAO_NAMESPACE_STORAGE_CLASS char* ObjectId_to_string (
    const PortableServer::ObjectId &id);

  TAO_NAMESPACE_STORAGE_CLASS PortableServer::ObjectId *string_to_ObjectId (
    const char *id);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_PORTABLESERVER_FUNCTIONS_H */
