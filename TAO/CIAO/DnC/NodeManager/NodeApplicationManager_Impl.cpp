// $Id$

#include "NodeApplicationManager_Impl.h"

CIAO::NodeApplicationManager_Impl::
NodeApplicationManager_Impl (CORBA::ORB_ptr orb,
                             PortableServer::POA_ptr poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    poa_ (PortableServer::POA::_duplicate (poa))
{
}


CIAO::NodeApplicationManager_Impl::~NodeApplicationManager_Impl ()
{

}

PortableServer::POA_ptr
CIAO::NodeApplicationManager_Impl::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

Deployment::Application_ptr
CIAO::NodeApplicationManager_Impl::startLaunch (const Deployment::Properties & configProperty,
                                                Deployment::Connections_out providedReference,
                                                CORBA::Boolean start
                                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException,
		   Deployment::ResourceNotAvailable,
		   Deployment::StartError,
		   Deployment::InvalidProperty))
{
  /**
   *  1. First Map properties to TAO/CIAO specific property/configurations
   *  2. Necessary property checking (needed?)
   *  3. Call create_nade_application to spawn new process.
   *  4. Initialize the NodeApplication.
   *  5. get the provided connection endpoints back and return them.
   */
}


void
CIAO::NodeApplicationManager_Impl::destroyApplication (Deployment::Application_ptr app
                                                       ACE_ENV_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException, Deployment::StopError))
{
  /*
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->cs_set_.object_in_set (app) == 0)
    ACE_THROW (Deployment::StopError ());

  app->remove (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Should we remove the server still even if the previous call failed.

  if (this->cs_set_.remove (app) == -1)
    ACE_THROW (Deployment::StopError ());
  */
}
