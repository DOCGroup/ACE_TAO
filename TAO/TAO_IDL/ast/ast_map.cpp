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

// AST_Map nodes represent IDL map declarations.
// AST_Map is a subclass of AST_ConcreteType.
// AST_Map nodes have a maximum size (an AST_Expression which
// must evaluate to a positive integer) and a base type (a subclass
// of AST_Type).

#include "ast_map.h"
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

#include <cstring>

AST_Decl::NodeType const
AST_Map::NT = AST_Decl::NT_map;

AST_Map::AST_Map (AST_Expression *ms,
                  AST_Type *key_bt,
                  AST_Type *val_bt,
                  UTL_ScopedName *n,
                  bool local,
                  bool abstract)
  : COMMON_Base (key_bt->is_local () || val_bt->is_local() || local,
                 abstract),
    AST_Decl (AST_Decl::NT_map,
              n,
              true),
    AST_Type (AST_Decl::NT_map,
              n),
    AST_ConcreteType (AST_Decl::NT_map,
                      n),
    pd_max_size (ms),
    key_pd_type (key_bt),
    value_pd_type (val_bt),
    unbounded_ (true),
    owns_key_type_ (false),
    owns_value_type_ (false)
{
  FE_Utils::tmpl_mod_ref_check (this, key_bt);

  AST_Decl::NodeType knt = key_bt->node_type ();

  if (knt == AST_Decl::NT_param_holder)
    {
      AST_Param_Holder *ph = dynamic_cast<AST_Param_Holder *> (key_bt);

      if (ph->info ()->type_ == AST_Decl::NT_const)
        {
          idl_global->err ()->not_a_type (key_bt);
          key_bt->destroy ();
          delete key_bt;
          key_bt = 0;
          throw Bailout ();
        }
    }

  FE_Utils::tmpl_mod_ref_check (this, val_bt);

  AST_Decl::NodeType vnt = val_bt->node_type ();

  if (vnt == AST_Decl::NT_param_holder)
    {
      AST_Param_Holder *ph = dynamic_cast<AST_Param_Holder*> (val_bt);

      if (ph->info ()->type_ == AST_Decl::NT_const)
        {
          idl_global->err ()->not_a_type (val_bt);
          val_bt->destroy ();
          delete val_bt;
          val_bt = 0;
          throw Bailout ();
        }
    }

  // Check if we are bounded or unbounded. An expression value of 0 means
  // unbounded. If our bound is a template parameter, skip the
  // check altogether, this node will trigger no code generation.
  if (ms->param_holder () == 0)
    {
      this->unbounded_ = (ms->ev ()->u.ulval == 0);
    }

  // A map data type is always VARIABLE.
  this->size_type (AST_Type::VARIABLE);

  this->owns_key_type_ =
    knt == AST_Decl::NT_array
    || knt == AST_Decl::NT_map
    || knt == AST_Decl::NT_param_holder;

  this->owns_value_type_ =
    vnt == AST_Decl::NT_array
    || vnt == AST_Decl::NT_map
    || vnt == AST_Decl::NT_param_holder;
}

AST_Map::~AST_Map ()
{}

// Public operations.

bool
AST_Map::in_recursion (ACE_Unbounded_Queue<AST_Type *> &list)
{
  if (list.is_empty ()) // only structs, unions and valuetypes can be recursive
    return false;

  list.enqueue_tail(this);

  AST_Type *key_type = dynamic_cast<AST_Type*> (this->key_type ());
  AST_Type *val_type = dynamic_cast<AST_Type*> (this->value_type());

  if (key_type == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("AST_Map::in_recursion - ")
                         ACE_TEXT ("bad key type\n")),
                        false);
    }

  if (val_type == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("AST_Map::in_recursion - ")
                         ACE_TEXT ("bad value type\n")),
                        false);
    }

  AST_Decl::NodeType kt = key_type->node_type ();
  AST_Decl::NodeType vt = val_type->node_type ();

  if (kt == AST_Decl::NT_typedef)
    {
      AST_Typedef *td = dynamic_cast<AST_Typedef*> (key_type);
      key_type = td->primitive_base_type ();
      kt = key_type->node_type ();
    }

  if (vt == AST_Decl::NT_typedef)
    {
      AST_Typedef *td = dynamic_cast<AST_Typedef*>(val_type);
      val_type = td->primitive_base_type();
      vt = val_type->node_type();
    }

  if (kt != AST_Decl::NT_struct
      && kt != AST_Decl::NT_union
      && kt != AST_Decl::NT_valuetype
      && kt != AST_Decl::NT_map
      && vt != AST_Decl::NT_struct
      && vt != AST_Decl::NT_union
      && vt != AST_Decl::NT_valuetype
      && vt != AST_Decl::NT_map)
    {
      return false;
    }

  bool recursion_found = false;
  AST_Type** recursable_type = 0;
  list.get (recursable_type, 0);
  if (!std::strcmp (key_type->full_name (),
                  (*recursable_type)->full_name ())
      || !std::strcmp(val_type->full_name (),
                  (*recursable_type)->full_name ()))
    {
      // They match.
      recursion_found = true;
      idl_global->recursive_type_seen_ = true;
    }
  else
    {
      // Check the element type.
      recursion_found = key_type->in_recursion (list) && val_type->in_recursion (list);
    }

  return recursion_found;
}

