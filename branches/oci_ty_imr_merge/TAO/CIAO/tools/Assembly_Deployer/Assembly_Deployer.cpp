// $Id$

#include "ciao/CCM_DeploymentC.h"
#include "Assembly_ServiceC.h"
#include "ciao/Client_init.h"
#include "ciao/Server_init.h"
#include "ciao/Cookies.h"
#include "ace/Get_Opt.h"
#include "ace/streams.h"
#include "ace/High_Res_Timer.h"
#include "ace/OS_NS_stdio.h"

const char *ior = "file://test.ior";
const char *cad = 0;
const char *cookie_output = 0;
const char *cookie_teardown = 0;
int benchmark = 0;
int shutdown_server = 0;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "a:k:o:d:tx");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        cad = get_opts.opt_arg ();
        break;

      case 't':
        benchmark = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'o':
        cookie_output = get_opts.opt_arg ();
        break;

      case 'd':
        cookie_teardown = get_opts.opt_arg ();
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s \n"
                           "-a <assembly descriptor (.cad)>\n"
                           "-t: turn on deployment timer\n"
                           "-o <filename for outputing assembled cookie>"
                           "-d <filename for cookie to be teardown'ed>"
                           "-k <Assembly_Manager ior> \n"
                           "\n",
                           argv [0]),
                          -1);
      }

  if (cookie_output != 0)
    {
      if (cad == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Must specify an assembly descriptor using -a flag.\n"),
                          -1);
    }
  else if (cookie_teardown == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Must specify to build or teardown an assembly using -o or -d flage.\n"),
                      -1);

  // Indicates sucessful parsing of the command line
  return 0;
}

void
create_assembly (Components::Deployment::AssemblyFactory_ptr factory
                 ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  ACE_High_Res_Timer overall, make_assembly, build_assembly;
  overall.start ();             // Start measuring the overall time to
                                // deploy an application.

  ACE_DEBUG ((LM_DEBUG, "Creating an Assembly with %s\n", cad));

  make_assembly.start ();   // Start measuring the time to parse a
  // cad file and create an assembly object.
  Components::Cookie_var ck =
    factory->create_assembly (cad
                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  make_assembly.stop ();    // Stop measuring

  Components::Deployment::Assembly_var assembly =
    factory->lookup (ck.in ()
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  build_assembly.start ();  // Start measuring the time to
  // instantiate and connect the
  // application.
  assembly->build (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  build_assembly.stop ();
  overall.stop ();

  if (benchmark != 0)
    {
      ACE_Time_Value overall_time, make_time, build_time;
      overall.elapsed_time (overall_time);
      make_assembly.elapsed_time (make_time);
      build_assembly.elapsed_time (build_time);

      ACE_DEBUG ((LM_DEBUG,
                  "Total deployment time: %d.%06d sec\n"
                  " create assembly time: %d.%06d sec\n"
                  "  build assembly time: %d.%06d sec\n",
                  overall_time.sec (), overall_time.usec (),
                  make_time.sec (), make_time.usec (),
                  build_time.sec (), build_time.usec ()));
    }

  ACE_Active_Map_Manager_Key key;
  CIAO::Map_Key_Cookie::extract (ck.in (), key);

  // Output Cookie value here.
  FILE *ckh = ACE_OS::fopen (cookie_output, "w");
  if (ckh != NULL)
    {
      ACE_OS::fprintf (ckh, "%d %d",
                       key.slot_index (),
                       key.slot_generation ());

      ACE_OS::fclose (ckh);
    }
  else
    ACE_ERROR ((LM_ERROR, "Unable to open cookie file: %s\n", cookie_output));
}

void
teardown_assembly (Components::Deployment::AssemblyFactory_ptr factory
                   ACE_ENV_ARG_DECL_WITH_DEFAULTS)
{
  // Extract and restore Cookie value (ck) here.
  ACE_UINT32 i, g;
  FILE *ckh = ACE_OS::fopen (cookie_teardown, "r");
  if (ckh != NULL)
    {
      fscanf (ckh, "%d %d", &i, &g);
      ACE_OS::fclose (ckh);
    }
  else
    ACE_ERROR ((LM_ERROR, "Unable to open cookie file: %s\n", cookie_teardown));

  CIAO::Map_Key_Cookie ck;
  ACE_Active_Map_Manager_Key key (i, g);
  ck.insert (key);

  // Look up the assembly from the factory.
  Components::Deployment::Assembly_var assembly =
    factory->lookup (&ck
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  assembly->tear_down (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  factory->destroy (&ck
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      CIAO::Server_init (orb.in ());

      CORBA::Object_var tmp =
        orb->string_to_object(ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CIAO::Assembly_Service_var as_svc
        = CIAO::Assembly_Service::_narrow (tmp.in ()
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (as_svc.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil Assembly Service reference <%s>\n",
                             ior),
                            1);
        }

      ::Components::Deployment::AssemblyFactory_var factory =
          as_svc->get_assemblyfactory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             "Nil AssemblyFactory reference <%s>\n",
                             ior),
                            1);
        }

      if (cookie_output != 0)
        {
          create_assembly (factory.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      else if (cookie_teardown != 0)
        {
          teardown_assembly (factory.in () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (shutdown_server != 0) // can only shutdown when tearing down stuff.
            {
              as_svc->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
