#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID (be_visitor_interface, base_proxy_broker_ss, "$Id$")

be_visitor_interface_strategized_proxy_broker_ss::be_visitor_interface_strategized_proxy_broker_ss (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
  // No-Op.
}


be_visitor_interface_strategized_proxy_broker_ss::~be_visitor_interface_strategized_proxy_broker_ss (void)
{
  // No-Op.
}

int
be_visitor_interface_strategized_proxy_broker_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the class declaration.
  os->indent ();

  *os << be_nl
      << "///////////////////////////////////////////////////////////////////////" << be_nl
      << "//            Strategized Proxy Broker Implementation" << be_nl
      << "//" << be_nl << be_nl;

  *os << "// Factory function Implementation." << be_nl
      << node->full_strategized_proxy_broker_name ()
      << " *" << node->full_strategized_proxy_broker_name () << "::the"
      << node->strategized_proxy_broker_name ()
      << " (void)" << be_nl
      << "{" << be_idt_nl
      << "static " << node->full_strategized_proxy_broker_name ()
      << " strategized_proxy_broker;" << be_nl
      << "return &strategized_proxy_broker;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Constructor Implementation.
  *os << node->full_strategized_proxy_broker_name () << "::"
      << node->strategized_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_idt_nl // idt = 1
      << "for (int i = 0; i < TAO_ORB_Core::COLLOCATION_STRATEGIES_NUM; ++i)"
      << be_idt_nl // idt = 2
      << "this->proxy_cache_[i] = 0;"
      << be_uidt_nl // idt = 1
      << be_uidt_nl // idt = 0
      << "}" << be_nl << be_nl;

  // Destructor Implementation.
  *os << node->full_strategized_proxy_broker_name () << "::~"
      << node->strategized_proxy_broker_name () << " (void)" << be_nl
      << "{" << be_idt_nl // idt = 1
      << "for (int i = 0; i < TAO_ORB_Core::COLLOCATION_STRATEGIES_NUM; ++i)"
      << be_idt_nl // idt = 2
      << "delete this->proxy_cache_[i];"
      << be_uidt_nl // idt = 1
      << be_uidt_nl // idt = 0
      << "}" << be_nl << be_nl;

  // select_proxy impementation
  *os << node->full_base_proxy_impl_name () << "&" << be_nl
      <<node->full_strategized_proxy_broker_name () << "::"
      << "select_proxy ("
      << be_idt << be_idt_nl // idt = 2
      << "::" << node->full_name () << " *object," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV"
      << be_uidt_nl // idt = 1
      << ")"
      << be_uidt_nl // idt = 0
      << "{"
      << be_idt_nl // idt = 1
      << "TAO_ORB_Core::TAO_Collocation_Strategies strategy ="
      << be_idt_nl // idt = 2
      << "TAO_ORB_Core::collocation_strategy (object);"
      << be_uidt_nl << be_nl // idt = 1
      << "if (this->proxy_cache_[strategy] != 0)"
      << be_idt_nl // idt = 2
      << "return *this->proxy_cache_[strategy];"
      << be_uidt_nl << be_nl // idt = 1
      << "this->create_proxy (strategy, ACE_TRY_ENV);"
      << be_nl
      << "ACE_CHECK_RETURN (*this->proxy_cache_[strategy]);"
      << be_nl << be_nl
      << "return *this->proxy_cache_[strategy];" << be_nl
      << be_uidt_nl // idt = 0
      << "}" << be_nl << be_nl;

  // create_proxy implementation
  *os << "void " << be_nl
      <<node->full_strategized_proxy_broker_name () << "::"
      << "create_proxy ("
      << be_idt << be_idt_nl // idt = 2
      << "TAO_ORB_Core::TAO_Collocation_Strategies strategy,"
      << be_nl
      << "CORBA::Environment &ACE_TRY_ENV"
      << be_uidt_nl // idt = 1
      << ")"
      << be_uidt_nl // idt = 0
      << "{"
      << be_idt_nl // idt = 1
      << "ACE_GUARD (ACE_SYNCH_MUTEX, guard, this->mutex_);"
      << be_nl << be_nl
      << "if (this->proxy_cache_[strategy] == 0)"
      << be_idt_nl // idt = 2
      << "{"
      << be_idt_nl // idt = 3
      << "switch (strategy)"
      << be_idt_nl // idt = 4
      << "{" << be_nl;
      if (be_global->gen_thru_poa_collocation ())
        {
        *os << "case TAO_ORB_Core::THRU_POA_STRATEGY:"
            << be_idt_nl // idt = 5
            << "ACE_NEW_THROW_EX ("
            << be_idt << be_idt_nl //idt = 7
            << "this->proxy_cache_[strategy]," << be_nl
            << node->full_thru_poa_proxy_impl_name () << "," << be_nl
            << "CORBA::NO_MEMORY ()"
            << be_uidt << be_uidt_nl // idt = 5
            << ");" << be_nl
            << "ACE_CHECK;" << be_nl
            << "break;"
            << be_nl << be_uidt_nl; // idt = 4
          }

      if (be_global->gen_direct_collocation ())
        {
          *os << "case TAO_ORB_Core::DIRECT_STRATEGY:"
              << be_idt_nl // idt = 5
              << "ACE_NEW_THROW_EX ("
              << be_idt << be_idt_nl //idt = 7
              << "this->proxy_cache_[strategy]," << be_nl
              << node->full_direct_proxy_impl_name () << "," << be_nl
              << "CORBA::NO_MEMORY ()"
              << be_uidt << be_uidt_nl // idt = 5
              << ");" << be_nl
              << "ACE_CHECK;" << be_nl
              << "break;"
              << be_nl << be_uidt_nl; // idt = 4
            }

      *os << "case TAO_ORB_Core::REMOTE_STRATEGY:" << be_nl
          << "default:"
          << be_idt_nl // idt = 5
          << "ACE_NEW_THROW_EX ("
          << be_idt << be_idt_nl //idt = 7
          << "this->proxy_cache_[strategy]," << be_nl
          << "::" << node->full_remote_proxy_impl_name () << "," << be_nl
          << "CORBA::NO_MEMORY ()"
          << be_uidt << be_uidt_nl // idt = 5
          << ");" << be_nl
          << "ACE_CHECK;" << be_nl
          << "break;"
          << be_uidt_nl // idt = 4
          << be_uidt_nl // idt = 3

      << "}"
      << be_uidt_nl // idt = 2
      << be_uidt_nl // idt = 1
      << "}"
      << be_uidt_nl // idt = 0

      << "}" << be_nl << be_nl;

  *os << be_nl
      << "//" << be_nl
      << "//        End Strategized Proxy Broker Implementation" << be_nl
      << "///////////////////////////////////////////////////////////////////////"
      << be_nl << be_nl;

  return 0;
}
