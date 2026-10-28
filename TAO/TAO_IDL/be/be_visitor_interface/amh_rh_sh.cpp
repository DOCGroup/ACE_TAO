//=============================================================================
/**
*  @file   amh_rh_sh.cpp
*
*  Specialized interface visitor for AMH-RH that generates code
*  for RH interfaces in skeleton source files
*
*  @author Darrell Brunsch <brunsch@cs.wustl.edu>
*/
//=============================================================================

#include "interface.h"

#include <string>
#include <string_view>

be_visitor_amh_rh_interface_sh::be_visitor_amh_rh_interface_sh (
    be_visitor_context *ctx)
  : be_visitor_interface_sh (ctx)
{
}

be_visitor_amh_rh_interface_sh::~be_visitor_amh_rh_interface_sh () = default;

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
  std::string rh_base_class_name (node->local_name ());
  // ...and prepend either the "TAO_" prefix...
  std::string rh_skel_class_name = "TAO_";

  if (!node->is_nested ())
    {
      // ...or the "POA_TAO_" prefix if we are in the global
      // namespace....
      rh_skel_class_name = "POA_TAO_";
    }

  TAO_INSERT_COMMENT (os);

  rh_skel_class_name += rh_base_class_name;

  *os << "class " << rh_skel_class_name << ";" << be_nl;

  // Generate the _ptr declaration.
  *os << "using " << rh_skel_class_name << "_ptr = "
      << rh_skel_class_name << " *;" << be_nl_2;

  std::string inherit_client_parent (node->client_enclosing_scope ());
  inherit_client_parent += rh_base_class_name;

  std::string_view const inherit_tao_parent = "TAO_AMH_Response_Handler";

  // Now generate the class definition
  *os << "class " << be_global->skel_export_macro ()
      << " " << rh_skel_class_name << be_idt_nl
      << ": public " << inherit_tao_parent << "," << be_idt_nl
      << "public ::" << inherit_client_parent << be_uidt << be_uidt;

  *os << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << rh_skel_class_name << " () = default;" << be_nl
      << "virtual ~" << rh_skel_class_name << " () = default;";

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
