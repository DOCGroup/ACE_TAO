//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interface_Fwd node in the client header.
//
// = AUTHOR
//    Torsten Kuepper
//    based on code from Aniruddha Gokhale (interface_fwd_ch.cpp)
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype_fwd.h"

ACE_RCSID(be_visitor_valuetype_fwd, valuetype_fwd_ch, "$Id$")


// ********************************************************************
// Visitor implementation for the Interface_Fwd type
// This one for the client header file
// ********************************************************************

be_visitor_valuetype_fwd_ch::be_visitor_valuetype_fwd_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_fwd_ch::"
                             "must be implemented\n"), -1);
}

be_visitor_valuetype_fwd_ch::~be_visitor_valuetype_fwd_ch (void)
{
}

// Visit the valuetype_fwd node and its scope.
int
be_visitor_valuetype_fwd_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_hdr_gen () && !node->imported ())
    {
      // All we do in this is generate a forward declaration of the class.
      *os << "class " << node->local_name () << ";" << be_nl;

      // Generate the ifdefined macro for the _ptr type.
      os->gen_ifdef_macro (node->flat_name (), "_ptr");

      // Generate the _ptr declaration.
      *os << "typedef " << node->local_name () << " *" << node->local_name ()
          << "_ptr;" << be_nl;

      os->gen_endif ();

      // Enclose under an ifdef macro.
      os->gen_ifdef_macro (node->flat_name (), "_var");

      // generate the _var declaration.
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_fwd_ch::"
                             "visit_valuetype_fwd - "
                             "codegen for _var failed\n"), 
                            s-1);
        }

      // Generate an endif.
      os->gen_endif ();

      // enclose under an ifdef macro.
      os->gen_ifdef_macro (node->flat_name (), "_out");

      // Generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec.
      if (node->gen_out_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_fwd_ch::"
                             "visit_valuetype_fwd - "
                             "codegen for _out failed\n"), 
                            -1);
        }

      // generate the endif macro.
      os->gen_endif ();
      node->cli_hdr_gen (I_TRUE);
    }

  return 0;
}
