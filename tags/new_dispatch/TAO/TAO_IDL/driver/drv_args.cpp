/*

COPYRIGHT

Copyright 1992, 1993, 1994 Sun Microsystems, Inc.  Printed in the United
States of America.  All Rights Reserved.

This product is protected by copyright and distributed under the following
license restricting its use.

The Interface Definition Language Compiler Front End (CFE) is made
available for your use provided that you include this license and copyright
notice on all media and documentation and the software program in which
this product is incorporated in whole or part. You may copy and extend
functionality (but may not remove functionality) of the Interface
Definition Language CFE without charge, but you are not authorized to
license or distribute it to anyone else except as part of a product or
program developed by you or with the express written consent of Sun
Microsystems, Inc. ("Sun").

The names of Sun Microsystems, Inc. and any of its subsidiaries or
affiliates may not be used in advertising or publicity pertaining to
distribution of Interface Definition Language CFE as permitted herein.

This license is effective until terminated by Sun for failure to comply
with this license.  Upon termination, you shall destroy or return all code
and documentation for the Interface Definition Language CFE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED AS IS WITH NO WARRANTIES OF
ANY KIND INCLUDING THE WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS
FOR A PARTICULAR PURPOSE, NONINFRINGEMENT, OR ARISING FROM A COURSE OF
DEALING, USAGE OR TRADE PRACTICE.

INTERFACE DEFINITION LANGUAGE CFE IS PROVIDED WITH NO SUPPORT AND WITHOUT
ANY OBLIGATION ON THE PART OF Sun OR ANY OF ITS SUBSIDIARIES OR AFFILIATES
TO ASSIST IN ITS USE, CORRECTION, MODIFICATION OR ENHANCEMENT.

SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES SHALL HAVE NO LIABILITY WITH
RESPECT TO THE INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY
INTERFACE DEFINITION LANGUAGE CFE OR ANY PART THEREOF.

IN NO EVENT WILL SUN OR ANY OF ITS SUBSIDIARIES OR AFFILIATES BE LIABLE FOR
ANY LOST REVENUE OR PROFITS OR OTHER SPECIAL, INDIRECT AND CONSEQUENTIAL
DAMAGES, EVEN IF SUN HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.

Use, duplication, or disclosure by the government is subject to
restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
Technical Data and Computer Software clause at DFARS 252.227-7013 and FAR
52.227-19.

Sun, Sun Microsystems and the Sun logo are trademarks or registered
trademarks of Sun Microsystems, Inc.

SunSoft, Inc.  
2550 Garcia Avenue 
Mountain View, California  94043

NOTE:

SunOS, SunSoft, Sun, Solaris, Sun Microsystems or the Sun logo are
trademarks or registered trademarks of Sun Microsystems, Inc.

 */

#pragma ident "%@(#)drv_args.cc	1.16% %92/06/10% Sun Microsystems"

// drv_args.c - Argument parsing for IDL compiler main driver

#include	"idl.h"
#include	"idl_extern.h"

#include	"drv_private.h"
#include        "drv_link.h"

#include	<stdio.h> 

/*
 * Push a file into the list of files to be processed
 */
static void
DRV_push_file(char *s)
{
  DRV_files[DRV_nfiles++] = s;
}

/*
 * Prepare a CPP argument
 */
static void
DRV_prep_cpp_arg(char *s)
{
  char *newarg = new char[512];  
  char *farg;

  newarg[0] = '\0';
  for (farg = strtok(s, ","); farg != NULL; farg = strtok(NULL, ","))
    strcat(newarg,farg);
  DRV_cpp_putarg(newarg);
}

/*
 * Print a usage message and exit
 */
void
DRV_usage()
{
  cerr << idl_global->prog_name()
       << GTDEVEL(": usage: ")
       << idl_global->prog_name()
       << GTDEVEL(" [flag | file]*\n");
  cerr << GTDEVEL("Legal flags:\n");
  cerr << GTDEVEL(" -A...\t\t\tlocal implementation-specific escape\n");
  cerr << GTDEVEL(" -Dname[=value]\t\tdefines name for preprocessor\n");
  cerr << GTDEVEL(" -E\t\t\truns preprocessor only, prints on stdout\n");
  cerr << GTDEVEL(" -Idir\t\t\tincludes dir in search path for preprocessor\n");
  cerr << GTDEVEL(" -Uname\t\t\tundefines name for preprocessor\n");
  cerr << GTDEVEL(" -V\t\t\tprints version info then exits\n");
  cerr << GTDEVEL(" -W[p|b],arg1,argn\tpasses args to preprocessor or BE\n");
  cerr << GTDEVEL(" -Yp,path\t\tdefines location of preprocessor\n");
  cerr << GTDEVEL(" -bback_end\t\tcauses specified back end to be used\n");
  cerr << GTDEVEL(" -u\t\t\tprints usage message and exits\n");
  cerr << GTDEVEL(" -v\t\t\ttraces compilation stages\n");
  cerr << GTDEVEL(" -w\t\t\tsuppresses IDL compiler warning messages\n");
}

