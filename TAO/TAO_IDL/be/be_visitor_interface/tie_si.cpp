//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    tie_si.cpp
//
// = DESCRIPTION
//    Visitor generating code for TIE classes for the Interface node in the
//    inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, tie_si, "$Id$")


// ************************************************************************
// Interface visitor for server inline
// ************************************************************************

be_visitor_interface_tie_si::be_visitor_interface_tie_si (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_tie_si::~be_visitor_interface_tie_si (void)
{
}

int
be_visitor_interface_tie_si::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  static char fulltiename [NAMEBUFSIZE]; // holds the class name
  static char localtiename [NAMEBUFSIZE]; // holds the tie name
  static char localskelname [NAMEBUFSIZE]; // holds the local skeleton name

  if (node->srv_inline_gen () || node->imported ())
    return 0;

  ACE_OS::memset (fulltiename, '\0', NAMEBUFSIZE);
  ACE_OS::memset (localtiename, '\0', NAMEBUFSIZE);
  ACE_OS::memset (localskelname, '\0', NAMEBUFSIZE);

  os = this->ctx_->stream ();

  // generate the skeleton class name which will be used to determine the TIE
  // class name

  // we are outermost
  ACE_OS::sprintf (fulltiename, "%s_tie", node->full_skel_name ());
  if (!node->is_nested ())
    {
      ACE_OS::sprintf (localskelname, "POA_%s",
                       node->local_name ());
      ACE_OS::sprintf (localtiename, "POA_%s_tie",
                       node->local_name ());
    }
  else
    {
      ACE_OS::sprintf (localskelname, "%s",
                       node->local_name ());
      ACE_OS::sprintf (localtiename, "%s_tie",
                       node->local_name ());
    }

  if (node->is_nested ())
    {
      *os << "#if defined (ACE_HAS_USING_KEYWORD)\n\n";
    }

  os->indent (); // start with whatever indentation level we are at

  *os << "template <class T> ACE_INLINE" << be_nl
      << fulltiename << "<T>::" << localtiename << " (T &t)" << be_nl
      << "\t: ptr_ (&t)," << be_nl
      << "\t  poa_ (PortableServer::POA::_nil ())," << be_nl
      << "\t  rel_ (0)" << be_nl
      << "{}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE" << be_nl
      << fulltiename << "<T>::" << localtiename
      << " (T &t, PortableServer::POA_ptr poa)" << be_nl
      << "\t: ptr_ (&t)," << be_nl
      << "\t  poa_ (PortableServer::POA::_duplicate (poa))," << be_nl
      << "\t  rel_ (0)" << be_nl
      << "{}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE" << be_nl
      << fulltiename << "<T>::" << localtiename
      << " (T *tp, CORBA::Boolean release)" << be_nl
      << "\t: ptr_ (tp)," << be_nl
      << "\t  poa_ (PortableServer::POA::_nil ())," << be_nl
      << "\t  rel_ (release)" << be_nl
      << "{}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE" << be_nl
      << fulltiename << "<T>::" << localtiename
      << " (T *tp, PortableServer::POA_ptr poa, CORBA::Boolean release)"
      << be_nl
      << "\t: ptr_ (tp)," << be_nl
      << "\t  poa_ (PortableServer::POA::_duplicate (poa))," << be_nl
      << "\t  rel_ (release)" << be_nl
      << "{}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE" << be_nl
      << fulltiename << "<T>::~" << localtiename << " (void)" << be_nl
      << "{" << be_idt_nl
      << "if (this->rel_) delete this->ptr_;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE T *" << be_nl
      << fulltiename << "<T>::_tied_object (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->ptr_;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE void" << be_nl
      << fulltiename << "<T>::_tied_object (T &obj)" << be_nl
      << "{" << be_idt_nl
      << "if (this->rel_) delete this->ptr_;" << be_nl
      << "this->ptr_ = &obj;" << be_nl
      << "this->rel_ = 0;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE void" << be_nl
      << fulltiename << "<T>::_tied_object (T *obj, "
      << "CORBA::Boolean release)" << be_nl
      << "{" << be_idt_nl
      << "if (this->rel_) delete this->ptr_;" << be_nl
      << "this->ptr_ = obj;" << be_nl
      << "this->rel_ = release;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE CORBA::Boolean" << be_nl
      << fulltiename << "<T>::_is_owner (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->rel_;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE void" << be_nl
      << fulltiename << "<T>::_is_owner (CORBA::Boolean b)" << be_nl
      << "{" << be_idt_nl
      << "this->rel_ = b;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "template <class T> ACE_INLINE "
      << "PortableServer::POA_ptr" << be_nl
      << fulltiename
      << "<T>::_default_POA (ACE_ENV_SINGLE_ARG_DECL)" << be_nl
      << "{" << be_idt_nl
      << "if (!CORBA::is_nil (this->poa_.in ()))" << be_idt_nl
      << "return PortableServer::POA::_duplicate (this->poa_.in ());\n"
      << be_uidt_nl
      << "return this->" << localskelname
      << "::_default_POA (ACE_ENV_SINGLE_ARG_PARAMETER);" << be_uidt_nl
      << "}\n\n";

  if (node->traverse_inheritance_graph (be_visitor_interface_tie_si::method_helper, os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_tie_sh_ss::"
                         "visit_interface - "
                         "traversal of inhertance graph failed\n"),
                        -1);
    }

  if (node->is_nested ())
    {
      *os << "#endif /* ACE_HAS_USING_KEYWORD */\n\n";
    }

  return 0;
}

int
be_visitor_interface_tie_si::method_helper (be_interface *derived,
                                            be_interface *node,
                                            TAO_OutStream *os)
{
  be_visitor_context ctx;
  ctx.state (TAO_CodeGen::TAO_INTERFACE_TIE_SI);
  ctx.interface (derived);
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
