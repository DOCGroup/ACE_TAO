
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

#include "ace/Log_Msg.h"

// The following header is #included automatically by ACE+TAO.
// Therefore, they don't need to be included explicitly.
//#include <iostream.h>
//#include <iomanip.h>

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv);

      // Check arguments.
      if  (argc != 2)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: client IOR_string\n"),
                            1);
        }

      // Destringify argv[1].
      CORBA::Object_var obj = orb->string_to_object (argv[1]);

      if  (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Time reference\n"),
                            1);
        }

      // Narrow.
      Time_var tm = Time::_narrow (obj.in ());

      if  (CORBA::is_nil (tm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Argument is not a Time reference\n"),
                            1);
        }

      // Get time.
      TimeOfDay tod = tm->get_gmt ();

      ACE_DEBUG ((LM_DEBUG,
                  "%s%s%d:%s%d:%d\n",
                  "Time in Greenwich is ",
                  tod.hour < 10 ? "0" : "",
                  tod.hour,
                  tod.minute < 10 ? "0" : "",
                  tod.minute,
                  tod.second));
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("client: a CORBA exception occured");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "client: an unknown exception was caught\n"),
                        1);
    }

  return 0;
}
