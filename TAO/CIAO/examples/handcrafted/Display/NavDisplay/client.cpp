// $Id$

#include "NavDisplayC.h"

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv ACE_ENV_ARG_PARAMETER);

      // Resolve HomeFinder interface

      CORBA::Object_var obj
        = orb->string_to_object ("file://NavDisplayHome.ior" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      HUDisplay::NavDisplayHome_var home
        = HUDisplay::NavDisplayHome::_narrow (obj
                                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (home.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire NavDisplayHome objref\n"), -1);

      HUDisplay::NavDisplay_var navdisplay
        = home->create(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      HUDisplay::tickConsumer_var consumer_refresh =
        navdisplay->get_consumer_Refresh(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      HUDisplay::tick_var ev = new OBV_HUDisplay::tick ();

      ACE_Time_Value sleep_time (0, 20000); // 20 milliseconds

      for(unsigned i = 0; i < 1000; i++)
      {
        consumer_refresh->push_tick(ev.in()
                              ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        ACE_OS::sleep(sleep_time);
      }

      ACE_OS::sleep (5);

      home->remove_component (navdisplay.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Who is the culprit \n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
