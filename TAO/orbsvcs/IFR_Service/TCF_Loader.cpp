/* -*- C++ -*- */
// $Id$

#include "tao/ORB.h"
#include "TCF_Loader.h"
#include "TypeCodeFactory_i.h"

ACE_RCSID(IFR_Service, TCF_Loader, "$Id$")

TCF_Loader::TCF_Loader (void)
{
}

CORBA::Object_ptr
TCF_Loader::create_object (CORBA::ORB_ptr orb,
                           int,
                           char * [],
                           CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC (())
{
  ACE_TRY
    {
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in (),
                                      ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var mgr =
        poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      mgr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      TAO_TypeCodeFactory_i *servant = 0;
      ACE_NEW_RETURN (servant,
                      TAO_TypeCodeFactory_i (),
                      CORBA::Object::_nil ());

      PortableServer::ServantBase_var servant_var (servant);
      obj = servant->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_TRY_CHECK;

      return obj._retn ();
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "TCF_Loader::create_object");
    }
  ACE_ENDTRY;
  return CORBA::Object::_nil ();
}

// ****************************************************************

ACE_FACTORY_DEFINE (TCF, TCF_Loader)
