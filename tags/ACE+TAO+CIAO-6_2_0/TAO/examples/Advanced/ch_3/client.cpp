
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  $Id$
 *
 *  @author Source code used in TAO has been modified and adapted from thecode provided in the book
 *  @author "Advanced CORBA Programming with C++"by Michi Henning and Steve Vinoski. Copyright1999. Addison-Wesley
 *  @author Reading
 *  @author MA.  Used with permission ofAddison-Wesley.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================


#include "timeC.h"
#include <ace/streams.h>

// The following header is #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
//#include <iostream.h>

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
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
      x._tao_print_exception ("Who is the culprit\n");
      cerr << "Uncaught CORBA exception" << endl;
      return 1;
    }
  catch  (...)
    {
      return 1;
    }
  return 0;
}
