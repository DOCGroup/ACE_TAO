#include "TX_Object_i.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdlib.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Task.h"

#include <memory>
#include <string>

static constexpr char cert_file[] = "cacert.pem";

struct Server_Options
{
  ACE_TCHAR const *ior_output_file {};
  int thread_count {1};
};

class OrbTask : public ACE_Task_Base
{
public:
  explicit OrbTask (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb))
  {
  }

  OrbTask (OrbTask const &) = delete;
  OrbTask (OrbTask &&) = delete;
  OrbTask &operator= (OrbTask const &) = delete;
  OrbTask &operator= (OrbTask &&) = delete;

  int svc () override
  {
    try
      {
        this->orb_->run ();
      }
    catch (CORBA::Exception const &)
      {
      }
    return 0;
  }

private:
  CORBA::ORB_var orb_;
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:t:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 't':
        options.thread_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage:  %s "
                           "-o <iorfile> "
                           "-t <thread count>\n",
                           argv [0]),
                          -1);
      }

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Server_Options options;
      std::string env ("SSL_CERT_FILE=");
      env += cert_file;
      ACE_OS::putenv (env.c_str ());

      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);
      CORBA::Object_var const poa_obj =
        orb->resolve_initial_references ("RootPOA");
      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_obj.in ());
      PortableServer::POAManager_var const poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      TX_Object_i impl_object (orb.in ());
      TX_Object_var const tx_object = impl_object._this ();
      CORBA::String_var const ior = orb->object_to_string (tx_object.in ());

      ACE_DEBUG ((LM_DEBUG, "Activated as <%C>\n", ior.in ()));

      if (options.ior_output_file != nullptr)
        {
          std::unique_ptr<FILE, int (*) (FILE *)> output_file (
            ACE_OS::fopen (options.ior_output_file, ACE_TEXT ("w")),
            ACE_OS::fclose);
          if (!output_file)
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("Cannot open output file for writing IOR: %s\n"),
                               options.ior_output_file),
                              1);
          ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
        }

      OrbTask task (orb.in ());
      if (task.activate (THR_NEW_LWP | THR_JOINABLE,
                         options.thread_count) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate threads\n"),
                          1);
      task.wait ();

      ACE_DEBUG ((LM_DEBUG,
                  "\n"
                  "Event loop finished.\n"));

      root_poa->destroy (1, 1);
      orb->destroy ();
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("ERROR");
      return -1;
    }

  return 0;
}
