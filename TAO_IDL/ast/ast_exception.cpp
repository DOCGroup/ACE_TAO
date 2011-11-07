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

// AST_Exceptions denote IDL exception declarations
// AST_Exceptions are a subclass of AST_Decl (they are not types!)
// and of UTL_Scope.

#include "ast_exception.h"
#include "ast_field.h"
#include "ast_union.h"
#include "ast_enum.h"
#include "ast_enum_val.h"
#include "ast_typedef.h"
#include "ast_visitor.h"

#include "utl_err.h"
#include "utl_identifier.h"
#include "utl_indenter.h"

AST_Decl::NodeType const
AST_Exception::NT = AST_Decl::NT_except;

AST_Exception::AST_Exception (UTL_ScopedName *n,
                              bool local,
                              bool abstract)
  : COMMON_Base (local,
                 abstract),
    AST_Decl (AST_Decl::NT_except,
              n),
    AST_Type (AST_Decl::NT_except,
              n),
    AST_ConcreteType (AST_Decl::NT_except,
                      n),
    UTL_Scope (AST_Decl::NT_except),
    AST_Structure (AST_Decl::NT_except,
                   n,
                   local,
                   abstract)
{
}

AST_Exception::~AST_Exception (void)
{
}

// Public operations.

// Are we or the parameter node involved in any recursion?
bool
AST_Exception::in_recursion (ACE_Unbounded_Queue<AST_Type *> &list)
{
  bool self_test = (list.size () == 0);

  // We should calculate this only once. If it has already been
  // done, just return it.
  if (self_test && this->in_recursion_ != -1)
    {
      return (this->in_recursion_ == 1);
    }

  if (list.size () > 1)
  {
    if (match_names (this, list))
      {
        // this happens when we are not recursed ourselves but instead
        // are part of another recursive type
        return false;
      }
  }

  list.enqueue_tail(this);

  // Proceed if the number of members in our scope is greater than 0.
  if (this->nmembers () > 0)
    {
      // Continue until each element is visited.
      for (UTL_ScopeActiveIterator i (this, IK_decls);!i.is_done ();i.next ())
        {
          AST_Field *field = AST_Field::narrow_from_decl (i.item ());

          if (field == 0)
            // This will be an enum value or other legitimate non-field
            // member - in any case, no recursion.
            {
              continue;
            }

          AST_Type *type = field->field_type ();

          if (type->node_type () == AST_Decl::NT_typedef)
            {
              AST_Typedef *td = AST_Typedef::narrow_from_decl (type);
              type = td->primitive_base_type ();
            }

          if (type == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("(%N:%l) AST_Exception::")
                                 ACE_TEXT ("in_recursion - ")
                                 ACE_TEXT ("bad field type\n")),
                                0);
            }

          if (type->in_recursion (list))
            {
              if (self_test)
                this->in_recursion_ = 1;
              idl_global->recursive_type_seen_ = true;
              return true;
            }
        }
    }

  // Not in recursion.
  if (self_test)
    this->in_recursion_ = 0;
  return 0; //this->in_recursion_;
}

// Dump this AST_Exception node to the ostream o.
void
AST_Exception::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "exception ");
  this->local_name ()->dump (o);
  this->dump_i (o, " {\n");
  UTL_Scope::dump (o);
  idl_global->indent ()->skip_to (o);
  this->dump_i (o, "}");
}

int
AST_Exception::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_exception (this);
}

void
AST_Exception::destroy (void)
{
  this->AST_Structure::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_Exception)
IMPL_NARROW_FROM_SCOPE(AST_Exception)
