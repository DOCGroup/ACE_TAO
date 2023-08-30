
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

  // This one gets set for all sequences, in addition to any specialized
  // one that may get set below.
  idl_global->seq_seen_ = true;
  idl_global->var_size_decl_seen_ = true;

  AST_Type *const key_type = primitive_key_type ();
  if (key_type && key_type->node_type () == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType *pdt = dynamic_cast<AST_PredefinedType*> (key_type);
      switch (pdt->pt ())
        {
          case AST_PredefinedType::PT_octet:
            idl_global->octet_map_seen_ = true;
            break;
          default:
            break;
        }
    }

  AST_Type *const value_type = primitive_value_type();
  if (value_type && value_type->node_type() == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType *pdt = dynamic_cast<AST_PredefinedType*> (key_type);
      switch(pdt->pt())
        {
          case AST_PredefinedType::PT_octet:
            idl_global->octet_map_seen_ = true;
            break;
          default:
            break;
        }
    }
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

  // If this is non-zero, add its local name to the generated name,
  // for uniqueness.
  be_field *fn = this->field_node_;

  // TODO Key or value types that are anonymous maps
  // if (bt->node_type () == AST_Decl::NT_sequence)
  //   {
  //     // Our base type is an anonymous sequence.
  //     be_sequence *seq = dynamic_cast<be_sequence*> (bt);

  //     if (seq == 0)
  //       {
  //         ACE_ERROR_RETURN ((LM_ERROR,
  //                            "(%N:%l) be_map::"
  //                            "gen_name - "
  //                            "error converting base type to map\n"),
  //                           0);
  //       }

  //     // If the nested sequence were defined in
  //     // the scope of the enclosing sequence, we would have to
  //     // not only define the nested class in two places, but also
  //     // deal with the fact that, for the template classes, the
  //     // enclosing sequence's template type is a class defined
  //     // inside it. So we define the nested sequence in the next
  //     // scope up, and the existing code generation works for both
  //     // template and non-template implementations of IDL sequences.
  //     UTL_Scope *parent = this->defined_in ();
  //     seq->set_defined_in (parent);
  //     char *seq_name = seq->gen_name ();

  //     ACE_OS::sprintf (namebuf,
  //                      "_tao_seq_%s_%s",
  //                      seq_name,
  //                      fn ? fn->local_name ()->get_string () : "");
  //     ACE::strdelete (seq_name);
  //   }
  // else
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
  static char *namebuf = 0;
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
be_map::gen_ostream_operator (TAO_OutStream *os,
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
  static char namebuf[NAMEBUFSIZE];
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  // be_type *const prim_type = primitive_base_type ();
  // if (!prim_type)
  //   {
  //     ACE_ERROR ((LM_ERROR,
  //                 "(%N:%l) be_visitor_sequence_ch::"
  //                 "gen_instantiate_name - "
  //                 "Bad element type\n"));

  //     return namebuf;
  //   }

  // // Generate the appropriate sequence type.
  // switch (this->managed_type ())
  //   {
  //   case be_sequence::MNG_PSEUDO:
  //   case be_sequence::MNG_OBJREF:
  //     if (this->unbounded ())
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "_TAO_unbounded_object_reference_sequence_%s",
  //                          prim_type->local_name ()->get_string ());
  //       }
  //     else
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "_TAO_bounded_object_reference_sequence_%s_"
  //                          ACE_UINT32_FORMAT_SPECIFIER_ASCII,
  //                          prim_type->local_name ()->get_string (),
  //                          this->max_size ()->ev ()->u.ulval);
  //       }

  //     break;
  //   case be_sequence::MNG_VALUE:
  //     if (this->unbounded ())
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "_TAO_unbounded_valuetype_sequence_%s",
  //                          prim_type->local_name ()->get_string ());
  //       }
  //     else
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "_TAO_bounded_valuetype_sequence_%s_"
  //                          ACE_UINT32_FORMAT_SPECIFIER_ASCII,
  //                          prim_type->local_name ()->get_string (),
  //                          this->max_size ()->ev ()->u.ulval);
  //       }

  //     break;
  //   case be_sequence::MNG_STRING:
  //     if (this->unbounded ())
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "::TAO::unbounded_basic_string_sequence<char>");
  //       }
  //     else
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "_TAO_unbounded_string_sequence_%s",
  //                          prim_type->local_name ()->get_string ());
  //       }

  //     break;
  //   case be_sequence::MNG_WSTRING:
  //     if (this->unbounded ())
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "::TAO::unbounded_basic_string_sequence<CORBA::WChar>");
  //       }
  //     else
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "_TAO_bounded_wstring_sequence_%s",
  //                          prim_type->local_name ()->get_string ());
  //       }

  //     break;
  //   default: // Not a managed type.
  //     if (this->unbounded ())
  //       {
  //         // TAO provides extensions for octet sequences, first find out
  //         // if the base type is an octet (or an alias for octet)
  //         be_predefined_type *predef =
  //           dynamic_cast<be_predefined_type*> (prim_type);

  //         if (predef != 0
  //             && predef->pt() == AST_PredefinedType::PT_octet)
  //           {
  //             ACE_OS::sprintf (namebuf,
  //                              "::TAO::unbounded_value_sequence<CORBA::Octet>");
  //           }
  //         else
  //           {
  //             ACE_OS::sprintf (namebuf,
  //                              "_TAO_unbounded_value_sequence_%s",
  //                              prim_type->local_name ()->get_string ());
  //           }
  //       }
  //     else
  //       {
  //         ACE_OS::sprintf (namebuf,
  //                          "_TAO_bounded_value_sequence_%s_"
  //                          ACE_UINT32_FORMAT_SPECIFIER_ASCII,
  //                          prim_type->local_name ()->get_string (),
  //                          this->max_size ()->ev ()->u.ulval);
  //       }

  //     break;
  //   }

  return namebuf;
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
