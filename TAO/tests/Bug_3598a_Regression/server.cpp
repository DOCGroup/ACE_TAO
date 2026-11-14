#include "Hello.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Strategies/OC_Endpoint_Selector_Loader.h"
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

      Test::Hello_var const hello = hello_impl->_this ();

      CORBA::String_var const ior = orb->object_to_string (hello.in ());

      // Output the IOR to the <ior_output_file>
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Cannot open output file for writing IOR: %s",
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      root_poa->destroy (true, true);

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "Event loop finished.\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
