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
CIAO::NodeDaemon_Impl::
~NodeDaemon_Impl ()
{}

ACE_INLINE
CIAO::NodeApplicationManager_Impl_Base*
CIAO::NodeDaemon_Impl::
create_node_app_manager (CORBA::ORB_ptr orb, 
                         PortableServer::POA_ptr poa)
{
  return new CIAO::NodeApplicationManager_Impl (orb, poa);
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
CIAO::Static_NodeDaemon_Impl::
~Static_NodeDaemon_Impl ()
{}

ACE_INLINE
CIAO::NodeApplicationManager_Impl_Base *
CIAO::Static_NodeDaemon_Impl::
create_node_app_manager (CORBA::ORB_ptr orb, 
                         PortableServer::POA_ptr poa)
{
  return new CIAO::Static_NodeApplicationManager_Impl (orb, poa, this->static_config_entrypoints_maps_);
}
