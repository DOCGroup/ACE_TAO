// $Id$

//========================================================================
//
// = LIBRARY
//     TAO/tests/POA/RootPOA
//
//
// = FILENAME
//     RootPOA.cpp
//
// = DESCRIPTION
//     This program gets the name of the Root POA and prints it out on
//     the standard output.
//
// = AUTHOR
//     Irfan Pyarali
//
//=========================================================================

#include "tao/corba.h"
#include "tao/PortableServer/PortableServer.h"

ACE_RCSID(RootPOA, RootPOA, "$Id$")

int
main (int argc, char **argv)
{
  TAO_ENV_DECLARE_NEW_ENV;

  ACE_TRY
    {
      // Initilize the ORB
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0 TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Resolve the initial references for the name RootPOA thus getting
      // an object of type CORBA::Object.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // apply _narrow on the object of type CORBA::Object, to make it
      // a POA class Object.
      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the name of the root POA.
      CORBA::String_var poa_name =
        root_poa->the_name (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "The RootPOA is : %s\n",
                  poa_name.in ()));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception raised");
      ACE_CHECK_RETURN (-1);
    }
  ACE_ENDTRY;

  return 0;
}
