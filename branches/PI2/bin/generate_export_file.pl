eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
    & eval 'exec perl -S $0 $argv:q'
    if 0;

# $Id$
# Replacement for the old trusty GenExportH.bat
# Creates the nice little *_export file which is used for
# importing and exporting of symbols in DLLs.

die "Error: Missing command line argument" if ($#ARGV < 0);

$name = $ARGV[0];
$ucname = uc $name;

## -NC- stands for normal case, the name as it is
## -UC- stands for the name all upper case
map { s/-NC-/$name/g; s/-UC-/$ucname/g; } @data=<DATA>;

print @data;

__DATA__
// -*- C++ -*-
// $Id$
// Definition for Win32 Export directives.
// This file is generated automatically by
// generate_export_file.pl
// ------------------------------
#if !defined (-UC-_EXPORT_H)
#define -UC-_EXPORT_H

#include "ace/config-all.h"

#if !defined (-UC-_HAS_DLL)
#define -UC-_HAS_DLL 1
#endif /* ! -UC-_HAS_DLL */

#if defined (-UC-_HAS_DLL)
#  if (-UC-_HAS_DLL == 1)
#    if defined (-UC-_BUILD_DLL)
#      define -NC-_Export ACE_Proper_Export_Flag
#      define -UC-_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)
#      define -UC-_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_EXPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#    else
#      define -NC-_Export ACE_Proper_Import_Flag
#      define -UC-_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)
#      define -UC-_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK) ACE_IMPORT_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#    endif /* -UC-_BUILD_DLL */
#  else
#    define -NC-_Export
#    define -UC-_SINGLETON_DECLARATION(T)
#    define -UC-_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#  endif   /* ! -UC-_HAS_DLL == 1 */
#else
#  define -NC-_Export
#  define -UC-_SINGLETON_DECLARATION(T)
#  define -UC-_SINGLETON_DECLARE(SINGLETON_TYPE, CLASS, LOCK)
#endif     /* -UC-_HAS_DLL */

#endif     /* -UC-_EXPORT_H */

// End of auto generated file.
