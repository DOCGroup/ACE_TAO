// $Id$

CIAO::Assembly_Builder_Visitor::Assembly_Builder_Visitor (CORBA::ORB_ptr o,
                                                          Assembly_Context &context,
                                                          Deployment_Configuration &config)
  : orb_ (CORBA::ORB::_duplicate (o)),
    context_ (context),
    deployment_config_ (config)
{
}
