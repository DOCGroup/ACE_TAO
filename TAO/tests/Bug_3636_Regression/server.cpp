#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Log_Msg.h"
#include <memory>
#include <string>

int
parse_args (int argc, ACE_TCHAR *argv[], std::basic_string<ACE_TCHAR> &ior_output_file)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
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

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  int idle_count {};
  std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("test.ior")};
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      if (parse_args (argc, argv, ior_output_file) != 0)
        return 1;

      Hello *hello_impl {};
      ACE_NEW_RETURN (hello_impl,
                      Hello (orb.in ()),
                      1);
      // ServantBase_var takes ownership of the servant pointer.
      PortableServer::ServantBase_var const owner_transfer (hello_impl);

      PortableServer::ObjectId_var const id = root_poa->activate_object (hello_impl);

      CORBA::Object_var const object = root_poa->id_to_reference (id.in ());

      Test::Hello_var const hello = Test::Hello::_narrow (object.in ());

      CORBA::String_var const ior = orb->object_to_string (hello.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s\n",
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      for (;;)
        {
          ACE_Time_Value tv (0, 500);
          while (orb->work_pending (tv))
            {
              ACE_DEBUG ((LM_DEBUG, "Work pending\n"));
              ACE_Time_Value tv2 (0, 500);
              if (orb->work_pending (tv2))
                {
                  ACE_Time_Value work_tv (0, 500);
                  orb->perform_work (work_tv);
                }
            }
          ++idle_count;
        }

      orb->destroy ();
    }
  catch (CORBA::BAD_INV_ORDER const &)
    {
      // Expected
    }
  catch (CORBA::Exception const & ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  if (idle_count == 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "ERROR: Got unexpected idle_count %d\n", idle_count), 1);
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Got %d idle moments\n", idle_count));
    }

  return 0;
}