/*
 * Parse arguments on command line
 */
void
DRV_parse_args(long ac, char **av)
{
  char	*buffer;
  char	*s;
  long	i;

  DRV_cpp_init();
  idl_global->set_prog_name(av[0]);
  for (i = 1; i < ac; i++) {
    if (av[i][0] == '-') {
      switch (av[i][1]) {
      case 0:
	DRV_push_file("standard input");
	break;
      case 'A':
	if (av[i][2] == '\0') {
	  if (i < ac - 1) {
	    i++;
	    s = av[i];
	  } else
	    exit(99);
	} else
	  s = av[i] + 2;
	strcat(idl_global->local_escapes(), s);
	strcat(idl_global->local_escapes(), " ");
	break;
      case 'D':
      case 'U':
      case 'I':
	if (av[i][2] == '\0') {
	  if (i < ac - 1) {
	    buffer = new char[strlen(av[i]) + strlen(av[i + 1]) + 2];
	    sprintf(buffer, "%s%s", av[i], av[i+1]);
	    DRV_cpp_putarg(buffer);
	    i++;
	  } else {
	    cerr << GTDEVEL("IDL: missing argument after '")
		 << av[i] 
		 << GTDEVEL("' flag\n");
	    exit(99);
	  }
	} else
	  DRV_cpp_putarg(av[i]);
	break;
      case 'E':
	idl_global->set_compile_flags(idl_global->compile_flags() |
				      IDL_CF_ONLY_PREPROC);
	break;
      case 'V':
	idl_global->set_compile_flags(idl_global->compile_flags() |
				      IDL_CF_VERSION);
	break;
      case 'W':
	if (av[i][2] == '\0') {
	  if (i < ac - 1) {
	    i++;
	    s = av[i];
	  } else {
	    cerr << GTDEVEL("IDL: missing argument after '")
		 << av[i]
		 << GTDEVEL("' flag\n");
	    exit(99);
	  }
        } else
	  s = av[i] + 2;
	switch (*s) {
	default:
	    cerr << GTDEVEL("IDL: -W must be followed by 'p' or 'b'\n");
	    exit(99);
	case 'p':
	  if (*(s + 1) == ',')
	    DRV_prep_cpp_arg(s + 2);
	  break;
	case 'b':
	  if (*(s + 1) == ',')
	    (*DRV_BE_prep_arg)(s + 2, I_TRUE);
	  break;
	}
	break;
      case 'Y':
	if (av[i][2] == '\0') {
	  if (i < ac - 1) {
	    i++;
	    s = av[i];
	  } else {
	    cerr << GTDEVEL("IDL: missing argument after '")
		 << av[i]
		 << GTDEVEL("' flag\n");
	    exit(99);
	  }
        } else
	  s = av[i] + 2;
	switch (*s) {
	case 'p':
	  if (*(s + 1) == ',') {
	    idl_global->set_cpp_location(s + 2);
	    DRV_cpp_new_location(s + 2);
	  }
	  break;
	default:
	  break;
	}
	break;
      case 'b':
	if (av[i][2] == '\0') {
	  if (i < ac - 1) {
	    i++;
	    s = av[i];
	  } else {
	    cerr << GTDEVEL("IDL: missing argument after '")
		 << av[i]
		 << GTDEVEL("' flag\n");
	    exit(99);
	  }
	} else
	  s = av[i] + 2;
	idl_global->set_be(s);
	break;
      case 'd':
	idl_global->set_compile_flags(idl_global->compile_flags() |
				      IDL_CF_DUMP_AST);
	break;
      case 'u':
	idl_global->set_compile_flags(idl_global->compile_flags() |
				      IDL_CF_ONLY_USAGE);
	break;
      case 'v':
	idl_global->set_compile_flags(idl_global->compile_flags() |
				      IDL_CF_INFORMATIVE);
	break;
      case 'w':
	idl_global->set_compile_flags(idl_global->compile_flags() |
				      IDL_CF_NOWARNINGS);
	break;
      default:
	cerr << GTDEVEL("IDL: Illegal option '") << av[i] << "'\n";
	idl_global->set_compile_flags(idl_global->compile_flags() |
				      IDL_CF_ONLY_USAGE);
	break;
      }
    } else
      DRV_push_file(av[i]);
  }
}
