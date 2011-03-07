//=============================================================================
/**
*  @file   be_visitor_arg_traits.cpp
*
*  $Id$
*
*  This visitor generates template specializations for argument traits classes.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

#include "be_visitor_arg_traits.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_valuebox.h"
#include "be_valuetype.h"
#include "be_interface_fwd.h"
#include "be_valuetype_fwd.h"
#include "be_component_fwd.h"
#include "be_connector.h"
#include "be_home.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_operation.h"
#include "be_attribute.h"
#include "be_argument.h"
#include "be_array.h"
#include "be_enum.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_field.h"
#include "be_union.h"
#include "be_union_branch.h"
#include "be_typedef.h"
#include "be_helper.h"
#include "be_extern.h"
#include "utl_identifier.h"
#include "idl_defines.h"
#include "nr_extern.h"
#include "ace/Log_Msg.h"

#include <string>

be_visitor_arg_traits::be_visitor_arg_traits (const char *S,
                                              be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    S_ (ACE::strnew (S))
{
}

be_visitor_arg_traits::~be_visitor_arg_traits (void)
{
  delete [] this->S_;
}

int
be_visitor_arg_traits::visit_root (be_root *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl
      << be_global->core_versioning_begin ();

  *os << be_nl_2
      << "// Arg traits specializations." << be_nl
      << "namespace TAO" << be_nl
      << "{" << be_idt;

  if (be_global->ami_call_back ())
    {
      int const status =
        this->visit_valuetype (be_global->messaging_exceptionholder ());

      if (-1 == status)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_arg_traits::"
                             "visit_root - visit "
                             "Messaging::ExceptionHolder failed\n"),
                            -1);
        }
    }

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "}" << be_nl;

  *os << be_global->core_versioning_end () << be_nl;

  return 0;
}

int
be_visitor_arg_traits::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_module - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_arg_traits::visit_interface (be_interface *node)
{
  if (this->generated (node))
    {
      return 0;
    }

  if (node->seen_in_operation ())
    {
      TAO_OutStream *os = this->ctx_->stream ();

      *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      std::string guard_suffix =
        std::string (this->S_) + std::string ("arg_traits");

      // This should be generated even for imported nodes. The ifdef
      // guard prevents multiple declarations.
      os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str (), false);

      *os << be_nl_2
          << "template<>" << be_nl
          << "class "
          << " " << this->S_ << "Arg_Traits<"
          << node->name () << ">" << be_idt_nl
          << ": public" << be_idt << be_idt_nl
          << "Object_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
          << node->name () << "_ptr," << be_nl
          << node->name () << "_var," << be_nl
          << node->name () << "_out";

      if (ACE_OS::strlen (this->S_) == 0)
        {
          *os << "," << be_nl
              << "TAO::Objref_Traits<" << node->name () << ">";
        }

      *os << "," << be_nl << this->insert_policy()
          << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "};";

      os->gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_interface - visit scope failed\n"),
                        -1);
    }

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_interface_fwd (be_interface_fwd *node)
{
  // If a full definition with the same name in the same scope
  // has been seen, then it will have gone through visit_interface()
  // already.
  if (this->generated (node))
    {
      return 0;
    }

  be_interface *fd =
    be_interface::narrow_from_decl (node->full_definition ());

  // The logic in visit_interface() should handle what gets generated
  // and what doesn't.
  if (this->visit_interface (fd) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_interface_fwd - code generation failed\n"),
                        -1);
    }

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_valuebox (be_valuebox *node)
{
  if (this->generated (node))
    {
      return 0;
    }

  if (node->seen_in_operation ())
    {
      TAO_OutStream & os = *this->ctx_->stream ();

      os << be_nl_2
         << "// TAO_IDL - Generated from" << be_nl
         << "// " << __FILE__ << ":" << __LINE__;

      std::string guard_suffix =
        std::string (this->S_) + std::string ("arg_traits");

      // This should be generated even for imported nodes. The ifdef
      // guard prevents multiple declarations.
      os.gen_ifdef_macro (node->flat_name (), guard_suffix.c_str (), false);

      os << be_nl_2
         << "template<>" << be_nl
         << "class "
         << this->S_ << "Arg_Traits<"
         << node->name () << ">" << be_idt_nl
         << ": public" << be_idt << be_idt_nl
         << "Object_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
         << node->name () << " *," << be_nl
         << node->name () << "_var," << be_nl
         << node->name () << "_out";

      // The SArgument classes don't need the traits parameter (yet?)
      if (ACE_OS::strlen (this->S_) == 0)
        {
          os << "," << be_nl
             << "TAO::Value_Traits<" << node->name () << ">";
        }

      os << "," << be_nl << this->insert_policy()
         << be_uidt_nl
         << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
         << "{" << be_nl
         << "};";

      os.gen_endif ();
    }

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_valuetype (be_valuetype *node)
{
  if (this->generated (node))
    {
      return 0;
    }

  if (node->seen_in_operation ())
    {
      TAO_OutStream & os = *this->ctx_->stream ();

      os << be_nl_2
         << "// TAO_IDL - Generated from" << be_nl
         << "// " << __FILE__ << ":" << __LINE__;

      std::string guard_suffix =
        std::string (this->S_) + std::string ("arg_traits");

      // This should be generated even for imported nodes. The ifdef
      // guard prevents multiple declarations.
      os.gen_ifdef_macro (node->flat_name (), guard_suffix.c_str (), false);

      os << be_nl_2
         << "template<>" << be_nl
         << "class "
         << this->S_ << "Arg_Traits<"
         << node->name () << ">" << be_idt_nl
         << ": public" << be_idt << be_idt_nl
         << "Object_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
         << node->name () << " *," << be_nl
         << node->name () << "_var," << be_nl
         << node->name () << "_out";

      // The SArgument classes don't need the traits parameter (yet?)
      if (ACE_OS::strlen (this->S_) == 0)
        {
          os << "," << be_nl
             << "TAO::Value_Traits<" << node->name () << ">";
        }

      os << "," << be_nl << this->insert_policy()
         << be_uidt_nl
         << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
         << "{" << be_nl
         << "};";

      os.gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_valuetype - visit scope failed\n"),
                        -1);
    }

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  if (this->generated (node))
    {
      return 0;
    }

  be_valuetype *fd =
    be_valuetype::narrow_from_decl (node->full_definition ());

  // The logic in visit_valuetype() should handle what gets generated
  // and what doesn't.
  int status = this->visit_valuetype (fd);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_valuetype_fwd - code generation failed\n"),
                        -1);
    }

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_arg_traits::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_arg_traits::visit_operation (be_operation *node)
{
  if (this->generated (node) || node->is_local () || node->imported ())
    {
      return 0;
    }

  AST_Type *rt = node->return_type ();
  AST_Decl::NodeType nt = rt->node_type ();

  // If our return type is an unaliased bounded (w)string, we create
  // an empty struct using the operation's flat name for the type,
  // and use this type as the Arg_Traits<> template parameter. All
  // this is necessary because there could be any number of such
  // return types, all identical, in the same interface, valuetype,
  // translation unit, or build, and we need a unique type for the
  // Arg_Traits<> template parameter.
  if (nt == AST_Decl::NT_string || nt == AST_Decl::NT_wstring)
    {
      AST_String *str = AST_String::narrow_from_decl (rt);
      ACE_CDR::ULong bound = str->max_size ()->ev ()->u.ulval;

      if (bound > 0)
        {
          TAO_OutStream *os = this->ctx_->stream ();

          *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__;

          bool wide = (str->width () != 1);

          *os << be_nl_2;

          bool const skel =
            (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS);

          // Avoid generating a duplicate structure in the skeleton
          // when generating Arg_Traits<> for ThruPOA and direct
          // collocation code.
          if (!skel
              || (skel && ACE_OS::strlen (this->S_) != 0))
            {
              *os << "struct " << node->flat_name () << " {};"
                  << be_nl_2;
            }

          *os << "template<>" << be_nl
              << "class "
              << this->S_ << "Arg_Traits<"
              << node->flat_name ()
              << ">" << be_idt_nl
              << ": public" << be_idt << be_idt_nl
              << "BD_String_" << this->S_ << "Arg_Traits_T<" << be_nl
              << "CORBA::" << (wide ? "W" : "") << "String_var," << be_nl
              << bound << "," << be_nl
              << this->insert_policy()
              << ">"
              << be_uidt << be_uidt << be_uidt_nl
              << "{" << be_nl
              << "};";
        }
    }

  // This will catch (in visit_argument() below) any parameters that
  // are unaliased, bounded (w)strings.
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_operation - visit scope failed\n"),
                        -1);
    }

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_attribute (be_attribute *node)
{
  if (this->ctx_->alias () != 0 || this->generated (node))
    {
      return 0;
    }

  AST_String *st = AST_String::narrow_from_decl (node->field_type ());

  if (st == 0)
    {
      return 0;
    }

  ACE_CDR::ULong bound = st->max_size ()->ev ()->u.ulval;

  if (bound == 0)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  bool wide = (st->width () != 1);

  // It is legal IDL to declare a bounded (w)string as an operation
  // parameter type. There could be any number of identical
  // declarations in the same build, translation unit, or even in
  // the same operation, so we use the argument's flat name to
  // declare an empty struct, and use that struct as the template
  // parameter for Arg_Traits<>.

  *os << be_nl;

  bool const skel =
    (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS);

  // Avoid generating a duplicate structure in the skeleton when
  // generating Arg_Traits<> for ThruPOA and direct collocation code.
  if (!skel
      || (skel && ACE_OS::strlen (this->S_) != 0))
    {
      *os << "struct " << node->flat_name () << " {};"
          << be_nl_2;
    }

  *os << "template<>" << be_nl
      << "class "
      << this->S_ << "Arg_Traits<"
      << node->flat_name ()
      << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "BD_String_" << this->S_ << "Arg_Traits_T<" << be_nl
      << "CORBA::" << (wide ? "W" : "") << "String_var," << be_nl
      << bound << "," << be_nl
      << this->insert_policy()
      << ">"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_argument (be_argument *node)
{
  if (this->ctx_->alias () != 0 || this->generated (node))
    {
      return 0;
    }

  AST_Type *bt = node->field_type ();
  AST_Decl::NodeType nt = bt->node_type ();

  // We are interested here only in unaliased, bounded
  // (w)strings.

  if (nt != AST_Decl::NT_string && nt != AST_Decl::NT_wstring)
    {
      return 0;
    }

  be_string *st = be_string::narrow_from_decl (bt);
  ACE_CDR::ULong bound = st->max_size ()->ev ()->u.ulval;

  if (bound == 0)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  bool wide = (st->width () != 1);

  // It is legal IDL to declare a bounded (w)string as an operation
  // parameter type. There could be any number of identical
  // declarations in the same build, translation unit, or even in
  // the same operation, so we use the argument's flat name to
  // declare an empty struct, and use that struct as the template
  // parameter for Arg_Traits<>.
  *os << be_nl_2;

  bool const skel =
    (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS);

  AST_Decl *op = ScopeAsDecl (node->defined_in ());
  AST_Decl *intf = ScopeAsDecl (op->defined_in ());
  ACE_CString arg_flat_name (intf->flat_name ());
  arg_flat_name += '_';
  arg_flat_name += op->local_name ()->get_string ();
  arg_flat_name += '_';
  arg_flat_name += node->local_name ()->get_string ();

  // Avoid generating a duplicate structure in the skeleton when
  // generating Arg_Traits<> for ThruPOA and direct collocation code.
  if (!skel
      || (skel && ACE_OS::strlen (this->S_) != 0))
    {
      *os << "struct " << arg_flat_name.c_str () << " {};"
          << be_nl_2;
    }

  *os << "template<>" << be_nl
      << "class "
      << this->S_ << "Arg_Traits<"
      << arg_flat_name.c_str ()
      << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "BD_String_" << this->S_ << "Arg_Traits_T<" << be_nl
      << "CORBA::" << (wide ? "W" : "") << "String_var," << be_nl
      << bound << "," << be_nl
      << this->insert_policy()
      << be_uidt_nl
      << ">"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_sequence (be_sequence *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *alias = this->ctx_->alias ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (alias->flat_name (),
                       guard_suffix.c_str (),
                       false);

  bool use_vec = (node->unbounded () && be_global->alt_mapping ());
  UTL_ScopedName *sn = alias->name ();

  *os << be_nl_2
      << "template<>" << be_nl
      << "class " << this->S_ << "Arg_Traits<" << sn << ">"
      << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << (use_vec ? "Vector_" : "Var_Size_")
      << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << sn << "," << be_nl
      << this->insert_policy() << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";


  os->gen_endif ();

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_string (be_string *node)
{
  if ((this->generated (node) && !this->ctx_->alias())
       || !node->seen_in_operation ())
    {
      return 0;
    }

  ACE_CDR::ULong bound = node->max_size ()->ev ()->u.ulval;
  be_type *alias = this->ctx_->alias ();

  // Unbounded (w)string args are handled as a predefined type.
  // Bounded (w)strings must come in as a typedef - they can't
  // be used directly as arguments or return types.
  if (bound == 0)
    {
      return 0;
    }

  bool wide = (node->width () != 1);

  TAO_OutStream *os = this->ctx_->stream ();

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.

  if (alias == 0)
    {
      os->gen_ifdef_macro (node->flat_name(), guard_suffix.c_str (), false);
    }
  else
    {
      // Form a unique macro name using the local name and the bound.
      ACE_CDR::ULong l = bound;
      int num_digits = 0;

      while (l > 0)
        {
          l /= 10 ;
          ++num_digits ;
        }

      size_t bound_length = num_digits + 1;
      char* bound_string = 0;
      ACE_NEW_RETURN (bound_string, char[bound_length], -1) ;
      ACE_OS::sprintf (bound_string, "%u", bound);

      size_t cat_length = ACE_OS::strlen (alias->local_name ()->get_string ()) +
                          ACE_OS::strlen (bound_string) +
                          1;
      char* cat_string = 0;
      ACE_NEW_RETURN (cat_string, char[cat_length], -1) ;
      ACE_OS::strcpy (cat_string, alias->local_name ()->get_string ()) ;
      ACE_OS::strcat (cat_string, bound_string);

      os->gen_ifdef_macro (cat_string, guard_suffix.c_str (), false);

      delete [] cat_string;
      delete [] bound_string;
    }

  bool const skel =
    (this->ctx_->state () == TAO_CodeGen::TAO_ROOT_SS);

  // Avoid generating a duplicate structure in the skeleton when
  // generating Arg_Traits<> for ThruPOA and direct collocation code.
  if (!skel
      || (skel && ACE_OS::strlen (this->S_) != 0))
    {
      // A workaround 'dummy' type, since bounded (w)strings are all
      // generated as typedefs of (w)char *.
      *os << be_nl_2
          << "struct ";

      if (alias == 0)
        {
          *os << node->flat_name ();
        }
      else
        {
          *os << alias->local_name () << "_" << bound;
        }

      *os << " {};";
    }

  *os << be_nl_2
      << "template<>" << be_nl
      << "class "
      << this->S_ << "Arg_Traits<";

  if (0 == alias)
    {
      *os << node->flat_name ();
    }
  else
    {
      *os << alias->local_name () << "_" << bound;
    }

  *os << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "BD_String_" << this->S_ << "Arg_Traits_T<"
      << be_idt << be_idt_nl
      << "CORBA::" << (wide ? "W" : "") << "String_var," << be_nl
      << bound << "," << be_nl
      << this->insert_policy()
      << be_uidt << be_uidt_nl
      << ">"
      << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_array (be_array *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  ACE_CString suffix (this->S_);
  suffix += "arg_traits";
  os->gen_ifdef_macro (node->flat_name (), suffix.c_str (), false);

  *os << be_nl_2
      << "template<>" << be_nl
      << "class "
      << this->S_ << "Arg_Traits<"
      << node->name () << "_tag>" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
      << "_Array_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << node->name ()
      << (node->size_type () == AST_Type::VARIABLE ? "_out" : "_var")
      << "," << be_nl;

  *os << node->name () << "_forany";

  *os << "," << be_nl << this->insert_policy();

  *os << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_enum (be_enum *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str (), false);

  *os << be_nl_2
      << "template<>" << be_nl
      << "class "
      << this->S_ << "Arg_Traits<"
      << node->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << "Basic_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << node->name () << "," << be_nl
      << this->insert_policy() << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_structure (be_structure *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str (), false);

  *os << be_nl_2
      << "template<>" << be_nl
      << "class "
      << this->S_ << "Arg_Traits<"
      << node->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
      << "_Size_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl;

  *os << node->name () << "," << be_nl << this->insert_policy() << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  /* Set this before visiting the scope so things like

      interface foo
      {
        struct bar
        {
          ....
          foo foo_member;
        };

        void op (in bar inarg);
      };

     will not cause infinite recursion in this visitor.
  */

  this->generated (node, true);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_structure - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_arg_traits::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  // Valuetypes may not be *declared* in a field, so this will
  // get handled elsewhere, and will also avoid nested valuetype
  // recursion. So we set the field node as processed (the
  // field *type* may not have been reached yet) and return.
  AST_Decl::NodeType nt = bt->base_node_type ();

  if (nt == AST_Decl::NT_valuetype || nt == AST_Decl::NT_eventtype)
    {
      node->cli_traits_gen (true);
      return 0;
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  this->generated (node, true);
  this->generated (bt, true);
  return 0;
}

