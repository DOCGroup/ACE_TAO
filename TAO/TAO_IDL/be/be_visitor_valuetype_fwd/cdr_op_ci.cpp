
//=============================================================================
/**
 *  @file    cdr_op_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for forward
 *  declarations of valuetypes. This uses compiled marshaling.
 *
 *
 *  @author Torsten Kuepper based on code from Aniruddha Gokhale & Carlos O'Ryan (cdr_op_ci.cpp)
 */
//=============================================================================


// ****************************************************************
// Interface visitor for generating CDR operator declarations in the
// client header
// ****************************************************************

be_visitor_valuetype_fwd_cdr_op_ci::be_visitor_valuetype_fwd_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_valuetype_fwd_cdr_op_ci::~be_visitor_valuetype_fwd_cdr_op_ci (
    void
  )
{
}

int
be_visitor_valuetype_fwd_cdr_op_ci::visit_valuetype_fwd (
    be_valuetype_fwd *node
  )
{
  AST_Interface *fd = node->full_definition ();
  be_valuetype *bfd = be_valuetype::narrow_from_decl (fd);

  // Check if it's imported vt or CDR ops already generated.
  if (node->cli_inline_cdr_op_gen () || node->imported ())
    {
      return 0;
    }

  // Is we are defined leater then let the real VT to generate
  // all this stuff.
  if (bfd->is_defined ())
    {
      return 0;
    }

  // Generate helper functions declaration.
  if (bfd->gen_helper_inline () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ch::"
                         "visit_valuetype - "
                         "codegen for helper functions failed\n"), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the CDR << and >> operator declarations (prototypes)

  //@@ Boris: Can I move this to be_valuetype? (as with _var, _out, etc?)

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__
      << be_nl << be_nl;

  //This is just declaration so no ACE_INLINE
  *os << be_global->stub_export_macro ()
      << " ::CORBA::Boolean operator<< (TAO_OutputCDR &, const "
      << node->full_name () << " *);" << be_nl;

  *os << be_global->stub_export_macro ()
      << " ::CORBA::Boolean operator>> (TAO_InputCDR &, "
      << node->full_name () << " *&);";

  // Done with this.
  node->cli_inline_cdr_op_gen (true);
  return 0;
}
