//
// $Id$
//

#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "test_impl.h"

ACE_TCHAR const *uipmc_url =
  ACE_TEXT ("corbaloc:miop:1.0@1.0-test-1/225.1.1.8:32158");
ACE_TCHAR const *ior_output_file = ACE_TEXT ("test.ior");
CORBA::ULong threads = 2u;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:u:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'u':
        uipmc_url = get_opts.opt_arg ();
        break;

      case 't':
        threads = ACE_OS::strtoul (get_opts.opt_arg (), 0, 10);
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-o <iorfile> ")
                           ACE_TEXT ("-u <uipmc_url> ")
                           ACE_TEXT ("-t <threads> ")
                           ACE_TEXT ("\n"),
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line.
  return 0;
}

class OrbThread : public ACE_Task_Base
{
public:
  OrbThread (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  virtual int svc (void)
  {
    try
      {
        this->orb_->run ();
      }
    catch (const CORBA::Exception& ex)
      {
        ex._tao_print_exception ("Exception caught in OrbThread:");
        return -1;
      }

    return 0;
  }

private:
  CORBA::ORB_var orb_;
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: wrong arguments\n")),
                          -1);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableGroup::GOA_var root_goa =
        PortableGroup::GOA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_goa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: nil RootPOA\n")),
                          -1);

      PortableServer::POAManager_var poa_manager = root_goa->the_POAManager ();

      // Create UIPMC reference.
      CORBA::Object_var obj = orb->string_to_object (uipmc_url);

      // Create id.
      PortableServer::ObjectId_var id =
        root_goa->create_id_for_reference (obj.in ());

      // Activate UIPMC Object.
      UIPMC_Object_Impl* uipmc_impl;
      ACE_NEW_RETURN (uipmc_impl,
                      UIPMC_Object_Impl (orb.in (), threads),
                      -1);
      PortableServer::ServantBase_var owner_transfer1 (uipmc_impl);
      root_goa->activate_object_with_id (id.in (), uipmc_impl);

      Test::UIPMC_Object_var uipmc_obj =
        Test::UIPMC_Object::_unchecked_narrow (obj.in ());

      if (CORBA::is_nil (uipmc_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: nil Hello object\n")),
                          -1);
      CORBA::String_var ior = orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("MIOP object is <%C>\n"), ior.in ()));

      //obj = hello_impl->_this ();

      // If the ior_output_file exists, output the ior to it.
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");

          if (output_file == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("Cannot open output file ")
                                 ACE_TEXT ("for writing IOR: %s"),
                                 ior_output_file),
                                -1);
            }

          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      {
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting %u thread, thread pool\n"), (unsigned) threads));
        // start server thread pool
        OrbThread orb_thr (orb.in ());
        orb_thr.activate (THR_NEW_LWP | THR_JOINABLE, threads);
        orb_thr.wait ();
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Stopping thread pool\n")));
      }

      root_goa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught in server main ():");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("\nServer finished successfully.\n")));
  return 0;
}
