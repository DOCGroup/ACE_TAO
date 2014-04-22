
//=============================================================================
/**
 *  @file    tie_ss.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TIE classes for the Interface node in the
 *  inline file.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_tie_ss::be_visitor_interface_tie_ss (
    be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_tie_ss::~be_visitor_interface_tie_ss (void)
{
}

int
be_visitor_interface_tie_ss::visit_interface (be_interface *node)
{
  if (node->srv_inline_gen ()
      || node->imported ()
      || node->is_abstract ())
    {
      return 0;
    }

  static char fulltiename [NAMEBUFSIZE];
  static char localtiename [NAMEBUFSIZE];
  static char localskelname [NAMEBUFSIZE];

  ACE_OS::memset (fulltiename,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (localtiename,
                  '\0',
                  NAMEBUFSIZE);
  ACE_OS::memset (localskelname,
                  '\0',
                   NAMEBUFSIZE);

  // Generate the skeleton class name which will be used to determine the TIE
  // class name.

  // We are outermost.
  ACE_OS::sprintf (fulltiename,
                   "%s_tie",
                   node->full_skel_name ());

  if (!node->is_nested ())
    {
      ACE_OS::sprintf (localskelname,
                       "POA_%s",
                       node->local_name ());
      ACE_OS::sprintf (localtiename,
                       "POA_%s_tie",
                       node->local_name ());
    }
  else
    {
      ACE_OS::sprintf (localskelname,
                       "%s",
                       node->local_name ());
      ACE_OS::sprintf (localtiename,
                       "%s_tie",
                       node->local_name ());
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "template <class T>" << be_nl
      << fulltiename << "<T>::" << localtiename << " (T &t)" << be_idt_nl
      << ": ptr_ (&t)," << be_idt_nl
      << "poa_ ( ::PortableServer::POA::_nil ())," << be_nl
      << "rel_ (false)" << be_uidt << be_uidt_nl
      << "{}" << be_nl_2;

  *os << "template <class T>" << be_nl
      << fulltiename << "<T>::" << localtiename
      << " (T &t, ::PortableServer::POA_ptr poa)" << be_idt_nl
      << ": ptr_ (&t)," << be_idt_nl
      << "poa_ ( ::PortableServer::POA::_duplicate (poa))," << be_nl
      << "rel_ (false)" << be_uidt << be_uidt_nl
      << "{}" << be_nl_2;

  *os << "template <class T>" << be_nl
      << fulltiename << "<T>::" << localtiename
      << " (T *tp, ::CORBA::Boolean release)" << be_idt_nl
      << ": ptr_ (tp)," << be_idt_nl
      << "poa_ ( ::PortableServer::POA::_nil ())," << be_nl
      << "rel_ (release)" << be_uidt << be_uidt_nl
      << "{}" << be_nl_2;

  *os << "template <class T>" << be_nl
      << fulltiename << "<T>::" << localtiename
      << " (T *tp, ::PortableServer::POA_ptr poa, ::CORBA::Boolean release)"
      << be_idt_nl
      << ": ptr_ (tp)," << be_idt_nl
      << "poa_ ( ::PortableServer::POA::_duplicate (poa))," << be_nl
      << "rel_ (release)" << be_uidt << be_uidt_nl
      << "{}" << be_nl_2;

  *os << "template <class T>" << be_nl
      << fulltiename << "<T>::~" << localtiename << " (void)" << be_nl
      << "{" << be_idt_nl
      << "if (this->rel_)" << be_idt_nl
      << "{" << be_idt_nl
      << "delete this->ptr_;" << be_uidt_nl
      << "}" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "template <class T> T *" << be_nl
      << fulltiename << "<T>::_tied_object (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->ptr_;" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "template <class T> void" << be_nl
      << fulltiename << "<T>::_tied_object (T &obj)" << be_nl
      << "{" << be_idt_nl
      << "if (this->rel_)" << be_idt_nl
      << "{" << be_idt_nl
      << "delete this->ptr_;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->ptr_ = &obj;" << be_nl
      << "this->rel_ = false;" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "template <class T> void" << be_nl
      << fulltiename << "<T>::_tied_object (T *obj, "
      << "::CORBA::Boolean release)" << be_nl
      << "{" << be_idt_nl
      << "if (this->rel_)" << be_idt_nl
      << "{" << be_idt_nl
      << "delete this->ptr_;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "this->ptr_ = obj;" << be_nl
      << "this->rel_ = release;" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "template <class T>  ::CORBA::Boolean" << be_nl
      << fulltiename << "<T>::_is_owner (void)" << be_nl
      << "{" << be_idt_nl
      << "return this->rel_;" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "template <class T> void" << be_nl
      << fulltiename << "<T>::_is_owner ( ::CORBA::Boolean b)" << be_nl
      << "{" << be_idt_nl
      << "this->rel_ = b;" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "template <class T> "
      << "PortableServer::POA_ptr" << be_nl
      << fulltiename
      << "<T>::_default_POA ()" << be_nl
      << "{" << be_idt_nl
      << "if (! ::CORBA::is_nil (this->poa_.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "return ::PortableServer::POA::_duplicate (this->poa_.in ());"
      << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return this->" << localskelname
      << "::_default_POA ();" << be_uidt_nl
      << "}";

  int const status =
    node->traverse_inheritance_graph (
              be_visitor_interface_tie_ss::method_helper,
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

int
be_visitor_interface_tie_ss::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_tie_ss::method_helper (be_interface *derived,
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
  ctx.interface (derived);
  ctx.stream (os);
  ctx.state (TAO_CodeGen::TAO_ROOT_TIE_SS);
  be_visitor_interface_tie_ss visitor (&ctx);

  if (visitor.visit_scope (node) == -1)

    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_tie_ss::"
                         "method_helper\n"),
                        -1);
    }

  return 0;
}
