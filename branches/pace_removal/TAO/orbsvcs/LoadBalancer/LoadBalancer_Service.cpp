// -*- C++ -*-

// $Id$

#include "LoadBalancer.h"

ACE_RCSID(LoadBalancer, LoadBalancer_Service, "$Id$")

int
main (int argc, char *argv[])
{
  // @@ Ossama: i'm not sure what the LoadBalancer class buys you.
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer balancer;

      balancer.init (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      balancer.run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "TAO Load Balancer");
      return -1;
    }
  ACE_ENDTRY;


  return 0;
}
