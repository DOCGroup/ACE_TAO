// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Explicit_Event_Loop
//
// = FILENAME
//    server.cpp
//
// = AUTHORS
//   Source code used in TAO has been modified and adapted from the
//   code provided in the book, "Advanced CORBA Programming with C++"
//   by Michi Henning and Steve Vinoski. Copyright
//   1999. Addison-Wesley, Reading, MA.  Used with permission of
//   Addison-Wesley.
//
//   Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
//
// ============================================================================

#include "server.h"

// The following headers are #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
//#include <time.h>
//#include <iostream.h>

TimeOfDay
Time_impl::
get_gmt ( CORBA_Environment &)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  time_t time_now = time (0);
  struct tm *time_p = gmtime (&time_now);

  TimeOfDay tod;
  tod.hour = time_p->tm_hour;
  tod.minute = time_p->tm_min;
  tod.second = time_p->tm_sec;

  return tod;
}

void do_something_else()
{
}

int
main (int argc, char *argv[])
{
  int ret = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "",
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA",
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;
      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in (),
                                        ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      mgr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create an object
      Time_impl time_servant;

      // Write its stringified reference to stdout
      Time_var tm = time_servant._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      CORBA::String_var str = orb->object_to_string (tm.in (),
                                                     ACE_TRY_ENV);
      ACE_TRY_CHECK;
      cout << str.in () << endl;

      // Explicit Event Loop
      while (1)
        {
          CORBA::Boolean pending =
            orb->work_pending(ACE_TRY_ENV);
          ACE_TRY_CHECK;
          if (pending)
            {
              orb->perform_work(ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          do_something_else();
        }
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A CORBA exception occured");
      ret = 1;
    }
  ACE_CATCHALL
    {
      cerr << "An unknown exception was caught" << endl;
      ret = 1;
    }
  ACE_ENDTRY;

  return ret;
}
