/* -*- C++  -*- */
// $Id$
ACE_INLINE
CIAO::ExecutionManager_Impl::ExecutionManager_Impl (CORBA::ORB_ptr orb,
						    PortableServer::poa_ptr poa,
						    char * config_file
						    ACE_ENV_ARG_DECL)
  : orb_ (CORBA::ORB::_duplicate  (orb)),
    poa_ (PortableServer::POA::_deplicate (poa)),
    deployment_config_file_ (CORBA::string_dup (config_file))
{
  ACE_NEW_THROW_EX (dam_list_,
		    Deployment::DomainApplicationManagers,
     		    CORBA::NO_MEMORY());
}
