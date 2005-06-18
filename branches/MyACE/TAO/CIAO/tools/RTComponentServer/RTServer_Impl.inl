/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::RTServer::RTComponentServer_Impl::RTComponentServer_Impl (CORBA::ORB_ptr o,
                                                                RTCORBA::RTORB_ptr rto,
                                                                PortableServer::POA_ptr p,
																int static_config_flag,
																const Static_Config_EntryPoints_Maps* static_entrypts_maps)
  : ComponentServer_Impl (o, p, static_config_flag, static_entrypts_maps),
    resource_manager_ (rto),
    policy_set_manager_ (resource_manager_, o, rto)
{
}

ACE_INLINE
CIAO::RTServer::RTContainer_Impl::RTContainer_Impl (CORBA::ORB_ptr o,
                                                    PortableServer::POA_ptr p,
                                                    Components::Deployment::ComponentServer_ptr s,
                                                    CIAO::RTPolicy_Set_Manager &pm,
						    int static_config_flag,
                                                    const Static_Config_EntryPoints_Maps* static_entrypts_maps)
  : Container_Impl (o,
                    p,
                    s,
		    static_config_flag,
		    static_entrypts_maps),
    policyset_manager_ (pm)
{
}

