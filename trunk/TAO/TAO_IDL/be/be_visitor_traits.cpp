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
#include "be_typedef.h"
#include "be_helper.h"
#include "be_extern.h"

ACE_RCSID (be,
           be_visitor_traits,
           "$Id$")

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
  if (!node->imported ())
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
  if (!node->imported ())
    {
      os->gen_ifdef_macro (node->flat_name (), "traits");

      *os << be_nl << be_nl
          << "ACE_TEMPLATE_SPECIALIZATION" << be_nl
          << "struct " << be_global->stub_export_macro () << " Value_Traits<"
          << node->name () << ">" << be_nl
          << "{" << be_idt_nl
          << "static void tao_add_ref (" << node->name () << " *);" << be_nl
          << "static void tao_remove_ref (" << node->name () << " *);" 
          << be_nl
          << "static void tao_release (" << node->name () << " *);"
          << be_uidt_nl
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
