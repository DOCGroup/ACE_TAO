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
{};



int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY 
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      // Activate POA manager
      PortableServer::POAManager_var mgr
        = poa->the_POAManager ();
      mgr->activate ();

      // Create an object
      Time_impl time_servant;

      // Write its stringified reference to stdout
      Time_var tm = time_servant._this ();
      CORBA::String_var str = orb->object_to_string (tm.in ());
      cout << str.in () << endl;

      // Explicit Event Loop
      while (1)
        {
          if (orb->work_pending())
            {  
              orb->perform_work();
            }
          do_something_else();
        }
      orb->shutdown(1);

    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION(ACE_ANY_EXCEPTION, "A CORBA exception occured");
      return 1;
    }
  ACE_CATCHALL
    {
      cerr << "An unknown exception was caught" << endl;
      return 1;
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN(-1);
  return 0;
}








