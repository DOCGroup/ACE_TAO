// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_sequence.cpp
//
// = DESCRIPTION
//    Extension of class AST_Sequence that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#include "be_sequence.h"
#include "be_typedef.h"
#include "be_interface.h"
#include "be_interface_fwd.h"
#include "be_predefined_type.h"
#include "be_field.h"
#include "be_visitor.h"
#include "be_helper.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "nr_extern.h"
#include "ace/Log_Msg.h"

ACE_RCSID (be, 
           be_sequence, 
           "$Id$")


be_sequence::be_sequence (void)
  : COMMON_Base (),
    AST_Decl (),
    AST_Type (),
    AST_ConcreteType (),
    AST_Sequence (),
    UTL_Scope (),
    be_scope (),
    be_decl (),
    be_type (),
    mt_ (be_sequence::MNG_UNKNOWN),
    field_node_ (0)
{
  // Always the case.
  this->has_constructor (I_TRUE);
}

be_sequence::be_sequence (AST_Expression *v,
                          AST_Type *t,
                          UTL_ScopedName *n,
                          idl_bool local,
                          idl_bool abstract)
  : COMMON_Base (t->is_local () || local,
                 abstract),
    AST_Decl (AST_Decl::NT_sequence,
              n,
              I_TRUE),
    AST_Type (AST_Decl::NT_sequence,
              n),
    AST_ConcreteType (AST_Decl::NT_sequence,
                      n),
    AST_Sequence (v,
                  t,
                  n,
                  t->is_local () || local,
                  abstract),
    UTL_Scope (AST_Decl::NT_sequence),
    be_scope (AST_Decl::NT_sequence),
    be_decl (AST_Decl::NT_sequence,
             n),
    be_type (AST_Decl::NT_sequence,
             n),
    mt_ (be_sequence::MNG_UNKNOWN),
    field_node_ (0)
{
  // Always the case.
  this->has_constructor (I_TRUE);
}

// Helper to create_name.
char *
be_sequence::gen_name (void)
{
  char namebuf [NAMEBUFSIZE];
  be_type *bt = 0;

  // Reset the buffer.
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  // Retrieve the base type.
  bt = be_type::narrow_from_decl (this->base_type ());

  if (bt == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_sequence::"
                         "gen_name - "
                         "bad base type\n"),
                        0);
    }

  // If this is non-zero, add its local name to the generated name,
  // for uniqueness.
  be_field *fn = this->field_node_;

  if (bt->node_type () == AST_Decl::NT_sequence)
    {
      // Our base type is an anonymous sequence.
      be_sequence *seq = be_sequence::narrow_from_decl (bt);

      if (seq == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_sequence::"
                             "gen_name - "
                             "error converting base type to sequence\n"),
                            0);
        }

      // If the nested sequence were defined in
      // the scope of the enclosing sequence, we would have to
      // not only define the nested class in two places, but also
      // deal with the fact that, for the template classes, the
      // enclosing sequence's template type is a class defined
      // inside it. So we define the nested sequence in the next
      // scope up, and the existing code generation works for both
      // template and non-template implementations of IDL sequences.
      UTL_Scope *parent = this->defined_in ();
      seq->set_defined_in (parent);
      parent->add_sequence (seq);

      ACE_OS::sprintf (namebuf,
                       "_tao_seq_%s_%s",
                       seq->gen_name (),
                       fn ? fn->local_name ()->get_string () : "");
    }
  else
    {
      ACE_OS::sprintf (namebuf,
                       "_tao_seq_%s_%s",
                       bt->local_name ()->get_string (),
                       fn ? fn->local_name ()->get_string () : "");
    }

  // Append the size (if any).
  if (this->unbounded () == I_FALSE)
    {
      char ulval_str [NAMEBUFSIZE];
      ACE_OS::sprintf (ulval_str,
                       "_%lu",
                       this->max_size ()->ev ()->u.ulval);
      ACE_OS::strcat (namebuf, 
                      ulval_str);
    }

  return ACE_OS::strdup (namebuf);
}