// Redefinition of inherited virtual operations.

// Dump this AST_Map node to the ostream o.
void
AST_Map::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "map <");
  AST_Annotation_Appls::iterator i,
    finished = key_type_annotations ().end ();
  for (i = key_type_annotations ().begin (); i != finished; ++i)
    {
      AST_Annotation_Appl *a = i->get ();
      a->dump (o);
      dump_i (o, " ");
    }
  this->key_pd_type->dump (o);
  this->dump_i (o, ", ");

  finished = value_type_annotations ().end ();
  for (i = value_type_annotations ().begin (); i != finished; ++i)
    {
      AST_Annotation_Appl *a = i->get ();
      a->dump (o);
      dump_i (o, " ");
    }
  this->value_pd_type->dump (o);
  this->dump_i (o, ", ");
  this->pd_max_size->dump (o);
  this->dump_i (o, ">");
}

int
AST_Map::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_map (this);
}

// Data accessors.

AST_Expression *
AST_Map::max_size () const
{
  return this->pd_max_size;
}

AST_Type *
AST_Map::key_type () const
{
  return this->key_pd_type;
}

AST_Type *
AST_Map::value_type () const
{
  return this->value_pd_type;
}

AST_Type *
AST_Map::primitive_key_type () const
{
  AST_Type *type_node = key_type ();
  if (type_node && type_node->node_type () == AST_Decl::NT_typedef)
    {
      AST_Typedef *const typedef_node = dynamic_cast<AST_Typedef *> (type_node);
      if (!typedef_node) return 0;
      type_node = typedef_node->primitive_base_type ();
    }
  return type_node;
}

AST_Type *
AST_Map::primitive_value_type () const
{
  AST_Type *type_node = value_type();
  if (type_node && type_node->node_type() == AST_Decl::NT_typedef)
    {
      AST_Typedef * const typedef_node = dynamic_cast<AST_Typedef*>(type_node);
      if (!typedef_node) return 0;
      type_node = typedef_node->primitive_base_type();
    }
  return type_node;
}

bool
AST_Map::unbounded () const
{
  return this->unbounded_;
}

bool
AST_Map::legal_for_primary_key () const
{
  return this->key_type ()->legal_for_primary_key ();
}

bool
AST_Map::is_defined ()
{
  return this->key_pd_type->is_defined () && this->value_pd_type->is_defined();
}

void
AST_Map::destroy ()
{
  if (this->owns_key_type_)
    {
      this->key_pd_type->destroy ();
      delete this->key_pd_type;
      this->key_pd_type = 0;
    }

  if (this->owns_value_type_)
    {
      this->value_pd_type->destroy();
      delete this->value_pd_type;
      this->value_pd_type = 0;
    }

  this->pd_max_size->destroy ();
  delete this->pd_max_size;
  this->pd_max_size = 0;

  this->AST_ConcreteType::destroy ();
}

AST_Annotation_Appls &
AST_Map::key_type_annotations ()
{
  return key_type_annotations_;
}

void
AST_Map::key_type_annotations (const AST_Annotation_Appls &annotations)
{
  key_type_annotations_ = annotations;
}

AST_Annotation_Appls &
AST_Map::value_type_annotations ()
{
  return value_type_annotations_;
}

void
AST_Map::value_type_annotations (const AST_Annotation_Appls &annotations)
{
  value_type_annotations_ = annotations;
}
