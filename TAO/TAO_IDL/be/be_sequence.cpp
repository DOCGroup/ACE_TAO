
//=============================================================================
/**
 *  @file    be_sequence.cpp
 *
 *  $Id$
 *
 *  Extension of class AST_Sequence that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#include "be_sequence.h"
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

be_sequence::be_sequence (AST_Expression *v,
                          AST_Type *t,
                          UTL_ScopedName *n,
                          bool local,
                          bool abstract)
  : COMMON_Base (t->is_local () || local,
                 abstract),
    AST_Decl (AST_Decl::NT_sequence,
              n,
              true),
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

  AST_Decl::NodeType nt = t->node_type ();
  AST_Typedef *td = 0;
  AST_Type *pbt = 0;

  if (nt == AST_Decl::NT_typedef)
    {
      td = AST_Typedef::narrow_from_decl (t);
      pbt = td->primitive_base_type ();
      nt = pbt->node_type ();
    }

  if (nt == AST_Decl::NT_pre_defined)
    {
      AST_PredefinedType *pdt =
        AST_PredefinedType::narrow_from_decl (pbt ? pbt : t);

      switch (pdt->pt ())
        {
          case AST_PredefinedType::PT_octet:
            idl_global->octet_seq_seen_ = true;
            break;
          default:
            break;
        }
    }
}

be_type *
be_sequence::base_type (void) const
{
  return
    be_type::narrow_from_decl (
      this->AST_Sequence::base_type ());
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
      char *seq_name = seq->gen_name ();

      ACE_OS::sprintf (namebuf,
                       "_tao_seq_%s_%s",
                       seq_name,
                       fn ? fn->local_name ()->get_string () : "");
      ACE::strdelete (seq_name);
    }
  else
    {
      ACE_OS::sprintf (namebuf,
                       "_tao_seq_%s_",
                       bt->flat_name ());
    }

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
be_sequence::create_name (be_typedef *node)
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

      ACE::strdelete (namebuf);
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

      if (!bt)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "TAO_IDL (%N:%l) "
                           "be_type::narrow_from_decl "
                           "failed\n"),
                          be_sequence::MNG_UNKNOWN);

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
          case AST_Decl::NT_interface_fwd:
          case AST_Decl::NT_component:
          case AST_Decl::NT_component_fwd:
          case AST_Decl::NT_connector:
            this->mt_ = be_sequence::MNG_OBJREF;
            break;
          case AST_Decl::NT_valuebox:
          case AST_Decl::NT_valuetype:
          case AST_Decl::NT_valuetype_fwd:
          case AST_Decl::NT_eventtype:
          case AST_Decl::NT_eventtype_fwd:
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
              be_predefined_type * const bpd =
                be_predefined_type::narrow_from_decl (prim_type);

              AST_PredefinedType::PredefinedType pt = bpd->pt ();

              switch (pt)
                {
                  case AST_PredefinedType::PT_pseudo:
                  case AST_PredefinedType::PT_object:
                  case AST_PredefinedType::PT_abstract:
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

// Overridden method
void
be_sequence::gen_ostream_operator (TAO_OutStream *os,
                                   bool /* use_underscore */)
{
  *os << be_nl
      << "std::ostream& operator<< (" << be_idt << be_idt_nl
      << "std::ostream &strm," << be_nl
      << "const " << this->name () << " &_tao_sequence" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "strm << \"" << this->name () << "[\";" << be_nl_2
      << "for (CORBA::ULong i = 0; i < _tao_sequence.length (); ++i)"
      << be_idt_nl
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
                           "_TAO_unbounded_object_reference_sequence_%s",
                           prim_type->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_bounded_object_reference_sequence_%s_"
                           ACE_UINT32_FORMAT_SPECIFIER_ASCII,
                           prim_type->local_name ()->get_string (),
                           this->max_size ()->ev ()->u.ulval);
        }

      break;
    case be_sequence::MNG_VALUE:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_unbounded_valuetype_sequence_%s",
                           prim_type->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_bounded_valuetype_sequence_%s_"
                           ACE_UINT32_FORMAT_SPECIFIER_ASCII,
                           prim_type->local_name ()->get_string (),
                           this->max_size ()->ev ()->u.ulval);
        }

      break;
    case be_sequence::MNG_STRING:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "::TAO::unbounded_basic_string_sequence<char>");
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_unbounded_string_sequence_%s",
                           prim_type->local_name ()->get_string ());
        }

      break;
    case be_sequence::MNG_WSTRING:
      if (this->unbounded ())
        {
          ACE_OS::sprintf (namebuf,
                           "::TAO::unbounded_basic_string_sequence<CORBA::WChar>");
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_bounded_wstring_sequence_%s",
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
                               "::TAO::unbounded_value_sequence<CORBA::Octet>");
            }
          else
            {
              ACE_OS::sprintf (namebuf,
                               "_TAO_unbounded_value_sequence_%s",
                               prim_type->local_name ()->get_string ());
            }
        }
      else
        {
          ACE_OS::sprintf (namebuf,
                           "_TAO_bounded_value_sequence_%s_"
                           ACE_UINT32_FORMAT_SPECIFIER_ASCII,
                           prim_type->local_name ()->get_string (),
                           this->max_size ()->ev ()->u.ulval);
        }

      break;
    }

  return namebuf;
}

