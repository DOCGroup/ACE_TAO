//
// $Id$
//

#include "idl.h"
#include "be.h"
#include "be_visitor.h"

be_visitor::~be_visitor (void)
{
}

int be_visitor::visit_decl (be_decl *)
{
  return 0;
}

int be_visitor::visit_scope (be_scope *node)
{
  if (node->nmembers () > 0)
    {
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
		      UTL_ScopeActiveIterator (node,
					       UTL_Scope::IK_decls),
		      -1);
      while (!si->is_done ())
	{
	  AST_Decl *d = si->item ();
	  be_decl *bd = be_decl::narrow_from_decl (d); 
	  if (bd == 0 || bd->accept (this) == -1)
	    {
	      delete si;
	      return -1;
	    }
	  si->next ();
	}
      delete si;
    }
  
  return 0;
}

int be_visitor::visit_type (be_type *)
{
  return 0;
}

int be_visitor::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int be_visitor::visit_module (be_module *)
{
  return 0;
}

int be_visitor::visit_interface (be_interface *)
{
  return 0;
}

int be_visitor::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

int be_visitor::visit_structure (be_structure *)
{
  return 0;
}

int be_visitor::visit_exception (be_exception *)
{
  return 0;
}

int be_visitor::visit_expression (be_expression *)
{
  return 0;
}

int be_visitor::visit_enum (be_enum *)
{
  return 0;
}

int be_visitor::visit_operation (be_operation *)
{
  return 0;
}

int be_visitor::visit_field (be_field *)
{
  return 0;
}

int be_visitor::visit_argument (be_argument *)
{
  return 0;
}

int be_visitor::visit_attribute (be_attribute *)
{
  return 0;
}

int be_visitor::visit_union (be_union *)
{
  return 0;
}

int be_visitor::visit_union_branch (be_union_branch *)
{
  return 0;
}

int be_visitor::visit_union_label (be_union_label *)
{
  return 0;
}

int be_visitor::visit_constant (be_constant *)
{
  return 0;
}

int be_visitor::visit_enum_val (be_enum_val *)
{
  return 0;
}

int be_visitor::visit_array (be_array *)
{
  return 0;
}

int be_visitor::visit_sequence (be_sequence *)
{
  return 0;
}

int be_visitor::visit_string (be_string *)
{
  return 0;
}

int be_visitor::visit_typedef (be_typedef *)
{
  return 0;
}

int be_visitor::visit_root (be_root *)
{
  return 0;
}

int be_visitor::visit_native (be_native *)
{
  return 0;
}
