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

// AST_Root nodes represent the roots of ASTs.
// AST_Root is a subclass of AST_Module, and is defined to allow BEs
// to subclass it to associate their own information with an entire
// AST.

#include "ast_root.h"
#include "ast_sequence.h"
#include "ast_string.h"
#include "ast_array.h"
#include "ast_visitor.h"
#include "utl_identifier.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_Memory.h"

AST_Decl::NodeType const
AST_Root::NT = AST_Decl::NT_root;

AST_Root::AST_Root (UTL_ScopedName *n)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_root,
              n),
    UTL_Scope (AST_Decl::NT_root),
    AST_Module (n)
{
}

AST_Root::~AST_Root (void)
{
}

// Overrides the one in UTL_Scope - this one doesn't
// count the predefined types.
unsigned long
AST_Root::nmembers (void)
{
  unsigned long retval = 0;

  for (UTL_ScopeActiveIterator si (this, UTL_Scope::IK_decls);
        !si.is_done ();
        si.next ())
    {
      if (si.item ()->node_type () == AST_Decl::NT_pre_defined)
        {
          continue;
        }

      ++retval;
    }

  return retval;
}

// Add this AST_Sequence to the locally defined types in this scope.
AST_Sequence *
AST_Root::fe_add_sequence (AST_Sequence *t)
{
  if (t == 0)
    {
      return 0;
    }

  this->add_to_local_types (t);
  return t;
}

// Add this AST_String to the locally defined types in this scope.
AST_String *
AST_Root::fe_add_string (AST_String *t)
{
  if (t == 0)
    {
      return 0;
    }

  this->add_to_local_types (t);
  return t;
}

// Add this AST_Array to the locally defined types in this scope.
AST_Array *
AST_Root::fe_add_array (AST_Array *t)
{
  if (t == 0)
    {
      return 0;
    }

  this->add_to_local_types (t);
  return t;
}

// Redefinition of inherited virtual operations.

// Dump this AST_Root node to the ostream o.
void
AST_Root::dump (ACE_OSTREAM_TYPE &o)
{
  UTL_Scope::dump (o);
}

int
AST_Root::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_root (this);
}

void
AST_Root::destroy (void)
{
  long i = 0;
  AST_Decl *d = 0;

  // Just destroy and delete everything but the CORBA
  // module, and the 'void' keyword, in case we are
  // processing multiple IDL files.
  // Final cleanup will be done in fini().
  long end = this->pd_decls_used;

  for (i = 2; i < end; ++i)
    {
      d = this->pd_decls[i];

      d->destroy ();
      delete d;
      d = 0;
      --this->pd_decls_used;
    }

  // Same goes for the references and the name
  // references, leave the first 2.

  // This array of pointers holds references, no need
  // for destruction. The array itself will be cleaned
  // up when AST_Root::fini() calls UTL_Scope::destroy ().
  for (i = 2; i < this->pd_referenced_used; ++i)
    {
      this->pd_referenced[i] = 0;
    }

  this->pd_referenced_used = 2;

  for (i = 2; i < this->pd_name_referenced_used; ++i)
    {
      Identifier *id = this->pd_name_referenced[i];
      id->destroy ();
      delete id;
      id = 0;
    }

  this->pd_name_referenced_used = 2;
}

void
AST_Root::fini (void)
{
  this->UTL_Scope::destroy ();
  this->AST_Decl::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_Root)
IMPL_NARROW_FROM_SCOPE(AST_Root)