// Create a name for ourselves.
int
be_sequence::create_name (be_typedef *node)
{
  static char *namebuf = 0;
  UTL_ScopedName *n = 0;

  // Scope in which we are defined.
  be_decl *scope = 0;

  // If there is a typedef node, we use its name as our name.
  if (node)
    {
      this->set_name (node->name ());
    }
  else
    {
      // Generate a local name.
      namebuf = this->gen_name ();

      // Now see if we have a fully scoped name and if so, generate one.
      UTL_Scope *us = this->defined_in ();

      scope = be_scope::narrow_from_scope (us)->decl ();

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

      ACE_OS::free (namebuf);
    }

  return 0;
}

// Does this sequence have a managed type sequence element?
be_sequence::MANAGED_TYPE
be_sequence::managed_type (void)
{
  if (this->mt_ == be_sequence::MNG_UNKNOWN) // Not calculated yet.
    {
      // Base types.
      be_type *bt = 0;
      be_type *prim_type = 0;

      bt = be_type::narrow_from_decl (this->base_type ());

      if (bt->node_type () == AST_Decl::NT_typedef)
        {
          // Get the primitive base type of this typedef node.
          be_typedef *t = be_typedef::narrow_from_decl (bt);
          prim_type = t->primitive_base_type ();
        }
      else
        {
          prim_type = bt;
        }

      // Determine if we need a managed type and which one.
      switch (prim_type->node_type ())
        {
        case AST_Decl::NT_interface:
          if (prim_type->is_abstract ())
            {
              this->mt_ = be_sequence::MNG_ABSTRACT;
            }
          else
            {
              this->mt_ = be_sequence::MNG_OBJREF;
            }

          break;
        case AST_Decl::NT_interface_fwd:
          this->mt_ = be_sequence::MNG_OBJREF;
          break;
        case AST_Decl::NT_valuetype:
        case AST_Decl::NT_valuetype_fwd:
          this->mt_ = be_sequence::MNG_VALUE;
          break;
        case AST_Decl::NT_string:
          this->mt_ = be_sequence::MNG_STRING;
          break;
        case AST_Decl::NT_wstring:
          this->mt_ = be_sequence::MNG_WSTRING;
          break;
        case AST_Decl::NT_pre_defined:
          {
            be_predefined_type *bpd =
              be_predefined_type::narrow_from_decl (prim_type);
            AST_PredefinedType::PredefinedType pt = bpd->pt ();

            switch (pt)
              {
                case AST_PredefinedType::PT_pseudo:
                  this->mt_ = be_sequence::MNG_PSEUDO;
                  break;
                case AST_PredefinedType::PT_object:
                  this->mt_ = be_sequence::MNG_PSEUDO;
                  break;
                case AST_PredefinedType::PT_value:
                  this->mt_ = be_sequence::MNG_VALUE;
                  break;
                default:
                  this->mt_ = be_sequence::MNG_NONE;
                  break;
              }
          }
          break;
        default:
          this->mt_ = be_sequence::MNG_NONE;
        }
    }

  return this->mt_;
}

// Add this be_sequence to the locally defined types in this scope
AST_Sequence *
be_sequence::fe_add_sequence (AST_Sequence *t)
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
be_sequence::decl (void)
{
  return this;
}

int
be_sequence::accept (be_visitor *visitor)
{
  return visitor->visit_sequence (this);
}


