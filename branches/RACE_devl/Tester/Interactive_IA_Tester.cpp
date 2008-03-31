#include "RACE/Input_Adapters/Interactive_Input_Adapter/Interactive_Input_AdapterC.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include <sstream>

// IOR file of the Interactive Input Adapter.
const char * ior = 0;
const char * filename = 0;
bool redeploy = false;
bool start = false;
size_t min = 1;
size_t max = 5;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "k:f:rm:M:s");
  int c = 0;

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
          case 'k':
            ior = get_opts.opt_arg ();
            break;

          case 'f':
            filename = get_opts.opt_arg ();
            break;

          case 'r':
            redeploy = true;
            break;

          case 's':
            start = true;
            break;

          case 'm':
            min = static_cast <size_t> (strtod (get_opts.opt_arg (),0));
            break;

          case 'M':
            max = static_cast <size_t> (strtod (get_opts.opt_arg (),0));
            break;

        case '?':  // display help for use of the server.
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                            "Usage:  %s"
                             "-k <Interactive IA IOR> "
                             "(default:file://Interactive_IA_Component.ior)\n",
                             "-f <Path to the deployment plan directory>\n",
                             "-r <redeploy>\n",
                             "-s <start controller>\n",
                             "-m <min>\n",
                             "-M <max>\n",
                            argv [0]),
                            -1);
          break;
        }
    }

  if (ior  == 0)
    {
      ior = "file://Interactive_IA_Component.ior";
    }

  return 0;
}

int
main (int argc, char *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv,
                                            "");

      if (parse_args (argc, argv) != 0)
        {
          return -1;
        }

      CORBA::Object_var obj =
        orb->string_to_object (ior);

      ::CIAO::RACE::Input_Adapter::Interactive_IA_Component_var iia =
      ::CIAO::RACE::Input_Adapter::Interactive_IA_Component::_narrow
          (obj.in ());

      if (CORBA::is_nil (iia.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to acquire Interactive Input Adapter's "
                             "objref\n"),
                            -1);
        }

      // Now that we have obtained the object reference to the IIA
      // component, we try to acquire the objref to the Admin interface.

      ::CIAO::RACE::Input_Adapter::Admin_var admin =
          iia->provide_admin ();

      if (filename)
      {
        admin->stop_controller ();
        for (size_t i = min; i <= max; ++i)
          {
            std::stringstream name;
            name << filename
                 << i
                 << ".cdp";
            CORBA::String_var id;
            if (admin->deploy_plan (name.str ().c_str (), id.out()))
              {
                ACE_DEBUG ((LM_DEBUG, "Plan %s has been successfully "
                            "deployed.\n", id.in ()));
              }
          }
        if (start)
        {
          admin->start_controller ();
        }


//         ACE_OS::sleep (10);
//         admin->stop_controller ();

//             if (filename2)
//               {
//                 ACE_OS::sleep (10);
//                 admin->stop_controller ();
//                 if (admin->deploy_plan (filename2, id.out()))
//                   {
//                     ACE_DEBUG ((LM_DEBUG, "Plan %s has been "
//                                 "successfully deployed.\n", id.in ()));
//                   }
//                 admin->start_controller ();
//               }

//             if (admin->tear_down_plan (id.in ()))
//               {
//                 ACE_DEBUG ((LM_DEBUG, "Plan %s has been successfully "
//                             "torn down.\n", id.in ()));
//                 ACE_OS::sleep (10);
//                 if (admin->deploy_plan (filename, id.out()))
//                   {
//                     ACE_DEBUG ((LM_DEBUG, "Again Plan %s has been "
//                                 "successfully deployed.\n", id.in ()));
//                   }
//               }
//             admin->start_controller ();
//           }
      }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Unknown exception \n");
      return -1;
    }

  return 0;
}
