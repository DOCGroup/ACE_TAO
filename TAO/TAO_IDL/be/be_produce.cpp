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

// BE_produce.cc - Produce the work of the BE - does nothing in the
//		   dummy BE

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include        "be_interpretive.h"

/*
 * Do the work of this BE. This is the dummy BE so we dont do anything in
 * particular here
 */
void
BE_produce()
{
  TAO_CodeGen *cg = TAO_CODEGEN::instance (); // code generator instance
  be_root *root;   // root of the AST made up of BE nodes

  // XXXASG - Here is where we will have a choice of what to initialize
  cg->visitor_factory (new TAO_Interpretive_Visitor_Factory);

  AST_Decl *d = idl_global->root ();
  root = be_root::narrow_from_decl (d);
  if (root == NULL) // no root
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_produce - "
                  "No Root\n"));
      BE_abort();
    }

  // get the appropriate visitor from the factory

  // (1) generate client header
  be_visitor *root_visitor_ch = cg->make_visitor (TAO_CodeGen::TAO_ROOT_CH);
  if (root->accept (root_visitor_ch) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_produce - "
                  "client header for Root failed\n"));
      BE_abort();
    }


  // (2) generate client inline
  be_visitor *root_visitor_ci = cg->make_visitor (TAO_CodeGen::TAO_ROOT_CI);
  if (root->accept (root_visitor_ci) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_produce - "
                  "client inline for Root failed\n"));
      BE_abort();
    }


  // (3) generate client stubs
  be_visitor *root_visitor_cs = cg->make_visitor (TAO_CodeGen::TAO_ROOT_CS);
  if (root->accept (root_visitor_cs) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_produce - "
                  "client stubs for Root failed\n"));
      BE_abort();
    }


  // (4) generate server header
  be_visitor *root_visitor_sh = cg->make_visitor (TAO_CodeGen::TAO_ROOT_SH);
  if (root->accept (root_visitor_sh) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_produce - "
                  "server header for Root failed\n"));
      BE_abort();
    }


  // (5) generate server inline
  be_visitor *root_visitor_si = cg->make_visitor (TAO_CodeGen::TAO_ROOT_SI);
  if (root->accept (root_visitor_si) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_produce - "
                  "server inline for Root failed\n"));
      BE_abort();
    }


  // (6) generate server skeletons
  be_visitor *root_visitor_ss = cg->make_visitor (TAO_CodeGen::TAO_ROOT_SS);
  if (root->accept (root_visitor_ss) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_produce - "
                  "server skeletons for Root failed\n"));
      BE_abort();
    }

#if 0
  // start the code generation process
  if (root->gen_idl2cplusplus_mapping() == -1)
    {
      cerr << "Mapping process failed" << endl;
      BE_abort();
    }
#endif
}

/*
 * Abort this run of the BE
 */
void
BE_abort()
{
  cerr << "Fatal Error" << endl;
  exit (1);
}
