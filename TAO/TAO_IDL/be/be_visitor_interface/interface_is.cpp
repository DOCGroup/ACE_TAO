// Id:$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_is.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the implementation skeletons file.
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_interface.h"


// ************************************************************
// Interface visitor for implementation skeletons.
// ************************************************************

ACE_RCSID (be_visitor_interface, 
           interface_is, 
           "$Id$")

be_visitor_interface_is::be_visitor_interface_is (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_is::~be_visitor_interface_is (void)
{
}

int
be_visitor_interface_is::visit_interface (be_interface *node)
{

  this->ctx_->interface (node);

  TAO_OutStream *os = this->ctx_->stream ();

  if (node->impl_skel_gen () || node->imported ())
    {
      return 0;
    }

  // Generate the skeleton class name.

  os->indent ();

  *os << "// Implementation skeleton constructor" << be_nl;

  // Find if we are at the top scope or inside some module.
  *os << be_global->impl_class_prefix () << node->flat_name ()
      << be_global->impl_class_suffix () <<"::"
      << be_global->impl_class_prefix () << node->flat_name ()
      << be_global->impl_class_suffix ()
      << " (void)" << be_idt_nl;

  *os << "{" << be_nl
      << "}" << be_nl << be_uidt_nl;

  os->indent ();
  *os << "// Implementation skeleton destructor" << be_nl;

  *os << be_global->impl_class_prefix () << node->flat_name ()
      << be_global->impl_class_suffix () <<"::~"
      << be_global->impl_class_prefix () << node->flat_name ()
      << be_global->impl_class_suffix ()
      << " (void)" << be_idt_nl;

  *os << "{" <<be_nl;
  *os << "}" << be_nl <<be_uidt_nl;

  if (be_global->gen_copy_ctor ())
    {
      *os << "//Implementation Skeleton Copy Constructor" << be_nl;

      *os << be_global->impl_class_prefix () << node->flat_name ()
          << be_global->impl_class_suffix () <<"::"
          << be_global->impl_class_prefix () << node->flat_name ()
          << be_global->impl_class_suffix () << " (const "
          << be_global->impl_class_prefix () << node->flat_name ()
          << be_global->impl_class_suffix () << "& t)"<< be_idt_nl;

      if (node->n_inherits () > 0)
        {
          node->gen_copy_ctors (os);
          *os << ", TAO_ServantBase (t)" << be_nl;
        }

      *os << "{" << be_nl
          << "}" << be_nl << be_uidt_nl;
    }

  if (be_global->gen_assign_op ())
    {
      *os << "//Implementation Skeleton Copy Assignment" << be_nl;

      *os << be_global->impl_class_prefix () << node->flat_name ()
          << be_global->impl_class_suffix () << "& "
          << be_global->impl_class_prefix () << node->flat_name ()
          << be_global->impl_class_suffix () << "::operator=(const "
          << be_global->impl_class_prefix () << node->flat_name ()
          << be_global->impl_class_suffix () << "& t)" <<be_idt_nl
          << "{" << be_idt_nl
          << "return *this;" << be_uidt_nl
          << "}" << be_nl << be_uidt_nl;
    }

  // Generate code for elements in the scope (e.g., operations).

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ss::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  int status =
    node->traverse_inheritance_graph (
              be_visitor_interface_is::method_helper, 
              os
            );

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_tie_sh_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }

  return 0;
}


// Helper method to generate the members in the scope of the base classes.
int
be_visitor_interface_is::method_helper (be_interface *derived,
					                              be_interface *node,
					                              TAO_OutStream *os)
{

  if (ACE_OS::strcmp (derived->flat_name (), node->flat_name ()) != 0)
    {
      be_visitor_context ctx;
      ctx.state (TAO_CodeGen::TAO_INTERFACE_IS);
      ctx.interface (derived);
      ctx.stream (os);
      be_visitor_interface_is visitor (&ctx);

      if (visitor.visit_scope (node) == -1)

        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_is::"
                             "method_helper\n"), 
                            -1);
        }

    }

  return 0;
}
