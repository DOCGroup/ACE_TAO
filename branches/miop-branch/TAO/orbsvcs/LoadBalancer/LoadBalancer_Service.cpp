// -*- C++ -*-

// $Id$

#include "LoadBalancer.h"

ACE_RCSID(LoadBalancer, LoadBalancer_Service, "$Id$")

int
main (int argc, char *argv[])
{
  // @@ Ossama: i'm not sure what the LoadBalancer class buys you.
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer balancer;

      balancer.init (argc, argv TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      balancer.run (TAO_ENV_SINGLE_ARG_PARAMETER);
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
