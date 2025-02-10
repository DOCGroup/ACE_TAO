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

// AST_Sequence nodes represent IDL sequence declarations.
// AST_Sequence is a subclass of AST_ConcreteType.
// AST_Sequence nodes have a maximum size (an AST_Expression which
// must evaluate to a positive integer) and a base type (a subclass
// of AST_Type).

#include "ast_sequence.h"
#include "ast_typedef.h"
#include "ast_expression.h"
#include "ast_param_holder.h"
#include "ast_visitor.h"
#include "ast_annotation_appl.h"

#include "utl_identifier.h"
#include "utl_err.h"

#include "global_extern.h"
#include "fe_extern.h"

#include "ace/Log_Msg.h"
#include "ace/OS_Memory.h"
#include "ace/OS_NS_string.h"

AST_Decl::NodeType const
AST_Sequence::NT = AST_Decl::NT_sequence;

AST_Sequence::AST_Sequence (AST_Expression *ms,
                            AST_Type *bt,
                            UTL_ScopedName *n,
                            bool local,
                            bool abstract)
  : COMMON_Base (bt->is_local () || local,
                 abstract),
    AST_Decl (AST_Decl::NT_sequence,
              n,
              true),
    AST_Type (AST_Decl::NT_sequence,
              n),
    AST_ConcreteType (AST_Decl::NT_sequence,
                      n),
    pd_max_size (ms),
    pd_base_type (bt),
    unbounded_ (true),
    owns_base_type_ (false)
{
  FE_Utils::tmpl_mod_ref_check (this, bt);

  AST_Decl::NodeType bnt = bt->node_type ();

  if (bnt == AST_Decl::NT_param_holder)
    {
      AST_Param_Holder *ph = dynamic_cast<AST_Param_Holder*> (bt);

      if (ph->info ()->type_ == AST_Decl::NT_const)
        {
          idl_global->err ()->not_a_type (bt);
          bt->destroy ();
          delete bt;
          bt = nullptr;
          throw Bailout ();
        }
    }

  // Check if we are bounded or unbounded. An expression value of 0 means
  // unbounded. If our bound is a template parameter, skip the
  // check altogether, this node will trigger no code generation.
  if (ms->param_holder () == nullptr)
    {
      this->unbounded_ = (ms->ev ()->u.ulval == 0);
    }

  // A sequence data type is always VARIABLE.
  this->size_type (AST_Type::VARIABLE);

  AST_Decl::NodeType nt = bt->node_type ();

  this->owns_base_type_ =
    nt == AST_Decl::NT_array
    || nt == AST_Decl::NT_sequence
    || nt == AST_Decl::NT_param_holder;
}

AST_Sequence::~AST_Sequence ()
{
}

// Public operations.

bool
AST_Sequence::in_recursion (ACE_Unbounded_Queue<AST_Type *> &list)
{
  if (list.size () == 0) // only structs, unions and valuetypes can be recursive
    return false;

  list.enqueue_tail(this);

  AST_Type *type = dynamic_cast<AST_Type*> (this->base_type ());

  if (type == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("AST_Sequence::in_recursion - ")
                         ACE_TEXT ("bad base type\n")),
                        false);
    }

  AST_Decl::NodeType nt = type->node_type ();

  if (nt == AST_Decl::NT_typedef)
    {
      AST_Typedef *td = dynamic_cast<AST_Typedef*> (type);
      type = td->primitive_base_type ();
      nt = type->node_type ();
    }

  if (nt != AST_Decl::NT_struct
      && nt != AST_Decl::NT_union
      && nt != AST_Decl::NT_valuetype
      && nt != AST_Decl::NT_sequence)
    {
      return false;
    }

  bool recursion_found = false;
  AST_Type** recursable_type = nullptr;
  list.get (recursable_type, 0);
  if (!ACE_OS::strcmp (type->full_name (),
                           (*recursable_type)->full_name ()))
    {
      // They match.
      recursion_found = true;
      idl_global->recursive_type_seen_ = true;
    }
  else
    {
      // Check the element type.
      recursion_found = type->in_recursion (list);
    }

  return recursion_found;
}

// Redefinition of inherited virtual operations.

// Dump this AST_Sequence node to the ostream o.
void
AST_Sequence::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "sequence <");
  AST_Annotation_Appls::iterator i,
    finished = base_type_annotations ().end ();
  for (i = base_type_annotations ().begin (); i != finished; ++i)
    {
      AST_Annotation_Appl *a = i->get ();
      a->dump (o);
      dump_i (o, " ");
    }
  this->pd_base_type->dump (o);
  this->dump_i (o, ", ");
  this->pd_max_size->dump (o);
  this->dump_i (o, ">");
}

int
AST_Sequence::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_sequence (this);
}

// Data accessors.

AST_Expression *
AST_Sequence::max_size ()
{
  return this->pd_max_size;
}

AST_Type *
AST_Sequence::base_type () const
{
  return this->pd_base_type;
}

AST_Type *
AST_Sequence::primitive_base_type () const
{
  AST_Type *type_node = base_type ();
  if (type_node && type_node->node_type () == AST_Decl::NT_typedef)
    {
      AST_Typedef *const typedef_node = dynamic_cast<AST_Typedef *> (type_node);
      if (!typedef_node) return nullptr;
      type_node = typedef_node->primitive_base_type ();
    }
  return type_node;
}

bool
AST_Sequence::unbounded () const
{
  return this->unbounded_;
}

bool
AST_Sequence::legal_for_primary_key () const
{
  return this->base_type ()->legal_for_primary_key ();
}

bool
AST_Sequence::is_defined ()
{
  return this->pd_base_type->is_defined ();
}

void
AST_Sequence::destroy ()
{
  if (this->owns_base_type_)
    {
      this->pd_base_type->destroy ();
      delete this->pd_base_type;
      this->pd_base_type = nullptr;
    }

  this->pd_max_size->destroy ();
  delete this->pd_max_size;
  this->pd_max_size = nullptr;

  this->AST_ConcreteType::destroy ();
}

AST_Annotation_Appls &
AST_Sequence::base_type_annotations ()
{
  return base_type_annotations_;
}

void
AST_Sequence::base_type_annotations (const AST_Annotation_Appls &annotations)
{
  base_type_annotations_ = annotations;
}
