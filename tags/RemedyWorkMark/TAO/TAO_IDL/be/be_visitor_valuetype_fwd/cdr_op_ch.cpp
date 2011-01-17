
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for forward
 *  declarations of valuetypes. This uses compiled marshaling.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru> base on code from Torsten Kuepper based on code from  Aniruddha Gokhale & Carlos O'Ryan (cdr_op_ci.cpp)
 */
//=============================================================================


be_visitor_valuetype_fwd_cdr_op_ch::be_visitor_valuetype_fwd_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_valuetype_fwd_cdr_op_ch::~be_visitor_valuetype_fwd_cdr_op_ch (void)
{
}

int
be_visitor_valuetype_fwd_cdr_op_ch::visit_valuetype_fwd (
    be_valuetype_fwd *node
  )
{
  AST_Interface *fd = node->full_definition ();
  be_valuetype *bfd = be_valuetype::narrow_from_decl (fd);

  // If this forward declared vt is defined later in the file,
  // the CDR operator declaration (along with the corresponding
  // declarations for members of the interface's scope) will be
  // generated then.
  if (fd->is_defined ())
    {
      return 0;
    }

  // Check if it's imported vt or CDR ops already generated.
  if (node->cli_hdr_cdr_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_valuetype *fvd =
    be_valuetype::narrow_from_decl (node->full_definition ());

  if (fvd->gen_helper_header () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                       "(%N:%l) be_visitor_valuetype_fwd_cdr_op_ch::"
                         "visit_valuetype_fwd - "
                         "codegen for helper functions failed\n"),
                        -1);
    }

  // generate the CDR << and >> operator declarations (prototypes)

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  *os << be_global->stub_export_macro ()
      << " ::CORBA::Boolean operator<< (TAO_OutputCDR &, const "
      << node->full_name () << " *);" << be_nl;

  *os << be_global->stub_export_macro ()
      << " ::CORBA::Boolean operator>> (TAO_InputCDR &, "
      << node->full_name () << " *&);" << be_nl;

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_hdr_cdr_op_gen (true);
  bfd->cli_hdr_cdr_op_gen (true);

  return 0;
}

int
be_visitor_valuetype_fwd_cdr_op_ch::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}
