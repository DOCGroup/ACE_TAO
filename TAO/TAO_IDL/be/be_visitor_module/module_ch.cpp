
//=============================================================================
/**
 *  @file    module_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Module in the client header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "module.h"

be_visitor_module_ch::be_visitor_module_ch (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_ch::~be_visitor_module_ch (void)
{
}

int
be_visitor_module_ch::visit_module (be_module *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  TAO_OutStream *aos = 0;

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "namespace " << node->local_name () << be_nl
      << "{" << be_idt;

  if (be_global->gen_anyop_files ())
    {
      aos = tao_cg->anyop_header ();

      *aos << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
           << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

      *aos << "namespace " << node->local_name () << be_nl
           << "{" << be_idt;
    }

  // Generate code for the module definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module_ch::"
                         "visit_module - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  *os << be_nl
      << "} // module " << node->name ();

  if (be_global->gen_anyop_files ())
    {
      *aos << be_uidt_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
           << "// " << __FILE__ << ":" << __LINE__ << be_nl;

      *aos << be_nl
           << "} // module " << node->name () << be_nl;
    }

  return 0;
}
