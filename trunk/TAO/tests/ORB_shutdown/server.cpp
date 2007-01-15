// $Id$

#include "Foo_Bar.h"
#include "ace/Time_Value.h"
#include "ace/Get_Opt.h"

ACE_RCSID (Hello,
           server,
           "$Id$")

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Foo_Bar *foobar_impl;
      ACE_NEW_RETURN (foobar_impl,
                      Foo_Bar (orb.in ()),
                      1);

      Test::Foo_var foo =
        foobar_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Dont unscope it or move it elsewhere.. It is here with a
      // purpose. If you dont understand this, please re-read the
      // README file.
      {
        PortableServer::ServantBase_var owner_transfer (foobar_impl);
      }

      ACE_Time_Value tv (10,
                         0);

      // Just run the ORB for a minute..
      orb->run (tv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) server - shutting down the ORB\n"));

      orb->shutdown (1
                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "(%P|%t) Finished shutting down the ORB\n"));

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) Test successful.. \n"));
  return 0;
}
