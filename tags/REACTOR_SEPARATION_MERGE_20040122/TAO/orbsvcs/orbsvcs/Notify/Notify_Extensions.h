// -*- C++ -*-
// $Id$

#ifndef TAO_NOTIFY_EXTENSIONS_H
#define TAO_NOTIFY_EXTENSIONS_H

#include /**/ "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#if defined (TAO_EXPORT_MACRO)
# undef TAO_EXPORT_MACRO
#endif /* TAO_EXPORT_MACRO */
#define TAO_EXPORT_MACRO TAO_Notify_Export

namespace TAO_Notify_Extensions
{
  TAO_NAMESPACE_STORAGE_CLASS const char *const BlockingPolicy;
}

#include /**/ "ace/post.h"

#endif /* TAO_NOTIFY_EXTENSIONS_H */
