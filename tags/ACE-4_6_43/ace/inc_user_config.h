/* -*- C++ -*- */
// $Id$

// Use this file to include config.h instead of including
// config.h directly.

#ifndef ACE_INC_USER_CONFIG_H
#define ACE_INC_USER_CONFIG_H

#include "ace/config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// By default, DO include RCS Id strings in object code.
#if ! defined (ACE_USE_RCSID)
#  define ACE_USE_RCSID 1
#endif /* #if ! defined (ACE_USE_RCSID) */

#if (defined (ACE_USE_RCSID) && (ACE_USE_RCSID != 0))
#  if ! defined (ACE_RCSID)

   // This hack has the following purposes:
   // 1. To define the RCS id string variable as a static char*, so
   //    that there won't be any duplicate extern symbols at link
   //    time.
   // 2. To have a RCS id string variable with a unique name for each
   //    file.
   // 3. To avoid warnings of the type "variable declared and never
   //    used".

#    define ACE_RCSID(path, file, id) \
      inline const char* get_rcsid_ ## path ## _ ## file (const char*) \
      { \
        return id ; \
      } \
      static const char* rcsid_ ## path ## _ ## file = \
        get_rcsid_ ## path ## _ ## file ( rcsid_ ## path ## _ ## file ) ;

#  endif /* #if ! defined (ACE_RCSID) */
#else

   // RCS id strings are not wanted.
#  if defined (ACE_RCSID)
#    undef ACE_RCSID
#  endif /* #if defined (ACE_RCSID) */
#  define ACE_RCSID(path, file, id) /* noop */
#endif /* #if (defined (ACE_USE_RCSID) && (ACE_USE_RCSID != 0)) */

#endif /* ACE_INC_USER_CONFIG_H */
