//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_ci, "$Id$")


// **************************************************
// Interface visitor for client inline
// **************************************************
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

  // A forward declared interface may have set this flag.
  if (node->cli_inline_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->flat_name (), "");

  // Global functions to allow non-defined forward declared interfaces
  // access to some methods in the full definition.
  *os << "ACE_INLINE " << node->full_name () << "_ptr" << be_nl
      << "tao_" << node->flat_name () << "_duplicate (" << be_idt << be_idt_nl
      << node->full_name () << "_ptr p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name () << "::_duplicate (p);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "ACE_INLINE void" << be_nl
      << "tao_" << node->flat_name () << "_release (" << be_idt << be_idt_nl
      << node->full_name () << "_ptr p" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "CORBA::release (p);" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "ACE_INLINE " << node->full_name () <<  "_ptr" << be_nl
      << "tao_" << node->flat_name () << "_nil (" << be_idt << be_idt_nl
      << "void" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name () << "::_nil ();" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "ACE_INLINE " << node->full_name () << "_ptr" << be_nl
      << "tao_" << node->flat_name () << "_narrow (" << be_idt << be_idt_nl
      << "CORBA::Object *p," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return " << node->full_name () << "::_narrow (p, ACE_TRY_ENV);" 
      << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "ACE_INLINE CORBA::Object *" << be_nl
      << "tao_" << node->flat_name () << "_upcast (" << be_idt << be_idt_nl
      << "void *src" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << node->full_name () << " **tmp =" << be_idt_nl
      << "ACE_static_cast (" << node->full_name () << " **, src);" 
      << be_uidt_nl
      << "return *tmp;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generate the constructor from stub and servant.
  node->gen_stub_ctor (os);

  os->gen_endif ();

  return 0;
}
