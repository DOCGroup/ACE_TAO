// $Id$

#include "Empty.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Multiple_Deactivations, server, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "ERROR: (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      Test::Empty_var empty;
      {
        Empty *empty_impl;
        ACE_NEW_RETURN (empty_impl,
                        Empty,
                        1);
        PortableServer::ServantBase_var empty_owner_transfer(empty_impl);

        empty = empty_impl->_this (ACE_TRY_ENV);
        ACE_TRY_CHECK;
      }

      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      CORBA::Boolean is_a_retval =
        empty->_is_a ("IDL:Test/Empty:1.0", ACE_TRY_ENV);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "_is_a returns %d\n", is_a_retval));

      // Try one deactivation
      empty->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      orb->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
