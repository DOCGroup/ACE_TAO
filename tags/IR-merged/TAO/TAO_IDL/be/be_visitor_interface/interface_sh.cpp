//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_sh, "$Id$")


// ************************************************************
// Interface visitor for server header
// ************************************************************

be_visitor_interface_sh::be_visitor_interface_sh (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_sh::~be_visitor_interface_sh (void)
{
}

int
be_visitor_interface_sh::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  long i; // loop index
  static char namebuf [NAMEBUFSIZE]; // holds the class name

  if (node->srv_hdr_gen () || node->imported ())
    return 0;

  ACE_OS::memset (namebuf, '\0', NAMEBUFSIZE);

  os = this->ctx_->stream ();

  // generate the skeleton class name

  os->indent (); // start with whatever indentation level we are at

  // we shall have a POA_ prefix only if we are at the topmost level
  if (!node->is_nested ())
    {
      // we are outermost
      ACE_OS::sprintf (namebuf, "POA_%s", node->local_name ()->get_string ());
    }
  else
    {
      ACE_OS::sprintf (namebuf, "%s", node->local_name ()->get_string ());
    }

  *os << "class " << namebuf << ";" << be_nl;

  // generate the _ptr declaration
  *os << "typedef " << namebuf << " *" << namebuf
      << "_ptr;" << be_nl;

  // now generate the class definition
  *os << "class " << idl_global->export_macro ()
      << " " << namebuf << " : ";
  if (node->n_inherits () > 0)
    {
      // this interface inherits from other interfaces
      be_interface *intf; // inherited interface

      *os << "public virtual ";
      intf = be_interface::narrow_from_decl (node->inherits ()[0]);
      *os << intf->relative_skel_name (node->full_skel_name ());
      for (i = 1; i < node->n_inherits (); i++)
        {
          *os << ", public virtual ";
          intf = be_interface::narrow_from_decl (node->inherits ()[i]);
          *os << intf->relative_skel_name (node->full_skel_name ());
        }  // end of for loop
    }
  else
    {
      // We don't inherit from another user defined object, hence our
      // base class is the ServantBase class.
      *os << " public virtual PortableServer::ServantBase";
    }
  *os << be_nl
      << "{" << be_nl
      << "protected:" << be_idt_nl
      << namebuf << " (void);\n" << be_uidt_nl
      << "public:" << be_idt_nl
      << namebuf << " (const " << namebuf << "& rhs);" << be_nl
      << "virtual ~" << namebuf << " (void);\n\n";

  *os << be_nl
      << "virtual CORBA::Boolean _is_a (" << be_idt << be_idt_nl
      << "const char* logical_type_id," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "CORBA::Environment::default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  *os << "virtual void* _downcast (" << be_idt << be_idt_nl
      << "const char* logical_type_id" << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // add a skeleton for our _is_a method
  *os << "static void _is_a_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &req," << be_nl
      << "void *obj," << be_nl
      << "void *context," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "CORBA::Environment::default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // add a skeleton for our _non_existent method
  os->indent ();
  *os << "static void _non_existent_skel (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &req," << be_nl
      << "void *obj," << be_nl
      << "void *context," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "CORBA::Environment::default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // add the dispatch method
  *os << "virtual void _dispatch (" << be_idt << be_idt_nl
      << "CORBA::ServerRequest &_tao_req," << be_nl
      << "void *_tao_context," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "CORBA::Environment::default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // Print out the _this() method.
  *os << node->name () << " *_this (" << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "CORBA::Environment::default_environment ()"
      << be_uidt << be_uidt_nl
      << ");\n" << be_uidt_nl;

  // the _interface_repository_id method
  *os << "virtual const char* _interface_repository_id "
      << "(void) const;" << be_uidt_nl;

  // generate code for elements in the scope (e.g., operations)
  if (this->visit_scope (node) ==  -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the call
  if (node->traverse_inheritance_graph (be_interface::gen_skel_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "inheritance graph traversal failed\n"),
                        -1);
    }

  *os << "};\n\n";

  // generate the collocated class
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_COLLOCATED_SH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for collocated class failed\n"),
                        -1);
    }

  // generate the TIE class
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_INTERFACE_TIE_SH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for TIE class failed\n"),
                        -1);
    }

  *os << "\n";
  ctx.stream (tao_cg->server_template_header ());

  return 0;
}
