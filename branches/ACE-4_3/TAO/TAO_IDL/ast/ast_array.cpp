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

#pragma ident "%@(#)ast_array.cc	1.46% %92/06/11% Sun Microsystems"

/*
 * ast_array.cc - Implementation of class AST_Array
 *
 * AST_Array nodes denote array type and field modifiers.
 * AST_Array nodes have a list of dimensions (a UTL_ExprList)
 * a count of the number of dimensions and a base type (a
 * subtype of AST_ConcreteType. This means that we cannot have
 * arrays of AST_Interfaces???
 */

#include	"idl.h"
#include	"idl_extern.h"
      
/*
 * Constructor(s) and destructor
 */
AST_Array::AST_Array()
	 : pd_n_dims(0), pd_base_type(NULL), pd_dims(NULL)
{
}

AST_Array::AST_Array(UTL_ScopedName *n, unsigned long nd, UTL_ExprList *ds)
	 : pd_n_dims(nd), pd_base_type(NULL),
	   AST_Decl(AST_Decl::NT_array, n, NULL)
{
  pd_dims = compute_dims(ds, nd);
}

/*
 * Private operations
 */

/*
 * Compute how many dimensions there are and collect their expressions
 * into an array
 */
AST_Expression **
AST_Array::compute_dims(UTL_ExprList *ds, unsigned long nds)
{
  AST_Expression	**result;
  UTL_ExprlistActiveIterator *l;
  long		i;

  if (ds == NULL)
    return NULL;

  result = new AST_Expression *[nds];
  l      = new UTL_ExprlistActiveIterator(ds);

  for (i = 0; !(l->is_done()) && i < nds; l->next(), i++)
    result[i] = l->item();
  delete l;
  return result;
}

/*
 * Public operations
 */


/*
 * Redefinition of inherited virtual operations
 */

/*
 * Dump this AST_Array node to the ostream o
 */
void
AST_Array::dump(ostream &o)
{
  long	i;

  pd_base_type->dump(o);
  o << " ";
  local_name()->dump(o);
  for (i = 0; i < pd_n_dims; i++) {
    o << "[";
    pd_dims[i]->dump(o);
    o << "]";
  }
}

/*
 * Data accessors
 */
const unsigned long 
AST_Array::n_dims()
{
  return pd_n_dims;
}

AST_Expression **
AST_Array::dims()
{
  return pd_dims;
}

AST_Type *
AST_Array::base_type()
{
  return pd_base_type;
}

void 
AST_Array::set_base_type(AST_Type *nbt)
{
  pd_base_type = nbt;
}

void
AST_Array::set_dims(AST_Expression **ds, unsigned long nds)
{
    pd_dims = ds;
    pd_n_dims = nds;
}

/*
 * Narrowing methods
 */
IMPL_NARROW_METHODS1(AST_Array, AST_ConcreteType)
IMPL_NARROW_FROM_DECL(AST_Array)
