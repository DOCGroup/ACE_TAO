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

#include "idl.h"
#include "idl_extern.h"
#include  "be.h"
#include "be_visitor_interface.h"

#include "be_visitor_operation/amh_rh_sh.h"

be_visitor_amh_rh_interface_sh::be_visitor_amh_rh_interface_sh
(be_visitor_context *ctx): be_visitor_interface_sh (ctx)
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

  os->indent ();

  // Generate the skeleton class name.
  // this is AMH_intf_ResponseHandler
  ACE_CString rh_base_class_name = node->local_name ();;
  // and this is TAO_AMH_intf_ResponseHandler
  ACE_CString rh_skel_class_name =  "TAO_";
  rh_skel_class_name += rh_base_class_name.c_str ();

  *os << "class " << rh_skel_class_name.c_str () << ";" << be_nl;

  // Generate the _ptr declaration.
  *os << "typedef " << rh_skel_class_name.c_str () << " *"
      << rh_skel_class_name.c_str()
      << "_ptr;" << be_nl;

  ACE_CString inherit_client_parent = node->client_enclosing_scope ();
  inherit_client_parent +=  rh_base_class_name;

  ACE_CString inherit_tao_parent = "TAO_AMH_Response_Handler";

  // Now generate the class definition
  *os << "class " << be_global->skel_export_macro ()
      << " " << rh_skel_class_name.c_str () << be_idt_nl
      << ": public " << inherit_client_parent.c_str () << be_nl
      << ", public " << inherit_tao_parent.c_str ();

  long n_parents = node->n_inherits ();
  for (int i = 0; i < n_parents; ++i)
    {
      *os << be_nl << ", public virtual "
          << node->inherits ()[i]->name ();
    }

  *os << be_uidt_nl << "{" << be_nl
      << "protected:" << be_idt_nl
      << rh_skel_class_name.c_str () << " (void);\n" << be_uidt_nl
      << "public:" << be_idt_nl;

  // No copy constructor for locality constraint interface.
  *os << rh_skel_class_name.c_str () << " (const "
      << rh_skel_class_name.c_str () << "& rhs);" << be_nl
      << "virtual ~" << rh_skel_class_name.c_str () << " (void);\n\n";

  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl << "};\n\n";
  return 0;
}

