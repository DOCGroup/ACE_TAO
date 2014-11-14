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

#include "fe_declarator.h"

#include "ast_array.h"
#include "ast_param_holder.h"

#include "utl_err.h"
#include "global_extern.h"
#include "nr_extern.h"

#include "ace/config-all.h"

FE_Declarator::FE_Declarator (UTL_ScopedName *n,
                              DeclaratorType dt,
                              AST_Decl *cp)
 : pd_complex_part (cp),
   pd_decl_type (dt)
{
  this->pd_name = n;
}

// Compose the type of the complex declarator (if any) with the base
// type supplied in ct.
AST_Type *
FE_Declarator::compose (AST_Decl *d)
{
  AST_Type *ct = AST_Type::narrow_from_decl (d);

  if (ct == 0)
    {
      idl_global->err ()->not_a_type (d);
      return 0;
    }
  else if (ct->node_type () == AST_Decl::NT_param_holder)
    {
      AST_Param_Holder *ph =
        AST_Param_Holder::narrow_from_decl (ct);

      // Every other template parameter kind is legal.
      if (ph->info ()->type_ == AST_Decl::NT_const)
        {
          idl_global->err ()->not_a_type (d);
          return 0;
        }
    }

  AST_Decl::NodeType nt = d->node_type ();

  if (nt == AST_Decl::NT_struct_fwd
      || nt == AST_Decl::NT_union_fwd
      || nt == AST_Decl::NT_struct
      || nt == AST_Decl::NT_union)
    {
      if (! ct->is_defined ())
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_ADD,
                                      d);

          return 0;
        }
    }

  AST_Array *arr = 0;

  // All uses of forward declared types must
  // not have a different prefix from the place of declaration.
  if (!ct->is_defined ())
    {
      const char *original_prefix = d->prefix ();
      AST_Decl *scope = d;

      // Maybe the prefix hasn't been applied yet.
      while (ACE_OS::strcmp (original_prefix, "") == 0)
        {
          scope = ScopeAsDecl (scope->defined_in ());

          // Are we at global scope?
          if (scope == 0)
            {
              break;
            }

          original_prefix = scope->prefix ();
        }

      // Skip if the pointers are the same.
      if (d->prefix () != original_prefix)
        {
          d->prefix (const_cast<char *> (original_prefix));
        }

      // (JP) This could give a bogus error, since typeprefix can
      // appear any time after the corresponding declaration.
      // The right way to do this is with a separate traversal
      // after the entire AST is built.
      /*
      char *current_prefix = 0;
      idl_global->pragma_prefixes ().top (current_prefix);

      if (current_prefix != 0
          && ACE_OS::strcmp (current_prefix, d->prefix ()) != 0)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_PREFIX_CONFLICT,
                                      d);

          return 0;
        }
      */
    }

  if (this->pd_decl_type == FD_simple || this->pd_complex_part == 0)
    {
      return ct;
    }

  if (this->pd_complex_part->node_type () == AST_Decl::NT_array)
    {
      arr = AST_Array::narrow_from_decl (this->pd_complex_part);

      // The base type of an array isn't set until after the array
      // has been created, so the check below gets done at this point.
      arr->set_base_type (ct);
      AST_Decl::NodeType nt = ct->unaliased_type ()->node_type ();

      if (nt == AST_Decl::NT_string || nt == AST_Decl::NT_wstring)
        {
          idl_global->string_member_seen_ = true;
        }

      return arr;
    }

  // We shouldn't get here.
  return 0;
}

void
FE_Declarator::destroy (void)
{
  this->pd_name->destroy ();
  delete this->pd_name;
  this->pd_name = 0;
}

// Data accessors.

AST_Decl *
FE_Declarator::complex_part (void)
{
  return this->pd_complex_part;
}

UTL_ScopedName *
FE_Declarator::name (void)
{
  return this->pd_name;
}

FE_Declarator::DeclaratorType
FE_Declarator::decl_type (void)
{
  return this->pd_decl_type;
}
