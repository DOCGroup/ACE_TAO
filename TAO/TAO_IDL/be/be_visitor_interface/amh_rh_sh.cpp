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


be_visitor_amh_rh_interface_sh::be_visitor_amh_rh_interface_sh 
(be_visitor_context *ctx): be_visitor_interface_sh (ctx)
{
}

be_visitor_amh_rh_interface_sh::~be_visitor_amh_rh_interface_sh (void)
{
}

int
be_visitor_amh_rh_interface_sh::visit_interface (be_interface *node)
{
  if (node->srv_hdr_gen () || node->imported () || node->is_local ())
    return 0;
  
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
  inherit_client_parent += "::";
  inherit_client_parent +=  rh_base_class_name;
  
  ACE_CString inherit_tao_parent = "TAO_AMH_";
  inherit_tao_parent += node->original_interface ()->local_name ();
  
  // Now generate the class definition
  *os << "class " << be_global->skel_export_macro ()
      << " " << rh_base_class_name.c_str () << be_idt_nl << ": " 
      << "public " << inherit_client_parent.c_str () <<"," << be_idt_nl
      << "public " << inherit_tao_parent.c_str () << be_nl;
  
  long n_parents = node->n_inherits ();
  
  if (n_parents > 0)
    {
      for (int i = 0; i < n_parents; ++i)
        {
          *os << "public virtual "
              << node->inherits ()[i]->name ();
          
          if (i < n_parents - 1)
            {
              *os << "," << be_nl;
            }
        }
    }
  
  *os << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "protected:" << be_idt_nl
      << rh_skel_class_name.c_str () << " (void);\n" << be_uidt_nl
      << "public:" << be_idt_nl;
  
  // No copy constructor for locality constraint interface.
  *os << rh_skel_class_name.c_str () << " (const " 
      << rh_skel_class_name.c_str () << "& rhs);" << be_nl
      << "virtual ~" << rh_skel_class_name.c_str () << " (void);\n\n"
      << be_nl;
    
  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }
  
  // Generate skeletons for operations of our base classes. These
  // skeletons just cast the pointer to the appropriate type
  // before invoking the call.
  if (node->traverse_inheritance_graph (be_interface::gen_skel_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }
  
  *os << "\n";
  return 0;
}
