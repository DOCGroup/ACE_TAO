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

CORBA_BOA_ptr boa;

int
create_servant (void *arg)
{
  char name [1024];
  int i = * (int *)arg;

  if (i == 0)
    strcpy (name, "HP_Cubit");
  else
    strcpy (name, "LP_Cubit");

  CORBA_Environment env;
  Cubit_ptr cubit;
  Cubit_Impl cubit_impl;

  COOL_bind (cubit_impl, cubit, env);

  if (env.exception ()) 
    {
      ACE_DEBUG ((LM_ERROR, "Bind failed.."));
      return -1;
    }
  
  COOL_NamingService_var naming = 
    thisCapsule->naming_service (env);

  naming->export (name, cubit, env);

  if (env.exception ()) {
    ACE_DEBUG ((LM_ERROR, "Object Reference Export Failed.."));
    return -1;
  }

  ACE_DEBUG ((LM_DEBUG, "Entering boa->run ()\n"));
  boa->run ();
  return 0;
}


int 
main (int argc, char** argv)
{

  CORBA_Environment env;
  CORBA_ORB_ptr orb = CORBA_ORB_init (argc, argv, 0, env);
  if (env.exception ()) {
    ACE_DEBUG ((LM_ERROR, "ORB_init failed.."));
    return -1;
  }

  boa = orb->OA_init (argc, argv, 0, env);
  if (env.exception ()) 
    {
      ACE_DEBUG ((LM_ERROR, "OA_init failed.."));
      return -1;
    }

  COOL_Activity_ptr new_activity [2];

  for (int i = 0; i < 2; i++)
    {
      if (thisCapsule->createActivity ((COOL_ActivityFunc) create_servant,
                                       (void *) &i, 
                                       new_activity [i]) != C_OK)
        {
        ACE_ERROR ((LM_ERROR, "%p\n", "activate failed"));
        }
    }

  CORBA_Long status;
  for (i = 0; i < 2; i++)
    {
      if (new_activity [i]->join (status) != C_OK)
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "join failed"));
        }
    }
  
  ACE_DEBUG ((LM_DEBUG, "Cubit server is exiting\n"));
  return 0;
}