const char *
be_sequence::instance_name ()
{
  static char namebuf[NAMEBUFSIZE];
  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  be_type *bt = 0;
  bt = be_type::narrow_from_decl (this->base_type ());

  if (bt == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%N:%l) be_visitor_sequence_ch::"
                  "gen_instantiate_name - "
                  "Bad element type\n"));

      return namebuf;
    }

  // Generate the class name.

  // The base type after removing all the aliases.
  be_type  *prim_type = bt;

  if (bt->node_type () == AST_Decl::NT_typedef)
    {
      // Get the primitive base type of this typedef node.
      be_typedef *t = be_typedef::narrow_from_decl (bt);
      prim_type = t->primitive_base_type ();
    }

  // Generate the appropriate sequence type.
  switch (this->managed_type ())
    {
    case be_sequence::MNG_PSEUDO:
    case be_sequence::MNG_OBJREF:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Unbounded_Object_Sequence_%s",
                           prim_type->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_Object_Sequence_%s_%lu",
                           prim_type->local_name ()->get_string (),
                           this->max_size ()->ev ()->u.ulval);
        }

      break;
    case be_sequence::MNG_ABSTRACT:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Unbounded_Abstract_Sequence_%s",
                           prim_type->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_Abstract_Sequence_%s_%lu",
                           prim_type->local_name ()->get_string (),
                           this->max_size ()->ev ()->u.ulval);
        }

      break;
    case be_sequence::MNG_VALUE:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Unbounded_Valuetype_Sequence_%s",
                           prim_type->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_Valuetype_Sequence_%s_%lu",
                           prim_type->local_name ()->get_string (),
                           this->max_size ()->ev ()->u.ulval);
        }

      break;
    case be_sequence::MNG_STRING:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "TAO_Unbounded_String_Sequence");
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_String_Sequence_%s",
                           prim_type->local_name ()->get_string ());
        }

      break;
    case be_sequence::MNG_WSTRING:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "TAO_Unbounded_WString_Sequence");
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_WString_Sequence_%s",
                           prim_type->local_name ()->get_string ());
        }

      break;
    default: // Not a managed type.
      if (this->unbounded ())
        {
          // TAO provides extensions for octet sequences, first find out
          // if the base type is an octet (or an alias for octet)
          be_predefined_type *predef =
            be_predefined_type::narrow_from_decl (prim_type);

          if (predef != 0
              && predef->pt() == AST_PredefinedType::PT_octet)
            {
              ACE_OS::sprintf (namebuf,
                               "TAO_Unbounded_Sequence<CORBA::Octet>");
            }
          else
            {
              ACE_OS::sprintf (namebuf,
                               "_TAO_Unbounded_Sequence_%s",
                               prim_type->local_name ()->get_string ());
            }
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_Bounded_Sequence_%s_%lu",
                            prim_type->local_name ()->get_string (),
                            this->max_size ()->ev ()->u.ulval);
        }

      break;
    }

  return namebuf;
}

