// $Id$
#include "NodeApp_CB_Impl.h"

CIAO::NodeApplication_Callback_Impl::
NodeApplication_Callback_Impl  (CORBA::ORB_ptr o,
				PortableServer::POA_ptr p,
				Deployment::NodeApplicationManager_ptr s,
				const Deployment::Properties &properties)
  : orb_ (CORBA::ORB::_duplicate (o)),
    poa_ (PortableServer::POA::_duplicate (p)),
    nam_ (s)
{
  this->properties_ = new Deployment::Properties (properties);
}

CIAO::NodeApplication_Callback_Impl::
~NodeApplication_Callback_Impl ()
{
}

PortableServer::POA_ptr
CIAO::NodeApplication_Callback_Impl::
_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

Deployment::NodeApplicationManager_ptr
CIAO::NodeApplication_Callback_Impl::
register_node_application (Deployment::NodeApplication_ptr na,
			   Deployment::Properties_out properties
			   ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  properties = this->properties_._retn ();

  this->nodeapp_ = Deployment::NodeApplication::_duplicate (na);
  return Deployment::NodeApplicationManager::_duplicate (this->nam_);
}

Deployment::NodeApplication_ptr
CIAO::NodeApplication_Callback_Impl::
get_nodeapp_ref (void)
{
  // Relinquish the ownership of the nodeapplication reference.
  //This method should only be called from the NodeApplicationManager.
  return this->nodeapp_._retn ();
}
