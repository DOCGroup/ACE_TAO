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
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Check arguments.
      if  (argc != 2)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: client IOR_string\n"),
                            1);
        }

      // Destringify argv[1].
      CORBA::Object_var obj = orb->string_to_object (argv[1]
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if  (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Time reference\n"),
                            1);
        }

      // Narrow.
      Time_var tm = Time::_narrow (obj.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if  (CORBA::is_nil (tm.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Argument is not a Time reference\n"),
                            1);
        }

      // Get time.
      TimeOfDay tod = tm->get_gmt (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s%s%d:%s%d:%d\n",
                  "Time in Greenwich is ",
                  tod.hour < 10 ? "0" : "",
                  tod.hour,
                  tod.minute < 10 ? "0" : "",
                  tod.minute,
                  tod.second));
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "client: a CORBA exception occured");
      return 1;
    }
  ACE_CATCHALL
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "client: an unknown exception was caught\n"),
                        1);
    }
  ACE_ENDTRY;

  ACE_CHECK_RETURN (-1);
  return 0;
}
