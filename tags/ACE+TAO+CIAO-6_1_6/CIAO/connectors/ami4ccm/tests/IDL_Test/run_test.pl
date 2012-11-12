# $Id$

eval '(exit $?0)' && eval 'exec perl -S $0 ${1+"$@"}'
     & eval 'exec perl -S $0 $argv:q'
     if 0;

# -*- perl -*-

$found = 0 ;
open (I_FILE, "Hello_Component_exec.cpp") ;
while( <I_FILE> ){
   chomp ;
   if( $_ =~ /sendc/ ){
       $found = 1 ;
       last ;
   }
}

if( $found == 1 ){
  print STDERR "ERROR: Found sendc in generated code\n";
  $status = 1 ;
} else {
  print STDERR "Generated code ok\n";
}

exit $status;
