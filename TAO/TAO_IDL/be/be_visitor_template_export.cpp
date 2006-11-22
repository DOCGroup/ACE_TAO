//=============================================================================
/**
*  @file   be_visitor_template_export.cpp
*
*  $Id$
*
*  This visitor generates template instantiations with export macro
*
*  @author Jeff Parsons <j.parsons@vanderbilt.edu>
*/
//=============================================================================

#include "be_visitor_template_export.h"
#include "be_visitor_context.h"
#include "be_root.h"
#include "be_module.h"
#include "be_typedef.h"
#include "be_sequence.h"
#include "be_extern.h"
#include "be_helper.h"

ACE_RCSID (be,
           be_visitor_template_export,
           "$Id$")

be_visitor_template_export::be_visitor_template_export (
  be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_template_export::~be_visitor_template_export (void)
{
}

int
be_visitor_template_export::visit_root (be_root *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "// Workaround for a Visual Studio .NET bug where this class is not" << be_nl
      << "// properly imported by an application if typedef'd or subclassed," << be_nl
      << "// resulting in 'multiply defined' link errors. The export macro" << be_nl
      << "// here forces an explicit import by the application. Please see" << be_nl
      << "// http://support.microsoft.com/default.aspx?scid=kb;en-us;309801" << be_nl
      << "// The problem stems from use of the type below in PortableServer," << be_nl
      << "// but we put the instantiation here because the application will" << be_nl
      << "// need to see it in *C.h to avoid the error." << be_nl
      << "#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_template_export::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */";

  return 0;
}

int
be_visitor_template_export::visit_module (be_module *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_template_export::"
                         "visit_module - visit scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_template_export::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl
      << "template class " << be_global->stub_export_macro ()
      << " ";

  if (node->gen_base_class_name (os, "", this->ctx_->scope ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_template_export::"
                         "visit_sequence - "
                         "Base class name generation failed\n"),
                        -1);
    }

  *os << ";";

  return 0;
}

int
be_visitor_template_export::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_template_export::"
                         "visit_typedef - "
                         "visit base type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}
