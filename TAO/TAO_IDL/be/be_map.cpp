
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
    mt_ (be_map::MNG_UNKNOWN),
    field_node_ (nullptr)
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

  // Don't need the return value - just set the member.
  (void) this->managed_type ();

  switch (this->mt_)
    {
      case MNG_OBJREF:
        idl_global->iface_seq_seen_ = true;
        break;
      case MNG_PSEUDO:
        idl_global->pseudo_seq_seen_ = true;
        break;
      case MNG_VALUE:
        idl_global->vt_seq_seen_ = true;
        break;
      case MNG_STRING:
        idl_global->string_seq_seen_ = true;
        break;
      case MNG_WSTRING:
        idl_global->wstring_seq_seen_ = true;
        break;
      default:
        break;
    }

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
      if (!typedef_node) return nullptr;
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
      if (!typedef_node) return nullptr;
      type_node = typedef_node->primitive_base_type ();
    }
  return type_node;
}

// Helper to create_name.
char *
be_map::gen_name ()
{
  char namebuf [NAMEBUFSIZE];
  be_type *kt = nullptr;
  be_type *vt = nullptr;

  // Reset the buffer.
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  // Retrieve the key type.
  kt = dynamic_cast<be_type*> (this->key_type ());

  if (kt == nullptr)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_map::"
                         "gen_name - "
                         "bad key type\n"),
                        0);
    }

  // Retrieve the key type.
  vt = dynamic_cast<be_type*> (this->value_type ());
  if (vt == nullptr)
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

  // if (kt->node_type () == AST_Decl::NT_map)
  //   {
  //     // Our base type is an anonymous map.
  //     be_map *map = dynamic_cast<be_map*> (kt);

  //     if (map == nullptr)
  //       {
  //         ACE_ERROR_RETURN ((LM_ERROR,
  //                            "(%N:%l) be_map::"
  //                            "gen_name - "
  //                            "error converting key type to map\n"),
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
  //     map->set_defined_in (parent);
  //     char *map_name = map->gen_name ();

  //     ACE_OS::sprintf (namebuf,
  //                      "_tao_seq_%s_%s",
  //                      map_name,
  //                      fn ? fn->local_name ()->get_string () : "");
  //     ACE::strdelete (map_name);
  //   }
  // else
  //   {
  //     ACE_OS::sprintf (namebuf,
  //                      "_tao_seq_%s_",
  //                      kt->flat_name ());
  //   }

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
  static char *namebuf = nullptr;
  UTL_ScopedName *n = nullptr;

  // Scope in which we are defined.
  be_decl *scope = nullptr;

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

      if (scope != nullptr)
        {
          // Make a copy of the enclosing scope's name.
          n = (UTL_ScopedName *) scope->name ()->copy ();

          Identifier *id = nullptr;
          ACE_NEW_RETURN (id,
                          Identifier (namebuf),
                          -1);

          UTL_ScopedName *conc_name = nullptr;
          ACE_NEW_RETURN (conc_name,
                          UTL_ScopedName (id,
                                          nullptr),
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

// Does this sequence have a managed type sequence element?
be_map::MANAGED_TYPE
be_map::managed_type ()
{
  // if (this->mt_ == be_map::MNG_UNKNOWN) // Not calculated yet.
  //   {
  //     // Base types.
  //     be_type *const base_type = primitive_base_type ();
  //     if (!base_type)
  //       ACE_ERROR_RETURN ((LM_ERROR,
  //                          "TAO_IDL (%N:%l) "
  //                          "dynamic_cast<be_type*> "
  //                          "failed\n"),
  //                         be_sequence::MNG_UNKNOWN);

  //     // Determine if we need a managed type and which one.
  //     switch (base_type->node_type ())
  //       {
  //         case AST_Decl::NT_interface:
  //         case AST_Decl::NT_interface_fwd:
  //         case AST_Decl::NT_component:
  //         case AST_Decl::NT_component_fwd:
  //         case AST_Decl::NT_connector:
  //           this->mt_ = be_sequence::MNG_OBJREF;
  //           break;
  //         case AST_Decl::NT_valuebox:
  //         case AST_Decl::NT_valuetype:
  //         case AST_Decl::NT_valuetype_fwd:
  //         case AST_Decl::NT_eventtype:
  //         case AST_Decl::NT_eventtype_fwd:
  //           this->mt_ = be_sequence::MNG_VALUE;
  //           break;
  //         case AST_Decl::NT_string:
  //           this->mt_ = be_sequence::MNG_STRING;
  //           break;
  //         case AST_Decl::NT_wstring:
  //           this->mt_ = be_sequence::MNG_WSTRING;
  //           break;
  //         case AST_Decl::NT_pre_defined:
  //           {
  //             be_predefined_type * const bpd =
  //               dynamic_cast<be_predefined_type*> (base_type);

  //             AST_PredefinedType::PredefinedType pt = bpd->pt ();

  //             switch (pt)
  //               {
  //                 case AST_PredefinedType::PT_pseudo:
  //                 case AST_PredefinedType::PT_object:
  //                 case AST_PredefinedType::PT_abstract:
  //                   this->mt_ = be_sequence::MNG_PSEUDO;
  //                   break;
  //                 case AST_PredefinedType::PT_value:
  //                   this->mt_ = be_sequence::MNG_VALUE;
  //                   break;
  //                 default:
  //                   this->mt_ = be_sequence::MNG_NONE;
  //                   break;
  //               }
  //           }
  //           break;
  //         default:
  //           this->mt_ = be_sequence::MNG_NONE;
  //       }
  //   }

  return this->mt_;
}

// Add this be_sequence to the locally defined types in this scope
AST_Map *
be_map::fe_add_map (AST_Map *t)
{
  if (t == nullptr)
    {
      return nullptr;
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
  *os << be_nl
      << "std::ostream& operator<< (" << be_idt << be_idt_nl
      << "std::ostream &strm," << be_nl
      << "const " << this->name () << " &_tao_sequence" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "strm << \"" << this->name () << "[\";" << be_nl_2;

  if (be_global->alt_mapping ())
    {
      *os << "for (CORBA::ULong i = 0; i < _tao_sequence.size (); ++i)";
    }
  else
    {
      *os << "for (CORBA::ULong i = 0; i < _tao_sequence.length (); ++i)";
    }

  *os << be_idt_nl
      << "{" << be_idt_nl
      << "if (i != 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "strm << \", \";" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "strm << _tao_sequence[i];" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return strm << \"]\";" << be_uidt_nl
      << "}" << be_nl;
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
  // ACE_OS::memset (namebuf,
  //                 '\0',
  //                 NAMEBUFSIZE);

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

  //         if (predef != nullptr
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

// Overriden method.
void
be_map::compute_tc_name ()
{
  // Sequence TypeCodes can only be accessed through an alias
  // TypeCode.  Generate a TypeCode name that is meant for internal
  // use alone.

  Identifier * tao_id = nullptr;
  ACE_NEW (tao_id,
           Identifier ("TAO"));

  ACE_NEW (this->tc_name_,
           UTL_ScopedName (tao_id,
                           nullptr));

  char bound[30] = { 0 };

  ACE_OS::sprintf (bound,
                   "_" ACE_UINT32_FORMAT_SPECIFIER_ASCII,
                   this->max_size ()->ev ()->u.ulval);

  ACE_CString local_tc_name =
    ACE_CString ("tc_")
    + ACE_CString (this->flat_name ())
    + ACE_CString (bound);

  Identifier * typecode_scope = nullptr;
  ACE_NEW (typecode_scope,
           Identifier ("TypeCode"));

  UTL_ScopedName * tc_scope_conc_name = nullptr;
  ACE_NEW (tc_scope_conc_name,
           UTL_ScopedName (typecode_scope,
                           nullptr));

  this->tc_name_->nconc (tc_scope_conc_name);

  Identifier * id = nullptr;
  ACE_NEW (id,
           Identifier (local_tc_name.c_str ()));

  UTL_ScopedName * conc_name = nullptr;
  ACE_NEW (conc_name,
           UTL_ScopedName (id,
                           nullptr));

  this->tc_name_->nconc (conc_name);
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
