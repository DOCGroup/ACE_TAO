//=============================================================================
/**
*  @file   amh_rh_ss.cpp
*
*  $Id$
*
*  Specialized interface visitor for AMH-ResponseHandler that generates code for
*  RH in the skeleton source files.
*
*  @author Mayur Deshpande <mayur@ics.uci.edu>
*/
//=============================================================================

be_visitor_amh_rh_interface_ss::be_visitor_amh_rh_interface_ss (
    be_visitor_context *ctx
  )
  : be_visitor_interface_ss (ctx)
{
}

be_visitor_amh_rh_interface_ss::~be_visitor_amh_rh_interface_ss (void)
{
}

int
be_visitor_amh_rh_interface_ss::visit_operation (be_operation *node)
{
  be_visitor_amh_rh_operation_ss amh_rh_op (this->ctx_);
  return amh_rh_op.visit_operation (node);
}

int
be_visitor_amh_rh_interface_ss::visit_interface (be_interface *node)
{
  if (node->srv_skel_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // @@ Mayur, the exact same code is duplicated in amh_rh_sh.cpp,
  //    need to fix that...
  // Generate the skeleton class name, use the AMH-node name as a
  // basis, this is AMH_<InterfaceName>ResponseHandler...
  ACE_CString rh_skel_prefix =  "TAO_";

  char *buf;
  node->compute_full_name (rh_skel_prefix.c_str(), "", buf);
  ACE_CString rh_skel_full_scope_name ("POA_");
  rh_skel_full_scope_name += buf;
  // buf was allocated by ACE_OS::strdup, so we need to use free instead
  // of delete.
  ACE_OS::free (buf);

  ACE_CString rh_skel_class_name_prefix (rh_skel_prefix.c_str ());

  if (!node->is_nested ())
    {
      // ...or the "POA_TAO_" prefix if we are in the global
      // namespace....
      rh_skel_class_name_prefix = "POA_TAO_";
    }

  ACE_CString rh_skel_class_name (rh_skel_class_name_prefix);
  rh_skel_class_name += node->local_name ();

  *os << be_nl_2 << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << rh_skel_full_scope_name.c_str ()
      << "::" << rh_skel_class_name.c_str () << " (void)" << be_nl
      << ": TAO_AMH_Response_Handler ()," << be_idt_nl
      << "::" << node->full_name () << " ()" << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}" << be_nl_2;

  *os << rh_skel_full_scope_name.c_str ()
      << "::~" << rh_skel_class_name.c_str ()
      << " (void)" << be_nl;
  *os << "{" << be_nl;
  *os << "}";


  // Generate code for elements in the scope (e.g., operations)
  // We'll rely on the base class (be_visitor_scope) to do the
  // right thing for us.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}
