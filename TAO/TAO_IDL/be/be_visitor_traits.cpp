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
#include "be_structure.h"
#include "be_union.h"
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
  if (!node->imported () && this->visit_scope (node) == -1)
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

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  os->gen_ifdef_macro (node->flat_name (), "traits");

  // Since the three blocks below generate specialized (i.e., non-template)
  // classes, we don't want to generate them unless it's necessary - thus
  // the logic surrounding each one.

  // This is used by the _var and _out classes, so it should always be
  // generated in the main file.
  if (!node->imported ())
    {
      *os << be_nl << be_nl
          << "template<>" << be_nl
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
    }

  // This should be generated even for imported nodes. The ifdef guard
  // around these and the one above will protect against double declaration.
  if (node->seen_in_operation ())
    {
      *os << be_nl << be_nl
          << "template<>" << be_nl
          << "class " << be_global->stub_export_macro () << " Arg_Traits<" 
          << node->name () << ">" << be_idt_nl
          << ": public" << be_idt << be_idt_nl
          << "Object_Arg_Traits_T<" << be_idt << be_idt_nl
          << node->name () << "," << be_nl
          << node->name () << "_var," << be_nl
          << node->name () << "_out" << be_uidt_nl
          << ">" << be_uidt << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "};";
    }

  os->gen_endif ();

  int status = this->visit_scope (node);

  if (status != 0)
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
  int status = this->visit_interface (fd);

  if (status != 0)
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
  if (node->imported () || node->cli_traits_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl << be_nl
      << "template<>" << be_nl
      << "struct " << be_global->stub_export_macro () << " Value_Traits<"
      << node->name () << ">" << be_nl
      << "{" << be_idt_nl
      << "static void tao_add_ref (" << node->name () << " *);" << be_nl
      << "static void tao_remove_ref (" << node->name () << " *);" 
      << be_uidt_nl
      << "};";

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
  if (node->imported () || node->cli_traits_gen ())
    {
      return 0;
    }

  node->cli_traits_gen (I_TRUE);
  be_decl *fd =
    be_decl::narrow_from_decl (node->full_definition ());
  fd->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_eventtype (be_eventtype *node)
{
  return 0;
}

int 
be_visitor_traits::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return 0;
}

int 
be_visitor_traits::visit_array (be_array *node)
{
  if (node->imported () || node->cli_traits_gen ())
    {
      return 0;
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_enum (be_enum *node)
{
  if (node->imported () || node->cli_traits_gen ())
    {
      return 0;
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}

int 
be_visitor_traits::visit_structure (be_structure *node)
{
  if (node->imported () || node->cli_traits_gen ())
    {
      return 0;
    }

  int status = this->visit_scope (node);

  if (status != 0)
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
be_visitor_traits::visit_union (be_union *node)
{
  if (node->imported () || node->cli_traits_gen ())
    {
      return 0;
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
be_visitor_traits::visit_typedef (be_typedef *node)
{
  if (node->imported () || node->cli_traits_gen ())
    {
      return 0;
    }

  node->cli_traits_gen (I_TRUE);
  return 0;
}


