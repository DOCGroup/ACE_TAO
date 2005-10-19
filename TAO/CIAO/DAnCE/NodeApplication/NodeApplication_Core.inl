// -*- C++ -*-   $Id$

ACE_INLINE
CIAO::NoOp_Configurator::NoOp_Configurator (CORBA::ORB_ptr o)
  : NodeApp_Configurator (o)
{
}

ACE_INLINE
CIAO::NodeApplication_Core::NodeApplication_Core (CORBA::ORB_ptr o,
                                                  CIAO::NodeApplication_Options &opts)
  : orb_ (CORBA::ORB::_duplicate (o)),
    options_ (opts),
    configurator_ (0)
{ 
}

