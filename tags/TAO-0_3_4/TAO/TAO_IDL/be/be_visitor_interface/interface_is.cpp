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

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"


// ************************************************************
// Interface visitor for implementation skeletons
// ************************************************************

ACE_RCSID(be_visitor_interface, interface_is, "$Id$")

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
  TAO_OutStream *os; // output stream

  if (node->impl_skel_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  // generate the skeleton class name

  os->indent (); // start with whatever indentation level we are at


  // constructor
  *os << "// Implementation skeleton constructor" << be_nl;
  // find if we are at the top scope or inside some module
  *os << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () <<"::" << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () <<
        " (void)" << be_idt_nl;
  

  if(node->n_inherits () >0)
    {
      //generate the default constructors of all the base classes      
      node->gen_def_ctors(os);
    }
  *os << "{" << be_nl
      << "}" << be_nl << be_uidt_nl;

  // destructor
  os->indent ();
  *os << "// Implementation skeleton destructor" << be_nl;
  
  *os << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () <<"::~" << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () <<
    " (void)" << be_idt_nl;
  
  *os << "{" <<be_nl;
  *os << "}" << be_nl <<be_uidt_nl;

  if (idl_global->gen_copy_ctor () )
    {
      *os << "//Implementation Skeleton Copy Constructor" << be_nl;
      
      *os <<idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () <<"::"
          << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () << " (const " 
          << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () << "& t)"<< be_idt_nl;
      if(node->n_inherits () >0)
        {
          node->gen_copy_ctors(os);
          *os << ", TAO_ServantBase (t)" << be_nl;
        }
      *os << "{" << be_nl
          << "}" << be_nl << be_uidt_nl;
    }

  if (idl_global->gen_assign_op ())
    {
      *os << "//Implementation Skeleton Copy Assignment" << be_nl;

      *os << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () << "& "
          << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () << "::operator=(const " 
          << idl_global->impl_class_prefix () << node->flatname () << idl_global->impl_class_suffix () << "& t)" <<be_idt_nl
          << "{" << be_idt_nl
          << "return *this;" << be_uidt_nl
          << "}" << be_nl << be_uidt_nl;
    }

  // generate code for elements in the scope (e.g., operations)
  
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


