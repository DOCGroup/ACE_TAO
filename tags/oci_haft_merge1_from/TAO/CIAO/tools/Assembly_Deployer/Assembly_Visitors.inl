// $Id$

ACE_INLINE
CIAO::Assembly_Builder_Visitor::Assembly_Builder_Visitor (CORBA::ORB_ptr o,
                                                          Assembly_Context &context,
                                                          CIAO::ID_IMPL_MAP &idmap,
                                                          Deployment_Configuration &config)
  : orb_ (CORBA::ORB::_duplicate (o)),
    context_ (context),
    impl_idref_map_ (idmap),
    deployment_config_ (config)
{
}
