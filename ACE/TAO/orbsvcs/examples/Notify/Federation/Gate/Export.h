// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// generate_export_file.pl
// ------------------------------
#if !defined (GATE_EXPORT_H)
#define GATE_EXPORT_H

#include "ace/config-all.h"

#if defined (ACE_AS_STATIC_LIBS) && !defined (GATE_HAS_DLL)
#  define GATE_HAS_DLL 0
#endif /* ACE_AS_STATIC_LIBS && GATE_HAS_DLL */

#if !defined (GATE_HAS_DLL)
#define GATE_HAS_DLL 1
#endif /* ! GATE_HAS_DLL */

#if defined (GATE_HAS_DLL)
#  if (GATE_HAS_DLL == 1)
#    if defined (GATE_BUILD_DLL)
#      define Gate_Export ACE_Proper_Export_Flag
#      define GATE_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#      define GATE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#    else
#      define Gate_Export ACE_Proper_Import_Flag
#      define GATE_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#      define GATE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#    endif /* GATE_BUILD_DLL */
#  else
#    define Gate_Export
#    define GATE_SINGLETON_DECLARATION(T)
#    define GATE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif   /* ! GATE_HAS_DLL == 1 */
#else
#  define Gate_Export
#  define GATE_SINGLETON_DECLARATION(T)
#  define GATE_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif     /* GATE_HAS_DLL */

#endif     /* GATE_EXPORT_H */

// End of auto generated file.
