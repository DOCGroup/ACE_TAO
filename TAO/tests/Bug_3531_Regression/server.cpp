#include "test_i.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "tao/ORB_Core.h"

#include <memory>
#include <string>

static int
parse_args (int argc,
            ACE_TCHAR *argv[],
            std::basic_string<ACE_TCHAR> &ior_output_file,
            std::basic_string<ACE_TCHAR> &srv_shutdown_file,
            int &nthreads)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:n:x:"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'n':
        nthreads = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'x':
        srv_shutdown_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s "
                                     "-o <iorfile>"
                                     "\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}

/**
 * Run a server thread
 *
 * Use the ACE_Task_Base class to run server threads
 */
class Worker : public ACE_Task_Base
{
public:
  explicit Worker (CORBA::ORB_ptr orb);
  Worker (Worker const &) = delete;
  Worker (Worker &&) = delete;
  Worker &operator= (Worker const &) = delete;
  Worker &operator= (Worker &&) = delete;

  int svc () override;
  // The thread entry point.

private:
  CORBA::ORB_var orb_;
  // The orb
};

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      std::basic_string<ACE_TCHAR> ior_output_file {ACE_TEXT ("file://test.ior")};
      std::basic_string<ACE_TCHAR> srv_shutdown_file {ACE_TEXT ("server_terminated")};
      int nthreads {4};
      CORBA::ORB_var const orb {CORBA::ORB_init (argc, argv)};

      CORBA::Object_var const poa_object {orb->resolve_initial_references ("RootPOA")};

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa {PortableServer::POA::_narrow (poa_object.in ())};

      PortableServer::POAManager_var const poa_manager {root_poa->the_POAManager ()};

      if (parse_args (argc, argv, ior_output_file, srv_shutdown_file, nthreads) != 0)
        return 1;

      Simple_Server_i server_impl (orb.in ());

      PortableServer::ObjectId_var const id {root_poa->activate_object (&server_impl)};

      CORBA::Object_var const object {root_poa->id_to_reference (id.in ())};

      Simple_Server_var const server {Simple_Server::_narrow (object.in ())};

      CORBA::String_var const ior {orb->object_to_string (server.in ())};

      ACE_DEBUG ((LM_DEBUG, "Activated as <%s>\n", ior.in ()));

      {
        // Close the IOR file before waiting for clients.
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (ior_output_file.c_str (), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      Worker worker (orb.in ());
      if (worker.activate (THR_NEW_LWP | THR_JOINABLE,
                           nthreads) != 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot activate client threads\n"),
                          1);

      worker.thr_mgr ()->wait ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const output_file (
          ACE_OS::fopen (ACE_TEXT_ALWAYS_CHAR (srv_shutdown_file.c_str ()), "w"), ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing: %s"),
                             ACE_TEXT_ALWAYS_CHAR (srv_shutdown_file.c_str ())),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", "OK\n");
      }
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}

// ****************************************************************

Worker::Worker (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

int
Worker::svc ()
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
