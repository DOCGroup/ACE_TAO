//**************************************************************************
//
// NAME :   tpr_server.cpp 
// DESCRIPTION:  
//
// Server mainline
//
//****************************************************************************

#include <api/api.H>
#include "cubit.H"
#include "cubit.C"
#include "cubit_impl.h"		// server header file
#include "sk_cubit.H"
#include "sk_cubit.C"
#include "ace/OS.h"

int 
main (int argc, char** argv)
{

  CORBA_Environment env;
  Cubit_ptr cubit = new Cubit ();
  Cubit_Impl cubit_impl;
  
  CORBA_ORB_ptr orb = CORBA_ORB_init (argc, argv, 0, env);
  if (env.exception ()) {
    ACE_DEBUG ((LM_ERROR, "ORB_init failed.."));
    return -1;
  }

  CORBA_BOA_ptr boa = orb->OA_init (argc, argv, 0, env);
  if (env.exception ()) {
    ACE_DEBUG ((LM_ERROR, "OA_init failed.."));
    return -1;
  }

  COOL_NamingService_var naming = thisCapsule->naming_service(env);
  naming->export ("Cubit", cubit, env);
  if (env.exception ()) {
    ACE_DEBUG ((LM_ERROR, "Object Reference Export Failed.."));
    return -1;
  }

  COOL_bind (cubit_impl, cubit, env);

  if (env.exception ()) {
    ACE_DEBUG ((LM_ERROR, "Bind failed.."));
    return -1;
  }
  
  ACE_DEBUG ((LM_DEBUG, "Entering boa->run ()"));

  boa->run ();


  cout << "Cubit server is exiting." << endl;


  return 0;
}