int
be_sequence::gen_base_class_name (TAO_OutStream *os,
                                  const char * linebreak,
                                  AST_Decl *ctx_scope)
{
  be_type *elem = be_type::narrow_from_decl (this->base_type ());
  /*
  if (be_global->alt_mapping () && this->unbounded ())
    {
      *os << "std::vector<" << elem->nested_type_name (ctx_scope)
          << ">";

      return 0;
    }
*/
  // Generate the appropriate base class type.
  switch (this->managed_type ())
    {
    case be_sequence::MNG_OBJREF:
    case be_sequence::MNG_PSEUDO:
      if (this->unbounded ())
        {
          *os << "::TAO::unbounded_object_reference_sequence<" << linebreak
              << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << linebreak
              << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << linebreak
              << be_uidt_nl
              << ">" << be_uidt;
        }
      else
        {
          *os << "::TAO::bounded_object_reference_sequence<" << linebreak
              << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << linebreak << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << ","
              << linebreak << be_nl;
          *os << this->max_size ()->ev ()->u.ulval << linebreak << be_uidt_nl
              << ">" << be_uidt;
        }

      break;
    case be_sequence::MNG_VALUE:
      if (this->unbounded ())
        {
          *os << "::TAO::unbounded_valuetype_sequence<" << linebreak
              << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << linebreak
              << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << linebreak
              << be_uidt_nl
              << ">" << be_uidt;
        }
      else
        {
          *os << "::TAO::bounded_valuetype_sequence<" << linebreak
              << be_idt << be_idt_nl
              << elem->nested_type_name (ctx_scope) << "," << linebreak
              << be_nl;
          *os << elem->nested_type_name (ctx_scope, "_var") << ","
              << linebreak << be_nl
              << this->max_size ()->ev ()->u.ulval << linebreak << be_uidt_nl
              << ">" << be_uidt;
        }

      break;
    case be_sequence::MNG_STRING:
      {
        be_type *prim_type = 0;
        if (elem->node_type () == AST_Decl::NT_typedef)
          {
            // Get the primitive base type of this typedef node.
            be_typedef *t = be_typedef::narrow_from_decl (elem);
            prim_type = t->primitive_base_type ();
          }
        else
          {
            prim_type = elem;
          }

        if (prim_type->node_type () == AST_Decl::NT_string)
          {
            be_string *str =
              be_string::narrow_from_decl (prim_type);
            if (!str)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_sequence::"
                                   "gen_base_class_name - "
                                   "bad string node\n"),
                                  -1);
              }

            // We need to make a distinction between bounded and
            // unbounded strings.
            if (str->max_size ()->ev ()->u.ulval != 0)
              {
                if (this->unbounded ())
                  {
                    *os << "::TAO::unbounded_bd_string_sequence<char, "
                        << str->max_size ()->ev ()->u.ulval << ">";
                  }
                else
                  {
                    *os << "::TAO::bounded_bd_string_sequence<char, "
                        << this->max_size ()->ev ()->u.ulval << ", "
                        << str->max_size ()->ev ()->u.ulval << ">";
                  }
              }
            else
              {
                if (this->unbounded ())
                  {
                    *os << "::TAO::unbounded_basic_string_sequence<char>";
                  }
                else
                  {
                    *os << "::TAO::bounded_basic_string_sequence<char, "
                        << this->max_size ()->ev ()->u.ulval << ">";
                  }
              }
          }
      }

      break;
    case be_sequence::MNG_WSTRING:
      {
        be_type *prim_type = 0;
        if (elem->node_type () == AST_Decl::NT_typedef)
          {
            // Get the primitive base type of this typedef node.
            be_typedef *t = be_typedef::narrow_from_decl (elem);
            prim_type = t->primitive_base_type ();
          }
        else
          {
            prim_type = elem;
          }

        if (prim_type->node_type () == AST_Decl::NT_wstring)
          {
            be_string *str =
              be_string::narrow_from_decl (prim_type);
            if (!str)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   "(%N:%l) be_sequence::"
                                   "gen_base_class_name - "
                                   "bad string node\n"),
                                  -1);
              }

            // We need to make a distinction between bounded and
            // unbounded strings.
            if (str->max_size ()->ev ()->u.ulval != 0)
              {
                if (this->unbounded ())
                  {
                    *os << "::TAO::unbounded_bd_string_sequence<CORBA::WChar, "
                        << str->max_size ()->ev ()->u.ulval << ">";
                  }
                else
                  {
                    *os << "::TAO::bounded_bd_string_sequence<CORBA::WChar, "
                        << this->max_size ()->ev ()->u.ulval << ", "
                        << str->max_size ()->ev ()->u.ulval << ">";
                  }
              }
            else
              {
                if (this->unbounded ())
                  {
                    *os << "::TAO::unbounded_basic_string_sequence<CORBA::WChar>";
                  }
                else
                  {
                    *os << "::TAO::bounded_basic_string_sequence<CORBA::WChar, "
                        << this->max_size ()->ev ()->u.ulval << ">";
                  }
              }
          }
      }

      break;
    default: // Not a managed type.
      switch (elem->base_node_type ())
        {
          case AST_Decl::NT_array:
            if (this->unbounded ())
              {
                *os << "::TAO::unbounded_array_sequence<" << linebreak
                     << be_idt << be_idt_nl
                     << elem->nested_type_name (ctx_scope) << "," << linebreak
                     << be_nl;
                 *os << elem->nested_type_name (ctx_scope) << "_slice,"
                     << linebreak << be_nl
                     << elem->nested_type_name (ctx_scope) << "_tag"
                     << linebreak << be_uidt_nl
                     << ">" << be_uidt;
              }
            else
              {
                *os << "::TAO::bounded_array_sequence<" << linebreak
                     << be_idt << be_idt_nl
                     << elem->nested_type_name (ctx_scope) << "," << linebreak
                     << be_nl;
                 *os << elem->nested_type_name (ctx_scope) << "_slice,"
                     << linebreak << be_nl
                     << elem->nested_type_name (ctx_scope) << "_tag,"
                     << linebreak << be_nl
                     << this->max_size ()->ev ()->u.ulval << linebreak
                     << be_uidt_nl
                     << ">" << be_uidt;
              }

            break;
          default:
            if (this->unbounded ())
              {
                *os << "::TAO::unbounded_value_sequence<" << linebreak
                    << be_idt << be_idt_nl
                    << elem->nested_type_name (ctx_scope) << linebreak
                    << be_uidt_nl
                    << ">" << be_uidt;
              }
            else
              {
                *os << "::TAO::bounded_value_sequence<" << linebreak
                    << be_idt << be_idt_nl
                    << elem->nested_type_name (ctx_scope) << "," << linebreak
                    << be_nl
                    << this->max_size ()->ev ()->u.ulval << linebreak
                    << be_uidt_nl
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

// Overriden method.
void
be_sequence::compute_tc_name (void)
{
  // Sequence TypeCodes can only be accessed through an alias
  // TypeCode.  Generate a TypeCode name that is meant for internal
  // use alone.

  Identifier * tao_id = 0;
  ACE_NEW (tao_id,
           Identifier ("TAO"));

  ACE_NEW (this->tc_name_,
           UTL_ScopedName (tao_id,
                           0));

  char bound[30] = { 0 };

  ACE_OS::sprintf (bound,
                   "_" ACE_UINT32_FORMAT_SPECIFIER_ASCII,
                   this->max_size ()->ev ()->u.ulval);

  ACE_CString local_tc_name =
    ACE_CString ("tc_")
    + ACE_CString (this->flat_name ())
    + ACE_CString (bound);

  Identifier * typecode_scope = 0;
  ACE_NEW (typecode_scope,
           Identifier ("TypeCode"));

  UTL_ScopedName * tc_scope_conc_name = 0;
  ACE_NEW (tc_scope_conc_name,
           UTL_ScopedName (typecode_scope,
                           0));

  this->tc_name_->nconc (tc_scope_conc_name);

  Identifier * id = 0;
  ACE_NEW (id,
           Identifier (local_tc_name.c_str ()));

  UTL_ScopedName * conc_name = 0;
  ACE_NEW (conc_name,
           UTL_ScopedName (id,
                           0));

  this->tc_name_->nconc (conc_name);
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
  this->be_scope::destroy ();
  this->be_type::destroy ();
  this->AST_Sequence::destroy ();
}



IMPL_NARROW_FROM_DECL (be_sequence)
