//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for a valuebox in the client
//    header.
//
// = AUTHOR
//    Gary Maxey
//
// ============================================================================

ACE_RCSID (be_visitor_valuebox, 
           any_op_ch, 
           "$Id$")

// ***************************************************************************
// Valuebox visitor for generating Any operator declarations.
// ***************************************************************************

be_visitor_valuebox_any_op_ch::be_visitor_valuebox_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuebox (ctx)
{
}

be_visitor_valuebox_any_op_ch::~be_visitor_valuebox_any_op_ch (void)
{
}

int
be_visitor_valuebox_any_op_ch::visit_valuebox (be_valuebox *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << macro << " void operator<<= (CORBA::Any &, " << node->name ()
      << " *); // copying" << be_nl;

  *os << macro << " void operator<<= (CORBA::Any &, " << node->name ()
      << " **); // non-copying" << be_nl;

  *os << macro << " CORBA::Boolean operator>>= (const CORBA::Any &, "
      << node->name () << " *&);";

  node->cli_hdr_any_op_gen (1);
  return 0;
}
