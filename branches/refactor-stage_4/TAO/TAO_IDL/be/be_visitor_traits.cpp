//=============================================================================
/**
*  @file   be_visitor_traits.cpp
*
*  $Id$
*
*  This visitor generates template specializations for traits of various
*  kinds for IDL declarations. These specialized template classes are then
*  used in other template classes in the ORB.
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

#include "be_visitor_traits.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_module.h"
#include "be_interface.h"
#include "be_valuetype.h"
#include "be_interface_fwd.h"
#include "be_valuetype_fwd.h"
#include "be_eventtype.h"
#include "be_eventtype_fwd.h"
#include "be_array.h"
#include "be_enum.h"
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

be_visitor_traits::be_visitor_traits (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_traits::~be_visitor_traits (void)
{
}

int
be_visitor_traits::visit_root (be_root *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl
      << "// Traits specializations." << be_nl
      << "namespace TAO" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_traits::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_module - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_traits::visit_interface (be_interface *node)
{
  if (node->cli_traits_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Since the three blocks below generate specialized (i.e., non-template)
  // classes, we don't want to generate them unless it's necessary - thus
  // the logic surrounding each one.

  // I think we need to generate this only for non-defined forward
  // declarations.
  if (!node->imported () && !node->is_defined ())
    {
      os->gen_ifdef_macro (node->flat_name (), "traits");

      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "struct " << be_global->stub_export_macro () << " Objref_Traits<"
          << node->name () << ">" << be_nl
          << "{" << be_idt_nl
          << "static " << node->name () << "_ptr tao_duplicate (" 
          << be_idt << be_idt_nl
          << node->name () << "_ptr" << be_uidt_nl
          << ");" << be_uidt_nl
          << "static void tao_release (" << be_idt << be_idt_nl
          << node->name () << "_ptr" << be_uidt_nl
          << ");" << be_uidt_nl
          << "static " << node->name () << "_ptr tao_nil (void);" << be_nl
          << "static CORBA::Boolean tao_marshal (" << be_idt << be_idt_nl
          << node->name () << "_ptr p," << be_nl
          << "TAO_OutputCDR & cdr" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "};";

      os->gen_endif ();
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "arg_traits");

      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "class " << be_global->stub_export_macro () << " Arg_Traits<" 
          << node->name () << ">" << be_idt_nl
          << ": public" << be_idt << be_idt_nl
          << "Object_Arg_Traits_T<" << be_idt << be_idt_nl
          << node->name () << "_ptr," << be_nl
          << node->name () << "_var," << be_nl
          << node->name () << "_out" << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "};";

      os->gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_interface - visit scope failed\n"),
                        -1);
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_interface_fwd (be_interface_fwd *node)
{
  if (node->cli_traits_gen ())
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
                         "(%N:%l) be_visitor_traits::"
                         "visit_interface_fwd - code generation failed\n"),
                        -1);
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_valuetype (be_valuetype *node)
{
  if (node->cli_traits_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // I think we need to generate this only for non-defined forward
  // declarations.
  if (!node->imported () && !node->is_defined ())
    {
      os->gen_ifdef_macro (node->flat_name (), "traits");

      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "struct " << be_global->stub_export_macro () << " Value_Traits<"
          << node->name () << ">" << be_nl
          << "{" << be_idt_nl
          << "static void tao_add_ref (" << node->name () << " *);" << be_nl
          << "static void tao_remove_ref (" << node->name () << " *);" 
          << be_uidt_nl
          << "};";

      os->gen_endif ();
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "arg_traits");

      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "class " << be_global->stub_export_macro () << " Arg_Traits<" 
          << node->name () << ">" << be_idt_nl
          << ": public" << be_idt << be_idt_nl
          << "Object_Arg_Traits_T<" << be_idt << be_idt_nl
          << node->name () << " *," << be_nl
          << node->name () << "_var," << be_nl
          << node->name () << "_out" << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "};";

      os->gen_endif ();
    }

  int status = this->visit_scope (node);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_valuetype - visit scope failed\n"),
                        -1);
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  if (node->cli_traits_gen ())
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
                         "(%N:%l) be_visitor_traits::"
                         "visit_valuetype_fwd - code generation failed\n"),
                        -1);
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int 
be_visitor_traits::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_traits::visit_sequence (be_sequence *node)
{
  if (node->cli_traits_gen () || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_typedef *alias = this->ctx_->alias ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os->gen_ifdef_macro (node->flat_name (), "traits");

  *os << be_nl << be_nl
      << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
      << "class " << be_global->stub_export_macro () << " Arg_Traits<"
      << alias->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "Var_Size_Arg_Traits_T<" << be_idt << be_idt_nl
      << alias->name () << "," << be_nl
      << alias->name () << "_var," << be_nl
      << alias->name () << "_out" << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_string (be_string *node)
{
  if (node->cli_traits_gen () || !node->seen_in_operation ())
    {
      return 0;
    }

  unsigned long bound = node->max_size ()->ev ()->u.ulval;
  be_typedef *alias = this->ctx_->alias ();

  // Unbounded (w)string args are handled as a predefined type.
  // Bounded (w)strings must come in as a typedef - they can't
  // be used directly as arguments or return types.
  if (bound == 0 || alias == 0)
    {
      return 0;
    }

  idl_bool wide = (node->width () != 1);

  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->flat_name (), "arg_traits");

  // A workaround 'dummy' type, since bounded (w)strings are all 
  // generated as typedefs of (w)char *.
  *os << be_nl << be_nl
      << "struct " << alias->local_name () << "_" << bound << " {};";

  *os << be_nl << be_nl
      << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
      << "class " << be_global->stub_export_macro () << " Arg_Traits<"
      << alias->local_name () << "_" << bound << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl
      << "BD_" << (wide ? "W" : "") 
      << "String_Arg_Traits<" << bound << ">" 
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_array (be_array *node)
{
  if (node->cli_traits_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // This is used by the _var and _out classes, so it should always be
  // generated in the main file.
  if (!node->imported () && !node->anonymous ())
    {
      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "struct " << be_global->stub_export_macro () << " Array_Traits<"
          << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_slice" << be_uidt_nl
          << ">" << be_uidt_nl
          << "{" << be_idt_nl
          << "static " << node->name () << "_slice * tao_alloc (void);" 
          << be_nl
          << "static void tao_free (" << node->name () 
          << "_slice *);" << be_nl
          << "static " << node->name () << "_slice * tao_dup (const "
          << node->name () << "_slice *);" << be_nl
          << "static void tao_copy (" << be_idt << be_idt_nl
          << node->name () << "_slice * tao_to," << be_nl
          << "const " << node->name () << "_slice * tao_from" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "};";
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  if (node->seen_in_operation ())
    {
      os->gen_ifdef_macro (node->flat_name (), "arg_traits");

      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "class " << be_global->stub_export_macro () << " Arg_Traits<" 
          << node->name () << ">" << be_idt_nl
          << ": public" << be_idt << be_idt_nl;

      *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
          << "_Array_Arg_Traits_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_slice," << be_nl
          << node->name () << "_var," << be_nl;

      if (node->size_type () == AST_Type::VARIABLE)
        {
          *os << node->name () << "_out," << be_nl;
        }

      *os << node->name () << "_forany" << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "};";

      os->gen_endif ();
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_enum (be_enum *node)
{
  if (node->cli_traits_gen () || !node->seen_in_operation ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  os->gen_ifdef_macro (node->flat_name (), "arg_traits");

  *os << be_nl << be_nl
      << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
      << "class " << be_global->stub_export_macro () << " Arg_Traits<" 
      << node->name () << ">" << be_idt_nl
      << ": public" << be_idt << be_idt_nl;

  *os << "Basic_Arg_Traits_T<" << be_idt << be_idt_nl
      << node->name () << be_uidt_nl
      << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "};";

  os->gen_endif ();

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_structure (be_structure *node)
{
  if (node->cli_traits_gen ())
    {
      return 0;
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  if (node->seen_in_operation ())
    {
      TAO_OutStream *os = this->ctx_->stream ();

      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      os->gen_ifdef_macro (node->flat_name (), "arg_traits");

      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "class " << be_global->stub_export_macro () << " Arg_Traits<" 
          << node->name () << ">" << be_idt_nl
          << ": public" << be_idt << be_idt_nl;

      *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
          << "_Size_Arg_Traits_T<" << be_idt << be_idt_nl
          << node->name ();

      if (node->size_type () == AST_Type::VARIABLE)
        {
          *os << "," << be_nl
              << node->name () << "_var," << be_nl
              << node->name () << "_out";
        }

      *os << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "};";

      os->gen_endif ();
    }

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_structure - visit scope failed\n"),
                        -1);
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int
be_visitor_traits::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_field - "
                         "Bad field type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_field - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  node->cli_traits_gen (I_TRUE);
  bt->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_union (be_union *node)
{
  if (node->cli_traits_gen ())
    {
      return 0;
    }

  // This should be generated even for imported nodes. The ifdef guard prevents
  // multiple declarations.
  if (node->seen_in_operation ())
    {
      TAO_OutStream *os = this->ctx_->stream ();

      *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__;

      os->gen_ifdef_macro (node->flat_name (), "arg_traits");

      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "class " << be_global->stub_export_macro () << " Arg_Traits<" 
          << node->name () << ">" << be_idt_nl
          << ": public" << be_idt << be_idt_nl;

      *os << (node->size_type () == AST_Type::FIXED ? "Fixed" : "Var")
          << "_Size_Arg_Traits_T<" << be_idt << be_idt_nl
          << node->name ();

      if (node->size_type () == AST_Type::VARIABLE)
        {
          *os << "," << be_nl
              << node->name () << "_var," << be_nl
              << node->name () << "_out";
        }

      *os << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "};";

      os->gen_endif ();
    }

  int status = this->visit_scope (node);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_union - visit scope failed\n"),
                        -1);
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int
be_visitor_traits::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"), 
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"), 
                        -1);
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_traits::"
                         "visit_typedef - "
                         "Bad primitive type\n"),
                        -1);
    }

  this->ctx_->alias (0);
  node->cli_traits_gen (I_TRUE);
  return 0;
}


