// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests
//
// = FILENAME
//    server.cpp
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee, and Sergio Flores-Gaitan
//
// ============================================================================

#include <api/binding.H>
#include <api/api.H>
#include <api/coolThreadPool.H>
#include <corba/eoa.H>
#include <corba/eorb.H>

#include "cubit.H"
#include "cubit_impl.h"		// server header file
#include "sk_cubit.H"
#include "sk_cubit.C"
#include "ace/OS.h"

ACE_RCSID(MT_Cubit, server, "$Id$")

CORBA_BOA_ptr boa;

int
create_servant (void *arg)
{
  char ** argv = (char **) arg;

  CORBA_Environment env;
  Cubit_ptr cubit;
  Cubit_Impl cubit_impl;

  COOL_ThreadPool*        MyPool_1;
  u_int concurrencyModel = 0;
  
  if (strcmp (argv[1], "tpool") == 0)
    {
      //
      // Defines a policy for using a pool of 20 threads.
      //
      concurrencyModel = THREAD_POOL;
      ACE_DEBUG ((LM_DEBUG, "Using Thread Pool concurrency model\n"));
    }
  else if (strcmp (argv[1], "tpr") == 0)
    {
      // Defines policy to create a thread for each incoming
      // request.
      concurrencyModel = THREAD_PER_REQUEST;
      ACE_DEBUG ((LM_DEBUG, "Using Thread Per Request concurrency model\n"));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "Concurrency model determination failed.\n"));
      return -1;
    }

  COOL_ThreadPoolControl poolCtl_1 (concurrencyModel, 1, 20, -1);
  COOL_QueueControl      qCtl;
  
  // Defines thread attributes for threads belonging to pools.
  COOL_ThreadAttributes  poolAttr;
  struct sched_param sp;
  poolAttr.init();
  poolAttr.stack_size(8192*2);
  sp.sched_priority = 43;
  poolAttr.sched_attr(&sp);
  //
  // Allocate and initialize first pool
  //
  MyPool_1  = new COOL_ThreadPool;
  MyPool_1->init_pool(poolCtl_1, poolAttr, qCtl);
  
  COOL::EOABindingData bindingData (argv[2]);
  bindingData.threadpool(MyPool_1);
  
  COOL_bind (cubit_impl, cubit, bindingData, env);

  if (env.exception ()) 
    {
      ACE_DEBUG ((LM_ERROR, "Bind failed.\n"));
      return -1;
    }
  
  if (env.exception ()) {
    ACE_DEBUG ((LM_ERROR, "Object Reference Export Failed.\n"));
    return -1;
  }

  ACE_DEBUG ((LM_DEBUG, "Entering boa->run ().\n"));
  boa->run ();
  return 0;
}


int 
main (int argc, char** argv)
{
  CORBA_Environment env;
  int i = 0;

  COOL::EOA::bind(argv[2], env);
  if (env.exception()) {
    ACE_DEBUG ((LM_ERROR, "Impossible to bind the ORB to the description: %s\n", argv[2]));
    return -1;
  }

  CORBA_ORB_ptr orb = CORBA_ORB_init (argc, argv, 0, env);
  if (env.exception ()) {
    ACE_DEBUG ((LM_ERROR, "ORB_init failed..\n"));
    CORBA::SystemException* ex;
    
    ex = CORBA::SystemException::_narrow(env.exception());
    if (ex) {
      CORBA::String_var msg = ex->message();
      fprintf(stderr, "%s.\n", (const char*) msg);
    } else {
      fprintf(stderr, "Unknown user exception.\n");
    }
    return -1;
  }

  boa = orb->OA_init (argc, argv, 0, env);
  if (env.exception ()) 
    {
      ACE_DEBUG ((LM_ERROR, "OA_init failed..\n"));
      return -1;
    }
  
  COOL_Activity_ptr new_activity [2];
  
  for (i = 0; i < 1; i++)
    {
      if (thisCapsule->createActivity ((COOL_ActivityFunc) create_servant,
				       (void *) argv,
                                       new_activity [i]) != C_OK)
        {
	  ACE_ERROR ((LM_ERROR, "%p\n", "activate failed\n"));
        }
    }
  
  CORBA_Long status;
  for (i = 0; i < 1; i++)
    {
      if (new_activity [i]->join (status) != C_OK)
        {
          ACE_ERROR ((LM_ERROR, "%p\n", "join failed\n"));
        }
    }
  
  ACE_DEBUG ((LM_DEBUG, "Cubit server is exiting\n"));
  return 0;
}