int
be_visitor_arg_traits::visit_union (be_union *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  std::string guard_suffix =
    std::string (this->S_) + std::string ("arg_traits");

  // This should be generated even for imported nodes. The ifdef
  // guard prevents multiple declarations.
  os->gen_ifdef_macro (node->flat_name (), guard_suffix.c_str (), false);

  *os << be_nl_2
      << "template<>" << be_nl
      << "class "
      << this->S_ << "Arg_Traits<"
      << node->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
      << "_Size_" << this->S_ << "Arg_Traits_T<" << be_idt << be_idt_nl
      << node->name () << "," << be_nl << this->insert_policy();

  *os << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  /* Set this before visiting the scope so things like

      interface foo
      {
        struct bar
        {
          ....
          foo foo_member;
        };

        void op (in bar inarg);
      };

     will not cause infinite recursion in this visitor.
  */

  this->generated (node, true);

  int status = this->visit_scope (node);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_union - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_arg_traits::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"),
                        -1);
    }

  this->generated (node, true);
  return 0;
}

int
be_visitor_arg_traits::visit_typedef (be_typedef *node)
{
  if (this->generated (node) || !node->seen_in_operation ())
    {
      return 0;
    }

  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  // We can't set seen_in_operation_ for the base type
  // in the be_typedef operation, since valuetype OBV
  // constructor code may reset it to FALSE, and the base
  // type may be used unaliased in another arg somewhere.
  // So we just set it to TRUE here, since we know it
  // has to be TRUE at this point. We also set the
  // 'generated' flag to false if the original value
  // of 'seen_in_operation' was false, since the base
  // type could have been processed already, as a member
  // for example, before the typedef was seen, which
  // would short-circuit things.
  if (!bt->seen_in_operation ())
    {
      bt->seen_in_operation (true);
      this->generated (bt, false);
    }

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_arg_traits::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  this->generated (node, true);
  return 0;
}

