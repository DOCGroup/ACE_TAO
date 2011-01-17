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

// AST_Attribute nodes denote IDL attribute declarations.
// AST_Attribute nodes are AST_Fields with a readonly indication.
// Hence they have a name (an UTL_ScopedName), a type (a subtype
// of AST_Type) and a boolean indicating whether the attribute is
// readonly.

#include "utl_namelist.h"
#include "utl_exceptlist.h"
#include "utl_scope.h"
#include "utl_err.h"
#include "global_extern.h"

#include "ast_attribute.h"
#include "ast_exception.h"
#include "ast_visitor.h"

AST_Decl::NodeType const
AST_Attribute::NT = AST_Decl::NT_attr;

AST_Attribute::AST_Attribute (bool ro,
                              AST_Type *ft,
                              UTL_ScopedName *n,
                              bool local,
                              bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_attr,
              n),
    AST_Field (AST_Decl::NT_attr,
               ft,
               n),
    pd_readonly (ro),
    pd_get_exceptions (0),
    pd_set_exceptions (0)
{
}

AST_Attribute::~AST_Attribute (void)
{
}

// Redefinition of inherited virtual operations.

// Dump this AST_Attribute to the ostream o.
void
AST_Attribute::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, (this->pd_readonly == true
                    ? "readonly attribute "
                    : "attribute "));

  this->AST_Field::dump (o);
}

int
AST_Attribute::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_attribute (this);
}

void
AST_Attribute::destroy (void)
{
  // No need to delete our exception lists, the
  // destroy() method does it. The UTL_ExceptList
  // destroy() method does NOT delete the contained
  // exception nodes.

  if (this->pd_get_exceptions != 0)
    {
      this->pd_get_exceptions->destroy ();
      this->pd_get_exceptions = 0;
    }

  if (this->pd_set_exceptions != 0)
    {
      this->pd_set_exceptions->destroy ();
      this->pd_set_exceptions = 0;
    }

  this->AST_Field::destroy ();
}

UTL_ExceptList *
AST_Attribute::be_add_get_exceptions (UTL_ExceptList *t)
{
  if (this->pd_get_exceptions != 0)
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_RAISES,
                                  this);
    }
  else
    {
      this->pd_get_exceptions = t;
    }

  return this->pd_get_exceptions;
}

UTL_ExceptList *
AST_Attribute::be_add_set_exceptions (UTL_ExceptList *t)
{
  if (this->pd_set_exceptions != 0)
    {
      idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_RAISES,
                                  this);
    }
  else
    {
      this->pd_set_exceptions = t;
    }

  return this->pd_set_exceptions;
}

// Data accessors.

bool
AST_Attribute::readonly (void) const
{
  return this->pd_readonly;
}

UTL_ExceptList *
AST_Attribute::get_get_exceptions (void) const
{
  return this->pd_get_exceptions;
}

UTL_ExceptList *
AST_Attribute::get_set_exceptions (void) const
{
  return this->pd_set_exceptions;
}

// NOTE: No attempt is made to ensure that exceptions are mentioned
//       only once..
UTL_NameList *
AST_Attribute::fe_add_get_exceptions (UTL_NameList *t)
{
  UTL_ScopedName *nl_n = 0;
  AST_Type *fe = 0;
  AST_Decl *d = 0;

  this->pd_get_exceptions = 0;

  for (UTL_NamelistActiveIterator nl_i (t);
       !nl_i.is_done ();
       nl_i.next ())
    {
      nl_n = nl_i.item ();

      d = this->defined_in ()->lookup_by_name (nl_n, true);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (nl_n);
          return 0;
        }

      AST_Decl::NodeType nt = d->node_type ();

      if (nt != AST_Decl::NT_except
          && nt != AST_Decl::NT_param_holder)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_RAISES,
                                      this);
          return 0;
        }

      fe = AST_Type::narrow_from_decl (d);

      UTL_ExceptList *el = 0;
      ACE_NEW_RETURN (el,
                      UTL_ExceptList (fe, 0),
                      0);

      if (this->pd_get_exceptions == 0)
        {
          this->pd_get_exceptions = el;
        }
      else
        {
          this->pd_get_exceptions->nconc (el);
        }
    }

  return t;
}

// NOTE: No attempt is made to ensure that exceptions are mentioned
//       only once..
UTL_NameList *
AST_Attribute::fe_add_set_exceptions (UTL_NameList *t)
{
  UTL_ScopedName *nl_n = 0;
  AST_Type *fe = 0;
  AST_Decl *d = 0;

  this->pd_set_exceptions = 0;

  for (UTL_NamelistActiveIterator nl_i (t); !nl_i.is_done (); nl_i.next ())
    {
      nl_n = nl_i.item ();

      d = this->defined_in ()->lookup_by_name (nl_n, true);

      if (d == 0)
        {
          idl_global->err ()->lookup_error (nl_n);
          return 0;
        }

      AST_Decl::NodeType nt = d->node_type ();

      if (nt != AST_Decl::NT_except
          && nt != AST_Decl::NT_param_holder)
        {
          idl_global->err ()->error1 (UTL_Error::EIDL_ILLEGAL_RAISES,
                                      this);
          return 0;
        }

      fe = AST_Type::narrow_from_decl (d);

      UTL_ExceptList *el = 0;
      ACE_NEW_RETURN (el,
                      UTL_ExceptList (fe, 0),
                      0);

      if (this->pd_set_exceptions == 0)
        {
          this->pd_set_exceptions = el;
        }
      else
        {
          this->pd_set_exceptions->nconc (el);
        }
    }

  return t;
}

IMPL_NARROW_FROM_DECL(AST_Attribute)