int
be_sequence::gen_base_class_name (TAO_OutStream *os,
                                  AST_Decl *ctx_scope)
{
  be_type *elem = be_type::narrow_from_decl (this->base_type ());

  // Generate the appropriate base class type.
  switch (this->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
      if (this->unbounded ())
        {
          *os << "TAO_Unbounded_Object_Sequence<" << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << "," << be_nl
              << this->smart_fwd_helper_name (ctx_scope, elem) 
              << "_life," << be_nl;
          *os << this->smart_fwd_helper_name (ctx_scope, elem) 
              << "_cast" << be_uidt_nl
              << ">" << be_uidt;
        }
      else
        {
          *os << "TAO_Bounded_Object_Sequence<" << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << "," << be_nl
              << this->smart_fwd_helper_name (ctx_scope, elem) 
              << "_life," << be_nl
              << this->smart_fwd_helper_name (ctx_scope, elem) 
              << "_cast," << be_nl
              << this->max_size ()->ev ()->u.ulval << be_uidt_nl
              << ">" << be_uidt;
        }

      break;
    case be_sequence::MNG_ABSTRACT:
      if (this->unbounded ())
        {
          *os << "TAO_Unbounded_Abstract_Sequence<" << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << "," << be_nl
              << this->smart_fwd_helper_name (ctx_scope, elem) 
              << "_life" << be_uidt_nl
              << ">" << be_uidt;
        }
      else
        {
          *os << "TAO_Bounded_Abstract_Sequence<" << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << "," << be_nl
              << this->smart_fwd_helper_name (ctx_scope, elem) 
              << "_life," << be_nl
              << this->max_size ()->ev ()->u.ulval << be_uidt_nl
              << ">" << be_uidt;
        }

      break;
    case be_sequence::MNG_PSEUDO:
      if (this->unbounded ())
        {
          *os << "TAO_Unbounded_Pseudo_Sequence<" << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << be_nl
              << elem->name () << "_var" << be_uidt_nl
              << ">" << be_uidt;
        }
      else
        {
          *os << "TAO_Bounded_Pseudo_Sequence<" << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << be_nl
              << elem->name () << "_var," << be_nl
              << this->max_size ()->ev ()->u.ulval << be_uidt_nl
              << ">" << be_uidt;
        }

      break;
    case be_sequence::MNG_VALUE:
      if (this->unbounded ())
        {
          *os << "TAO_Unbounded_Valuetype_Sequence<" << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << "," << be_nl
              << this->smart_fwd_helper_name (ctx_scope, elem) 
              << "_life" << be_uidt_nl
              << ">" << be_uidt;
        }
      else
        {
          *os << "TAO_Bounded_Valuetype_Sequence<" << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << "," << be_nl
              << this->smart_fwd_helper_name (ctx_scope, elem) 
              << "_life," << be_nl
              << this->max_size ()->ev ()->u.ulval << be_uidt_nl
              << ">" << be_uidt;
        }

      break;
    case be_sequence::MNG_STRING:
      if (this->unbounded ())
        {
          *os << "TAO_Unbounded_String_Sequence";
        }
      else
        {
          *os << "TAO_Bounded_String_Sequence<"
              << this->max_size ()->ev ()->u.ulval << ">";
        }

      break;
    case be_sequence::MNG_WSTRING:
      if (this->unbounded ())
        {
          *os << "TAO_Unbounded_WString_Sequence";
        }
      else
        {
          *os << "TAO_Bounded_WString_Sequence<"
              << this->max_size ()->ev ()->u.ulval << ">";
        }

      break;
    default: // Not a managed type.
      switch (elem->base_node_type ())
        {
          case AST_Decl::NT_array:
            if (this->unbounded ())
              {
                *os << "TAO_Unbounded_Array_Sequence<"
                    << be_idt << be_idt_nl
                    << elem->nested_type_name (ctx_scope) << "," << be_nl
                    << this->smart_fwd_helper_name (ctx_scope, elem) 
                    << "_life" << be_uidt_nl
                    << ">" << be_uidt;
              }
            else
              {
                *os << "TAO_Bounded_Array_Sequence<"
                    << be_idt << be_idt_nl
                    << elem->nested_type_name (ctx_scope) << "," << be_nl
                    << this->smart_fwd_helper_name (ctx_scope, elem) 
                    << "_life," << be_nl
                    << this->max_size ()->ev ()->u.ulval << be_uidt_nl
                    << ">" << be_uidt;
              }

            break;
          default:
            if (this->unbounded ())
              {
                *os << "TAO_Unbounded_Sequence<" << be_idt << be_idt_nl
                    << elem->nested_type_name (ctx_scope) << be_uidt_nl
                    << ">" << be_uidt;
              }
            else
              {
                *os << "TAO_Bounded_Sequence<" << be_idt << be_idt_nl
                    << elem->nested_type_name (ctx_scope) << "," << be_nl
                    << this->max_size ()->ev ()->u.ulval << be_uidt_nl
                    << ">" << be_uidt;
              }

            break;
        }

      break;      
  }

  return 0;
}

be_field *
be_sequence::field_node (void) const
{
  return this->field_node_;
}

void
be_sequence::field_node (be_field *node)
{
  this->field_node_ = node;
}

const char *
be_sequence::smart_fwd_helper_name (AST_Decl *ctx_scope,
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
be_sequence::destroy (void)
{
  // Call the destroy methods of our base classes.
  be_scope::destroy ();
  be_type::destroy ();
}

// Narrowing
IMPL_NARROW_METHODS3 (be_sequence, AST_Sequence, be_scope, be_type)
IMPL_NARROW_FROM_DECL (be_sequence)
