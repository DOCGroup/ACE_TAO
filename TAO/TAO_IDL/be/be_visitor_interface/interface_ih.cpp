// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ih.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the implementation header
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_ih, "$Id$")

// ************************************************************
// Interface visitor for implementation header
// ************************************************************

be_visitor_interface_ih::be_visitor_interface_ih (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ih::~be_visitor_interface_ih (void)
{
}

int
be_visitor_interface_ih::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  static char namebuf [NAMEBUFSIZE]; // holds the class name


  if (node->impl_hdr_gen () || node->imported ())
    return 0;

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  os = this->ctx_->stream ();

  // generate the skeleton class name

  os->indent (); // start with whatever indentation level we are at

  ACE_OS::sprintf (namebuf, "%s", node->flat_name ());

  *os << "//Class " << be_global->impl_class_prefix ()
      << namebuf << be_global->impl_class_suffix () << be_nl;
  // now generate the class definition
  *os << "class " << be_global->stub_export_macro ()
      << " " << be_global->impl_class_prefix () << namebuf
      << be_global->impl_class_suffix () << " : ";
  //inherit from the base skeleton file
  *os <<"public virtual "<< node->full_skel_name ();

  *os << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "//Constructor " << be_nl
      <<  be_global->impl_class_prefix () << namebuf
      << be_global->impl_class_suffix () << " (void);" << be_nl << be_nl;

  if (be_global->gen_copy_ctor ())
    {
      *os << "//Copy Constructor"<<be_nl
          << be_global->impl_class_prefix () << namebuf
          << be_global->impl_class_suffix () << " (const "
          << be_global->impl_class_prefix () << namebuf
          << be_global->impl_class_suffix () << "&);" <<be_nl <<be_nl;
    }

  if (be_global->gen_assign_op ())
    {
      *os << "//Copy Assignment" << be_nl
          << be_global->impl_class_prefix () << namebuf
          << be_global->impl_class_suffix () << "& " << "operator=(const "
          << be_global->impl_class_prefix () << namebuf
          << be_global->impl_class_suffix () << "&);" << be_nl << be_nl;

    }

  *os << "//Destructor " << be_nl
      << "virtual " << "~" << be_global->impl_class_prefix () << namebuf
      << be_global->impl_class_suffix () << " (void);" << be_nl << be_uidt_nl;


  // generate code for elements in the scope (e.g., operations)
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ih::"
                        "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  /*
 if (node->n_inherits () > 0)
    {
      // this interface inherits from other interfaces
      be_interface *intf; // inherited interface

      for (i = 0; i < node->n_inherits (); i++)
	{
	  intf = be_interface::narrow_from_decl (node->inherits ()[i]);
	  // generate code for elements in the scope (e.g., operations)
	  if (this->visit_scope (intf) ==  -1)
	    {
	      ACE_ERROR_RETURN ((LM_ERROR,
				 "be_visitor_interface_ih::"
				 "visit_interface - "
			     "codegen for scope failed\n"),
				-1);
	    }
	}

    }
  */

  //Generate the code for the members of the derived classes
  if (node->traverse_inheritance_graph (be_visitor_interface_ih::method_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_tie_sh_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }


  *os << "};" << be_nl <<be_nl;
  return 0;
}


//Helper method to generate members within the scope of the base classes
int
be_visitor_interface_ih::method_helper (be_interface *derived,
					    be_interface *node,
					    TAO_OutStream *os)
{

  if (strcmp (derived->flat_name (), node->flat_name ()) != 0)
    {
      be_visitor_context ctx;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_IH);
      ctx.interface (derived);
      ctx.stream (os);

      be_visitor* visitor = tao_cg->make_visitor (&ctx);
      if (visitor == 0 || visitor->visit_scope (node) == -1)
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_is::"
                             "method_helper\n"), -1);
        }
      delete visitor;
    }
  return 0;
}
