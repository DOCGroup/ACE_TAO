// $Id$

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

#ifndef _FE_EXTERN_FE_EXTERN_HH
#define _FE_EXTERN_FE_EXTERN_HH

#include "TAO_IDL_FE_Export.h"
#include "ace/SString.h"
#include "ace/Unbounded_Queue.h"

// Declares all global functions for the FE

TAO_IDL_FE_Export void FE_init (void);
TAO_IDL_FE_Export void FE_populate (void);
TAO_IDL_FE_Export void FE_extract_env_include_paths (ACE_Unbounded_Queue<ACE_CString> &list);
TAO_IDL_FE_Export void FE_store_env_include_paths (void);
TAO_IDL_FE_Export const char *FE_get_cpp_loc_from_env (void);
TAO_IDL_FE_Export const char *FE_get_cpp_args_from_env (void);

TAO_IDL_FE_Export int
FE_yyparse (void);

TAO_IDL_FE_Export void
#ifdef USE_MCPP_BUFFER_LEXING
FE_set_yyin (char *);    // Set yyin
#else
FE_set_yyin (FILE *);    // Set yyin
#endif /* USE_MCPP_BUFFER_LEXING */

// Interface to constructors for objects need by the driver

class UTL_Error;
class UTL_Indenter;

TAO_IDL_FE_Export UTL_Error *
FE_new_UTL_Error (void);

TAO_IDL_FE_Export UTL_Indenter *
FE_new_UTL_Indenter (void);

// Exception thrown when exiting prematurely in the front end.
// Also used when command line options limit the output to
// usage, version or preprocessed files. Catch block is just
// before front end cleanup and exit.
class TAO_IDL_FE_Export Bailout
{
};

#endif           // _FE_EXTERN_FE_EXTERN_HH
