// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Explicit_Event_Loop
//
// = FILENAME
//    client.cpp
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

#include "timeC.h"

// The following header is #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
//#include <iostream.h>
//#include <iomanip.h>

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY 
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Check arguments
      if  (argc != 2) 
        {
          cerr << "Usage: client IOR_string" << endl;
          return 1;
        }

      // Destringify argv[1]
      CORBA::Object_var obj = orb->string_to_object (argv[1]);
      if  (CORBA::is_nil (obj.in ())) 
        {
          cerr << "Nil Time reference" << endl;
          return 1;
        }

      // Narrow
      Time_var tm = Time::_narrow (obj.in ());

      if  (CORBA::is_nil (tm.in ())) 
        {
          cerr << "Argument is not a Time reference" << endl;
          return 1;
        }

      // Get time
      TimeOfDay tod = tm->get_gmt (ACE_TRY_ENV);
      ACE_TRY_CHECK;
      cout << "Time in Greenwich is "
           << setw (2) << setfill ('0') << tod.hour << ":"
           << setw (2) << setfill ('0') << tod.minute << ":"
           << setw (2) << setfill ('0') << tod.second << endl;
    }

  ACE_CATCHANY 
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "A CORBA exception occured");
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
