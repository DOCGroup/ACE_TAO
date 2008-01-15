#include "ciao/Dynamic_Component_Activator.h"
#include "ciao/Dynamic_Component_Servant_Base.h"

ACE_RCSID (ciao,
           Dynamic_Component_Activator,
           "$Id$")

#if !defined (__ACE_INLINE__)
# include "Dynamic_Component_Activator.inl"
#endif /* __ACE_INLINE__ */

namespace CIAO
{
  Dynamic_Component_Activator::Dynamic_Component_Activator (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o))
  {
  }

  Dynamic_Component_Activator::~Dynamic_Component_Activator (void)
  {
  }

  PortableServer::Servant
  Dynamic_Component_Activator::incarnate (const PortableServer::ObjectId &oid,
                                          PortableServer::POA_ptr)
  {
    Dynamic_Component_Servant_Base* servant = 0;
    PortableServer::ObjectId new_oid (oid);

    if (this->servant_map_.find (oid, servant) == 0)
      {
        return servant->create (new_oid);
      }

    return 0;
  }

  void
  Dynamic_Component_Activator::etherealize (
    const PortableServer::ObjectId &,
    PortableServer::POA_ptr,
    PortableServer::Servant,
    CORBA::Boolean,
    CORBA::Boolean)
  {
  }
}
