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
#include "tao/debug.h"
#include "ace/Get_Opt.h"

const char *ior_output_file = "server.ior";
int done = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'd':
        TAO_debug_level++;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}


TimeOfDay
Time_impl::
get_gmt ( ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  time_t time_now = time (0);
  struct tm *time_p = gmtime (&time_now);

  TimeOfDay tod;
  tod.hour = time_p->tm_hour;
  tod.minute = time_p->tm_min;
  tod.second = time_p->tm_sec;

  done = 1;

  return tod;
}

void do_something_else()
{
  // Sleep a bit so we don't eat up
  // a ton of cpu
  ACE_OS::sleep(3);
}

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

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA"
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ()
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate POA manager.
      PortableServer::POAManager_var mgr
        = poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      mgr->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create an object.
      Time_impl time_servant;

      // Write its stringified reference to stdout.
      Time_var tm = time_servant._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str = orb->object_to_string (tm.in ()
                                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  str.in ()));

      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");

          if (output_file == 0)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  "Cannot open output file for writing IOR: %s",
                  ior_output_file
                ),
                1
              );
            }

          ACE_OS::fprintf (output_file,
                           "%s",
                           str.in ());
          ACE_OS::fclose (output_file);
        }

      // Explicit Event Loop.
      while (!done)
        {
          CORBA::Boolean pending =
            orb->work_pending (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (pending)
            {
              orb->perform_work (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
          do_something_else ();
        }

      orb->shutdown ();
      orb->destroy ();
    }

  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "server: a CORBA exception occured");
      return 1;
    }
  ACE_CATCHALL
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s\n",
                         "client: an unknown exception was caught\n"),
                         1);
    }
  ACE_ENDTRY;

  return 0;
}
