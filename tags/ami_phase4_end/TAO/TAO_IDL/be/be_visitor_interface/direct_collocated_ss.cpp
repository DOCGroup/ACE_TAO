//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    direct_collocated_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for direct_collocated classes for an Interface in the
//    server skeleton file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, direct_collocated_ss, "$Id$")


// ************************************************************
//  be_visitor_interface_collacted_ss
// ************************************************************

be_visitor_interface_direct_collocated_ss::be_visitor_interface_direct_collocated_ss
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_direct_collocated_ss::~be_visitor_interface_direct_collocated_ss (void)
{
}

int be_visitor_interface_direct_collocated_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  this->ctx_->node (node);

  os->indent ();
  *os << node->full_coll_name (be_interface::DIRECT) << "::"
      << node->local_coll_name (be_interface::DIRECT) << " (\n";

  os->incr_indent (0);
  os->incr_indent ();
  *os << node->full_skel_name () << "_ptr "
      << " servant," << be_nl;

  *os << "TAO_Stub *stub\n";
  os->decr_indent ();
  *os << ")\n";
  os->decr_indent (0);

  os->incr_indent ();

  if (node->is_nested ())
    {
      be_decl* scope =
	be_scope::narrow_from_scope (node->defined_in ())->decl ();

      *os << ": ACE_NESTED_CLASS ("
	  << scope->name () << ","
	  << node->local_name ()
	  << ") ()," << be_nl;
    }
  else
    {
      *os << ": " << node->name ()
	  << " ()," << be_nl;
    }

  // @@ We should call the constructor for all base classes, since we
  // are using multiple inheritance.

  if (node->traverse_inheritance_graph (be_visitor_interface_direct_collocated_ss::collocated_ctor_helper, os)
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_direct_collocated_ss::"
                         "visit_interface - "
                         "codegen for base class ctor init failed\n"),
                        -1);
    }

  *os << "  CORBA_Object (stub, servant, 1)," << be_nl
      << "  servant_ (servant)";

  *os << "\n";
  os->decr_indent ();
  *os << "{\n";
  *os << "}\n\n";

  os->indent ();

  // Generate _is_a implementation.
  *os << "CORBA::Boolean " << node->full_coll_name (be_interface::DIRECT) << "::"
      << "_is_a"  << be_idt
      << "(" << be_idt_nl
      << "const CORBA::Char *logical_type_id," << be_nl
      << "CORBA_Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl << be_nl;
  *os << "{" << be_idt_nl
      << "return this->servant_->_is_a (logical_type_id, ACE_TRY_ENV);" << be_uidt_nl
      << "}\n\n" << be_uidt_nl;

  // Generate _get_servant implementation.
  *os << node->full_skel_name () << "_ptr "
      << node->full_coll_name (be_interface::DIRECT) << "::"
      << "_get_servant (void) const\n"
      << "{\n";
  os->incr_indent ();
  *os << "return this->servant_;\n";
  os->decr_indent ();
  *os << "}\n\n";

  os->indent ();

  // Generate _non_existent implementation.
  *os << "CORBA::Boolean " << node->full_coll_name (be_interface::DIRECT) << "::"
      << "_non_existent"  << be_idt
      << "(" << be_idt_nl
      << "CORBA_Environment &ACE_TRY_ENV" << be_uidt_nl
      << ")" << be_uidt_nl << be_nl;
  *os << "{" << be_idt_nl
      << "return this->servant_->_non_existent (ACE_TRY_ENV);" << be_uidt_nl
      << "}\n\n" << be_uidt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_direct_collocated_ss::"
                         "visit_scope - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_interface_direct_collocated_ss::collocated_ctor_helper (be_interface *derived,
                                                                   be_interface *base,
                                                                   TAO_OutStream *os)
{
  if (derived == base)
    // we are the same. Don't do anything, otherwise we will end up calling
    // ourself
    return 0;

  if (base->is_nested ())
    {
      be_decl *scope;
      scope = be_scope::narrow_from_scope (base->defined_in ())->decl ();
      *os << "  ACE_NESTED_CLASS (POA_" << scope->name () << ","
          << base->local_coll_name (be_interface::DIRECT) << ") (servant, stub)," << be_nl;
    }
  else
    {
      *os << "  " << base->full_coll_name (be_interface::DIRECT) << " (servant, stub)," << be_nl;
    }

  return 0;
}
