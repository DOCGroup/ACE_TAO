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

print "// -*- C++ -*-\n";
print "// \$"."Id\$\n";
print "// Definition for Win32 Export directives.\n";
print "// This file is generated automatically by\n";
print "// generate_export_file.pl\n";
print "// ------------------------------\n";
print "#if !defined ($ucname"."_EXPORT_H)\n";
print "#define $ucname"."_EXPORT_H\n";
print "\n";
print "#include \"ace/config-all.h\"\n";
print "\n";
print "#if !defined ($ucname"."_HAS_DLL)\n";
print "#define $ucname"."_HAS_DLL 1\n";
print "#endif /* ! $ucname"."_HAS_DLL */\n";
print "\n";
print "#if defined ($ucname"."_HAS_DLL)\n";
print "#  if ($ucname"."_HAS_DLL == 1)\n";
print "#    if defined ($ucname"."_BUILD_DLL)\n";
print "#      define $name"."_Export ACE_Proper_Export_Flag\n";
print "#      define $ucname"."_SINGLETON_DECLARATION(T) ACE_EXPORT_SINGLETON_DECLARATION (T)\n";
print "#    else\n";
print "#      define $name"."_Export ACE_Proper_Import_Flag\n";
print "#      define $ucname"."_SINGLETON_DECLARATION(T) ACE_IMPORT_SINGLETON_DECLARATION (T)\n";
print "#    endif /* $ucname"."_BUILD_DLL */\n";
print "#  else\n";
print "#    define $name"."_Export\n";
print "#    define $ucname"."_SINGLETON_DECLARATION(T)\n";
print "#  endif   /* ! $ucname"."_HAS_DLL == 1 */\n";
print "#else\n";
print "#  define $name"."_Export\n";
print "#  define $ucname"."_SINGLETON_DECLARATION(T)\n";
print "#endif     /* $ucname"."_HAS_DLL */\n";
print "\n";
print "#endif     /* $ucname"."_EXPORT_H */\n";
print "\n";
print "// End of auto generated file.\n";
