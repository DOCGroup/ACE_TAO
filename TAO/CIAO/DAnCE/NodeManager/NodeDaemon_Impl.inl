/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::NodeDaemon_Impl::
NodeDaemon_Impl (const char *name,
                 CORBA::ORB_ptr orb,
                 PortableServer::POA_ptr poa,
                 const char * nodapp_loc,
                 int spawn_delay)
  : NodeDaemon_Impl_Base (name, orb, poa, nodapp_loc, spawn_delay)
{}


ACE_INLINE
::CIAO::NodeApplicationManager_Impl_Base *
::CIAO::NodeDaemon_Impl::
create_node_app_manager (CORBA::ORB_ptr orb, 
                         PortableServer::POA_ptr poa
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CIAO::NodeApplicationManager_Impl_Base *app_mgr;
  ACE_NEW_THROW_EX (app_mgr,
                    CIAO::NodeApplicationManager_Impl (orb,
                                                       poa),
                    CORBA::NO_MEMORY ());
  return app_mgr;
}

ACE_INLINE
CIAO::Static_NodeDaemon_Impl::
Static_NodeDaemon_Impl (const char *name,
                        CORBA::ORB_ptr orb,
                        PortableServer::POA_ptr poa,
                        const char * nodapp_loc,
                        int spawn_delay,
                        Static_Config_EntryPoints_Maps* static_config_entrypoints_maps)
  : NodeDaemon_Impl_Base (name, orb, poa, nodapp_loc, spawn_delay),
    static_config_entrypoints_maps_ (static_config_entrypoints_maps)
{}

ACE_INLINE
::CIAO::NodeApplicationManager_Impl_Base *
::CIAO::Static_NodeDaemon_Impl::
create_node_app_manager (CORBA::ORB_ptr orb, 
                         PortableServer::POA_ptr poa
                         ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CIAO::NodeApplicationManager_Impl_Base *app_mgr;
  ACE_NEW_THROW_EX (app_mgr,
    CIAO::Static_NodeApplicationManager_Impl (orb,
      poa,
      this->static_config_entrypoints_maps_),
    CORBA::NO_MEMORY ());
  return app_mgr;
}

