
//=============================================================================
/**
 *  @file    be_map.cpp
 *
 *  Extension of class AST_Sequence that provides additional means for C++
 *  mapping.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_map.h"
#include "ast_decl.h"
#include "ast_predefined_type.h"
#include "be_typedef.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_predefined_type.h"
#include "be_field.h"
#include "be_string.h"
#include "be_visitor.h"
#include "be_helper.h"
#include "be_extern.h"

#include "utl_identifier.h"
#include "idl_defines.h"
#include "nr_extern.h"
#include "global_extern.h"

#include "ace/Log_Msg.h"

be_map::be_map (AST_Expression *v,
                AST_Type *kt,
                AST_Type *vt,
                UTL_ScopedName *n,
                bool local,
                bool abstract)
  : COMMON_Base (kt->is_local () || vt->is_local() || local,
                 abstract),
    AST_Decl (AST_Decl::NT_map,
              n,
              true),
    AST_Type (AST_Decl::NT_map,
              n),
    AST_ConcreteType (AST_Decl::NT_map,
                      n),
    AST_Map (v,
             kt,
             vt,
             n,
             kt->is_local () || vt->is_local () || local,
             abstract),
    UTL_Scope (AST_Decl::NT_map),
    be_scope (AST_Decl::NT_map),
    be_decl (AST_Decl::NT_map,
             n),
    be_type (AST_Decl::NT_map,
             n),
    field_node_ (0)
{
  // Always the case.
  this->has_constructor (true);

  // Don't want to set any bits below for imported nodes.
  if (this->imported ())
    {
      return;
    }

  // This one gets set for all maps
  idl_global->map_seen_ = true;
  idl_global->var_size_decl_seen_ = true;
}

be_type *
be_map::key_type () const
{
  return
    dynamic_cast<be_type*> (
      this->AST_Map::key_type ());
}

be_type *
be_map::value_type () const
{
  return
    dynamic_cast<be_type*> (
      this->AST_Map::value_type ());
}

be_type *
be_map::primitive_key_type () const
{
  be_type *type_node = key_type ();
  if (type_node && type_node->node_type () == AST_Decl::NT_typedef)
    {
      be_typedef *const typedef_node = dynamic_cast<be_typedef *> (type_node);
      if (!typedef_node) return 0;
      type_node = typedef_node->primitive_base_type ();
    }
  return type_node;
}

be_type *
be_map::primitive_value_type () const
{
  be_type *type_node = value_type ();
  if (type_node && type_node->node_type () == AST_Decl::NT_typedef)
    {
      be_typedef *const typedef_node = dynamic_cast<be_typedef *> (type_node);
      if (!typedef_node) return 0;
      type_node = typedef_node->primitive_base_type ();
    }
  return type_node;
}

// Helper to create_name.
char *
be_map::gen_name ()
{
  char namebuf [NAMEBUFSIZE];
  be_type *kt = 0;
  be_type *vt = 0;

  // Reset the buffer.
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  // Retrieve the base type.
  kt = dynamic_cast<be_type*> (this->key_type ());
  vt = dynamic_cast<be_type*> (this->value_type ());

  if (kt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_map::"
                         "gen_name - "
                         "bad key type\n"),
                        0);
    }

  if (kt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_map::"
                         "gen_name - "
                         "bad value type\n"),
                        0);
    }

  ACE_OS::sprintf (namebuf,
                    "_tao_map_%s_%s_",
                    kt->flat_name (), vt->flat_name ());

  // Append the size (if any).
  if (this->unbounded () == false)
    {
      char ulval_str [NAMEBUFSIZE];
      ACE_OS::sprintf (ulval_str,
                       "_" ACE_UINT32_FORMAT_SPECIFIER_ASCII,
                       this->max_size ()->ev ()->u.ulval);
      ACE_OS::strcat (namebuf,
                      ulval_str);
    }

  return ACE::strnew (namebuf);
}

// Create a name for ourselves.
int
be_map::create_name (be_typedef *node)
{
  UTL_ScopedName *n = 0;

  // Scope in which we are defined.
  be_decl *scope = 0;

  // If there is a typedef node, we use its name as our name.
  if (node)
    {
      this->set_name (
          dynamic_cast<UTL_ScopedName *> (node->name ()->copy ())
        );
    }
  else
    {
      // Generate a local name.
      static char *namebuf = 0;
      namebuf = this->gen_name ();

      // Now see if we have a fully scoped name and if so, generate one.
      UTL_Scope *us = this->defined_in ();

      scope = dynamic_cast<be_scope*> (us)->decl ();

      if (scope != 0)
        {
          // Make a copy of the enclosing scope's name.
          n = (UTL_ScopedName *) scope->name ()->copy ();

          Identifier *id = 0;
          ACE_NEW_RETURN (id,
                          Identifier (namebuf),
                          -1);

          UTL_ScopedName *conc_name = 0;
          ACE_NEW_RETURN (conc_name,
                          UTL_ScopedName (id,
                                          0),
                          -1);

          // Add our local name as the last component.
          n->nconc (conc_name);

          // Set the fully scoped name.
          this->set_name (n);
        }
      else
        {
          // We better be not here because we must be inside some scope,
          // at least the ROOT scope.
          return -1;
        }

      ACE::strdelete (namebuf);
    }

  return 0;
}

// Add this be_sequence to the locally defined types in this scope
AST_Map *
be_map::fe_add_map (AST_Map *t)
{
  if (t == 0)
    {
      return 0;
    }

  this->add_to_local_types (t);
  return t;
}

// Overridden method
be_decl *
be_map::decl ()
{
  return this;
}

// Overridden method
void
be_map::gen_ostream_operator (TAO_OutStream * /*os*/,
                                   bool /* use_underscore */)
{
// TODO Gen ostream operator
}

int
be_map::accept (be_visitor *visitor)
{
  return visitor->visit_map (this);
}


const char *
be_map::instance_name ()
{
  return "";
}

be_field *
be_map::field_node () const
{
  return this->field_node_;
}

void
be_map::field_node (be_field *node)
{
  this->field_node_ = node;
}

const char *
be_map::smart_fwd_helper_name (AST_Decl *ctx_scope,
                                    be_type *elem)
{
  if (ScopeAsDecl (elem->defined_in ()) == ctx_scope)
    {
      ACE_CString retval = "tao_";
      retval += elem->local_name ()->get_string ();
      return retval.rep ();
    }

  return elem->fwd_helper_name ();
}

void
be_map::destroy ()
{
  // Call the destroy methods of our base classes.
  this->be_scope::destroy ();
  this->be_type::destroy ();
  this->AST_Map::destroy ();
}
