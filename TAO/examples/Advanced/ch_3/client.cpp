
//=============================================================================
/**
 *  @file    client.cpp
 *
 *  @author Source code used in TAO has been modified and adapted from thecode provided in the book
 *  @author "Advanced CORBA Programming with C++"by Michi Henning and Steve Vinoski. Copyright1999. Addison-Wesley
 *  @author Reading
 *  @author MA.  Used with permission ofAddison-Wesley.Modified for TAO by Mike Moran <mm4@cs.wustl.edu>
 */
//=============================================================================
#include "timeC.h"
#include <iomanip>
#include <iostream>

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Check arguments
      if (argc != 2)
        {
          std::cerr << "Usage: client IOR_string" << std::endl;
          return 1;
        }

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Destringify argv[1]
      CORBA::Object_var obj = orb->string_to_object (argv[1]);
      if (CORBA::is_nil (obj.in ()))
        {
          std::cerr << "Nil Time reference" << std::endl;
          return 1;
        }

      // Narrow
      Time_var tm = Time::_narrow (obj.in ());

      if (CORBA::is_nil (tm.in ()))
        {
          std::cerr << "Argument is not a Time reference" << std::endl;
          return 1;
        }

      {
        std::cout << "Validating connection" << std::endl;
        CORBA::PolicyList_var pols {};
        tm->_validate_connection (pols.out ());
        std::cout << "Successful " << std::endl;
      }
      // Get time
      TimeOfDay const tod = tm->get_gmt ();
      std::cout << "Time in Greenwich is "
                << std::setw (2) << std::setfill ('0') << tod.hour << ":"
                << std::setw (2) << std::setfill ('0') << tod.minute << ":"
                << std::setw (2) << std::setfill ('0') << tod.second
                << std::endl;
    }
  catch (CORBA::Exception const& x)
    {
      x._tao_print_exception ("Who is the culprit\n");
      std::cerr << "Uncaught CORBA exception" << std::endl;
      return 1;
    }
  catch (...)
    {
      return 1;
    }
  return 0;
}
