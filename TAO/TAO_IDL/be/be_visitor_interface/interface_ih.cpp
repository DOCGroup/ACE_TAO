
//=============================================================================
/**
 *  @file    interface_ih.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the implementation header
 *
 *
 *  @author Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
 */
//=============================================================================

// ************************************************************
// Interface visitor for implementation header.
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
  if (node->impl_hdr_gen () || node->imported () || node->is_abstract ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  static char namebuf [NAMEBUFSIZE];


  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);

  // Generate the skeleton class name.

  ACE_OS::sprintf (namebuf, "%s", node->flat_name ());

  if (be_global->gen_impl_debug_info ())
    {
      *os << "// TAO_IDL - Generated from" << be_nl
          << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;
    }

  // Now generate the class definition.
  *os << "class " << be_global->stub_export_macro ()
      << " " << be_global->impl_class_prefix () << namebuf
      << be_global->impl_class_suffix () << be_idt_nl << ": public virtual ";

  // Inherit from the base skeleton name, unless the interface
  // is local.
  if (node->is_local ())
    {
      *os << node->full_name ();
    }
  else
    {
      *os << node->full_skel_name ();
    }

  if (node->is_local ())
    {
      *os << "," << be_idt_nl
          << "public virtual ::CORBA::LocalObject"
          << be_uidt;
    }

  *os << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << "// Constructor " << be_nl
      <<  be_global->impl_class_prefix () << namebuf
      << be_global->impl_class_suffix () << " (void);" << be_nl_2;

  if (be_global->gen_copy_ctor () && !node->is_local ())
    {
      *os << "// Copy Constructor"<<be_nl
          << be_global->impl_class_prefix () << namebuf
          << be_global->impl_class_suffix () << " (const "
          << be_global->impl_class_prefix () << namebuf
          << be_global->impl_class_suffix () << "&);" <<be_nl <<be_nl;
    }

  if (be_global->gen_assign_op ())
    {
      *os << "// Copy Assignment" << be_nl
          << be_global->impl_class_prefix () << namebuf
          << be_global->impl_class_suffix () << "& " << "operator=(const "
          << be_global->impl_class_prefix () << namebuf
          << be_global->impl_class_suffix () << "&);" << be_nl_2;

    }

  *os << "// Destructor " << be_nl
      << "virtual " << "~" << be_global->impl_class_prefix () << namebuf
      << be_global->impl_class_suffix () << " (void);";


  // Generate code for elements in the scope (e.g., operations).
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_ih::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }


  // Generate the code for the members of the derived classes.
  int status =
    node->traverse_inheritance_graph (
      be_visitor_interface_ih::method_helper,
      os);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_tie_sh_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }


  *os << be_uidt_nl
      << "};" << be_nl_2;

  return 0;
}


// Helper method to generate members within the scope of the base classes.
int
be_visitor_interface_ih::method_helper (be_interface *derived,
                                        be_interface *node,
                                        TAO_OutStream *os)
{

  if (ACE_OS::strcmp (derived->flat_name (), node->flat_name ()) != 0)
    {
      be_visitor_context ctx;
      ctx.state (TAO_CodeGen::TAO_ROOT_IH);
      ctx.interface (derived);
      ctx.stream (os);
      be_visitor_interface_ih visitor (&ctx);

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
