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
  long i; // loop index
  static char namebuf [NAMEBUFSIZE]; // holds the class name


  if (node->impl_hdr_gen () || node->imported ())
    return 0;

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  os = this->ctx_->stream ();

  // generate the skeleton class name

  os->indent (); // start with whatever indentation level we are at

  
  ACE_OS::sprintf (namebuf, "%s", node->flatname ());
  
  *os << "//Class " << idl_global->impl_class_prefix ()<<namebuf << idl_global->impl_class_suffix ()<< be_nl;
  // now generate the class definition
  *os << "class " << idl_global->export_macro ()
      << " " <<idl_global->impl_class_prefix () << namebuf << idl_global->impl_class_suffix () << " : ";
  
  if (node->n_inherits () > 0)
    {
      // this interface inherits from other interfaces
      be_interface *intf; // inherited interface


      *os << "public virtual ";
     
      intf = be_interface::narrow_from_decl (node->inherits ()[0]);
      *os << idl_global->impl_class_prefix () << intf->flatname () << idl_global->impl_class_suffix ();//intf->relative_skel_name (node->full_skel_name ());
      for (i = 1; i < node->n_inherits (); i++)
        {
          *os << ", public virtual ";
          intf = be_interface::narrow_from_decl (node->inherits ()[i]);
          *os << idl_global->impl_class_prefix () <<intf->flatname () << idl_global->impl_class_suffix ();//intf->relative_skel_name (node->full_skel_name ());
        }  // end of for loop

      //inherit from the base skeleton file
      *os<<", public virtual "<<node->full_skel_name ();
    }
  
  else
    {
      //inherit from the base skeleton file
      *os<<"public virtual "<<node->full_skel_name ();
      }
  
    
  *os << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "//Constructor " << be_nl
      <<  idl_global->impl_class_prefix () << namebuf << idl_global->impl_class_suffix () << " (void);" << be_nl << be_nl;

  if (idl_global->gen_copy_ctor ())
    {
      *os << "//Copy Constructor"<<be_nl
          << idl_global->impl_class_prefix () << namebuf << idl_global->impl_class_suffix () << " (const " 
          << idl_global->impl_class_prefix () << namebuf << idl_global->impl_class_suffix () << "&);" <<be_nl <<be_nl;
    }

  if (idl_global->gen_assign_op ()) 
    {
      *os << "//Copy Assignment" << be_nl
          << idl_global->impl_class_prefix () << namebuf << idl_global->impl_class_suffix () << "& "
          << "operator=(const " << idl_global->impl_class_prefix () << namebuf << idl_global->impl_class_suffix () << "&);"<<be_nl << be_nl;

    }
  
  *os << "//Destructor " << be_nl
      << "virtual " << "~" << idl_global->impl_class_prefix () << namebuf << idl_global->impl_class_suffix () << " (void);" << be_nl << be_uidt_nl;

   
  // generate code for elements in the scope (e.g., operations)
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ih::"
                        "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }
  
  
  *os << "};" << be_nl <<be_nl;
  return 0;
}