bool
be_visitor_arg_traits::generated (be_decl *node) const
{
  if (ACE_OS::strcmp (this->S_, "") == 0)
    {
      switch (this->ctx_->state ())
        {
          case TAO_CodeGen::TAO_ROOT_CS:
            return node->cli_arg_traits_gen ();
          case TAO_CodeGen::TAO_ROOT_SS:
            return node->srv_arg_traits_gen ();
          default:
            return 0;
        }
    }

  return node->srv_sarg_traits_gen ();
}

void
be_visitor_arg_traits::generated (be_decl *node,
                                  bool val)
{
  if (ACE_OS::strcmp (this->S_, "") == 0)
    {
      switch (this->ctx_->state ())
        {
          case TAO_CodeGen::TAO_ROOT_CS:
            node->cli_arg_traits_gen (val);
            return;
          case TAO_CodeGen::TAO_ROOT_SS:
            node->srv_arg_traits_gen (val);
            return;
          default:
            return;
        }
    }

  node->srv_sarg_traits_gen (val);
}

int
be_visitor_arg_traits::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_arg_traits::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_arg_traits::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

int
be_visitor_arg_traits::visit_home (be_home *node)
{
  return this->visit_interface (node);
}

const char *
be_visitor_arg_traits::insert_policy (void)
{
  if (be_global->any_support ())
    {
      return "TAO::Any_Insert_Policy_Stream";
    }
  else
    {
      return "TAO::Any_Insert_Policy_Noop";
    }
}
