
//=============================================================================
/**
 *  @file    server.cpp
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
#include "ace/SString.h"
#include <memory>

bool done {};

struct Server_Options
{
  ACE_TString ior_output_file {ACE_TEXT ("server.ior")};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:d"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'd':
        TAO_debug_level++;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -o <iorfile>\n"),
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
  time_t const time_now = ACE_OS::time (nullptr);
  tm const *const time_p = ACE_OS::gmtime (&time_now);

  TimeOfDay tod {};
  tod.hour = time_p->tm_hour;
  tod.minute = time_p->tm_min;
  tod.second = time_p->tm_sec;

  done = true;

  return tod;
}

void
do_something_else ()
{
  // Sleep a bit so we don't eat up
  // a ton of cpu
  ACE_OS::sleep (3);
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Server_Options options;
  try
    {
      // Initialize orb
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv, options) != 0)
        {
          return 1;
        }

      // Get reference to Root POA.
      CORBA::Object_var const obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const poa =
        PortableServer::POA::_narrow (obj.in ());

      // Activate POA manager.
      PortableServer::POAManager_var const mgr = poa->the_POAManager ();

      mgr->activate ();

      // Create an object.
      Time_impl time_servant;

      // Write its stringified reference to stdout.
      PortableServer::ObjectId_var const id =
        poa->activate_object (&time_servant);

      CORBA::Object_var const object = poa->id_to_reference (id.in ());

      Time_var const tm = Time::_narrow (object.in ());

      CORBA::String_var const str = orb->object_to_string (tm.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "%s\n",
                  str.in ()));

      if (!options.ior_output_file.empty ())
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);

          if (!output_file)
            {
              ACE_ERROR_RETURN ((
                  LM_ERROR,
                  ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                  options.ior_output_file.c_str ()),
                1);
            }

          ACE_OS::fprintf (output_file.get (),
                           "%s",
                           str.in ());
        }

      // Explicit Event Loop.
      while (!done)
        {
          CORBA::Boolean const pending =
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

  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("server: a CORBA exception occurred");
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
