// $Id$

/* ===================================================================== */
/*
 * This file is part of CARDAMOM (R) which is jointly developed by THALES
 * and SELEX-SI. It is derivative work based on PERCO Copyright (C) THALES 
 * 2000-2003. All rights reserved.
 * Copyright (C) THALES 2004-2005. All rights reserved.
 * Copyright (C) SELEX-SI and THALES 2006. All rights reserved.
 * 
 * CARDAMOM is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Library General Public License as published
 * by the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * CARDAMOM is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Library General Public
 * License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with CARDAMOM; see the file COPYING. If not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
/* ===================================================================== */
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

#include "CdmwBEExtern.hpp"

#include "global_extern.h"
#include "be_extern.h"
#include "fe_extern.h"
#include "ast_root.h"
#include "ast_extern.h"
#include "utl_string.h"
#include "drv_extern.h"
#include "tao/Version.h"

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
// FUZZ: disable check_for_streams_include
#  include "ace/streams.h"
#endif /* ! ACE_LACKS_IOSTREAM_TOTALLY */

ACE_RCSID (TAO_IDL,
           tao_idl,
           "$Id$")

char *DRV_files[NFILES];
long DRV_nfiles = 0;
long DRV_file_index = -1;

#ifndef TAO_CXX
  // make a guess
  #ifdef WIN32
    #ifdef _MSC_VER
      #define TAO_CXX "Microsoft Visual C++"
    #elif defined ( CYGWIN32 )
      #ifdef __GNUG__
        #define TAO_CXX "g++"
      #else
        #define TAO_CXX "unknown"
      #endif
    #endif
  #elif defined ( __GNUG__ )
     #define TAO_CXX "g++"
  #else
    #define TAO_CXX "unknown"
  #endif
#endif

#ifndef TAO_CXX_VERSION
  // make a guess
  #ifdef WIN32
    #ifdef _MSC_VER
      #if _MSC_VER < 1300
        #define TAO_CXX_VERSION "6.0"
      #else
        #define TAO_CXX_VERSION "7.1"
      #endif
    #elif defined ( CYGWIN32 )
      #ifdef __GNUG__
        #define TAO_CXX_VERSION __VERSION__
      #else
        #define TAO_CXX_VERSION "unknown"
      #endif
    #else
      #define TAO_CXX_VERSION "unknown"
    #endif
  #elif defined ( __GNUG__ )
    #define TAO_CXX_VERSION __VERSION__
  #else
    #define TAO_CXX_VERSION "unknown"
  #endif
#endif

#ifndef TAO_PLATFORM
  // make a guess
  #ifdef WIN32
    #ifdef _MSC_VER
      #define TAO_PLATFORM "Microsoft Win32"
    #elif defined ( CYGWIN32 )
        #define TAO_PLATFORM "Cygwin"
    #else
        #define TAO_PLATFORM "unknown"
    #endif
  #else
    #define TAO_PLATFORM "unknown"
  #endif
#endif

#ifndef TAO_PROJECT_CONFIGURATION
  #ifdef __ACE_INLINE__
    #define TAO_PROJECT_CONFIGURATION "inline"
  #else
    #ifdef ACE_NDEBUG
      #define TAO_PROJECT_CONFIGURATION "standard"
    #else
      #define TAO_PROJECT_CONFIGURATION "debug"
    #endif
  #endif
#endif

void
DRV_version (void)
{
  char build_compiler[] =
    TAO_CXX
    " "
    TAO_CXX_VERSION
  ;

  char build_platform[] =
    TAO_PLATFORM
  ;

  char build_libtype[] =
    TAO_PROJECT_CONFIGURATION
  ;

  ACE_DEBUG ((LM_DEBUG,
              "%s\n"
              "%s %s (%s %s)\n"
              "%s\n"
              "%s\n"
              " %s %s\n %s %s\n"
              " %s %s\n %s %s\n",
              idl_global->prog_name (),
              ACE_TEXT ("TAO_IDL_FE, version"),
              ACE_TEXT (TAO_VERSION),
              ACE_TEXT ("Based on Sun IDL FE, version"),
              ACE_TEXT (SUN_IDL_FE_VERSION),
              ACE_TEXT ("PrismTech Build No: 070308 "),
              ACE_TEXT ("PrismTech Version No: TAO 1.5.1.1"),
              ACE_TEXT ("PreProcessor is"),
              idl_global->cpp_location (),
              ACE_TEXT ("Compiler is"),
              build_compiler,
              ACE_TEXT ("Platform is"),
              build_platform,
              ACE_TEXT ("Library  is"),
              build_libtype));

  CdmwBE_version ();
}

int
DRV_init (int &argc, char *argv[])
{
  // Initialize BE.
  FE_init ();

  // Initialize driver private data
  DRV_nfiles = 0;
  DRV_file_index = 0;

#if defined (TAO_IDL_PREPROCESSOR)
  idl_global->set_cpp_location (TAO_IDL_PREPROCESSOR);
#elif defined (ACE_CC_PREPROCESSOR)
  idl_global->set_cpp_location (ACE_CC_PREPROCESSOR);
#else
  // Just default to cc
  idl_global->set_cpp_location ("cc");
#endif /* TAO_IDL_PREPROCESSOR */

  // Does nothing for IDL compiler, stores -ORB args, initializes
  // ORB and IFR for IFR loader.
  return CdmwBE_init (argc, argv);
}

void
DRV_refresh (void)
{
  idl_global->set_err_count (0);
  idl_global->set_filename (0);
  idl_global->set_main_filename (0);
  idl_global->set_real_filename (0);
  idl_global->set_stripped_filename (0);
//  idl_global->set_import (true);
//  idl_global->set_in_main_file (false);
  idl_global->set_lineno (-1);
  idl_global->reset_flag_seen ();
}

