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

// The check ensures that for every forward declared interface we also
// saw a full definition of that interface.

#include "ast_extern.h"
#include "ast_interface.h"
#include "ast_interface_fwd.h"
#include "global_extern.h"
#include "utl_err.h"
#include "ace/config-all.h"

ACE_RCSID (ast, 
           ast_check, 
           "$Id$")

// Static storage for remembering nodes.
static AST_InterfaceFwd	**ast_fwds = 0;
static long	ast_n_fwds_used = 0;
static long	ast_n_fwds_alloc = 0;

#undef	INCREMENT
#define	INCREMENT	64

// Store a node representing a forward declared interface.
void
AST_record_fwd_interface (AST_InterfaceFwd *n)
{
  AST_InterfaceFwd **o_ast_fwds = 0;
  long o_ast_n_fwds_alloc = 0;

  // Make sure there's space to store one more.
  if (ast_n_fwds_used == ast_n_fwds_alloc)
    {
      if (ast_n_fwds_alloc == 0)
        {
          ast_n_fwds_alloc = INCREMENT;
          ACE_NEW (ast_fwds,
                   AST_InterfaceFwd *[ast_n_fwds_alloc]);
        }
      else
        {
          o_ast_fwds = ast_fwds;
          o_ast_n_fwds_alloc = ast_n_fwds_alloc;

          ast_n_fwds_alloc += INCREMENT;
          ACE_NEW (ast_fwds,
                   AST_InterfaceFwd *[ast_n_fwds_alloc]);

          for (long i = 0; i < o_ast_n_fwds_alloc; i++)
            {
              ast_fwds[i] = o_ast_fwds[i];
            }

          delete o_ast_fwds;
        }
    }

  // Insert new node.
  ast_fwds[ast_n_fwds_used++] = n;
}

// Check that all forward declared interfaces were also defined.
void
AST_check_fwd_interface (void)
{
  AST_InterfaceFwd *d = 0;
  AST_Interface	*itf = 0;

  for (long i = 0; i < ast_n_fwds_used; i++)
    {
      d = ast_fwds[i];
      itf = d->full_definition ();

      if (!itf->is_defined ())
        {
          // The old pointer may now be garbage.
          itf->set_file_name (idl_global->filename ());

          idl_global->err ()->fwd_decl_not_defined (itf);
        }
    }
}
