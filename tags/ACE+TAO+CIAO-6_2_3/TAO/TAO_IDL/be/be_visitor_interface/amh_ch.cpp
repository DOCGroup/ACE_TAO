//=============================================================================
/**
*  @file   amh_ch.cpp
*
*  $Id$
*
*  Specialized interface visitor for AMH-RH generates code that is
*  specific to AMH interfaces.
*
*  @author Mayur Deshpande <mayur@ics.uci.edu>
*/
//=============================================================================

#include "interface.h"

be_visitor_amh_interface_ch::be_visitor_amh_interface_ch (
    be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_amh_interface_ch::~be_visitor_amh_interface_ch (void)
{
}

int
be_visitor_amh_interface_ch::visit_interface (be_interface *node)
{
  // If not already generated and not imported.
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // This will be a no-op if it has already been done by a forward
  // declaration.
  node->gen_var_out_seq_decls ();

  TAO_OutStream *os = this->ctx_->stream ();

  // Now generate the class definition.
  *os << "class " << be_global->stub_export_macro ()
      << " " << node->local_name () << be_idt_nl
      << ": " ;

  // If node interface inherits from other interfaces.
  if (node->n_inherits () > 0)
    {
      *os << be_idt;

      for (int i = 0; i < node->n_inherits (); i++)
        {
          *os << "public virtual "
              << node->inherits ()[i]->name ();

          if (i < node->n_inherits () - 1)
            {
              // Node has multiple inheritance, so put a comma.
              *os << "," << be_nl;
            }
        }

      *os << be_uidt << be_uidt_nl;
    }
  else
    {
      // We do not inherit from anybody, hence we do so from the base
      // CORBA::Object class.
      *os << "public virtual ::CORBA::Object" << be_uidt_nl;
    }

  // Generate the body.

  *os << "{" << be_nl
      << "public:" << be_idt_nl

      // Generate the _ptr_type and _var_type typedefs.
      << "typedef " << node->local_name () << "_ptr _ptr_type;"
      << be_nl
      << "typedef " << node->local_name () << "_var _var_type;"
      << be_nl
      << "typedef " << node->local_name () << "_out _out_type;"
      << be_nl;

  // Generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_hdr_gen (true);
  return 0;
}
