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

/*
 * fe_extern.cc - export FE interfaces to driver
 */

#include "fe_extern.h"

#include "global_extern.h"
#include "utl_err.h"
#include "utl_indenter.h"

#include "ast_root.h"

#include "ace/OS_NS_stdio.h"

extern int tao_yyparse ();

#ifdef USE_MCPP_BUFFER_LEXING
char *tao_preproc_buffer = 0;
int tao_preproc_buffer_length = 0;
int tao_preproc_buffer_pos = 0;
#else
extern FILE *tao_yyin;
#endif /* USE_MCPP_BUFFER_LEXING */

int
FE_yyparse ()
{
  int const result = tao_yyparse ();

#ifdef USE_MCPP_BUFFER_LEXING
  ACE_OS::free (tao_preproc_buffer);
  tao_preproc_buffer_length = 0;
  tao_preproc_buffer_pos = 0;
#else
  if (tao_yyin)
    {
      ACE_OS::fclose (tao_yyin);
    }
#endif /* USE_MCPP_BUFFER_LEXING */

  return result;
}

extern int tao_yydebug;
void FE_yydebug (bool value)
{
  tao_yydebug = value;
}

#ifdef USE_MCPP_BUFFER_LEXING
void
FE_set_yyin (char * f)
{
  tao_preproc_buffer = f;
  tao_preproc_buffer_length = ACE_OS::strlen (f);
}
#else
void
FE_set_yyin (FILE * f)
{
  tao_yyin = f;
}
#endif /* USE_MCPP_BUFFER_LEXING */

// Constructor interfaces.

UTL_Error *
FE_new_UTL_Error ()
{
  UTL_Error *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  UTL_Error,
                  nullptr);

  return retval;
}

UTL_Indenter *
FE_new_UTL_Indenter ()
{
  UTL_Indenter *retval = nullptr;
  ACE_NEW_RETURN (retval,
                  UTL_Indenter,
                  nullptr);

  return retval;
}
