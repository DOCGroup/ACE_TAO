//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    thru_poa_collocated_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for thru_poa_collocated classes for the Interface node
//    inside the server header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, thru_poa_collocated_sh, "$Id$")


// ************************************************************
//  thru_poa_collocated class in header
// ************************************************************

be_visitor_interface_thru_poa_collocated_sh::be_visitor_interface_thru_poa_collocated_sh
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_thru_poa_collocated_sh::~be_visitor_interface_thru_poa_collocated_sh (void)
{
}

int be_visitor_interface_thru_poa_collocated_sh::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  os->gen_ifdef_macro (node->flat_name (), "_thru_poa_collocated");

  // output the class defn
  os->indent ();
  *os << "class " << be_global->skel_export_macro ()
      << " " << node->local_coll_name (be_interface::THRU_POA);
  os->incr_indent ();
  *os << " : public virtual ::" << node->full_name ();

  // generate base classes if any
  if (node->n_inherits () > 0)
    {
      for (int i = 0; i < node->n_inherits (); ++i)
        {
          *os << "," << be_nl;
          be_interface* parent =
            be_interface::narrow_from_decl (node->inherits()[i]);
          *os << "  public virtual "
              << be_interface::relative_name (parent->full_coll_name (be_interface::THRU_POA),
                                              node->full_coll_name (be_interface::THRU_POA));
        }
    }
  *os << "\n";
  os->decr_indent ();
  *os << "{" << be_nl;
  *os << "public:\n";
  os->incr_indent ();

  *os << node->local_coll_name (be_interface::THRU_POA) << " (\n";

  os->incr_indent (0);
  os->incr_indent ();

  *os << "TAO_Stub *stub\n";
  os->decr_indent ();
  *os << ");\n";
  os->decr_indent (0);

  os->indent ();

  *os << "virtual CORBA::Boolean _is_a" << be_idt
      << "(" << be_idt_nl
      << "const CORBA::Char *logical_type_id" << be_nl
      << "ACE_ENV_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  *os << "virtual CORBA::Boolean _non_existent" << be_idt
      << "(" << be_idt_nl
      << "ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_thru_poa_collocated_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  os->decr_indent ();

  *os << "};\n\n";

  os->gen_endif ();

  return 0;
}
