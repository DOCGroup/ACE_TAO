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

// AST_UnionBranch nodes represent a single branch of an IDL union
// declaration.
// AST_UnionBranch is a subclass of AST_Field, adding a label (which
// is a subclass of AST_UnionLabel).

#include "idl.h"
#include "idl_extern.h"

ACE_RCSID(ast, ast_union_branch, "$Id$")

// Constructor(s) and destructor.
AST_UnionBranch::AST_UnionBranch (void)
 : pd_ll (0)
{
}

AST_UnionBranch::AST_UnionBranch (UTL_LabelList *ll,
                                  AST_Type *ft,
				                          UTL_ScopedName *n)
  : AST_Field (AST_Decl::NT_union_branch, 
               ft, 
               n),
	  AST_Decl (AST_Decl::NT_union_branch, 
              n),
	  pd_ll (ll)
{
}

AST_UnionBranch::~AST_UnionBranch (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_UnionBranch node to the ostream o.
void
AST_UnionBranch::dump (ACE_OSTREAM_TYPE &o)
{
  for (unsigned long i = 0; i < this->label_list_length (); ++i)
    {
      o << "case ";
      AST_UnionLabel *ul = this->label (i);
      ul->dump (o);
      o << ": \n";
    }

  AST_Field::dump (o);
}

int
AST_UnionBranch::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_union_branch (this);
}

// Data accessors.

AST_UnionLabel *
AST_UnionBranch::label (unsigned long index)
{
  unsigned long i = 0;
  UTL_LabellistActiveIterator iter (this->pd_ll);

  for (; !iter.is_done (); iter.next ())
    {
      if (i == index)
        {
          return iter.item ();
        }

      ++i;
    }

  return 0;
}

unsigned long
AST_UnionBranch::label_list_length (void)
{
  if (this->pd_ll)
    {
      return this->pd_ll->length ();
    }
  else
    {
      return 0;
    }
}

// Narrowing.
IMPL_NARROW_METHODS1(AST_UnionBranch, AST_Field)
IMPL_NARROW_FROM_DECL(AST_UnionBranch)
