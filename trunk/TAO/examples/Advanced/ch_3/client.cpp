// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/examples/Advanced/ch_3
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
#include <iomanip.h>

// The following header is #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
//#include <iostream.h>

int
main (int argc, char *argv[])
{
  try 
    {
      // Check arguments
      if  (argc != 2) 
        {
          cerr << "Usage: client IOR_string" << endl;
          throw 0;
        }

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Destringify argv[1]
      CORBA::Object_var obj = orb->string_to_object (argv[1]);
      if  (CORBA::is_nil (obj.in ())) 
        {
          cerr << "Nil Time reference" << endl;
          throw 0;
        }

      // Narrow
      Time_var tm = Time::_narrow (obj.in ());

      if  (CORBA::is_nil (tm.in ())) 
        {
          cerr << "Argument is not a Time reference" << endl;
          throw 0;
        }

       {
       cout << "Validating connection"<<endl;
       CORBA::PolicyList_var pols;
       tm->_validate_connection (pols.out ());
       cout << "Succesfull " <<endl;
       }
      // Get time
      TimeOfDay tod = tm->get_gmt ();
      cout << "Time in Greenwich is "
           << setw (2) << setfill ('0') << tod.hour << ":"
           << setw (2) << setfill ('0') << tod.minute << ":"
           << setw (2) << setfill ('0') << tod.second << endl;
    }
  catch  (const CORBA::Exception &x) 
    {
      ACE_PRINT_EXCEPTION (x,
                           "Who is the culprit \n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  catch  (...) 
    {
      return 1;
    }
  return 0;
}
