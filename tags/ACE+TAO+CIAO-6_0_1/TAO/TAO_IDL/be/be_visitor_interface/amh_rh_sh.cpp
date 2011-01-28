//=============================================================================
/**
*  @file   amh_rh_sh.cpp
*
*  $Id$
*
*  Specialized interface visitor for AMH-RH that generates code
*  for RH interfaces in skeleton source files
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

be_visitor_amh_rh_interface_sh::be_visitor_amh_rh_interface_sh (
    be_visitor_context *ctx
  )
  : be_visitor_interface_sh (ctx)
{
}

be_visitor_amh_rh_interface_sh::~be_visitor_amh_rh_interface_sh (void)
{
}

int
be_visitor_amh_rh_interface_sh::visit_operation (be_operation *node)
{
  be_visitor_amh_rh_operation_sh amh_rh_op (this->ctx_);
  return amh_rh_op.visit_operation (node);
}

int
be_visitor_amh_rh_interface_sh::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the skeleton class name, use the AMH-node name as a
  // basis, this is AMH_<InterfaceName>ResponseHandler...
  ACE_CString rh_base_class_name = node->local_name ();
  // ...and prepend either the "TAO_" prefix...
  ACE_CString rh_skel_class_name =  "TAO_";

  if (!node->is_nested ())
    {
      // ...or the "POA_TAO_" prefix if we are in the global
      // namespace....
      rh_skel_class_name = "POA_TAO_";
    }

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  rh_skel_class_name += rh_base_class_name.c_str ();

  *os << "class " << rh_skel_class_name.c_str () << ";" << be_nl;

  // Generate the _ptr declaration.
  *os << "typedef " << rh_skel_class_name.c_str () << " *"
      << rh_skel_class_name.c_str()
      << "_ptr;" << be_nl_2;

  ACE_CString inherit_client_parent = node->client_enclosing_scope ();
  inherit_client_parent +=  rh_base_class_name;

  ACE_CString inherit_tao_parent = "TAO_AMH_Response_Handler";

  // Now generate the class definition
  *os << "class " << be_global->skel_export_macro ()
      << " " << rh_skel_class_name.c_str () << be_idt_nl
      << ": public " << inherit_tao_parent.c_str () << "," << be_idt_nl
      << "public ::" << inherit_client_parent.c_str () << be_uidt << be_uidt;

  *os << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << rh_skel_class_name.c_str () << " (void);" << be_nl
      << "virtual ~" << rh_skel_class_name.c_str () << " (void);";

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl
      << "};";

  return 0;
}