/*
** Drive the compilation
**
** LOGIC:
**
** 2. Initialize the BE. This builds an instance of the generator
** 3. Initialize the FE. This builds the global scope
**    and populates it with the predefined types
** 4. Invoke FE_yyparse
** 5. Check for errors from FE_yyparse. If any, exit now
** 6. Check for undefined forward declared interfaces. If any, exit now
** 7. Check if asked to dump AST. If so, do.
** 8. Invoke BE.
*/

void
DRV_drive (const char *s)
{
  // Set the name of the IDL file we are parsing. This is useful to
  // the backend when it generates C++ headers and files.
  UTL_String *src_file = 0;
  ACE_NEW (src_file,
           UTL_String (s));

  idl_global->idl_src_file (src_file);

  // Pass through CPP.
  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
  {
    ACE_DEBUG ((LM_DEBUG,
                "%s%s %s\n",
                idl_global->prog_name (),
                ACE_TEXT (": preprocessing"),
                s));
  }

  DRV_pre_proc (s);

  // Parse.
  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
  {
    ACE_DEBUG ((LM_DEBUG,
                "%s%s %s\n",
                idl_global->prog_name(),
                ACE_TEXT (": parsing"),
                s));
  }

  FE_yyparse ();

  // We must do this as late as possible to make sure any
  // forward declared structs or unions contained in a
  // primary key at some level have been fully defined.
  idl_global->check_primary_keys ();

  // If there were any errors, stop.
  if (idl_global->err_count () > 0)
  {
    ACE_ERROR ((LM_ERROR,
                "%s%s %s%s %d %s%s\n",
                idl_global->prog_name (),
                ACE_TEXT (":"),
                s,
                ACE_TEXT (": found"),
                idl_global->err_count (),
                ACE_TEXT ("error"),
                ACE_TEXT ((idl_global->err_count () > 1
                  ? ACE_TEXT ("s")
                  : ACE_TEXT ("")))));

    // Call BE_abort to allow a BE to clean up after itself.
    CdmwBE_abort ();

    ACE_OS::exit (static_cast<int> (idl_global->err_count ()));
  }

  // Dump the code.
  if ((idl_global->compile_flags () & IDL_CF_INFORMATIVE)
      && (idl_global->compile_flags () & IDL_CF_DUMP_AST))
  {
    ACE_DEBUG ((LM_DEBUG,
                "%s%s %s\n",
                idl_global->prog_name (),
                ACE_TEXT (": dump"),
                s));
  }

  if (idl_global->compile_flags () & IDL_CF_DUMP_AST)
  {
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Dump of AST:\n")));

    idl_global->root ()->dump (*ACE_DEFAULT_LOG_STREAM);
  }

  // Call the main entry point for the BE.
  if (idl_global->compile_flags () & IDL_CF_INFORMATIVE)
  {
    ACE_DEBUG ((LM_DEBUG,
                "%s%s %s\n",
                idl_global->prog_name (),
                ACE_TEXT (": BE processing on"),
                s));
  }

  // Make sure all forward declared structs and unions are defined
  // before proceeding to code generation.
  AST_check_fwd_decls ();
  long error_count = idl_global->err_count ();

  if (error_count == 0)
  {
    CdmwBE_produce ();
  }
  else
  {
    ACE_OS::exit (static_cast<int> (error_count));
  }

  DRV_refresh ();
}

/*
** LOGIC:
**
** 1. Initialize compiler driver
** 2. Parse command line args
** 3. If more than one file to parse, fork
** 4. Otherwise, for the single file, invoke DRV_drive
*/

int
main (int argc, char *argv[])
{
  // Initialize driver and global variables.
  int init_status = DRV_init (argc, argv);

  if (init_status != 0)
  {
    ACE_OS::exit (init_status);
  }

  // Parse arguments.
  DRV_parse_args (argc, argv);

  // If a version message is requested, print it and exit.
  if (idl_global->compile_flags () & IDL_CF_VERSION)
  {
    DRV_version ();
    ACE_OS::exit (0);
  }

  // If a usage message is requested, give it and exit.
  if (idl_global->compile_flags () & IDL_CF_ONLY_USAGE)
  {
    DRV_usage ();
    ACE_OS::exit (0);
  }

  // If there are no input files, and we are not using the
  // directory recursion option, there's no sense going any further.
  if (DRV_nfiles == 0 && idl_global->recursion_start () == 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("IDL: No input files\n")));

    ACE_OS::exit (99);
  }

  AST_Generator *gen = be_global->generator_init ();

  if (gen == 0)
  {
    ACE_ERROR ((
        LM_ERROR,
        ACE_TEXT ("IDL: DRV_generator_init() failed to create ")
        ACE_TEXT ("generator, exiting\n")
      ));

    ACE_OS::exit (99);
  }
  else
  {
    idl_global->set_gen (gen);
  }

  // Initialize AST and load predefined types.
  FE_populate ();

  // Does various things in various backends.
  CdmwBE_post_init (DRV_files, DRV_nfiles);

  for (DRV_file_index = 0;
       DRV_file_index < DRV_nfiles;
       ++DRV_file_index)
  {
    DRV_drive (DRV_files[DRV_file_index]);
  }

  cdmw_be_global->destroy ();
  delete cdmw_be_global;
  cdmw_be_global = 0;

  idl_global->fini ();
  delete idl_global;
  idl_global = 0;

  for (DRV_file_index = 0;
       DRV_file_index < DRV_nfiles;
       ++DRV_file_index)
  {
    ACE::strdelete (DRV_files[DRV_file_index]);
  }

  ACE_OS::exit (0);

  // NOT REACHED
  return 0;
}
