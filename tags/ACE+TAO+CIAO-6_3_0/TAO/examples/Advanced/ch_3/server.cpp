
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  @author Source code used in TAO has been modified and adapted from thecode provided in the book
 *  @author "Advanced CORBA Programming with C++"by Michi Henning and Steve Vinoski. Copyright1999. Addison-Wesley
 *  @author Reading
 *  @author MA.  Used with permission ofAddison-Wesley.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================


#include "server.h"
#include <ace/streams.h>
#include <ace/OS_NS_time.h>

// The following headers are #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
//#include <time.h>
//#include <iostream.h>

TimeOfDay
Time_impl::
get_gmt (void)
{
  time_t time_now = ACE_OS::time (0);
  struct tm *time_p = ACE_OS::gmtime (&time_now);

  TimeOfDay tod;
  tod.hour = time_p->tm_hour;
  tod.minute = time_p->tm_min;
  tod.second = time_p->tm_sec;

  return tod;
}

// $Id$
// Changelog for Henning and Vinoski's chapter 18 example
// ______________________________________________________

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
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

      // Accept requests
      orb->run ();
    }
  catch (const CORBA::Exception &)
    {
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  return 0;
}








