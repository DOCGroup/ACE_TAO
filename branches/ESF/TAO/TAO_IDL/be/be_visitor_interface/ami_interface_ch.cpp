//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_interface_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for AMI Interfaces in the client header
//
// = AUTHOR
//    Aniruddha Gokhale,
//    Michael Kircher
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, ami_interface_ch, "$Id$")


// ******************************************************
// Interface visitor for client header
// ******************************************************

be_visitor_ami_interface_ch::be_visitor_ami_interface_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_ami_interface_ch::~be_visitor_ami_interface_ch (void)
{
}

int
be_visitor_ami_interface_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (!node->cli_hdr_gen () && !node->imported ()) // not already generated and
                                                   // not imported
    {
      // Grab the stream.
      os = this->ctx_->stream ();


      os->gen_ifdef_macro (node->replacement ()->flat_name (), "_ptr");

      os->indent (); // start with whatever indentation level we are at
      // forward declaration
      *os << "class " << node->replacement ()->local_name () << ";" << be_nl;

      // generate the _ptr declaration
      *os << "typedef " << node->replacement ()->local_name () 
          << " *" << node->replacement ()->local_name ()
          << "_ptr;" << be_nl << be_nl;


/*      // forward declaration
      *os << "class AMI_" << node->local_name () << "Handler;" << be_nl;

      // generate the _ptr declaration
      *os << "typedef AMI_" << node->local_name () 
          << "Handler *AMI_" << node->local_name ()
          << "Handler_ptr;" << be_nl << be_nl;
*/
      os->gen_endif ();
    }

  return 0;
}
