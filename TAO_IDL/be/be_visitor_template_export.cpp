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
#include "be_predefined_type.h"
#include "ace/Log_Msg.h"

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

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "#if defined ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_template_export::"
                         "visit_root - visit scope failed\n"),
                        -1);
    }

  *os << be_nl << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION_EXPORT */";

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

  be_type *bt = be_type::narrow_from_decl (node->base_type ());

  // TAO provides extensions for octet sequences, first find out if
  // the base type is an octet (or an alias for octet).
  be_predefined_type *predef = 0;

  if (bt->base_node_type () == AST_Type::NT_pre_defined)
    {
      be_typedef* alias =
            be_typedef::narrow_from_decl (bt);

      if (alias == 0)
        {
          predef = be_predefined_type::narrow_from_decl (bt);
        }
      else
        {
          predef =
            be_predefined_type::narrow_from_decl (
                alias->primitive_base_type ()
              );
        }
    }

  // When it is a sequence add a special guard
  if (predef != 0 && predef->pt () == AST_PredefinedType::PT_octet
      && node->unbounded ())
    {
      *os << "\n#if (TAO_NO_COPY_OCTET_SEQUENCES == 0)";
    }

  *os << be_idt << be_nl
      << "template class " << be_global->stub_export_macro ()
      << " ";

  if (node->gen_base_class_name (os,
                                 "",
                                 this->ctx_->scope ()->decl ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_template_export::"
                         "visit_sequence - "
                         "Base class name generation failed\n"),
                        -1);
    }

  *os << ";" << be_uidt;

  if (predef != 0 && predef->pt () == AST_PredefinedType::PT_octet
      && node->unbounded ())
    {
      *os << "\n#endif /* TAO_NO_COPY_OCTET_SEQUENCE == 0 */";
    }

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
