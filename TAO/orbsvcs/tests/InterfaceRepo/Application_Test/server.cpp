// -*- C++ -*-
#include "test_i.h"
#include "ace/OS_NS_stdio.h"
#include "orbsvcs/Shutdown_Utilities.h"
#include "ace/Get_Opt.h"
#include <memory>

static ACE_TCHAR const *ior_output_file {ACE_TEXT ("iorfile")};

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <ior> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

class Service_Shutdown_Functor : public Shutdown_Functor
{
public:
  explicit Service_Shutdown_Functor (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  void operator() (int which_signal) override
  {
    ACE_DEBUG ((LM_DEBUG,
                "shutting down on signal %d\n", which_signal));
    (void) this->orb_->shutdown ();
  }

private:
  CORBA::ORB_var orb_ {};
};

static int
write_ior_to_file (char const* ior)
{
  std::unique_ptr<FILE, int (*) (FILE *)> output_file (
    ACE_OS::fopen (ior_output_file, "w"), ACE_OS::fclose);

  if (!output_file)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Cannot open output files for writing IOR: %s\n",
                         ior_output_file),
                        -1);
    }

  int const result = ACE_OS::fprintf (output_file.get (),
                                "%s",
                                ior);
  if (result < 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "ACE_OS::fprintf failed while writing %s to %s\n",
                         ior,
                         ior_output_file),
                        -1);
    }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) != 0)
        return 1;

      Service_Shutdown_Functor killer (orb.in ());
      Service_Shutdown kill_contractor (killer);

      CORBA::Object_var const poa_object =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      inventory_i servant_impl;

      warehouse::inventory_var const servant =
        servant_impl._this ();

      CORBA::String_var const ior =
        orb->object_to_string (servant.in ());

      int const write_result = write_ior_to_file (ior.in ());

      if (write_result != 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%p\n",
                             "write_ior_to_file"),
                            -1);
        }

      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("server exception:");
      return 1;
    }

  return 0;
}
