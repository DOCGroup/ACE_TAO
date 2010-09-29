// $Id$

#ifndef CIAO_CONTAINERBASE_IMPL_T_C
#define CIAO_CONTAINERBASE_IMPL_T_C

#include "ciao/Logger/Log_Macros.h"

namespace CIAO
{
  template <typename BASE>
  Container_i<BASE>::Container_i (CORBA::ORB_ptr o,
                                  PortableServer::POA_ptr root_poa)
    : orb_ (CORBA::ORB::_duplicate (o)),
      root_poa_ (PortableServer::POA::_duplicate (root_poa))
  {
  }

  template <typename BASE>
  Container_i<BASE>::~Container_i (void)
  {
    if (! CORBA::is_nil (this->component_poa_.in ()))
      {
        this->component_poa_->destroy (1, 1);
      }

    if (! CORBA::is_nil (this->facet_cons_poa_.in ()))
      {
        this->facet_cons_poa_->destroy (1, 1);
      }

    if (! CORBA::is_nil (this->home_servant_poa_.in ()))
      {
        this->home_servant_poa_->destroy (1, 1);
      }
  }

  template <typename BASE>
  CORBA::Object_ptr
  Container_i<BASE>::resolve_service_reference(const char *service_id)
  {
    if (ACE_OS::strcmp (service_id, "POA") == 0)
      {
        return ::PortableServer::POA::_duplicate (this->component_poa_.in ());
      }
    return CORBA::Object::_nil ();
  }

  template <typename BASE>
  PortableServer::POA_ptr
  Container_i<BASE>::the_POA (void)
  {
    return this->component_poa_.in ();
  }

  template <typename BASE>
  PortableServer::POA_ptr
  Container_i<BASE>::the_port_POA (void)
  {
    return this->facet_cons_poa_.in ();
  }

  template <typename BASE>
  CORBA::ORB_ptr
  Container_i<BASE>::the_ORB (void) const
  {
    return this->orb_.in ();
  }
}

#endif /* CIAO_CONTAINERBASE_IMPL_T_C */

