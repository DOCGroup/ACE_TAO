//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    tie_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for TIE class for an Interface in the header
//    file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, tie_sh, "$Id$")


// ************************************************************
// Interface visitor for server header
// ************************************************************

be_visitor_interface_tie_sh::be_visitor_interface_tie_sh (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_tie_sh::~be_visitor_interface_tie_sh (void)
{
}

int
be_visitor_interface_tie_sh::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  static char namebuf [NAMEBUFSIZE]; // holds the class name
  static char tiename [NAMEBUFSIZE]; // holds the tie name

  if (node->srv_hdr_gen () || node->imported ())
    return 0;

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);
  ACE_OS::memset (tiename, '\0', NAMEBUFSIZE);

  os = this->ctx_->stream ();

  // generate the skeleton class name which will be used to determine the TIE
  // class name

  // we shall have a POA_ prefix only if we are at the topmost level
  if (!node->is_nested ())
    {
      // we are outermost
      ACE_OS::sprintf (namebuf, "POA_%s", node->local_name ()->get_string ());
      ACE_OS::sprintf (tiename, "POA_%s_tie",
                       node->local_name ()->get_string ());
    }
  else
    {
      ACE_OS::sprintf (namebuf, "%s", node->local_name ()->get_string ());
      ACE_OS::sprintf (tiename, "%s_tie",
                       node->local_name ()->get_string ());
    }

  // now generate the class definition
  os->indent (); // start with whatever indentation level we are at

  // Since templates nested inside of classes are broken on most C++ compilers,
  // we generate code for this inside a conditional macro. The code is
  // activated only if "namespaces" are supported on the platform
  if (node->is_nested ())
    {
      *os << "\n#if defined (ACE_HAS_USING_KEYWORD)" << be_nl;
    }

  *os << "// TIE class: Refer to CORBA v2.2, Section 20.34.4" << be_nl;
  *os << "template <class T>" << be_nl;
  *os << "class " << idl_global->export_macro ()
      << " " << tiename << " : public " << namebuf << be_nl;
  *os << "{" << be_nl
      << "public:" << be_idt_nl
      << tiename << " (T &t);" << be_nl
      << "// the T& ctor" << be_nl
      << tiename << " (T &t, PortableServer::POA_ptr poa);" << be_nl
      << "// ctor taking a POA" << be_nl
      << tiename << " (T *tp, CORBA::Boolean release=1);" << be_nl
      << "// ctor taking pointer and an ownership flag" << be_nl
      << tiename << " (T *tp, PortableServer::POA_ptr poa, "
      << "CORBA::Boolean release=1);" << be_nl
      << "// ctor with T*, ownership flag and a POA" << be_nl
      << "~" << tiename << " (void);" << be_nl
      << "// dtor" << be_nl << be_nl
      << "// TIE specific functions" << be_nl
      << "T *_tied_object (void);" << be_nl
      << "// return the underlying object" << be_nl
      << "void _tied_object (T &obj);" << be_nl
      << "// set the underlying object" << be_nl
      << "void _tied_object (T *obj, CORBA::Boolean release=1);" << be_nl
      << "// set the underlying object and the ownership flag" << be_nl
      << "CORBA::Boolean _is_owner (void);" << be_nl
      << "// do we own it" << be_nl
      << "void _is_owner (CORBA::Boolean b);" << be_nl
      << "// set the ownership" << be_nl << be_nl
      << "// overridden ServantBase operations" << be_nl
      << "PortableServer::POA_ptr _default_POA (" << be_idt << be_idt_nl
      << "CORBA::Environment &env = " << be_idt_nl
      << "CORBA::default_environment ()"
      << be_uidt << be_uidt_nl
      << ");" << be_uidt << "\n";

  if (node->traverse_inheritance_graph (be_visitor_interface_tie_sh::method_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_tie_sh_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }

  *os << be_uidt << "private:" << be_idt_nl
      << "T *ptr_;" << be_nl
      << "PortableServer::POA_var poa_;" << be_nl
      << "CORBA::Boolean rel_;" << be_nl << be_nl
      << "// copy and assignment are not allowed" << be_nl
      << tiename << " (const " << tiename << " &);" << be_nl
      << "void operator= (const " << tiename << " &);" << be_uidt_nl
      << "};\n\n";

  if (node->is_nested ())
    {
      *os << "#endif /* ACE_HAS_USING_KEYWORD */\n";
    }

  return 0;
}

int
be_visitor_interface_tie_sh::method_helper (be_interface *,
					    be_interface *node,
					    TAO_OutStream *os)
{
  be_visitor_context ctx;
  ctx.state (TAO_CodeGen::TAO_INTERFACE_TIE_SH);
  ctx.stream (os);
  be_visitor* visitor = tao_cg->make_visitor (&ctx);
  if (visitor == 0 || visitor->visit_scope (node) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
			 "be_visitor_interface_tie_sh::"
			 "method_helper\n"), -1);
    }
  delete visitor;
  return 0;
}
