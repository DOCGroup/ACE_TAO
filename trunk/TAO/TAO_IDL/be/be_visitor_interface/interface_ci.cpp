
//=============================================================================
/**
 *  @file    interface_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the client inline file
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_ci::be_visitor_interface_ci (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ci::~be_visitor_interface_ci (void)
{
}

int
be_visitor_interface_ci::visit_interface (be_interface *node)
{
  if (node->imported ())
    {
      return 0;
    }

  // Generate inline methods for elements of our scope.
  // This will always be done here for any non-imported node,
  // whether the _var and _out code has been generated already
  // at a forward declaration or not.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  // Nothing to generate for a local interface except from it scope.
  if (node->is_local ())
    {
      node->cli_inline_gen (true);
      return 0;
    }

  // A forward declared interface may have set this flag.
  if (node->cli_inline_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  if (node->is_abstract ())
    {
      *os << be_nl_2
          << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name ()
          << " (void)" << be_nl
          << "{}" << be_nl_2;

      *os << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name ()
          << " (const " << node->local_name () << " &rhs)" << be_idt_nl
          << ": ::CORBA::AbstractBase (rhs)" << be_uidt_nl
          << "{}";
    }

  // Generate the constructor from stub and servant.
  node->gen_stub_ctor (os);

  if (!node->is_local () && !node->is_abstract ())
    {
      *os << be_nl_2
          << "ACE_INLINE" << be_nl;
      *os << node->name () << "::"
          << node->local_name () << " ("
          << be_idt << be_idt_nl
          << "::IOP::IOR *ior," << be_nl
          << "TAO_ORB_Core *oc)" << be_uidt_nl;
      *os << ": ::CORBA::Object (ior, oc)";

      *os << be_uidt_nl
          << "{" << be_nl
          << "}" ;
    }

  node->cli_inline_gen (true);
  return 0;
}

int
be_visitor_interface_ci::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_ci::visit_connector (be_connector *node)
{
  return this->visit_interface (node);
}

