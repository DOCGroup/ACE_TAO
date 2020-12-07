
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  Visitor generating code for Any operators for the Sequence
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "sequence.h"

be_visitor_sequence_any_op_ch::be_visitor_sequence_any_op_ch (
                                                              be_visitor_context *ctx
                                                              )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_any_op_ch::~be_visitor_sequence_any_op_ch ()
{
}

int
be_visitor_sequence_any_op_ch::visit_sequence (be_sequence *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ()
      || (node->is_local ()
          && !be_global->gen_local_iface_anyops ()))
    {
      return 0;
    }

  if (idl_global->dcps_sequence_type_defined (node->full_name ()))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl_2;

  TAO_INSERT_COMMENT (os);

  *os << be_nl_2;

  ACE_CString name;

  bool alt = be_global->alt_mapping ();

  if (alt)
    {
      be_type *bt =
        dynamic_cast<be_type*> (node->base_type ());

      name = "std::vector< ::";
      name += bt->full_name ();
      name += ">";
    }
  else
    {
      name = node->full_name ();
    }

  *os << be_global->anyops_versioning_begin () << be_nl;

  // Generate the Any <<= and >>= operators.
  *os << macro
      << " void"
      << " operator<<= (::CORBA::Any &, const ::"
      << name.c_str ()
      << " &); // copying version" << be_nl;

  if (!alt)
    {
      *os << macro
          << " void"
          << " operator<<= (::CORBA::Any &, ::"
          << name.c_str ()
          << "*); // noncopying version" << be_nl;
    }

  *os << macro
      << " ::CORBA::Boolean"
      << " operator>>= (const ::CORBA::Any &, const ::"
      << name.c_str ()
      << " *&);";

  *os << be_global->anyops_versioning_end () << be_nl;

  node->cli_hdr_any_op_gen (true);
  return 0;
}
