/* -*- C++ -*- */
// $Id$

ACE_INLINE
CIAO::RTServer::RTComponentServer_Impl::RTComponentServer_Impl (CORBA::ORB_ptr o,
                                                                RTCORBA::RTORB_ptr rto,
                                                                PortableServer::POA_ptr p)
  : ComponentServer_Impl (o, p),
    resource_manager_ (rto),
    policy_set_manager_ (resource_manager_, o, rto)
{
}

ACE_INLINE
CIAO::RTServer::RTContainer_Impl::RTContainer_Impl (CORBA::ORB_ptr o,
                                                    PortableServer::POA_ptr p,
                                                    Components::Deployment::ComponentServer_ptr s,
                                                    CIAO::RTPolicy_Set_Manager &pm)
  : Container_Impl (o,
                    p,
                    s),
    policyset_manager_ (pm)
{
}
