
//=============================================================================
/**
 *  @file    tie_sh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TIE class for an Interface in the header
 *  file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_tie_sh::be_visitor_interface_tie_sh (
    be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_tie_sh::~be_visitor_interface_tie_sh (void)
{
}

int
be_visitor_interface_tie_sh::visit_interface (be_interface *node)
{
  if (node->imported () || node->is_abstract ())
    {
      return 0;
    }

  static char namebuf [NAMEBUFSIZE];
  static char tiename [NAMEBUFSIZE];

  ACE_OS::memset (namebuf,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (tiename,
                  '\0',
                  NAMEBUFSIZE);

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the skeleton class name which will be used to determine the TIE
  // class name.

  // We shall have a POA_ prefix only if we are at the topmost level.
  if (node->is_nested ())
    {
      ACE_OS::sprintf (namebuf,
                       "%s",
                       node->local_name ());
      ACE_OS::sprintf (tiename,
                       "%s_tie",
                       node->local_name ());
    }
  else
    {
      // We are outermost.
      ACE_OS::sprintf (namebuf,
                       "POA_%s",
                       node->local_name ());
      ACE_OS::sprintf (tiename,
                       "POA_%s_tie",
                       node->local_name ());
    }

  // Now generate the class definition.
  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "// TIE class: Refer to CORBA v2.2, Section 20.34.4" << be_nl;
  *os << "template <class T>" << be_nl;
  *os << "class " << " " << tiename << " : public " << namebuf << be_nl;
  *os << "{" << be_nl
      << "public:" << be_idt_nl
      << "/// the T& ctor" << be_nl
      << tiename << " (T &t);" << be_nl
      << "/// ctor taking a POA" << be_nl
      << tiename << " (T &t, PortableServer::POA_ptr poa);" << be_nl
      << "/// ctor taking pointer and an ownership flag" << be_nl
      << tiename << " (T *tp, ::CORBA::Boolean release = true);" << be_nl
      << "/// ctor with T*, ownership flag and a POA" << be_nl
      << tiename << " (" << be_idt << be_idt_nl
      << "T *tp," << be_nl
      << "PortableServer::POA_ptr poa," << be_nl
      << "::CORBA::Boolean release = true" << be_uidt_nl
      << ");" << be_uidt_nl
      << "/// dtor" << be_nl_2
      << "~" << tiename << " (void);" << be_nl
      << "// TIE specific functions" << be_nl
      << "/// return the underlying object" << be_nl
      << "T *_tied_object (void);" << be_nl
      << "/// set the underlying object" << be_nl
      << "void _tied_object (T &obj);" << be_nl
      << "/// set the underlying object and the ownership flag" << be_nl
      << "void _tied_object (T *obj, ::CORBA::Boolean release = true);" << be_nl
      << "/// do we own it" << be_nl
      << "::CORBA::Boolean _is_owner (void);" << be_nl
      << "/// set the ownership" << be_nl_2
      << "void _is_owner ( ::CORBA::Boolean b);" << be_nl
      << "// overridden ServantBase operations" << be_nl
      << "PortableServer::POA_ptr _default_POA (void);";

  int status =
    node->traverse_inheritance_graph (
              be_visitor_interface_tie_sh::method_helper,
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

  *os << be_uidt_nl << be_nl
      << "private:" << be_idt_nl
      << "T *ptr_;" << be_nl
      << "PortableServer::POA_var poa_;" << be_nl
      << "::CORBA::Boolean rel_;" << be_nl_2
      << "// copy and assignment are not allowed" << be_nl
      << tiename << " (const " << tiename << " &);" << be_nl
      << "void operator= (const " << tiename << " &);" << be_uidt_nl
      << "};";

  return 0;
}

int
be_visitor_interface_tie_sh::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_tie_sh::method_helper (be_interface *,
                                            be_interface *node,
                                            TAO_OutStream *os)
{
  // Any methods from abstract parents have already been
  // "added" to the derived interface scope by the overridden
  // visit_scope() method in be_visitor_interface, so we can skip
  // this base interface, if it is abstract.
  if (node->is_abstract ())
    {
      return 0;
    }

  be_visitor_context ctx;
  ctx.stream (os);
  ctx.state (TAO_CodeGen::TAO_ROOT_TIE_SH);
  be_visitor_interface_tie_sh visitor (&ctx);

  if (visitor.visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_tie_sh::"
                         "method_helper\n"),
                        -1);
    }

  return 0;
}
