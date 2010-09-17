
//=============================================================================
/**
 *  @file    serializer_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TAO::DCPS::Serializer operators for unions.
 *  This uses compiled marshaling.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


// ***************************************************************************
// Union visitor for generating Serializer operator declarations in the client header
// ***************************************************************************

be_visitor_union_serializer_op_ch::be_visitor_union_serializer_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_union (ctx)
{
}

be_visitor_union_serializer_op_ch::~be_visitor_union_serializer_op_ch (void)
{
}

int
be_visitor_union_serializer_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_serializer_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

    //- _dcps_max_marshaled_size(type) method
  *os << be_global->stub_export_macro ()
      << " size_t _dcps_max_marshaled_size (const "
      << node->name() << "& _tao_union);" << be_nl;
  //- _tao_is_bounded_size(type) method
  *os << be_global->stub_export_macro ()
      << " ::CORBA::Boolean _tao_is_bounded_size (const "
      << node->name() << "& _tao_union);" << be_nl;
  //- _dcps_find_size(type) method
  *os << be_global->stub_export_macro ()
      << " size_t _dcps_find_size (const "
      << node->name() << "& _tao_union);" << be_nl;


  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (TAO::DCPS::Serializer &, const " << node->name ()
      << " &);" << be_nl;
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (TAO::DCPS::Serializer &, "
      << node->name () << " &);";


  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_serializer_op_ch::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_hdr_serializer_op_gen (1);
  return 0;
}
