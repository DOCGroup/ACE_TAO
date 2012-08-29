
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
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_time.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("server.ior");
int done = 0;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:d"));
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
  // Indicates successful parsing of the command line
  return 0;
}


TimeOfDay
Time_impl::
get_gmt ()
{
  time_t time_now = ACE_OS::time (0);
  struct tm *time_p = ACE_OS::gmtime (&time_now);

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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        {
          return 1;
        }

      // Get reference to Root POA.
      CORBA::Object_var obj
        = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa
        = PortableServer::POA::_narrow (obj.in ());

      // Activate POA manager.
      PortableServer::POAManager_var mgr
        = poa->the_POAManager ();

      mgr->activate ();

      // Create an object.
      Time_impl time_servant;

      // Write its stringified reference to stdout.
      PortableServer::ObjectId_var id =
        poa->activate_object (&time_servant);

      CORBA::Object_var object = poa->id_to_reference (id.in ());

      Time_var tm = Time::_narrow (object.in ());

      CORBA::String_var str = orb->object_to_string (tm.in ());

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
            orb->work_pending ();

          if (pending)
            {
              orb->perform_work ();
            }
          do_something_else ();
        }

      orb->shutdown ();
      orb->destroy ();
    }

  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("server: a CORBA exception occured");
      return 1;
    }
  catch (...)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s\n",
                         "client: an unknown exception was caught\n"),
                         1);
    }

  return 0;
}
