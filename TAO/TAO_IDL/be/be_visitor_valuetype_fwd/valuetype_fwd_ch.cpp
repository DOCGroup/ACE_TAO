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
//    Boris Kolpackov <bosk@ipmce.ru>
//    based on code from  Torsten Kuepper
//    based on code from Aniruddha Gokhale (interface_fwd_ch.cpp)
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype_fwd.h"

ACE_RCSID(be_visitor_valuetype_fwd, valuetype_fwd_ch, "$Id$")


be_visitor_valuetype_fwd_ch::be_visitor_valuetype_fwd_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_valuetype_fwd_ch::~be_visitor_valuetype_fwd_ch (void)
{
}

// Visit the valuetype_fwd node and its scope.
int
be_visitor_valuetype_fwd_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  AST_Interface *fd = node->full_definition ();
  be_valuetype *bfd = be_valuetype::narrow_from_decl (fd);

  if (!fd->is_defined ())
    {
      // To generate extern declarations after all modules are closed.
      be_global->non_defined_interfaces.enqueue_tail (node);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "// Valuetype class" << be_nl;

  // == STEP 1: Generate the class name and class names we inherit ==

  // Forward declaration.
  *os << "class " << node->local_name () << ";" << be_nl;

  // Generate _ptr declaration
  os->gen_ifdef_macro (node->flat_name (), "_ptr");  

  *os << "typedef " << node->local_name ()
      << " *" << node->local_name () << "_ptr;" << be_nl;

  os->gen_endif ();

  // Generate the ifdefined macro for the _var type.
  os->gen_ifdef_macro (node->flat_name (), "_var");

  // Generate the _var declaration.
  if (bfd->gen_var_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_valuetype - "
                         "codegen for _var failed\n"), 
                        -1);
    }

  os->gen_endif ();

  // Generate the ifdef macro for the _out class.
  os->gen_ifdef_macro (node->flat_name (), "_out");

  // Generate the _out declaration
  if (bfd->gen_out_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ch::"
                         "visit_valuetype - "
                         "codegen for _out failed\n"), -1);
    }

  // generate the endif macro.
  os->gen_endif ();

  node->cli_hdr_gen (I_TRUE);
  bfd->cli_hdr_gen (I_TRUE);

  return 0;
}
