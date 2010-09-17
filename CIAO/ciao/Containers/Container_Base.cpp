// $Id$

#include "Container_Base.h"
#include "ciao/Logger/Log_Macros.h"

#if !defined (__ACE_INLINE__)
# include "Container_Base.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  Container_i::Container_i (CORBA::ORB_ptr o,
                            PortableServer::POA_ptr root_poa)
    : orb_ (CORBA::ORB::_duplicate (o)),
      root_poa_ (PortableServer::POA::_duplicate (root_poa))
  {
  }

  Container_i::~Container_i (void)
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
}
