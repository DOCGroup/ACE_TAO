//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating fwd declaration code for the AMI Handler
//    interface.
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu> 
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_ami_handler_fwd, ami_handler_fwd_ch, "$Id$")


// ********************************************************************
// Visitor implementation generating fwd declaration code for the AMI
// Handler interface.
// ********************************************************************
  
be_visitor_interface_ami_handler_fwd_ch::be_visitor_interface_ami_handler_fwd_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_ami_handler_fwd_ch::~be_visitor_interface_ami_handler_fwd_ch (void)
{
}

// visit the interface node and its scope
int
be_visitor_interface_ami_handler_fwd_ch::visit_interface (be_interface *node)
{
  // Grab the out stream.
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate code, if the client header is not generated already.
  if (!node->cli_hdr_gen () && !node->imported ())
    {
      // start from the current indentation.
      os->indent ();

      // Generate forward declaration class.
      *os << "class " << node->local_name ()  << ";"
          << be_nl;
      
      // Generate the ifdefined macro for the _ptr type.
      os->gen_ifdef_macro (node->flat_name (), "_ptr");

      // Generate the _ptr declaration
      *os << "typedef " << node->local_name () 
          << " *" <<  node->local_name () << "_ptr;"
          << be_nl;

      // Generate the endif.
      os->gen_endif ();
      
      // Generate the var class.
      
      // Enclose under an ifdef macro
      os->gen_ifdef_macro (node->local_name (), "_var");
      
      // Generate the _var declaration.
      if (node->gen_var_defn ((char *) node->local_name ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ami_handler_fwd_ch::"
                             "visit_interface_fwd - "
                             "codegen for _var failed\n"), -1);
        }
      
      // Gen an endif.
      os->gen_endif ();

      // Generate the our class.

      // Enclose under an ifdef macro.
      os->gen_ifdef_macro (node->local_name (), "_out");

      // Generate the _out declaration - ORBOS/97-05-15 pg 16-20
      // spec. 
      if (node->gen_out_defn ( (char *) node->local_name ()) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ami_handler_fwd_ch::"
                             "visit_interface_fwd - "
                             "codegen for _out failed\n"), -1);
        }

      // Generate the endif macro.
      os->gen_endif ();
    }
  return 0;
}
