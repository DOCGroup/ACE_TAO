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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_amh_interface, interface, "$Id$")

be_visitor_amh_interface_ch::be_visitor_amh_interface_ch (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_amh_interface_ch::~be_visitor_amh_interface_ch (void)
{
}

int
be_visitor_amh_interface_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // If not already generated and not imported.
  if (!node->cli_hdr_gen () && !node->imported ())
    {
      // == STEP 1:  generate the class name and class names we inherit ==

      // Generate the ifdefined macro for  the _ptr type.
      os->gen_ifdef_macro (node->flat_name (),
                           "_ptr");

      // The following two are required to be under the ifdef macro to avoid
      // multiple declarations.

      // Forward declaration.
      *os << "class " << node->local_name () << ";" << be_nl;
      // Generate the _ptr declaration.
      *os << "typedef " << node->local_name () << " *"
          << node->local_name () << "_ptr;" << be_nl;

      os->gen_endif ();

      // Generate the ifdefined macro for the var type.
      os->gen_ifdef_macro (node->flat_name (), "_var");

      // Generate the _var declaration.
      if (node->gen_var_defn () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for _var failed\n"),
                            -1);
        }

      os->gen_endif ();

    }

  // The above code could have been executed by the forward declaration
  // as long as it wasn't imported. The code below can only be
  // executed by an interface definition, also non-imported.
  if (node->imported ())
    {
      return 0;
    }

  // Now the interface definition itself.
  os->gen_ifdef_macro (node->flat_name ());

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
      *os << "public virtual CORBA_Object" << be_uidt_nl;
    }

  // Generate the body.

  *os << "{" << be_nl
      << "public:" << be_nl

      // Generate the _ptr_type and _var_type typedefs
      // but we must protect against certain versions of g++.
      << "#if !defined(__GNUC__) || !defined (ACE_HAS_GNUG_PRE_2_8)"
      << be_idt_nl
      << "typedef " << node->local_name () << "_ptr _ptr_type;"
      << be_nl
      << "typedef " << node->local_name () << "_var _var_type;"
      << be_uidt_nl
      << "#endif /* ! __GNUC__ || g++ >= 2.8 */\n" << be_idt_nl;

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

  node->cli_hdr_gen (I_TRUE);

  return 0;
}
