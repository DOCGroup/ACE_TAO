
//=============================================================================
/**
 *  @file    component_svts.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Components in the templated servant source.
 *
 *
 *  @author Marcel Smit
 */
//=============================================================================

#include "component.h"

be_visitor_component_svts::be_visitor_component_svts (be_visitor_context *ctx)
  : be_visitor_component (ctx),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->svnt_export_macro ())
{
  /// All existing CIAO examples set the servant export values in the CIDL
  /// compiler to equal the IDL compiler's skel export values. Below is a
  /// partial effort to decouple them, should be completely decoupled
  /// sometime. See comment in codegen.cpp, line 1173.
  if (export_macro_ == "")
    {
      export_macro_ = be_global->skel_export_macro ();
    }
}

be_visitor_component_svts::~be_visitor_component_svts (void)
{
}

int
be_visitor_component_svts::visit_component (be_component *node)
{
  if (node->imported ())
    {
      return 0;
    }

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl_2
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  be_visitor_context_svts context_visitor (this->ctx_);

  if (context_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svts::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("context visitor failed\n")),
                        -1);
    }

//   be_visitor_servant_svs servant_visitor (this->ctx_);
//
//   if (servant_visitor.visit_component (node) == -1)
//     {
//       ACE_ERROR_RETURN ((LM_ERROR,
//                          ACE_TEXT ("be_visitor_component_svts::")
//                          ACE_TEXT ("visit_component - ")
//                          ACE_TEXT ("servant visitor failed\n")),
//                         -1);
//     }
//
  os_ << be_uidt_nl
      << "}";
//
  return 0;
}

int
be_visitor_component_svts::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

