//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_fwd_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interface_Fwd node in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_fwd, interface_fwd_ci, "$Id$")


// ********************************************************************
// Visitor implementation for the Interface_Fwd type
// This one for the client inline file
// ********************************************************************

be_visitor_interface_fwd_ci::be_visitor_interface_fwd_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_ci::~be_visitor_interface_fwd_ci (void)
{
}

// visit the Interface_Fwd_ci node and its scope
int
be_visitor_interface_fwd_ci::visit_interface_fwd (be_interface_fwd *node)
{
  if (node->cli_inline_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  if (! node->is_local ())
    {
      os->gen_ifdef_macro (node->flat_name (), "");

      *os << "ACE_INLINE" << be_nl;
      *os << node->name () << "::" << node->local_name () << " (" 
          << be_idt << be_idt_nl 
          << "TAO_Stub *objref," << be_nl
          << "CORBA::Boolean _tao_collocated," << be_nl 
	        << "TAO_Abstract_ServantBase *servant" << be_uidt_nl
	        << ")" // constructor 
	        << be_nl;
      *os << ": CORBA_Object (objref, _tao_collocated, servant)" << be_uidt_nl;
      *os << "{" << be_idt_nl
	        << "this->" << node->flat_name () 
          << "_setup_collocation (_tao_collocated);" << be_uidt_nl;
      *os << "}" << be_nl << be_nl;
      
      os->gen_endif ();
    }

  // Generate the constructor from stub and servant.
  os->gen_ifdef_macro (node->flat_name (), "_var");
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _var failed\n"), -1);
    }
  os->gen_endif ();

  // generate the ifdefined macro for  the _out type
  os->gen_ifdef_macro (node->flat_name (), "_out");
  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _out failed\n"), -1);
    }
  os->gen_endif ();

  node->cli_inline_gen (I_TRUE);

  // Set the flag in the corresponding interface definition as well.
  AST_Interface *fd = node->full_definition ();
  be_interface *bfd = be_interface::narrow_from_decl (fd);
  bfd->cli_inline_gen (I_TRUE);

  return 0;
 }
