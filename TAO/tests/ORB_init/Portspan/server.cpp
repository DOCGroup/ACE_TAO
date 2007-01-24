// $Id$

#include "simpleI.h"

int
main( int argc, char *argv[] )
{
  ACE_TRY_NEW_ENV
    {

      CORBA::ORB_var orb = CORBA::ORB_init(argc, argv, ""
                                           ACE_ENV_ARG_PARAMETER);

      CORBA::Object_var obj = orb->resolve_initial_references("RootPOA"
                                                              ACE_ENV_ARG_PARAMETER);

      PortableServer::POA_var poa = PortableServer::POA::_narrow(obj.in()
                                                                 ACE_ENV_ARG_PARAMETER);

      PortableServer::POAManager_var man = poa->the_POAManager();

      man->activate();

      simple_i simp;

      PortableServer::ObjectId_var objId = poa->activate_object(&simp
                                                                ACE_ENV_ARG_PARAMETER);

      orb->run();
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception in main():");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
