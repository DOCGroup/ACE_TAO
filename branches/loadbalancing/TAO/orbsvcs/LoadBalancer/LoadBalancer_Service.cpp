// -*- C++ -*-

// $Id$

ACE_RCSID(LoadBalancer, LoadBalancer_Service, "$Id$")


int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      TAO_LoadBalancer balancer;

      balancer.init (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      balancer.run (ACE_TRY_ENV);
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
