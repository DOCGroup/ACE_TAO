#include "ace/Task.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/SString.h"
#include "ace/OS_NS_stdlib.h"
#include "orbsvcs/PortableGroup/GOA.h"
#include "test_impl.h"

#include <cstdio>
#include <memory>

struct Server_Options
{
  ACE_TString uipmc_url { ACE_TEXT ("corbaloc:miop:1.0@1.0-test-1/225.1.1.8:32158") };
  ACE_TString ior_output_file { ACE_TEXT ("test.ior") };
  CORBA::ULong threads { 2u };
};

int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:u:t:"));
  int c {};

  while ((c = get_opts ()) != -1)
    {
      switch (c)
        {
        case 'o':
          options.ior_output_file = get_opts.opt_arg ();
          break;

        case 'u':
          options.uipmc_url = get_opts.opt_arg ();
          break;

        case 't':
          options.threads = ACE_OS::strtoul (get_opts.opt_arg (), nullptr, 10);
          break;

        case '?':
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("usage: %s -o <iorfile> -u <uipmc_url> -t <threads>\n"),
                             argv[0]),
                            -1);
        }
    }
  return 0;
}

class OrbThread : public ACE_Task_Base
{
public:
  explicit OrbThread (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  OrbThread (OrbThread const &) = delete;
  OrbThread (OrbThread &&) = delete;
  OrbThread &operator= (OrbThread const &) = delete;
  OrbThread &operator= (OrbThread &&) = delete;

  int svc () override
  {
    try
      {
        this->orb_->run ();
      }
      catch (CORBA::Exception const &ex)
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
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      Server_Options options;

      if (parse_args (argc, argv, options) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: wrong arguments\n")),
                          -1);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableGroup::GOA_var const root_goa =
        PortableGroup::GOA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_goa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: nil RootPOA\n")),
                          -1);

      PortableServer::POAManager_var const poa_manager = root_goa->the_POAManager ();

      CORBA::Object_var const obj = orb->string_to_object (options.uipmc_url.c_str ());

      PortableServer::ObjectId_var const id =
        root_goa->create_id_for_reference (obj.in ());

      UIPMC_Object_Impl *uipmc_impl {};
      ACE_NEW_RETURN (uipmc_impl,
                      UIPMC_Object_Impl (orb.in (), options.threads),
                      -1);
      // ServantBase_var adopts the initial reference returned by ACE_NEW_RETURN.
      PortableServer::ServantBase_var owner_transfer (uipmc_impl);
      root_goa->activate_object_with_id (id.in (), uipmc_impl);

      Test::UIPMC_Object_var const uipmc_obj =
        Test::UIPMC_Object::_unchecked_narrow (obj.in ());

      if (CORBA::is_nil (uipmc_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("ERROR: nil Hello object\n")),
                          -1);
      CORBA::String_var const ior = orb->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("MIOP object is <%C>\n"), ior.in ()));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file.c_str ()),
                            -1);
        if (std::fprintf (output_file.get (), "%s", ior.in ()) < 0)
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Cannot write IOR to output file\n")), -1);
      }

      poa_manager->activate ();

      {
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Starting %u thread, thread pool\n"),
                    static_cast<unsigned int> (options.threads)));
        OrbThread orb_thr (orb.in ());
        orb_thr.activate (THR_NEW_LWP | THR_JOINABLE,
                          static_cast<int> (options.threads));
        orb_thr.wait ();
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Stopping thread pool\n")));
      }

      root_goa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught in server main ():");
      return -1;
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nServer finished successfully.\n")));
  return 0;
}
